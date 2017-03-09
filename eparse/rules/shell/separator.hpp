#pragma once

#include "separator_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"
#include "../../support/vector_shell.hpp"
#include "../../core/skip.hpp"
#include "../../support/range.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename Parser , typename Sep , typename Range >
class Separator
  : public core::expression< Separator<Parser,Sep,Range> >
{
protected:
  Parser parser_;
  Sep sep_;
  Range range_;
public:
  constexpr Separator( Parser parser , Sep sep , Range range )
    : parser_( std::move(parser) ) ,
      sep_( std::move(sep) ) ,
      range_( std::move(range) )
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
  Sep& separator()
  {
    return sep_;
  }
  Sep const& separator() const
  {
    return sep_;
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
      typename core::attribute_of<Parser,I>::type
    >;

  template < typename I , typename S >
  core::optional_t< typename vector_shell<I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    I last_end = begin_;
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
        last_end = begin;
        core::skip( begin , end , skipper );
        if( separator().parse( begin , end , skipper ) == false )
        {
          goto parse_fail;
        }
        core::skip( begin , end , skipper );
      }
      continue;

parse_fail:
      if( range_( count ) )
      {
        begin = last_end;
        return vec;
      }
      begin = begin_;
      return core::none;
    }
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    I last_end = begin_;
    unsigned int count = 0;
    while( 1 )
    {
      if( parser().parse( begin , end , skipper ) )
      {
        ++count;
        if( range_.end( count ) )
        {
          return true;
        }
        last_end = begin;
        core::skip( begin , end , skipper );
        if( separator().parse( begin , end , skipper ) == false )
        {
          goto parse_fail;
        }
        core::skip( begin , end , skipper );
      }
      continue;

parse_fail:
      if( range_( count ) )
      {
        begin = last_end;
        return true;
      }
      begin = begin_;
      return false;
    }
  }

};

template < typename Parser , typename Sep , typename Range >
constexpr Separator<Parser,Sep,Range>
separator( Parser parser , Sep sep , Range const& range )
{
  return { std::move(parser) ,
           std::move(sep) ,
           range };
}

}}}

namespace ep {

template < typename Parser , typename Sep >
constexpr auto
separator( Parser parser , Sep sep , unsigned int begin , unsigned int end )
{
  return rules::shell::separator( std::move(parser) ,
      std::move(sep) ,
      support::range<unsigned int,unsigned int>( begin , end ) );
}
template < typename Parser , typename Sep >
constexpr auto
separator( Parser parser , Sep sep , unsigned int count )
{
  return rules::shell::separator( std::move(parser) ,
      std::move(sep) ,
      support::range_one<unsigned int>( count ) );
}
template < typename Parser , typename Sep >
constexpr auto
separator( Parser parser , Sep sep )
{
  return rules::shell::separator( std::move(parser) ,
      std::move(sep) ,
      support::range< core::unused_type , core::unused_type >() );
}
template < typename Parser , typename Sep >
constexpr auto
separator_min( Parser parser , Sep sep , unsigned int min )
{
  return rules::shell::separator( std::move(parser) ,
      std::move(sep) ,
      support::range< unsigned int , core::unused_type >( min , {} ) );
}


}

namespace ep { namespace core {

template < typename P , typename S , typename R , typename I >
struct attribute_of< rules::shell::Separator<P,S,R> , I >
{
  using type = typename support::vector_shell<
    typename attribute_of<P,I>::type
  >::type;
};

}}
