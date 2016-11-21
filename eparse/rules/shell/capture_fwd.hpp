#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Capture;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Capture< std::decay_t<P> >
capture( P&& parser );

}
