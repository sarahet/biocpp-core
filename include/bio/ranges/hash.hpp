// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Enrico Seiler <enrico.seiler AT fu-berlin.de>
 * \brief Provides overloads for std::hash.
 */

#pragma once

#include <ranges>

#include <bio/alphabet/hash.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/type_traits.hpp>

namespace std
{
/*!\brief Struct for hashing a range of characters.
 * \ingroup range
 * \tparam urng_t The type of the range; Must model std::ranges::input_range and the reference type of the range of the
                  range must model bio::alphabet::semialphabet.
 */
template <ranges::input_range urng_t>
    //!\cond
    requires bio::alphabet::semialphabet<std::ranges::range_reference_t<urng_t>>
//!\endcond
struct hash<urng_t>
{
    /*!\brief Compute the hash for a range of characters.
     * \tparam urng2_t  The same as `urng_t` (+- cvref); used to get forwarding reference in the interface.
     * \param[in] range The input range to process. Must model std::ranges::input_range and the reference type of the
                        range of the range must model bio::alphabet::semialphabet.
     * \returns size_t.
     */
    template <ranges::input_range urng2_t>
        //!\cond
    requires bio::alphabet::semialphabet<std::ranges::range_reference_t<urng2_t>>
      //!\endcond
      size_t operator()(urng2_t && range) const noexcept
    {
        using alphabet_t = std::ranges::range_reference_t<urng_t>;
        size_t           result{0};
        hash<alphabet_t> h{};
        for (alphabet_t character : range)
        {
            result *= bio::alphabet::size<alphabet_t>;
            result += h(character);
        }
        return result;
    }
};

} // namespace std
