/**
 * @date 2025-10-06
 *
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

constexpr bool c_debug{ false };

template<typename T>
constexpr void print_vector( const std::vector<T> &v )
{
	if constexpr (not c_debug) return;
	for ( const T &e : v )
	{
		std::cout << '[' << e << "] ";
	}
	std::cout << std::endl;
}

struct View
{
	std::shared_ptr<std::vector<std::int64_t>> data;
	size_t offset{};
	size_t len{};
	bool isBase{ false };
};

static std::vector<std::int64_t> parseNumbers( const std::string &s )
{
	std::vector<std::int64_t> out;
	std::int64_t cur{0};
	std::int64_t sign{1};
	bool inNum{false};

	for ( const char c : s)
	{
		if (c == '-')
		{
			if (inNum)
			{
				out.push_back(sign * cur);
				cur = 0; inNum = false;
			}
			sign = -1;
		}
		else if (c >= '0' && c <= '9')
		{
			inNum = true;
			cur = cur * 10 + static_cast<std::int64_t>(c - '0');
		}
		else
		{
			if (inNum)
			{
				out.push_back(sign * cur);
				cur = 0; inNum = false;
				sign = 1;
			}
			else
			{
				sign = 1;
			}
		}
	}
	if (inNum) out.push_back(sign * cur);
	return out;
}

static std::string readUntil( std::istream &is, const char target )
{
	std::string s;
	char c{};
	while (is.get(c))
	{
		s.push_back(c);
		if (c == target) break;
	}
	return s;
}

auto readProgram( const std::string_view path ) -> std::optional<std::vector<std::string>>
{
	std::ifstream ifs( path.data() );
	if (not ifs) return std::nullopt;

	std::int32_t n{};
	if (not (ifs >> n)) return std::nullopt;

	std::vector<std::string> lines;
	lines.reserve( static_cast<std::size_t>(n) );

	// Каждую строку дочитываем целиком до ';' или ')'
	for (std::int32_t i{0}; i < n; ++i)
	{
		std::string first;
		if (!(ifs >> first)) return std::nullopt;

		std::string full = first;

		// если это "List", надо взять ещё имя и rhs до ';'
		if (first == "List")
		{
			std::string name, eq, rhs;
			if (!(ifs >> name)) return std::nullopt;
			if (!(ifs >> eq)) return std::nullopt; // '='
			if (!(ifs >> rhs)) return std::nullopt;

			full += ' ';
			full += name;
			full += ' ';
			full += eq;
			full += ' ';
			full += rhs;

			if (full.find(')') == std::string::npos) full += readUntil(ifs, ')');
			if (full.back() != ';') full += readUntil(ifs, ';');
		}
		else
		{
			// операция вида a.get(...), возможно, разбита
			if (full.find(')') == std::string::npos) full += readUntil(ifs, ')');
			if (full.back() != ';') full += readUntil(ifs, ';');
		}

		lines.push_back( std::move(full) );
	}
	return lines;
}

static std::string execute(const std::vector<std::string> &program)
{
	std::unordered_map<std::string, View> env;
	env.reserve(program.size() * 2);

	std::string out;
	out.reserve( program.size() * 3 );

	for (const std::string &cmd : program)
	{
		if (c_debug) std::cout << "CMD: " << cmd << std::endl;

		if (cmd.starts_with("List "))
		{
			// "List a = new List(...);" or "List b = a.subList(l,r);"
			auto p1 = cmd.find(' ');
			auto p2 = cmd.find(' ', p1 + 1);
			const std::string var = cmd.substr(p1 + 1, p2 - (p1 + 1));

			auto pEq = cmd.find('=', p2);
			auto rhs = cmd.substr(pEq + 1);
			// trim leading space
			if (!rhs.empty() && rhs.front() == ' ') rhs.erase(0,1);

			if (rhs.starts_with("new"))
			{
				// rhs: "new List(args);"
				auto lp = rhs.find('(');
				auto rp = rhs.find(')');
				const std::string inside = rhs.substr(lp + 1, rp - lp - 1);
				auto nums = parseNumbers(inside);

				auto vec = std::make_shared<std::vector<std::int64_t>>( std::move(nums) );
				View v{ vec, 0u, vec->size(), true };
				env[var] = std::move(v);
			}
			else
			{
				// rhs: "a.subList(l,r);"
				auto dot = rhs.find('.');
				const std::string src = rhs.substr(0, dot);

				auto lp = rhs.find('(');
				auto rp = rhs.find(')');
				const std::string inside = rhs.substr(lp + 1, rp - lp - 1);
				auto nums = parseNumbers(inside);
				const std::size_t from = static_cast<std::size_t>(nums[0]);
				const std::size_t to   = static_cast<std::size_t>(nums[1]);

				const View &sv = env.at(src);
				View v{ sv.data, sv.offset + from, to - from, false };
				env[var] = std::move(v);
			}
		}
		else
		{
			// "<name>.<op>(...);"
			auto dot = cmd.find('.');
			const std::string var = cmd.substr(0, dot);

			auto lp = cmd.find('(', dot);
			auto rp = cmd.find(')', lp);
			const std::string op = cmd.substr(dot + 1, lp - (dot + 1));
			const std::string inside = cmd.substr(lp + 1, rp - lp - 1);
			auto nums = parseNumbers(inside);

			View &v = env.at(var);

			if (op == "get")
			{
				const size_t i = static_cast<size_t>(nums[0]);
				const int64_t val = (*(v.data))[v.offset + i];
				out.append( std::to_string(val) );
				out.push_back('\n');
			}
			else if (op == "set")
			{
				const size_t i = static_cast<size_t>(nums[0]);
				const std::int64_t x = nums[1];
				(*(v.data))[v.offset + i] = x;
			}
			else if (op == "add")
			{
				const std::int64_t x = nums[0];
				v.data->push_back(x);
				if (v.isBase) v.len += 1;
			}
		}
	}
	return out;
}

int main()
{
	const std::optional program{ readProgram("input.txt") };
	if (not program) return -1;

	const std::string output{ execute( program.value() ) };

	if (std::ofstream ofs("output.txt"); ofs)
		ofs << output;

	return 0;
}