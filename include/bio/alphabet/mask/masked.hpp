// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Joshua Kim <joshua.kim AT fu-berlin.de>
 * \brief Extends a given alphabet with the mask alphabet.
 */

#pragma once

#include <bio/alphabet/composite/tuple_base.hpp>
#include <bio/alphabet/detail/to_lower.hpp>
#include <bio/alphabet/mask/mask.hpp>

namespace bio::alphabet
{
/*!\brief Implementation of a masked composite, which extends a given alphabet
 * with a mask.
 * \ingroup mask
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \tparam sequence_alphabet_t Type of the first letter; must satisfy bio::alphabet::writable_alphabet and std::regular.
 *
 * \details
 * The masked composite represents a bio::alphabet::tuple_base of any given alphabet with the
 * masked alphabet. It allows one to specify which portions of a sequence should be masked,
 * without losing additional information by replacing the sequence directly.
 *
 * \include test/snippet/alphabet/mask/masked.cpp
 */
template <writable_alphabet sequence_alphabet_t>
    //!\cond
    requires std::regular<sequence_alphabet_t>
//!\endcond
class masked : public tuple_base<masked<sequence_alphabet_t>, sequence_alphabet_t, mask>
{
private:
    //!\brief The base type.
    using base_t = tuple_base<masked<sequence_alphabet_t>, sequence_alphabet_t, mask>;

public:
    //!\brief First template parameter as member type.
    using sequence_alphabet_type = sequence_alphabet_t;

    //!\brief Equals the char_type of sequence_alphabet_type.
    using char_type = char_t<sequence_alphabet_type>;

    using base_t::alphabet_size;
    using typename base_t::rank_type;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr masked()                           = default; //!< Defaulted.
    constexpr masked(masked const &)             = default; //!< Defaulted.
    constexpr masked(masked &&)                  = default; //!< Defaulted.
    constexpr masked & operator=(masked const &) = default; //!< Defaulted.
    constexpr masked & operator=(masked &&)      = default; //!< Defaulted.
    ~masked()                                    = default; //!< Defaulted.

    using base_t::base_t; // Inherit non-default constructors
    //!\}

    // Inherit operators from base
    using base_t::operator=;

    /*!\name Write functions
     * \{
     */
    //!\brief Assign from a character.
    constexpr masked & assign_char(char_type const c) noexcept
    {
        using index_t = std::make_unsigned_t<char_type>;
        base_t::assign_rank(char_to_rank[static_cast<index_t>(c)]);
        return *this;
    }
    //!\}

    /*!\name Read functions
     * \{
     */
    //!\brief Return a character.
    constexpr char_type to_char() const noexcept { return rank_to_char[base_t::to_rank()]; }
    //!\}

protected:
    //!\brief Rank to char conversion table.
    static constexpr std::array<char_type, alphabet_size> rank_to_char = []()
    {
        std::array<char_type, alphabet_size> ret{};

        for (size_t i = 0; i < alphabet_size / 2; ++i)
        {
            ret[i]                     = alphabet::to_char(assign_rank_to(i, sequence_alphabet_type{}));
            ret[i + alphabet_size / 2] = detail::to_lower(ret[i]);
        }

        return ret;
    }();

    //!\brief Char to rank conversion table.
    static constexpr std::array<rank_type, meta::detail::size_in_values_v<char_type>> char_to_rank = []()
    {
        std::array<rank_type, meta::detail::size_in_values_v<char_type>> ret{};

        for (size_t i = 0; i < meta::detail::size_in_values_v<char_type>; ++i)
        {
            char_type c = static_cast<char_type>(i);

            ret[i] =
              detail::is_lower(c)
                ? alphabet::to_rank(assign_char_to(detail::to_upper(c), sequence_alphabet_type{})) + alphabet_size / 2
                : alphabet::to_rank(assign_char_to(c, sequence_alphabet_type{}));
        }

        return ret;
    }();
};

//!\brief Type deduction guide enables usage of masked without specifying template args.
//!\relates masked
template <typename sequence_alphabet_type>
masked(sequence_alphabet_type &&, mask const &) -> masked<std::decay_t<sequence_alphabet_type>>;
} //namespace bio::alphabet
