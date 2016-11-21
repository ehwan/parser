#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P , typename Range >
class Repeat;

template < typename Parser , typename Range >
constexpr Repeat< std::decay_t<Parser> , Range >
repeat( Parser&& parser , Range const& range );

}}}

namespace ep {

template < typename Parser >
constexpr auto
repeat( Parser&& parser , unsigned int begin , unsigned int end );
template < typename Parser >
constexpr auto
repeat( Parser&& parser , unsigned int count );
template < typename Parser >
constexpr auto
repeat( Parser&& parser );
template < typename Parser >
constexpr auto
repeat_min( Parser&& parser , unsigned int min );
template < typename Parser >
constexpr auto
plus( Parser&& parser );
template < typename Parser >
constexpr auto
star( Parser&& parser );

}
