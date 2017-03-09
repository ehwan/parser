#pragma once

namespace ep { namespace rules { namespace shell {

template < typename Parser , typename Functor >
class Action;

}}}

namespace ep {

template < typename Parser , typename Functor >
constexpr rules::shell::Action<Parser,Functor>
action( Parser parser , Functor functor );

}
