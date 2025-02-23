// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Joshua Kim <joshua.kim AT fu-berlin.de>
 * \brief Provides bio::alphabet::aa20, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <bio/alphabet/aminoacid/aminoacid_base.hpp>
#include <bio/alphabet/aminoacid/concept.hpp>
#include <bio/alphabet/detail/to_lower.hpp>

namespace bio::alphabet
{

/*!\brief The canonical amino acid alphabet.
 * \ingroup aminoacid
 * \implements bio::alphabet::aminoacid_alphabet
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 * The alphabet consists of letters A, C, D, E, F, G, H, I, K, L, M, N, P, Q, R, S, T, V, W, Y
 *
 * The alphabet may be brace initialized from the static letter members (see above). Note that you cannot assign
 * regular characters, but additional functions for this are available.
 *
 * *Note:* Letters which belong in the extended alphabet will be automatically converted based on the frequency
 * of their options.\n Terminator characters are converted to W, because the most commonly occurring
 * stop codon in higher eukaryotes is UGA<sup>2</sup>.
 * Anything unknown is converted to S, because it occurs most frequently across 53 vertebrates<sup>1</sup>.
 *
 * |Input Letter  |Converts to  |
 * |--------------|-------------|
 * |B             |D<sup>1</sup>|
 * |J             |L<sup>1</sup>|
 * |O             |L<sup>1</sup>|
 * |U             |C<sup>1</sup>|
 * |Z             |E<sup>1</sup>|
 * |X (Unknown)   |S<sup>1</sup>|
 * |* (Terminator)|W<sup>2</sup>|
 * <sup><b>1</b></sup>King, J. L., & Jukes, T. H. (1969). Non-Darwinian Evolution.
 * Science, 164(3881), 788-798. doi:10.1126/science.164.3881.788\n
 * <sup><b>2</b></sup>Trotta, E. (2016). Selective forces and mutational biases drive stop codon usage
 * in the human genome: a comparison with sense codon usage.
 * BMC Genomics, 17, 366. https://doi.org/10.1186/s12864-016-2692-4
 *
 * \include test/snippet/alphabet/aminoacid/aa20_construction.cpp
 */
class aa20 : public aminoacid_base<aa20, 20>
{
private:
    //!\brief The base class.
    using base_t = aminoacid_base<aa20, 20>;

    //!\brief Befriend bio::alphabet::aminoacid_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr aa20() noexcept                         = default; //!< Defaulted.
    constexpr aa20(aa20 const &) noexcept             = default; //!< Defaulted.
    constexpr aa20(aa20 &&) noexcept                  = default; //!< Defaulted.
    constexpr aa20 & operator=(aa20 const &) noexcept = default; //!< Defaulted.
    constexpr aa20 & operator=(aa20 &&) noexcept      = default; //!< Defaulted.
    ~aa20() noexcept                                  = default; //!< Defaulted.

    using base_t::base_t;
    //!\}

protected:
    //!\brief Value to char conversion table.
    static constexpr char_type rank_to_char[alphabet_size]{
      'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y',
    };

    //!\brief Char to value conversion table.
    static constexpr std::array<rank_type, 256> char_to_rank = []() constexpr
    {
        std::array<rank_type, 256> ret{};

        // initialize with UNKNOWN (std::array::fill unfortunately not constexpr)
        for (auto & c : ret)
            c = 15; // value of 'S', because that appears most frequently

        // reverse mapping for characters and their lowercase
        for (rank_type rnk = 0u; rnk < alphabet_size; ++rnk)
        {
            ret[static_cast<rank_type>(rank_to_char[rnk])]                   = rnk;
            ret[static_cast<rank_type>(detail::to_lower(rank_to_char[rnk]))] = rnk;
        }

        ret['B'] = ret['D'];
        ret['b'] = ret['D']; // Convert b (either D/N) to D, since D occurs more frequently.
        ret['J'] = ret['L'];
        ret['j'] = ret['L']; // Convert j (either I/L) to L, since L occurs more frequently.
        ret['O'] = ret['L'];
        ret['o'] = ret['L']; // Convert Pyrrolysine to lysine.
        ret['U'] = ret['C'];
        ret['u'] = ret['C']; // Convert Selenocysteine to cysteine.
        ret['X'] = ret['S'];
        ret['x'] = ret['S']; // Convert unknown amino acids to serine.
        ret['Z'] = ret['E'];
        ret['z'] = ret['E']; // Convert z (either E/Q) to E, since E occurs more frequently.
        ret['*'] = ret['W']; // The most common stop codon is UGA. This is most similar to a Tryptophan.
        return ret;
    }
    ();
};

// ------------------------------------------------------------------
// containers
// ------------------------------------------------------------------

//!\brief Alias for an std::vector of bio::alphabet::aa20.
//!\relates aa20
using aa20_vector = std::vector<aa20>;

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

/*!\brief The bio::alphabet::aa20 char literal.
 * \param[in] c The character to assign.
 * \relates bio::alphabet::aa20
 * \returns bio::alphabet::aa20
 *
 * \include test/snippet/alphabet/aminoacid/aa20_char_literal.cpp
 *
 */
consteval aa20 operator""_aa20(char const c) noexcept
{
    return aa20{}.assign_char(c);
}

/*!\brief The bio::alphabet::aa20  string literal.
 * \param[in] s A pointer to the character string to assign.
 * \param[in] n The size of the character string to assign.
 * \relates bio::alphabet::aa20
 * \returns bio::alphabet::aa20_vector
 *
 * You can use this string literal to easily assign to aa20_vector:
 *
 * \include test/snippet/alphabet/aminoacid/aa20_literal.cpp
 *
 * \attention
 * All BioC++ literals are in the namespace bio::alphabet!
 */

inline aa20_vector operator""_aa20(char const * s, std::size_t n)
{
    aa20_vector r;
    r.resize(n);

    for (size_t i = 0; i < n; ++i)
        r[i].assign_char(s[i]);

    return r;
}
//!\}

} // namespace literals

} // namespace bio::alphabet
