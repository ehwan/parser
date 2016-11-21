#pragma once

namespace ep { namespace rules { namespace primitive {

template < typename Attr >
class Attribute;

}}}

namespace ep {

template < typename Attr >
rules::primitive::Attribute<Attr> attribute( Attr attr );

}
