#pragma once

namespace ep { namespace core {

struct nothing_t
{
  template < typename ... Ts >
  nothing_t( Ts&& ... ) {}
};

}}
