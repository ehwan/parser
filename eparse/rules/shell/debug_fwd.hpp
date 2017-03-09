#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P >
class Debug;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Debug<P>
debug( P parser );

}
