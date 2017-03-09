#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P >
class Bad;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Bad<P>
bad( P parser );

}
