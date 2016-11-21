#pragma once

#include "../../core/unused_type.hpp"

namespace ep { namespace rules { namespace shell {

template < typename Attr , typename Iterator >
class AnyParser;

template < typename Iterator >
class AnySkipper;

}}}

namespace ep {

template < typename Iterator , typename Attr=core::unused_type >
using any_parser = rules::shell::AnyParser<Attr,Iterator>;

}
