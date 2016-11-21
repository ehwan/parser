#pragma once

#include "repeat_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/unused_type.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/vector_shell.hpp"
#include "../../core/skip.hpp"
#include "../../support/range.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P , typename Range >
class Repeat
  : public core::expression< Repeat<P,Range> >
{
protected:
  P parser_;
  Range range_;

public:
  constexpr Repeat( P parser , Range range )
    : parser_( std::move(parser) ) ,
      range_( std::move(range) )
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
  Range& range()
  {
    return range_; 
  }
  Range const& range() const
  {
    return range_; 
  }

  template < typename I >
  using vector_shell = 
    support::vector_shell<
      typename traits::attribute_of<P,I>::type
    >;

  template < typename I , typename S >
  core::optional_t< typename vector_shell<I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    typename vector_shell<I>::type vec;
    unsigned int count = 0;
    while( 1 )
    {
      if( auto i = parser().parse_attribute( begin , end , skipper ) )
      {
        ++count;
        vector_shell<I>::push_back( vec , std::move( *i ) );
        if( range_.end( count ) )
        {
          return vec;
        }
        core::skip( begin , end , skipper );
      }else
      {
        if( range_( count ) )
        {
          return vec;
        }
        begin = begin_;
        return core::none;
      }
    }
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    unsigned int count = 0;
    while( 1 )
    {
      if( parser().parse( begin , end , skipper ) )
      {
        ++count;
        if( range_.end( count ) ){ return true; }
        core::skip( begin , end , skipper );
      }else
      {
        if( range_( count ) )
        {
          return true;
        }
        begin = begin_;
        return false;
      }
    }
  }
};

template < typename Parser , typename Range >
constexpr Repeat< std::decay_t<Parser> , Range >
repeat( Parser&& parser , Range const& range )
{
  return { static_cast< Parser&& >( parser ) ,
           range };
}

}}}

namespace ep {

template < typename Parser >
constexpr auto
repeat( Parser&& parser , unsigned int begin , unsigned int end )
{
  return rules::shell::repeat( static_cast< Parser&& >( parser ) ,
      support::range<unsigned int,unsigned int>( begin , end ) );
}
template < typename Parser >
constexpr auto
repeat( Parser&& parser , unsigned int count )
{
  return rules::shell::repeat( static_cast< Parser&& >( parser ) ,
      support::range_one<unsigned int>( count ) );
}
template < typename Parser >
constexpr auto
repeat( Parser&& parser )
{
  return rules::shell::repeat( static_cast< Parser&& >( parser ) ,
      support::range< core::unused_type , core::unused_type >() );
}
template < typename Parser >
constexpr auto
repeat_min( Parser&& parser , unsigned int min )
{
  return rules::shell::repeat( static_cast< Parser&& >( parser ) ,
      support::range<unsigned int,core::unused_type>( min , {} ) );
}
template < typename Parser >
constexpr auto
plus( Parser&& parser )
{
  return repeat_min( static_cast< Parser&& >( parser ) , 1 );
}
template < typename Parser >
constexpr auto
star( Parser&& parser )
{
  return rules::shell::repeat( static_cast< Parser&& >( parser ) ,
      support::range<core::unused_type,core::unused_type>( {} , {} ) );
}

}

namespace ep { namespace traits {

template < typename P , typename Range , typename I >
struct attribute_of< rules::shell::Repeat<P,Range> , I >
{
  using type = typename support::vector_shell<
    typename attribute_of<P,I>::type
  >::type;
};

}}
