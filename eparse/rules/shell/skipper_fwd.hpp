#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P , typename S >
class Skipper;

}}}

namespace ep {

template < typename P , typename S >
constexpr rules::shell::Skipper<P,S>
skipper( P p , S s );

}
