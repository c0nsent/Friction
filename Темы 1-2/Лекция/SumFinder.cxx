/**
 * @date 2025-10-03
 * 
 * @author amitayus_
 */

#include <unordered_set>
#include <print>
#include <cstdint>
#include <vector>

void print( const std::vector<std::pair<uint32_t, uint32_t>> &v )
{
	for ( const auto &pair : v )
	{
		std::print( "[{}, {}] ", pair.first, pair.second );
	}
}


auto findSums( const std::vector<uint32_t> &v, const uint32_t sum ) -> std::vector<std::pair<uint32_t, uint32_t>>
{
	auto initSet = [&v]
	{
		std::unordered_set<uint32_t> s;
		s.reserve( v.size() );

		for ( const uint32_t num : v )
			s.insert( num );

		return s;
	};

	const auto set{ initSet() };
	std::vector<std::pair<uint32_t, uint32_t>> result;
	result.reserve( set.size() );

	for ( size_t i{ 0 }; i < sum/2; i++  )
	{
		if ( not set.contains( i ) ) continue;

		if ( set.contains( sum - i ) )
			result.emplace_back(i, sum - i ) ;
	}

	return result;
}

int main()
{
	const std::vector<uint32_t> v{ 4, 6 ,2 ,5, 7, 4, 6, 7, 3 };
	const auto result{ findSums( v, 10 ) };
	print( result );
}