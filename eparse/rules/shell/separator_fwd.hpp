#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename Parser , typename Sep , typename Range >
class Separator;

template < typename Parser , typename Sep , typename Range >
constexpr Separator< std::decay_t<Parser> , std::decay_t<Sep> , Range >
separator( Parser&& parser , Sep&& sep , Range const& range );

}}}

namespace ep {

template < typename Parser , typename Sep >
constexpr auto
separator( Parser&& parser , Sep&& sep , unsigned int begin , unsigned int end );
template < typename Parser , typename Sep >
constexpr auto
separator( Parser&& parser , Sep&& sep , unsigned int count );
template < typename Parser , typename Sep >
constexpr auto
separator( Parser&& parser , Sep&& sep );
template < typename Parser , typename Sep >
constexpr auto
separator_min( Parser&& parser , Sep&& sep , unsigned int min );


}
