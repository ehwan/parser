#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P >
class Until;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Until<P>
until( P parser );
;

}
