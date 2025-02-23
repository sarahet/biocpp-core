// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides quality alphabet composites.
 */

#pragma once

#include <bio/alphabet/composite/tuple_base.hpp>
#include <bio/alphabet/nucleotide/concept.hpp>
#include <bio/alphabet/quality/concept.hpp>

namespace bio::alphabet
{

/*!\brief Joins an arbitrary alphabet with a quality alphabet.
 * \ingroup quality
 * \tparam sequence_alphabet_t Type of the alphabet; must satisfy bio::alphabet::writable_alphabet.
 * \tparam quality_alphabet_t  Type of the quality; must satisfy bio::alphabet::writable_quality_alphabet.
 * \implements bio::alphabet::writable_quality_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 *
 * This composite pairs an arbitrary alphabet with a quality alphabet, where
 * each alphabet character is stored together with its quality score in a
 * single value. That way, you can can conveniently access the character and
 * score information at each position of the qualified-sequence.
 * The use case that this was designed for is a nucleotide sequence with
 * corresponding quality scores, e.g. obtained when reading in a FASTQ file
 * of Illumina reads.
 * The composite also allows to store quality scores for different or extended
 * alphabets like a `qualified<char, phred42>` or a `qualified<gapped<dna4>, phred42>`
 * sequence.
 * The rank values correspond to numeric values in the size of the composite,
 * while the character values are taken from the sequence alphabet and the phred
 * values are taken from the quality alphabet.
 *
 * As with all `bio::alphabet::tuple_base` s you may access the individual
 * alphabet letters in regular c++ tuple notation, i.e. `get<0>(t)` and objects
 * can be brace-initialised with the individual members.
 *
 * \include test/snippet/alphabet/quality/qualified.cpp
 *
 * This bio::alphabet::tuple_base itself fulfils both bio::alphabet::writable_alphabet and bio::alphabet::writable_quality_alphabet.
 */
template <writable_alphabet sequence_alphabet_t, writable_quality_alphabet quality_alphabet_t>
class qualified :
  public tuple_base<qualified<sequence_alphabet_t, quality_alphabet_t>, sequence_alphabet_t, quality_alphabet_t>
{
private:
    //!\brief The base type.
    using base_type =
      tuple_base<qualified<sequence_alphabet_t, quality_alphabet_t>, sequence_alphabet_t, quality_alphabet_t>;

public:
    //!\brief First template parameter as member type.
    using sequence_alphabet_type = sequence_alphabet_t;
    //!\brief Second template parameter as member type.
    using quality_alphabet_type  = quality_alphabet_t;

    //!\brief Equals the char_type of sequence_alphabet_type.
    using char_type  = char_t<sequence_alphabet_type>;
    //!\brief Equals the phred_type of the quality_alphabet_type.
    using phred_type = phred_t<quality_alphabet_type>;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr qualified() noexcept                              = default; //!< Defaulted.
    constexpr qualified(qualified const &) noexcept             = default; //!< Defaulted.
    constexpr qualified(qualified &&) noexcept                  = default; //!< Defaulted.
    constexpr qualified & operator=(qualified const &) noexcept = default; //!< Defaulted.
    constexpr qualified & operator=(qualified &&) noexcept      = default; //!< Defaulted.
    ~qualified() noexcept                                       = default; //!< Defaulted.

    // Inherit from base:
    using base_type::alphabet_size;
    using base_type::base_type; // non-default constructors
    using base_type::to_rank;
    using base_type::operator=;

    //!\copydoc tuple_base::tuple_base(component_type const alph)
    BIOCPP_DOXYGEN_ONLY((constexpr qualified(component_type const alph) noexcept {}))
    //!\copydoc tuple_base::tuple_base(indirect_component_type const alph)
    BIOCPP_DOXYGEN_ONLY((constexpr qualified(indirect_component_type const alph) noexcept {}))
    //!\copydoc tuple_base::operator=(component_type const alph)
    BIOCPP_DOXYGEN_ONLY((constexpr qualified & operator=(component_type const alph) noexcept {}))
    //!\copydoc tuple_base::operator=(indirect_component_type const alph)
    BIOCPP_DOXYGEN_ONLY((constexpr qualified & operator=(indirect_component_type const alph) noexcept {}))
    //!\}

    /*!\name Write functions
     * \{
     */
    //!\brief Assign from a character. This modifies the internal sequence letter.
    constexpr qualified & assign_char(char_type const c) noexcept
    {
        base_type::assign_rank((bio::alphabet::to_rank(bio::alphabet::assign_char_to(c, sequence_alphabet_type{})) *
                                base_type::cummulative_alph_sizes[0]) +
                               (base_type::template to_component_rank<1>() * base_type::cummulative_alph_sizes[1]));

        // The above is noticeably faster than (no subtraction and no division):
        // base_type::template assign_component_rank<0>(
        //     bio::alphabet::to_rank(bio::alphabet::assign_char_to(c, sequence_alphabet_type{})));
        return *this;
    }

    //!\brief Assign from a phred value. This modifies the internal quality letter.
    constexpr qualified & assign_phred(phred_type const c) noexcept
    {
        bio::alphabet::assign_phred_to(c, get<1>(*this));
        return *this;
    }
    //!\}

    /*!\name Read functions
     * \{
     */
    //!\brief Return the phred value. This reads the internal quality letter.
    constexpr phred_type to_phred() const noexcept { return rank_to_phred[to_rank()]; }

    //!\brief Return a character. This reads the internal sequence letter.
    constexpr char_type to_char() const noexcept { return rank_to_char[to_rank()]; }

    /*!\brief Return a qualified where the quality is preserved, but the sequence letter is complemented.
     * \sa bio::alphabet::complement
     * \sa bio::alphabet::nucleotide_alphabet::complement
     */
    constexpr qualified complement() const noexcept requires nucleotide_alphabet<sequence_alphabet_t>
    {
        return qualified{bio::alphabet::complement(get<0>(*this)), get<1>(*this)};
    }
    //!\}

    //!\brief Validate whether a character is valid in the sequence alphabet.
    static constexpr bool char_is_valid(char_type const c) noexcept
    {
        return char_is_valid_for<sequence_alphabet_type>(c);
    }

protected:
    //!\privatesection

    //!\brief Rank to char conversion table.
    static constexpr std::array<char_type, alphabet_size> rank_to_char = []() constexpr
    {
        std::array<char_type, alphabet_size> ret{};

        for (size_t i = 0; i < alphabet_size; ++i)
        {
            size_t seq_rank = (i / base_type::cummulative_alph_sizes[0]) % bio::alphabet::size<quality_alphabet_type>;

            ret[i] = bio::alphabet::to_char(bio::alphabet::assign_rank_to(seq_rank, sequence_alphabet_type{}));
        }

        return ret;
    }
    ();

    //!\brief Rank to phred conversion table.
    static constexpr std::array<char_type, alphabet_size> rank_to_phred = []() constexpr
    {
        std::array<char_type, alphabet_size> ret{};

        for (size_t i = 0; i < alphabet_size; ++i)
        {
            size_t qual_rank = (i / base_type::cummulative_alph_sizes[1]) % bio::alphabet::size<quality_alphabet_type>;

            ret[i] = bio::alphabet::to_phred(bio::alphabet::assign_rank_to(qual_rank, quality_alphabet_type{}));
        }

        return ret;
    }
    ();
};

//!\brief Type deduction guide enables usage of qualified without specifying template args.
//!\relates qualified
template <typename sequence_alphabet_type, typename quality_alphabet_type>
qualified(sequence_alphabet_type &&, quality_alphabet_type &&)
  -> qualified<std::decay_t<sequence_alphabet_type>, std::decay_t<quality_alphabet_type>>;

} // namespace bio::alphabet
