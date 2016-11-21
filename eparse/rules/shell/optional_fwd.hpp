#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Optional;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Optional< std::decay_t<P> >
optional( P&& p );

}
