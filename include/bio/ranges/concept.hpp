// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Additional non-standard concepts for ranges.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <ranges>

#include <bio/alphabet/concept.hpp>

namespace bio::ranges
{

/*!\interface bio::ranges::const_iterable_range <>
 * \ingroup range
 * \extends std::input_range
 * \brief Specifies requirements of an input range type for which the `const` version of that type satisfies the
 * same strength input range concept as the non-const version.
 *
 * \details
 *
 * For a type `t` it usually holds that if `t` is a range, `t const` is also a range with similar properties, but
 * there are cases where this does not hold:
 *
 *   * a `const` range is usually not writable so std::ranges::output_range is lost; pure output ranges
 * (those that are not also input ranges) are therefore not `const`-iterable;
 *   * single-pass input ranges, like BioC++ readers, are not `const`-iterable, because "single-pass-ness" implies that
 * there is something in the range that changes on every iterator increment (and `const` ranges can't change);
 *   * certain views store a state with their algorithm that also changes when `begin()` is called or an
 * iterator is incremented; these may be not be `const`-iterable, because the standard library
 * (and also BioC++) guarantees that it is safe to call `const`-qualified functions concurrently.
 */
//!\cond
template <typename type>
concept const_iterable_range = std::ranges::input_range<std::remove_const_t<type>> &&
  std::ranges::input_range<type const> &&
  (std::ranges::forward_range<std::remove_const_t<type>> == std::ranges::forward_range<type const>)&&(
    std::ranges::bidirectional_range<std::remove_const_t<type>> ==
    std::ranges::bidirectional_range<type const>)&&(std::ranges::random_access_range<std::remove_const_t<type>> ==
                                                    std::ranges::random_access_range<type const>);
//!\endcond

/*!\interface bio::ranges::back_insertable_with <>
 * \ingroup range
 * \extends std::ranges::output_range
 * \tparam rng_t The container type.
 * \tparam val_t The type to append to the container.
 * \brief Describes range types that can grow in amortised constant time by appending an element of type val_t.
 */
//!\cond
template <typename rng_t, typename val_t>
concept back_insertable_with = std::ranges::output_range<rng_t, val_t> && requires(rng_t & v)
{
    v.push_back(std::declval<val_t>());
};
//!\endcond

/*!\interface bio::ranges::back_insertable <>
 * \ingroup range
 * \extends std::ranges::output_range
 * \extends std::ranges::input_range
 * \tparam rng_t The container type.
 * \brief Describes range types that can grow in amortised constant time by appending an element.
 */
//!\cond
template <typename rng_t>
concept back_insertable =
  std::ranges::input_range<rng_t> && back_insertable_with<rng_t, std::ranges::range_reference_t<rng_t>>;
//!\endcond

} // namespace bio::ranges
