#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P >
class Unused;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Unused<P>
unused( P parser );

}
