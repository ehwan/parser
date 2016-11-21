#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P , typename E >
class Except;

}}}

namespace ep {

template < typename P , typename E >
constexpr rules::shell::Except< std::decay_t<P> , std::decay_t<E> >
except( P&& parser , E&& except );

}
