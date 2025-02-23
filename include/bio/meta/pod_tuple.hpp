// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::meta::pod_tuple
 */

#pragma once

#include <tuple>
#include <type_traits>

#include <bio/meta/type_list/traits.hpp>

namespace bio::meta
{

//!\brief Behaves like std::tuple but is an aggregate [PODType](https://en.cppreference.com/w/cpp/concept/PODType).
template <typename... types>
struct pod_tuple
{};

/*!\brief Behaves like std::tuple but is an aggregate [PODType](https://en.cppreference.com/w/cpp/concept/PODType).
 * \ingroup meta
 * \implements bio::meta::tuple_like
 * \tparam type0    The first type (the first type).
 * \tparam ...types 0-n types (the remaining types of the values to be stored).
 *
 * This class behaves like std::tuple, but it is itself a POD type while std::tuple is not (even
 * if all contained types are POD). Since the only benefit of this class is that it stays POD it
 * actually enforces this on all types in the tuple (if you want to add non POD types, just use
 * std::tuple instead).
 *
 * It (only) supports [aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization),
 * i.e. you must use brace-initializiers and cannot
 * use paranthesis. You can use bio::meta::get or std::get and also
 * [structured bindings](https://en.cppreference.com/w/cpp/language/declarations#Structured_binding_declaration)
 * to access the elements in the tuple.
 *
 * \include test/snippet/meta/pod_tuple.cpp
 *
 */
template <typename type0, typename... types>
struct pod_tuple<type0, types...>
{
    //!\cond DEV
    //!\brief The first element as member.
    type0               _head;
    //!\brief The rest of the elements defined as a "recursive member".
    pod_tuple<types...> _tail;
    //!\endcond

    /*!\name Comparison operators
     * \{
     */

    //!\brief Checks whether `*this` is equal to `rhs`.
    constexpr friend bool operator==(pod_tuple const & lhs, pod_tuple const & rhs) noexcept = default;

    //!\brief Checks whether `*this` is equal to `rhs`.
    constexpr friend auto operator<=>(pod_tuple const & lhs, pod_tuple const & rhs) noexcept
    {
        return std::tie(lhs._head, lhs._tail) <=> std::tie(rhs._head, rhs._tail);
    }
    //!\}
};

/*!\brief Recursion anchor for pod_tuple.
 * \ingroup meta
 * \tparam type0 The value's type (every tuple must contain at least one type).
 */
template <typename type0>
struct pod_tuple<type0>
{
    //!\cond DEV
    //!\brief The first element as member.
    type0 _head;
    //!\endcond

    /*!\name Comparison operators
     * \brief Lexicographically compares the values in the tuple.
     * \{
     */
    //!\brief Checks whether `*this` is equal to `rhs`.
    constexpr friend bool operator==(pod_tuple const & lhs, pod_tuple const & rhs) noexcept = default;

    //!\brief Checks whether `*this` is equal to `rhs`.
    constexpr friend auto operator<=>(pod_tuple const & lhs, pod_tuple const & rhs) noexcept = default;
    //!\}
};

#undef SEQAN_NOT_POD

//!\brief User defined deduction guide enables easy use.
//!\relates pod_tuple
template <typename... types>
pod_tuple(types &&...) -> pod_tuple<types...>;

/*!\name Access an element of a pod_tuple by index
 * \{
 * \brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
 *
 * Note that these functions are available, both, in the bio namespace and in namespace std.
 */
//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <std::size_t i, typename... types>
constexpr auto & get(bio::meta::pod_tuple<types...> & t) noexcept
  //!\cond
  requires(i < sizeof...(types))
//!\endcond
{
    if constexpr (i == 0)
        return t._head;
    else
        return bio::meta::get<i - 1>(t._tail);
}

//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <std::size_t i, typename... types>
constexpr auto const & get(bio::meta::pod_tuple<types...> const & t) noexcept
  //!\cond
  requires(i < sizeof...(types))
//!\endcond
{
    if constexpr (i == 0)
        return t._head;
    else
        return bio::meta::get<i - 1>(t._tail);
}

// extra overloads for temporaries required, because members of temporaries may only be returned as temporaries
//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <std::size_t i, typename... types>
constexpr auto && get(bio::meta::pod_tuple<types...> && t) noexcept
  //!\cond
  requires(i < sizeof...(types))
//!\endcond
{
    if constexpr (i == 0)
        return std::move(t._head);
    else
        return bio::meta::get<i - 1>(std::move(t._tail));
}

//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <std::size_t i, typename... types>
constexpr auto const && get(bio::meta::pod_tuple<types...> const && t) noexcept
  //!\cond
  requires(i < sizeof...(types))
//!\endcond
{
    if constexpr (i == 0)
        return std::move(t._head);
    else
        return bio::meta::get<i - 1>(std::move(t._tail));
}
//!\}

/*!\name Access an element of a pod_tuple by type
 * \brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
 *
 * Note that these functions are available, both, in the bio namespace and in namespace std.
 * As is the case with std::tuple, this function is only defined if the type appears once
 * in the tuple, i.e. `std::get<int>(std::tuple<int, int>{1,2})` is not defined.
 * \{
 */
//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <typename type, typename... arg_types>
constexpr auto & get(bio::meta::pod_tuple<arg_types...> & t) noexcept
  //!\cond
  requires(meta::detail::pack_traits::count<type, arg_types...> == 1)
//!\endcond
{
    return bio::meta::get<meta::detail::pack_traits::find<type, arg_types...>>(t);
}

//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <typename type, typename... arg_types>
constexpr auto const & get(bio::meta::pod_tuple<arg_types...> const & t) noexcept
  //!\cond
  requires(meta::detail::pack_traits::count<type, arg_types...> == 1)
//!\endcond
{
    return bio::meta::get<meta::detail::pack_traits::find<type, arg_types...>>(t);
}

//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <typename type, typename... arg_types>
constexpr auto && get(bio::meta::pod_tuple<arg_types...> && t) noexcept
  //!\cond
  requires(meta::detail::pack_traits::count<type, arg_types...> == 1)
//!\endcond
{
    return bio::meta::get<meta::detail::pack_traits::find<type, arg_types...>>(std::move(t));
}

//!\brief The same as [std::get](https://en.cppreference.com/w/cpp/utility/tuple/get) on an std::tuple.
//!\relates bio::meta::pod_tuple
template <typename type, typename... arg_types>
constexpr auto const && get(bio::meta::pod_tuple<arg_types...> const && t) noexcept
  //!\cond
  requires(meta::detail::pack_traits::count<type, arg_types...> == 1)
//!\endcond
{
    return bio::meta::get<meta::detail::pack_traits::find<type, arg_types...>>(std::move(t));
}
//!\}

} // namespace bio::meta

namespace std
{

//!\cond
template <std::size_t i, typename... types>
constexpr auto & get(bio::meta::pod_tuple<types...> & t) noexcept requires(i < sizeof...(types))
{
    return bio::meta::get<i>(t);
}

template <std::size_t i, typename... types>
constexpr auto const & get(bio::meta::pod_tuple<types...> const & t) noexcept requires(i < sizeof...(types))
{
    return bio::meta::get<i>(t);
}

template <std::size_t i, typename... types>
constexpr auto && get(bio::meta::pod_tuple<types...> && t) noexcept requires(i < sizeof...(types))
{
    return bio::meta::get<i>(std::move(t));
}

template <std::size_t i, typename... types>
constexpr auto const && get(bio::meta::pod_tuple<types...> const && t) noexcept requires(i < sizeof...(types))
{
    return bio::meta::get<i>(std::move(t));
}

template <typename type, typename... types>
constexpr auto & get(bio::meta::pod_tuple<types...> & t) noexcept
  requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return bio::meta::get<type>(t);
}

template <typename type, typename... types>
constexpr auto const & get(bio::meta::pod_tuple<types...> const & t) noexcept
  requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return bio::meta::get<type>(t);
}

template <typename type, typename... types>
constexpr auto && get(bio::meta::pod_tuple<types...> && t) noexcept
  requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return bio::meta::get<type>(std::move(t));
}

template <typename type, typename... types>
constexpr auto const && get(bio::meta::pod_tuple<types...> const && t) noexcept
  requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return bio::meta::get<type>(std::move(t));
}
//!\endcond

/*!\brief Obtains the type of the specified element.
 * \implements bio::meta::transformation_trait
 * \relates bio::meta::pod_tuple
 * \see [std::tuple_element](https://en.cppreference.com/w/cpp/utility/tuple/tuple_element)
 */
template <std::size_t i, template <typename...> typename t, typename... types>
    //!\cond
    requires(i < sizeof...(types))
&&std::is_base_of_v<bio::meta::pod_tuple<types...>, t<types...>>
  //!\endcond
  struct tuple_element<i, t<types...>>
{
    //!\brief Element type.
    using type = bio::meta::detail::pack_traits::at<i, types...>;
};

/*!\brief Provides access to the number of elements in a tuple as a compile-time constant expression.
 * \implements bio::meta::unary_type_trait
 * \see std::tuple_size_v
 * \relates bio::meta::pod_tuple
 */
template <template <typename...> typename t, typename... types>
    requires std::is_base_of_v<bio::meta::pod_tuple<types...>, t<types...>>
struct tuple_size<t<types...>> : public std::integral_constant<std::size_t, sizeof...(types)>
{};

} // namespace std
