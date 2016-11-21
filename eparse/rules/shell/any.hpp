#pragma once

#include "any_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/unused_type.hpp"
#include "../../core/skip.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include "../primitive/always.hpp"

#include <memory>
#include <utility>
#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename Iterator >
class AnySkipper
{
protected:
  struct holder_t
  {
    virtual ~holder_t(){}
    virtual void apply( Iterator& begin , Iterator const& end ) const = 0;
  };
  template < typename Parser >
  struct template_holder_t
    : holder_t
  {
    Parser parser_;

    template_holder_t( Parser parser )
      : parser_( std::move(parser) )
    {
    }

    void apply( Iterator& begin , Iterator const& end ) const
    {
      core::skip( begin , end , parser_ );
    }
  };

  std::unique_ptr< holder_t > parser_;
  
public:
  AnySkipper( AnySkipper<Iterator> const& ) = delete;
  AnySkipper( AnySkipper<Iterator>&& rhs )
    : parser_( std::move( rhs.parser_ ) )
  {
  }
  template < typename P >
  AnySkipper( P parser )
    : parser_( new template_holder_t<P>( std::move(parser) ) )
  {
  }

  void apply( Iterator& begin , Iterator const& end ) const
  {
    parser_->apply( begin , end );
  }
};

}}}

namespace ep { namespace core {

template < typename I >
void skip( I& begin , I const& end , rules::shell::AnySkipper<I> const& skipper )
{
  skipper.apply( begin , end );
}

}}

namespace ep { namespace rules { namespace shell {

template < typename Attr , typename Iterator >
class AnyParser
  : public core::expression< AnyParser<Attr,Iterator> >
{
  using this_type = AnyParser<Attr,Iterator>;
protected:
  struct holder_t
  {
    virtual ~holder_t(){}
    virtual holder_t* clone() const = 0;
    virtual core::optional_t<Attr> parse_attribute( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const = 0;
    virtual bool parse( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const = 0;
  };
  template < typename Parser >
  struct template_holder_t
    : holder_t
  {
    Parser parser_;

    template_holder_t( Parser parser )
      : parser_( std::move(parser) )
    {
    }

    holder_t* clone() const
    {
      return new template_holder_t<Parser>( parser_ );
    }
    core::optional_t<Attr>
    parse_attribute( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const
    {
      return parser_.parse_attribute( begin , end , skipper );
    }
    bool parse( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const
    {
      return parser_.parse( begin , end , skipper );
    }
  };

  std::unique_ptr<holder_t> parser_;
  
public:
  
  AnyParser(){}
  AnyParser( this_type const& rhs )
  {
    if( rhs.parser_ )
    {
      parser_.reset( rhs.parser_->clone() );
    }
  }
  AnyParser( this_type&& rhs )
    : parser_( std::move( rhs.parser_ ) )
  {
  }
  template < typename P >
  AnyParser( P parser )
    : parser_( new template_holder_t<P>( std::move(parser) ) )
  {
  }

  template < typename P >
  void set( P parser )
  {
    parser_.reset(
      new template_holder_t<P>( std::move(parser) )
    );
  }

  this_type& operator = ( this_type const& rhs )
  {
    if( rhs.parser_ )
    {
      parser_.reset( rhs.parser_->clone() );
    }else
    {
      parser_.reset();
    }
    return *this;
  }
  this_type& operator = ( this_type&& rhs )
  {
    parser_ = std::move( rhs.parser_ );
    return *this;
  }
  template < typename P >
  this_type& operator = ( P&& parser )
  {
    set( static_cast<P&&>(parser) );
    return *this;
  }


  core::optional_t<Attr>
  parse_attribute( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const
  {
    return parser_->parse_attribute( begin , end , skipper );
  }
  bool parse( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const
  {
    return parser_->parse( begin , end , skipper );
  }
};
template < typename Iterator >
class AnyParser< core::unused_type , Iterator >
{
  using this_type = AnyParser<core::unused_type,Iterator>;
protected:
  struct holder_t
  {
    virtual ~holder_t(){}
    virtual holder_t* clone() const = 0;
    virtual bool parse( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const = 0;
  };
  template < typename Parser >
  struct template_holder_t
    : holder_t
  {
    Parser parser_;

    template_holder_t( Parser parser )
      : parser_( std::move(parser) )
    {
    }

    holder_t* clone() const
    {
      return new template_holder_t<Parser>( parser_ );
    }
    bool parse( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const
    {
      return parser_.parse( begin , end , skipper );
    }
  };

  std::unique_ptr<holder_t> parser_;
  
public:
  
  AnyParser(){}
  AnyParser( this_type const& rhs )
  {
    if( rhs.parser_ )
    {
      parser_.reset( rhs.parser_->clone() );
    }
  }
  AnyParser( this_type&& rhs )
    : parser_( std::move( rhs.parser_ ) )
  {
  }
  template < typename P >
  AnyParser( P parser )
    : parser_( new template_holder_t<P>( std::move(parser) ) )
  {
  }

  template < typename P >
  void set( P parser )
  {
    parser_.reset(
      new template_holder_t<P>( std::move(parser) )
    );
  }

  this_type& operator = ( this_type const& rhs )
  {
    if( rhs.parser_ )
    {
      parser_.reset( rhs.parser_->clone() );
    }else
    {
      parser_.reset();
    }
    return *this;
  }
  this_type& operator = ( this_type&& rhs )
  {
    parser_ = std::move( rhs.parser_ );
    return *this;
  }
  template < typename P >
  this_type& operator = ( P&& parser )
  {
    set( static_cast<P&&>(parser) );
    return *this;
  }


  core::optional_t<core::unused_type>
  parse_attribute( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const
  {
    if( parse( begin , end , skipper ) )
    {
      return core::unused_type();
    }else
    {
      return core::none;
    }
  }
  bool parse( Iterator& begin , Iterator const& end , AnySkipper<Iterator> const& skipper ) const
  {
    return parser_->parse( begin , end , skipper );
  }
};

}}}


namespace ep { namespace traits {

template < typename Attr , typename I >
struct attribute_of< rules::shell::AnyParser<Attr,I> , I >
{
  using type = Attr;
};

}}
