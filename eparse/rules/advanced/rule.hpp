#pragma once

namespace ep { namespace rules { namespace advanced {

template < typename Attr , typename Sfinae >
class Rule;

template < typename Attr , typename Sfinae , typename Iterator >
Attr parse_attribute_rule( Rule<Attr,Sfinae> const& rule , Iterator& begin , Iterator const& end );

template < typename Attr , typename Sfinae >
class Rule
{
};

}}}
