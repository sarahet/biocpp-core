// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::rna4, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/nucleotide_base.hpp>

// ------------------------------------------------------------------
// rna4
// ------------------------------------------------------------------

namespace bio::alphabet
{

/*!\brief The four letter RNA alphabet of A,C,G,U.
 * \ingroup nucleotide
 * \implements bio::alphabet::nucleotide_alphabet
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 * This alphabet has the same internal representation as bio::alphabet::dna4, the only difference is that it prints 'U' on
 * character conversion instead of 'T'. You can assign between values of bio::alphabet::dna4 and bio::alphabet::rna4.
 *
 * Like most alphabets, this alphabet cannot be initialised directly from its character representation.
 * Instead initialise/assign from the character literal or use the
 * function bio::alphabet::rna4::assign_char().
 *
 *\include test/snippet/alphabet/nucleotide/rna4.cpp
 */
class rna4 : public nucleotide_base<rna4, 4>
{
private:
    //!\brief The base class.
    using base_t = nucleotide_base<rna4, 4>;

    //!\brief Befriend bio::alphabet::nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr rna4() noexcept                         = default; //!< Defaulted.
    constexpr rna4(rna4 const &) noexcept             = default; //!< Defaulted.
    constexpr rna4(rna4 &&) noexcept                  = default; //!< Defaulted.
    constexpr rna4 & operator=(rna4 const &) noexcept = default; //!< Defaulted.
    constexpr rna4 & operator=(rna4 &&) noexcept      = default; //!< Defaulted.
    ~rna4() noexcept                                  = default; //!< Defaulted.

    using base_t::base_t;

    //!\brief Allow implicit construction from dna/rna of the same size.
    constexpr rna4(dna4 const & r) noexcept
#if BIOCPP_WORKAROUND_GCC_90897
      requires true
#endif
    {
        assign_rank(r.to_rank());
    }
    //!\}

    /*!\brief Convert to complement. Faster implementation than the default-generated.
     * \private
     */
    constexpr rna4 complement() const noexcept
    {
        return rna4{}.assign_rank(to_rank() ^ 0b11);
    }

protected:
    //!\privatesection

    //!\copydoc bio::alphabet::dna4::rank_to_char
    static constexpr char_type rank_to_char[alphabet_size]{'A', 'C', 'G', 'U'};

    //!\copydoc bio::alphabet::dna4::char_to_rank
    static constexpr std::array<rank_type, 256> char_to_rank = dna4::char_to_rank;
};

// ------------------------------------------------------------------
// containers
// ------------------------------------------------------------------

//!\brief Alias for an std::vector of bio::alphabet::rna4.
//!\relates rna4
using rna4_vector = std::vector<rna4>;

} // namespace bio::alphabet

// ------------------------------------------------------------------
// literals
// ------------------------------------------------------------------

namespace bio::alphabet
{

inline namespace literals
{

/*!\name Literals
 * \{
 */

/*!\brief The bio::alphabet::rna4 char literal.
 * \relates bio::alphabet::rna4
 * \returns bio::alphabet::rna4
 */
consteval rna4 operator""_rna4(char const c) noexcept
{
    return rna4{}.assign_char(c);
}

/*!\brief The bio::alphabet::rna4 string literal.
 * \relates bio::alphabet::rna4
 * \returns bio::alphabet::rna4_vector
 *
 * You can use this string literal to easily assign to rna4_vector:
 *
 * \include test/snippet/alphabet/nucleotide/rna4_literal.cpp
 *
 */
inline rna4_vector operator""_rna4(char const * s, std::size_t n)
{
    rna4_vector r;
    r.resize(n);

    for (size_t i = 0; i < n; ++i)
        r[i].assign_char(s[i]);

    return r;
}
//!\}

} // namespace literals

} // namespace bio::alphabet
