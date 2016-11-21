#pragma once

#include "expression_fwd.hpp"

#include "../rules/shell/action_fwd.hpp"
#include "../rules/shell/except_fwd.hpp"
#include "../rules/shell/optional_fwd.hpp"
#include "../rules/shell/or_fwd.hpp"
#include "../rules/shell/repeat_fwd.hpp"
#include "../rules/shell/separator_fwd.hpp"
#include "../rules/shell/sequence_fwd.hpp"
#include "../rules/shell/skipper_fwd.hpp"
#include "../rules/shell/and_fwd.hpp"
#include "../rules/shell/until_fwd.hpp"
#include "../rules/shell/unused_fwd.hpp"
#include "../rules/shell/bad_fwd.hpp"
#include "../rules/shell/ref_fwd.hpp"
#include "../rules/primitive/attribute_fwd.hpp"

#include "../support/action_set_attribute.hpp"


namespace ep { namespace core {

template < typename Attr , typename Crtp >
auto operator << ( Attr& , expression<Crtp> const& );
template < typename Attr , typename Crtp >
auto operator << ( Attr& , expression<Crtp>&& );

template < typename Attr , typename Crtp >
auto operator < ( Attr& , expression<Crtp> const& );
template < typename Attr , typename Crtp >
auto operator < ( Attr& , expression<Crtp>&& );

template < typename Crtp >
class expression
{
  template < typename Attr >
  friend auto operator << ( Attr& attr , expression<Crtp> const& exp )
  {
    return ::ep::action( exp.derived() , support::action_set_attribute_t<Attr,false>(attr) );
  }
  template < typename Attr >
  friend auto operator << ( Attr& attr , expression<Crtp>&& exp )
  {
    return ::ep::action( exp.derived() , support::action_set_attribute_t<Attr,false>(attr) );
  }

  template < typename Attr >
  friend auto operator < ( Attr& attr , expression<Crtp> const& exp )
  {
    return ::ep::action( exp.derived() , support::action_set_attribute_t<Attr,true>(attr) );
  }
  template < typename Attr >
  friend auto operator < ( Attr& attr , expression<Crtp>&& exp )
  {
    return ::ep::action( exp.derived() , support::action_set_attribute_t<Attr,true>(attr) );
  }


protected:
  Crtp& derived() &
  {
    return *static_cast< Crtp* >( this );
  }
  Crtp const& derived() const&
  {
    return *static_cast< Crtp const* >( this );
  }
  Crtp&& derived() &&
  {
    return static_cast< Crtp&& >( *this );
  }
public:
  using parser_type = Crtp;


  rules::shell::Ref<Crtp> ref() &;
  rules::shell::Ref<Crtp const> ref() const&;
  
  template < typename Functor >
  constexpr auto operator[]( Functor&& functor ) const&
  {
    return ::ep::action( derived() , static_cast< Functor&& >( functor ) );
  }
  template < typename Functor >
  constexpr auto operator[]( Functor&& functor ) &&
  {
    return ::ep::action( derived() , static_cast< Functor&& >( functor ) );
  }

  template < typename Attr >
  constexpr auto operator ^ ( Attr&& attr ) const&
  {
    return ::ep::sequence<false>( unused(derived()) , ::ep::attribute(static_cast<Attr&&>(attr)) );
  }
  template < typename Attr >
  constexpr auto operator ^ ( Attr&& attr ) &&
  {
    return ::ep::sequence<false>( unused(derived()) , ::ep::attribute(static_cast<Attr&&>(attr)) );
  }

  template < typename Next >
  constexpr auto operator >> ( Next&& next ) const&
  {
    return ::ep::sequence<true>( derived() , static_cast<Next&&>(next) );
  }
  template < typename Next >
  constexpr auto operator >> ( Next&& next ) &&
  {
    return ::ep::sequence<true>( derived() , static_cast<Next&&>(next) );
  }

  template < typename Next >
  constexpr auto operator > ( Next&& next ) const&
  {
    return ::ep::sequence<false>( derived() , static_cast<Next&&>(next) );
  }
  template < typename Next >
  constexpr auto operator > ( Next&& next ) &&
  {
    return ::ep::sequence<false>( derived() , static_cast<Next&&>(next) );
  }

  template < typename B >
  constexpr auto operator | ( B&& b ) const&
  {
    return ::ep::or_( derived() , static_cast< B&& >( b ) );
  }
  template < typename B >
  constexpr auto operator | ( B&& b ) &&
  {
    return ::ep::or_( derived() , static_cast< B&& >( b ) );
  }

  template < typename B >
  constexpr auto operator & ( B&& b ) const&
  {
    return ::ep::and_( derived() , static_cast< B&& >( b ) );
  }
  template < typename B >
  constexpr auto operator & ( B&& b ) &&
  {
    return ::ep::and_( derived() , static_cast< B&& >( b ) );
  }

  constexpr auto operator * () const&
  {
    return ::ep::star( derived() );
  }
  constexpr auto operator * () &&
  {
    return ::ep::star( derived() );
  }

  constexpr auto operator * ( unsigned int count ) const&
  {
    return ::ep::repeat( derived() , count );
  }
  constexpr auto operator * ( unsigned int count ) &&
  {
    return ::ep::repeat( derived() , count );
  }

  constexpr auto operator + () const&
  {
    return ::ep::plus( derived() );
  }
  constexpr auto operator + () &&
  {
    return ::ep::plus( derived() );
  }
  constexpr auto operator - () const&
  {
    return ::ep::optional( derived() );
  }
  constexpr auto operator - () &&
  {
    return ::ep::optional( derived() );
  }

  template < typename B >
  constexpr auto operator - ( B&& b ) const&
  {
    return ::ep::except( derived() , static_cast< B&& >( b ) );
  }
  template < typename B >
  constexpr auto operator - ( B&& b ) &&
  {
    return ::ep::except( derived() , static_cast< B&& >( b ) );
  }

  template < typename S >
  constexpr auto operator % ( S&& s ) const&
  {
    return ::ep::separator( derived() , static_cast< S&& >( s ) );
  }
  template < typename S >
  constexpr auto operator % ( S&& s ) &&
  {
    return ::ep::separator( derived() , static_cast< S&& >( s ) );
  }

  template < typename S >
  constexpr auto operator / ( S&& s ) const&
  {
    return ::ep::skipper( derived() , static_cast< S&& >( s ) );
  }
  template < typename S >
  constexpr auto operator / ( S&& s ) &&
  {
    return ::ep::skipper( derived() , static_cast< S&& >( s ) );
  }

  constexpr auto operator ~ () const&
  {
    return ::ep::until( derived() );
  }
  constexpr auto operator ~ () &&
  {
    return ::ep::until( derived() );
  }

  constexpr auto operator -- () const&
  {
    return ::ep::unused( derived() );
  }
  constexpr auto operator -- () &&
  {
    return ::ep::unused( derived() );
  }

  auto operator & () const&
  {
    return ref();
  }
  auto operator & () &
  {
    return ref();
  }

  constexpr auto operator ! () const&
  {
    return bad( derived() );
  }
  constexpr auto operator ! () &&
  {
    return bad( derived() );
  }
};



}}
