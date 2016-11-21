#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Debug;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Debug< std::decay_t<P> >
debug( P&& parser );

}
