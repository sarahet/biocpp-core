// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::base.
 */

#pragma once

#include <cassert>
#include <concepts>
#include <type_traits>

#include <bio/alphabet/concept.hpp>
#include <bio/meta/detail/int_types.hpp>

namespace bio::alphabet
{

/*!\brief A CRTP-base that makes defining a custom alphabet easier.
 * \ingroup alphabet
 * \tparam derived_type The CRTP parameter type.
 * \tparam size         The size of the alphabet.
 * \tparam char_t       The character type of the alphabet (set this to `void` when defining just a
 *                      bio::alphabet::semialphabet).
 *
 * \details
 *
 * You can use this class to define your own alphabet, but types are not required to be based on it to model
 * bio::alphabet::alphabet, it is purely a way to avoid code duplication.
 *
 * The base class represents the alphabet value as the rank and automatically deduces the rank type from the size and
 * defines all required member functions. The derived type needs to define only the following two tables as static
 * member variables (can be private if the base class is befriended):
 *
 *   * `static std::array<char_type, alphabet_size> constexpr rank_to_char` that defines for every possible rank value
 *     the corresponding char value.
 *   * `static std::array<rank_type, 256> constexpr char_to_rank` that defines for every possible character value the
 *     corresponding rank value (adapt size if char_type isn't `char`).
 *
 * ### Example
 *
 * This creates an alphabet called `ab` which has size two and the two letters 'A' and 'B':
 * \include test/snippet/alphabet/detail/base.cpp
 *
 */
template <typename derived_type, size_t size, typename char_t = char>
class base
{
protected:
    static_assert(size != 0, "alphabet size must be >= 1"); // == 1 is handled below in separate specialisation

    /*!\name Member types
     * \{
     */
    //!\brief The char representation; conditional needed to make semi alphabet definitions legal.
    using char_type = std::conditional_t<std::same_as<char_t, void>, char, char_t>;
    //!\brief The type of the alphabet when represented as a number (e.g. via to_rank()).
    using rank_type = meta::detail::min_viable_uint_t<size - 1>;
    //!\}

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr base() noexcept                         = default; //!< Defaulted.
    constexpr base(base const &) noexcept             = default; //!< Defaulted.
    constexpr base(base &&) noexcept                  = default; //!< Defaulted.
    constexpr base & operator=(base const &) noexcept = default; //!< Defaulted.
    constexpr base & operator=(base &&) noexcept      = default; //!< Defaulted.
    ~base() noexcept                                  = default; //!< Defaulted.
    //!\}

    /*!\name Read functions
     * \{
     */
    /*!\brief Return the letter as a character of char_type.
     *
     * \details
     *
     * Provides an implementation for bio::alphabet::to_char, required to model bio::alphabet::alphabet.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Guaranteed not to throw.
     */
    constexpr char_type to_char() const noexcept
      //!\cond
      requires(!std::same_as<char_t, void>)
    //!\endcond
    {
        return derived_type::rank_to_char[rank];
    }

    /*!\brief Return the letter's numeric value (rank in the alphabet).
     *
     * \details
     *
     * Provides an implementation for bio::alphabet::to_rank, required to model bio::alphabet::semialphabet.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Guaranteed not to throw.
     */
    constexpr rank_type to_rank() const noexcept { return rank; }
    //!\}

    /*!\name Write functions
     * \{
     */
    /*!\brief Assign from a character, implicitly converts invalid characters.
     * \param c The character to be assigned.
     *
     * \details
     *
     * Provides an implementation for bio::alphabet::assign_char_to, required to model bio::alphabet::alphabet.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Guaranteed not to throw.
     */
    constexpr derived_type & assign_char(char_type const c) noexcept
      //!\cond
      requires(!std::same_as<char_t, void>)
    //!\endcond
    {
        using index_t = std::make_unsigned_t<char_type>;
        rank          = derived_type::char_to_rank[static_cast<index_t>(c)];
        return static_cast<derived_type &>(*this);
    }

    /*!\brief Assign from a numeric value.
     * \param c The rank to be assigned.
     *
     * \details
     *
     * Provides an implementation for bio::alphabet::assign_rank_to, required to model bio::alphabet::semialphabet.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Guaranteed not to throw.
     */
    constexpr derived_type & assign_rank(rank_type const c) noexcept
    {
        assert(static_cast<size_t>(c) < static_cast<size_t>(alphabet_size));
        rank = c;
        return static_cast<derived_type &>(*this);
    }
    //!\}

    //!\brief The size of the alphabet, i.e. the number of different values it can take.
    static constexpr size_t alphabet_size = size;

    //!\name Comparison operators
    //!\{

    //!\brief Checks whether the letters `lhs` and `rhs` are equal.
    friend constexpr bool operator==(derived_type const lhs, derived_type const rhs)
    {
        return lhs.to_rank() == rhs.to_rank();
    }

    //!\brief Checks order of `lhs` and `rhs`.
    friend constexpr auto operator<=>(derived_type const lhs, derived_type const rhs)
    {
        return lhs.to_rank() <=> rhs.to_rank();
    }
    //!\}

private:
    //!\brief The value of the alphabet letter is stored as the rank.
    rank_type rank;

    //!\brief tag_invoke() wrapper around member.
    friend constexpr auto tag_invoke(cpo::to_rank, derived_type const a) noexcept { return a.to_rank(); }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type & tag_invoke(cpo::assign_rank_to, rank_type const r, derived_type & a) noexcept
      requires(requires { {a.assign_rank(r)}; })
    {
        return a.assign_rank(r);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr auto tag_invoke(cpo::to_char, derived_type const a) noexcept requires(requires { {a.to_char()}; })
    {
        return a.to_char();
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type & tag_invoke(cpo::assign_char_to, char_type const c, derived_type & a) noexcept
      requires(requires { {a.assign_char(c)}; })
    {
        return a.assign_char(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr bool tag_invoke(cpo::char_is_valid_for, char_type const c, derived_type) noexcept
      requires(requires { {derived_type::char_is_valid(c)}; })
    {
        return derived_type::char_is_valid(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr bool tag_invoke(cpo::char_is_valid_for,
                                     char_type const c,
                                     std::type_identity<derived_type>) noexcept
      //!\cond REQ
      requires(requires { {derived_type::char_is_valid(c)}; } && !meta::constexpr_default_initializable<derived_type>)
    //!\endcond
    {
        return derived_type::char_is_valid(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend consteval auto tag_invoke(cpo::size,
                                     derived_type) noexcept requires meta::constexpr_default_initializable<derived_type>
    {
        return size;
    }

    //!\brief tag_invoke() wrapper around member.
    friend consteval auto tag_invoke(cpo::size, std::type_identity<derived_type>) noexcept
      requires(!meta::constexpr_default_initializable<derived_type>)
    {
        return size;
    }
};

/*!\brief Specialisation of bio::alphabet::base for alphabets of size 1.
 * \ingroup alphabet
 * \tparam derived_type The CRTP parameter type.
 * \tparam char_t The character type (always set to `char` for alphabets of size 1 and to `void` for semi alphabets of
 *                size 1).
 *
 * \details
 *
 * This specialisation holds no member variable and many functions are NO-OPs because if the alphabet has only
 * one valid value there is no state that can be changed.
 */
template <typename derived_type, typename char_t>
class base<derived_type, 1ul, char_t>
{
protected:
    /*!\name Member types
     * \{
     */
    //!\copybrief bio::alphabet::base::char_type
    using char_type = std::conditional_t<std::same_as<char_t, void>, char, char_t>;
    //!\copybrief bio::alphabet::base::rank_type
    using rank_type = uint8_t;
    //!\}

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr base() noexcept                         = default; //!< Defaulted.
    constexpr base(base const &) noexcept             = default; //!< Defaulted.
    constexpr base(base &&) noexcept                  = default; //!< Defaulted.
    constexpr base & operator=(base const &) noexcept = default; //!< Defaulted.
    constexpr base & operator=(base &&) noexcept      = default; //!< Defaulted.
    ~base() noexcept                                  = default; //!< Defaulted.
    //!\}

    /*!\name Read functions
     * \{
     */
    //!\copybrief bio::alphabet::base::to_char
    constexpr char_type to_char() const noexcept
      //!\cond
      requires(!std::same_as<char_t, void>)
    //!\endcond
    {
        return derived_type::char_value;
    }

    //!\copybrief bio::alphabet::base::to_rank
    constexpr rank_type to_rank() const noexcept { return 0; }
    //!\}

    /*!\name Write functions
     * \{
     */
    //!\copybrief bio::alphabet::base::assign_char
    constexpr derived_type & assign_char(char_type const) noexcept
      //!\cond
      requires(!std::same_as<char_t, void>)
    //!\endcond
    {
        return static_cast<derived_type &>(*this);
    }

    //!\copybrief bio::alphabet::base::assign_rank
    constexpr derived_type & assign_rank(rank_type const) noexcept { return static_cast<derived_type &>(*this); }
    //!\}

    //!\brief The size of the alphabet, i.e. the number of different values it can take.
    static constexpr size_t alphabet_size = 1;

    //!\name Comparison operators
    //!\{

    //!\brief Letters are always equal.
    friend constexpr bool operator==(derived_type const, derived_type const) noexcept { return true; }

    //!\brief One letter cannot be smaller than another.
    friend constexpr bool operator<(derived_type const, derived_type const) noexcept { return false; }

    //!\brief One letter cannot be bigger than another.
    friend constexpr bool operator>(derived_type const, derived_type const) noexcept { return false; }

    //!\brief Letters are always equal.
    friend constexpr bool operator<=(derived_type const, derived_type const) noexcept { return true; }

    //!\brief Letters are always equal.
    friend constexpr bool operator>=(derived_type const, derived_type const) noexcept { return true; }
    //!\}
private:
    //!\brief tag_invoke() wrapper around member.
    friend constexpr rank_type tag_invoke(cpo::to_rank, derived_type) noexcept { return 0; }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type & tag_invoke(cpo::assign_rank_to, rank_type const, derived_type & a) noexcept
    {
        return a;
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr auto tag_invoke(cpo::to_char, derived_type const a) noexcept requires(requires { {a.to_char()}; })
    {
        return a.to_char();
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type & tag_invoke(cpo::assign_char_to, char_type const c, derived_type & a) noexcept
      requires(requires { {a.assign_char(c)}; })
    {
        return a;
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr bool tag_invoke(cpo::char_is_valid_for, char_type const c, derived_type) noexcept
      requires(requires { {derived_type::char_is_valid(c)}; } && std::is_nothrow_default_constructible_v<derived_type>)
    {
        return derived_type::char_is_valid(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr bool tag_invoke(cpo::char_is_valid_for,
                                     char_type const c,
                                     std::type_identity<derived_type>) noexcept
      requires(requires { {derived_type::char_is_valid(c)}; } && !std::is_nothrow_default_constructible_v<derived_type>)
    {
        return derived_type::char_is_valid(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend consteval auto tag_invoke(cpo::size,
                                     derived_type) noexcept requires meta::constexpr_default_initializable<derived_type>
    {
        return alphabet_size;
    }

    //!\brief tag_invoke() wrapper around member.
    friend consteval auto tag_invoke(cpo::size, std::type_identity<derived_type>) noexcept
      requires(!meta::constexpr_default_initializable<derived_type>)
    {
        return alphabet_size;
    }
};

} // namespace bio::alphabet
