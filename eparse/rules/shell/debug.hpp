#pragma once

#include "debug_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include <iostream>
#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Debug
  : public core::expression< Debug<P> >
{
protected:
  P parser_;
public:
  constexpr Debug( P parser )
    : parser_( std::move(parser) )
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
  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<P,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    std::cout << "Parse result : ";
    if( auto res = parser().parse_attribute( begin , end , skipper ) )
    {
      std::cout << "SUCCESS, " << std::distance( begin_ , begin ) << " iterates, ";
      for( auto i=begin_; i!=begin; ++i )
      {
        if( std::iscntrl( *i )==false )
        {
          std::cout << *i;
        }
      }
      std::cout << std::endl;
      return res;
    }else
    {
      std::cout << "FAILED" << std::endl;
      if( begin != begin_ )
      {
        std::cout << "ERROR : Iterator have not been rerolled" << std::endl;
      }
      return core::none;
    }
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse( begin , end , skipper );
  }
};

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Debug<P>
debug( P parser )
{
  return { std::move(parser) };
}

}

namespace ep { namespace core {

template < typename P , typename I >
struct attribute_of< rules::shell::Debug<P> , I >
{
  using type = typename attribute_of<P,I>::type;
};

}}
