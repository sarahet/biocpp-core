// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::move.
 */

#pragma once

#include <ranges>

#include <bio/core.hpp>
#include <bio/meta/overloaded.hpp>

namespace bio::ranges::views
{

/*!\name General purpose views
 * \{
 */

/*!\brief               A view that turns lvalue-references into rvalue-references.
 * \tparam urng_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \returns             A range whose elements will be moved from.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/move.hpp}
 *
 * ### View properties
 *
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)                     |
 * |----------------------------------|:-------------------------------------:|:--------------------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                                        |
 * | std::ranges::forward_range       |                                       | *preserved*                                        |
 * | std::ranges::bidirectional_range |                                       | *preserved*                                        |
 * | std::ranges::random_access_range |                                       | *preserved*                                        |
 * | std::ranges::contiguous_range    |                                       | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::viewable_range      | *required*                            | *guaranteed*                                       |
 * | std::ranges::view                |                                       | *guaranteed*                                       |
 * | std::ranges::sized_range         |                                       | *preserved*                                        |
 * | std::ranges::common_range        |                                       | *preserved*                                        |
 * | std::ranges::output_range        |                                       | *lost*                                             |
 * | bio::ranges::const_iterable_range     |                                       | *preserved*                                        |
 * | std::semiregular                 |                                       | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::range_reference_t   |                                       | `t &` -> `t &&` but `t` -> `t`                     |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ### Example
 *
 * This is a slightly more verbose version of calling `std::ranges::move` on the range.
 *
 * \include test/snippet/ranges/views/move.cpp
 *
 * \hideinitializer
 */
inline auto const move = std::views::transform(
  meta::overloaded{[](auto && arg) -> std::remove_cvref_t<decltype(arg)> { return std::move(arg); },
                   [](auto & arg) -> decltype(auto) { return std::move(arg); }});
//!\}

} // namespace bio::ranges::views
