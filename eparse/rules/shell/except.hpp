#pragma once

#include "except_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"

namespace ep { namespace rules { namespace shell {

template < typename P , typename E >
class Except
  : public core::expression< Except<P,E> >
{
protected:
  P parser_;
  E except_;
public:
  constexpr Except( P parser , E except )
    : parser_( std::move(parser) ) ,
      except_( std::move(except) )
  {
  }


  P& parser()
  {
    return parser_;
  }
  P const& parser() const
  {
    return parser_;
  }
  E& except()
  {
    return except_;
  }
  E const& except() const
  {
    return except_;
  }

  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<P,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I e_begin = begin;
    if( except().parse( e_begin , end , skipper ) == false )
    {
      return parser().parse_attribute( begin , end , skipper );
    }
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I e_begin = begin;
    if( except().parse( e_begin , end , skipper ) == false )
    {
      return parser().parse( begin , end , skipper );
    }
    return false;
  }
};

}}}

namespace ep {

template < typename P , typename E >
constexpr rules::shell::Except< std::decay_t<P> , std::decay_t<E> >
except( P&& parser , E&& except )
{
  return { static_cast< P&& >( parser ) ,
           static_cast< E&& >( except ) };
}

}

namespace ep { namespace core {

template < typename P , typename E , typename I >
struct attribute_of< rules::shell::Except<P,E> , I >
{
  using type = typename attribute_of< P , I >::type;
};

}}
