#pragma once

namespace ep { namespace support {

template < typename Attr , bool Ret >
struct action_set_attribute_t;

template < typename Attr >
struct action_set_attribute_t< Attr , false >
{
  Attr& attr_;

  action_set_attribute_t( Attr& attr )
    : attr_( attr )
  {
  }
  
  template < typename Attr_ >
  void operator ()( Attr_& attr ) const
  {
    attr_ = Attr( std::move( attr ) );
  }
};
template < typename Attr >
struct action_set_attribute_t< Attr , true >
{
  Attr& attr_;

  action_set_attribute_t( Attr& attr )
    : attr_( attr )
  {
  }
  
  template < typename Attr_ >
  Attr_ operator ()( Attr_& attr ) const
  {
    attr_ = static_cast< Attr >( attr );
    return attr;
  }
};

}}
