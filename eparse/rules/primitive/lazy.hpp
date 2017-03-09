#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include <type_traits>
#include <tuple>
#include <utility>

namespace ep { namespace rules { namespace primitive {

template < typename Functor >
class Lazy
  : public core::expression< Lazy<Functor> >
{
  protected:
    Functor functor_;
public:
  
  constexpr Lazy( Functor functor )
    : functor_( std::move(functor) )
  {
  }

  Functor& functor()
  {
    return functor_;
  }
  Functor const& functor() const
  {
    return functor_;
  }
  constexpr decltype(auto) parser()
  {
    return functor_();
  }
  constexpr decltype(auto) parser() const
  {
    return functor_();
  }
  constexpr decltype(auto) operator ()()
  {
    return functor_();
  }
  constexpr decltype(auto) operator ()() const
  {
    return functor_();
  }

  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<
    std::decay_t< typename std::result_of< Functor() >::type > , I
  >::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse_attribute( begin , end , skipper );
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse( begin , end , skipper );
  }

};

}}}

namespace ep {

template < typename Functor >
constexpr rules::primitive::Lazy< Functor >
lazy( Functor functor )
{
  return { std::move(functor) };
}

}

namespace ep { namespace core {

template < typename Functor , typename I >
struct attribute_of< rules::primitive::Lazy<Functor> , I >
{
  using type = typename attribute_of< 
    typename std::decay_t< typename std::result_of< Functor() >::type > , I
  >::type;
};

}}
