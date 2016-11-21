#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Unused;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Unused< std::decay_t<P> >
unused( P&& parser );

}
