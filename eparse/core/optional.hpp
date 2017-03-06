#pragma once

#include <experimental/optional>

namespace ep { namespace core
{

constexpr decltype(auto) none = std::experimental::nullopt;

template < typename T >
using optional_t = std::experimental::optional< T >;

template < typename T >
struct remove_optional;

template < typename T >
struct remove_optional< optional_t<T> >
{
  using type = T;
};

}}
