/**
 * @date 10/4/25
 * 
 * @author amitayus_
 *
 * лол получается смысла данный код не имеет, ведь он предназначен для огромных чисел а у нас здесь ограничение в 64 бита
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_set>

/*constexpr uint64_t fibonacciAt( const uint64_t order ) noexcept
{
	if ( order < 2 ) return order;
	return fibonacciAt( order - 1 ) + fibonacciAt( order - 2 );
}*/

void print( const std::array<std::unordered_set<uint64_t>, 3> &usedHashes )
{
	for ( const auto &set : usedHashes )
	{
		for ( const auto &num : set )
			std::cout << num << ' ';
		std::cout << "\n\n" << std::endl;
	}
}

static constexpr std::array<uint64_t, 3> c_fibonacciMods
{
	1'000'000'007, 1'000'000'011, 1'000'000'013
};

void fillFibonacciHashes( std::array<std::unordered_set<uint64_t>, 3> &fibonacciHashes, uint64_t reservedSetSize=93)
{
	uint64_t f1{1}, f2{1};
	while ( reservedSetSize-- )
	{
		const auto fn{ f1 + f2  };
		f1 = f2;
		f2 = fn;

		for ( size_t i{ 0 }; i != fibonacciHashes.size(); i++ )
		{
			fibonacciHashes[i].insert( fn % c_fibonacciMods[i] );
		}
	}
}

auto initFibonacciHashes( const uint64_t reservedSetSize = 93 ) -> std::array<std::unordered_set<uint64_t>, 3>
{
	std::array<std::unordered_set<uint64_t>, 3> usedHashes;

	for ( auto &set : usedHashes )
		set.reserve( reservedSetSize );

	return usedHashes;
}

bool isFibonacci( const uint64_t num)
{
	auto usedHashes{ initFibonacciHashes() };
	fillFibonacciHashes( usedHashes );

//	print( usedHashes );

	bool result{ false };

	for ( size_t i{ 0 }; i != usedHashes.size(); i++ )
	{
		std::cout << num << " % " << num % c_fibonacciMods[i] << std::endl;
		if ( usedHashes[i].contains( num % c_fibonacciMods[i] ) )
		{
			result = true;
		}

	}

	return result;
}

int main()
{
//	std::cout << std::numeric_limits<uint64_t>::max() << std::endl;
	while (true)
	{
		uint64_t input{};
		std::cin >> input;
		std::cout << (isFibonacci(input ) ? "True" : "False") << std::endl;
		//std::cout << c_fibonacciMods[0] << std::endl;
	}
}