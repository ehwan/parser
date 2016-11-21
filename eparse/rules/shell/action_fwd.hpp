#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename Parser , typename Functor >
class Action;

}}}

namespace ep {

template < typename Parser , typename Functor >
constexpr rules::shell::Action< std::decay_t<Parser> , std::decay_t<Functor> >
action( Parser&& parser , Functor&& functor );

}
