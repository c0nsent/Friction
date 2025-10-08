#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

constexpr bool c_debug{ true };

template<typename T>
void vectorDebugPrint( const std::vector<T> &v )
{
	if constexpr (not c_debug) return;

	for ( const T &e : v )
		std::cout << '[' << e << "] ";

	std::cout << std::endl;
}

void joyDebugPrint( const int64_t whoseTurn, const uint16_t weight, const int64_t baseJoy )
{
	if constexpr (not c_debug) return;

	const std::string strWhoseTurn{ (whoseTurn == 1) ? "Vasya" : "Masha" };
	std::cout << strWhoseTurn << " turn, weight: " << weight << ", base joy: " << baseJoy << std::endl;
}

auto readInput( const std::string_view path ) -> std::vector<uint16_t>
{
	std::ifstream ifs( path.data() );

	size_t mushroomWeightCount{};
  ifs >> mushroomWeightCount;

	std::vector<uint16_t> v_mushroomWeights;
	v_mushroomWeights.reserve( mushroomWeightCount );

	uint16_t mushroomWeight{};
	while ( ifs >> mushroomWeight )
	{
		v_mushroomWeights.emplace_back( mushroomWeight );
	}

	vectorDebugPrint( v_mushroomWeights );

	return v_mushroomWeights;
}

int64_t calcBaseJoy( const std::vector<uint16_t> &v_mushroomWeights )
{
	int64_t whoseTurn{ -1 }, baseJoy{};
	for ( const auto weight : v_mushroomWeights )
	{
		whoseTurn *= -1; //Если -1 то Машин
		baseJoy += weight * whoseTurn;

		joyDebugPrint( whoseTurn, weight, baseJoy );
	}

	return baseJoy;
}

int64_t calcMaxJoy( const std::vector<uint16_t> &v_mushroomWeights )
{
	uint16_t maxWeight{0};
	uint16_t minWeight{ std::numeric_limits<uint16_t>::max() };

	for ( size_t i{ 0 }; i != v_mushroomWeights.size(); i++ )
	{
		if ( i % 2 == 0)
			minWeight = std::min( minWeight, v_mushroomWeights[i] );

		else
			maxWeight = std::max( maxWeight, v_mushroomWeights[i] );
	}

	const int64_t baseJoy{ calcBaseJoy( v_mushroomWeights ) };
	const int64_t maxJoy{ std::max( baseJoy, baseJoy + 2 * ( maxWeight - minWeight )) };

	if constexpr ( c_debug )
		std::cout << "Max weight: " << maxWeight << ", min weight: " << minWeight << ", max joy: " << maxJoy << std::endl;

	return maxJoy;
}

int main()
{
	const auto maxJoy{ calcMaxJoy( readInput("input.txt") ) };

	std::ofstream ofs("output.txt");
	ofs << maxJoy;
}