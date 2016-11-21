#pragma once

#include <boost/variant/variant_fwd.hpp>

namespace ep { namespace core {

template < typename ... Ts >
using variant = boost::variant< Ts... >;

}}
