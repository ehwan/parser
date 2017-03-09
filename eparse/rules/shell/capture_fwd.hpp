#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P >
class Capture;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Capture<P>
capture( P parser );

}
