#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/endian.hpp"

#include <cstdint>

namespace ep { namespace rules { namespace primitive {

template < typename T >
class Binary
  : public core::expression< Binary<T> >
{
protected:
  bool little_;
  unsigned int count_;

public:
  constexpr Binary( bool little , unsigned int count )
    : little_( little ) ,
      count_( count )
  {
  }

  template < typename I >
  core::optional_t<T>
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( little_ )
    {
      return support::parse_little_endian<T>( begin , end , count_ );
    }
    return support::parse_big_endian<T>( begin , end , count_ );
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    I begin_ = begin;
    unsigned int c = count_;
    while( c-- )
    {
      if( (begin != end) == false )
      {
        begin = begin_;
        return false;
      }
      ++begin;
    }
    return true;
  }
};

}}}

namespace ep {

template < typename T = unsigned int >
constexpr rules::primitive::Binary<T>
binary( unsigned int bytes , bool little_endian )
{
  return { little_endian , bytes };
}

template < typename T = unsigned int >
constexpr auto little_endian( unsigned int bytes )
{
  return binary<T>( bytes , true );
}
template < typename T = unsigned int >
constexpr auto big_endian( unsigned int bytes )
{
  return binary<T>( bytes , false );
}
constexpr auto  word_little = little_endian< std::uint16_t >( 2 );
constexpr auto dword_little = little_endian< std::uint32_t >( 4 );
constexpr auto qword_little = little_endian< std::uint64_t >( 8 );
constexpr auto  word_big = big_endian< std::uint16_t >( 2 );
constexpr auto dword_big = big_endian< std::uint32_t >( 4 );
constexpr auto qword_big = big_endian< std::uint64_t >( 8 );

}

namespace ep { namespace traits {

template < typename T , typename I >
struct attribute_of< rules::primitive::Binary<T> , I >
{
  using type = T;
};

}}
