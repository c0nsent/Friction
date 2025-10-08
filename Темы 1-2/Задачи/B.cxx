#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

static auto readInput(std::istream &is = std::cin)
{
	std::array<long double, 6> x{};
	for (auto &e : x) is >> e;
	return x;
}

static long double seg(long double d, long double s)
{
	return d / s;
}

int main()
{
	const auto [a,b,c, v0,v1,v2]{ readInput(std::cin) };

	const long double variants[][6] = {
		{ a, v0,  c, v1,  b, v2 },
		{ b, v0,  c, v1,  a, v2 },
		{ a, v0,  a, v1,  b, v0 },
		{ b, v0,  b, v1,  a, v0 },
		{ a, v0,  c, v1,  b, v2 },
		{ b, v0,  c, v1,  a, v2 }
	};

	long double ans{ std::numeric_limits<long double>::infinity() };

	for (int i = 0; i < 6; ++i)
	{
		long double t = seg(variants[i][0], variants[i][1])
									+ seg(variants[i][2], variants[i][3])
									+ seg(variants[i][4], variants[i][5]);
		if (i == 4) t += seg(a, v0);
		if (i == 5) t += seg(b, v0);
		if (t < ans) ans = t;
	}

	std::cout.setf(std::ios::fixed);
	std::cout << std::setprecision(15) << ans;
}