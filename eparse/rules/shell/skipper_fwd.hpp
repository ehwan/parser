#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P , typename S >
class Skipper;

}}}

namespace ep {

template < typename P , typename S >
constexpr rules::shell::Skipper< std::decay_t<P> , std::decay_t<S> >
skipper( P&& p , S&& s );

}
