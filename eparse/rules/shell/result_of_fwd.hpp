#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P >
class ResultOf;

}}}

namespace ep {

template < typename P >
constexpr rules::shell::ResultOf<P>
result_of( P p );

}
