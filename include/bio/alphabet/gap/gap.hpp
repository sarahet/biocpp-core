// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Marcel Ehrhardt <marcel.ehrhardt AT fu-berlin.de>
 * \author David Heller <david.heller AT fu-berlin.de>
 * \brief Provides bio::alphabet::gap.
 */

#pragma once

#include <cassert>

#include <bio/alphabet/base.hpp>

namespace bio::alphabet
{

/*!\brief The alphabet of a gap character '-'
 * \ingroup gap
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * The alphabet always has the same value ('-').
 *
 * \include test/snippet/alphabet/gap/gap.cpp
 */

class gap : public base<gap, 1ull, char>
{
private:
    //!\brief The base class.
    using base_t = base<gap, 1ull, char>;

    //!\brief Befriend bio::alphabet::base.
    friend base_t;

    //!\brief The character that will be printed.
    static constexpr char char_value = '-';

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr gap() noexcept               = default; //!< Defaulted.
    constexpr gap(gap const &)             = default; //!< Defaulted.
    constexpr gap(gap &&)                  = default; //!< Defaulted.
    constexpr gap & operator=(gap const &) = default; //!< Defaulted.
    constexpr gap & operator=(gap &&)      = default; //!< Defaulted.
    ~gap()                                 = default; //!< Defaulted.

    using base_t::base_t;
    //!\}
};

} // namespace bio::alphabet
