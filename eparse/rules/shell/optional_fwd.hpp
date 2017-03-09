#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P >
class Optional;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Optional<P>
optional( P p );

}
