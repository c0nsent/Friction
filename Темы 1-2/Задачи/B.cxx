#include <array>
#include <fstream>
#include <iomanip>


auto readInput( const char *path ) -> std::array<double, 6>
{
	std::ifstream ifs( path );

	std::array<double, 6> out{};

	for ( size_t i{ 0 }; i != 6; ++i )
		ifs >> out[i];

	return out;
}

double calcMinTime( const std::array<double, 6> &arr )
{
	const auto &[ a, b, c, v0, v1, v2 ] = arr;

	const double minA{ std::min( a, b + c ) };
	const double minB{ std::min( b, a + c ) };
	const double minC{  std::min( c, a + b ) };

	double minTime{ minA / v0 + minA / v1 + minB / v0 + minB / v1 };
	minTime = std::min( minTime, minA / v0 + minC / v1 + minB / v2 );
	minTime = std::min( minTime, minB/v0 + minC / v1 + minA /v2 );

	return minTime;
}

int main()
{
	std::ofstream ofs("output.txt");
	ofs << std::fixed << std::setprecision(20) << calcMinTime( readInput("input.txt") );
}
