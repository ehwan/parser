#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class ResultOf;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::ResultOf< std::decay_t<P> >
result_of( P&& p );

}
