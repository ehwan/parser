#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Until;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Until< std::decay_t<P> >
until( P&& parser );
;

}
