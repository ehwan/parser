#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Bad;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Bad< std::decay_t<P> >
bad( P&& parser );

}
