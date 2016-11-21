#pragma once

#include "attribute_of_fwd.hpp"

namespace ep { namespace traits {

template < typename Parser , typename Iterator >
struct attribute_of
{
  using parser_type = typename Parser::parser_type;
  using type = typename attribute_of<parser_type,Iterator>::type;
};

}}
