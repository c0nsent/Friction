#include <algorithm>
#include <cstdint>
#include <print>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <iostream>
#include <ranges>


class benchmark
{
	[[nodiscard]] uint64_t duration() const
	{
		const auto current{ std::chrono::high_resolution_clock::now() };
		const auto duration{ std::chrono::duration_cast<std::chrono::microseconds>( current - this->m_start ).count() };

		return duration;
	}

public:

	benchmark()
	: m_start{ std::chrono::high_resolution_clock::now() } {};

	~benchmark()
	{
		std::cout << "Total time: " << duration() << " microseconds" << std::endl;
	}

	void timePassed() const
	{
		std::cout << "Time passed: " << duration()<< " microseconds" << std::endl;
	}

private:

	const std::chrono::high_resolution_clock::time_point m_start{};
};

using themeMap = std::unordered_map<uint32_t, uint32_t>;

template<typename T>
void print(	const themeMap &map )
{
	for ( auto &[ k, n ] : map )
		std::print( "[{}, {}], ", k, n  );
}

void findMaxThemeCount( std::ofstream &ofs, themeMap &map )
{
	std::pair maxThemeCount{ map.begin()->first, map.begin()->second };

	for ( const auto &themeCount : map )
		if ( themeCount.second > maxThemeCount.second )
			maxThemeCount = themeCount;

	ofs << maxThemeCount.first << ' ';

	map.find( maxThemeCount.first )->second--;
}

int main()
{
	benchmark b{};
	std::ifstream ifs{"input.txt"};
	std::ofstream ofs{"output.txt"};

	uint32_t designedTaskCount{}, taskLimit{};
	themeMap taskCountByTheme;

	ifs >> designedTaskCount >> taskLimit;
	taskCountByTheme.reserve( taskLimit );

	uint32_t theme{};
	while ( ifs >> theme )
	{
		if (taskCountByTheme.contains( theme ))
			taskCountByTheme[theme]++;
		else
			taskCountByTheme.emplace( theme, 1 );
	}

	for ( uint32_t i{1}; i <= taskLimit; ++i )
	{
		findMaxThemeCount(ofs, taskCountByTheme );
	}

}