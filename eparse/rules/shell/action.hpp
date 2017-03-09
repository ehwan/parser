#pragma once

#include "action_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/attribute_of_fwd.hpp"
#include "../../support/action_invoke.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename Parser , typename Functor >
class Action
  : public core::expression< Action<Parser,Functor> >
{
  using this_type = Action<Parser,Functor>;

protected:
  Parser parser_;
  Functor functor_;

public:
  constexpr Action( Parser parser , Functor functor )
    : parser_( std::move(parser) ) ,
      functor_( std::move(functor) )
  {
  }

  Parser& parser()
  {
    return parser_;
  }
  Parser const& parser() const
  {
    return parser_;
  }
  Functor& functor()
  {
    return functor_;
  }
  Functor const& functor() const
  {
    return functor_;
  }

  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<this_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    if( auto i = parser().parse_attribute( begin , end , skipper ) )
    {
      return { support::action_void_filter_t()( functor() , *i ) };
    }
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return support::action_unused_parse_t()( begin , end , skipper , parser() , functor() );
  }
};

}}}

namespace ep {

template < typename Parser , typename Functor >
constexpr rules::shell::Action<Parser,Functor>
action( Parser parser , Functor functor )
{
  return { std::move(parser) , std::move(functor) };
}

}

namespace ep { namespace core {

template < typename P , typename F , typename I >
struct attribute_of< rules::shell::Action<P,F> , I >
{
  using child = typename attribute_of<P,I>::type;
  using type = std::decay_t<
    typename std::result_of< support::action_void_filter_t( F const& , child& ) >::type
  >;
};

}}
