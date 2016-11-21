#pragma once

#include <experimental/optional>

namespace ep { namespace core
{

constexpr decltype(auto) none = std::experimental::nullopt;

template < typename T >
using optional_t = std::experimental::optional< T >;

}}
