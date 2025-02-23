// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Meta-header for the alphabet module.
 *
 * \defgroup alphabet Alphabet
 *
 * # Introduction
 *
 * Alphabets are a core component in BioC++. They enable us to represent the smallest
 * unit of biological sequence data, e.g. a nucleotide or an amino acid.
 *
 * In theory, these could just be represented as a `char` and this is how many people
 * perceive them, but it makes sense to use a smaller, stricter and well-defined alphabet
 * in almost all cases, because:
 *
 *   * Most of our sequences alphabets are actually smaller and can possibly be **represented by less bits**
 * than a `char`, e.g. a `char` can have 256 values and thus must be represented by 8 bits of memory, but a DNA
 * character *could* be represented by 2 bits, because it only has four values in the smallest representation
 * ('A', 'C', 'G', 'T').
 *   * From a programmer's point of view it is very important to also access the **rank of a letter**, i.e. we need to
 * be able to convert 'A', 'C', 'G', 'T' to `0`, `1`, `2`, `3` respectively. In fact the rank representation is used a
 * lot more often than the visual representation which is only used in input/output.
 *   * You may want to prevent the user from selecting **letters that aren't part of that alphabet**, e.g. 'J' is
 * not part of any nucleotide definition so people shouldn't be able to select 'J'. With a specialised
 * alphabet you could instead convert those to a special "unknown"-character, e.g. you might want 'J' to be
 * always converted to 'N'.
 *
 * In BioC++ there are alphabet types for typical sequence alphabets like DNA and amino acid, but also
 * for qualities, RNA structures and alignment gaps. In addition there are templates for combining alphabet
 * types into new alphabets, and wrappers for existing data types like the canonical `char`.
 *
 * In addition to concrete alphabet types, BioC++ provides multiple *concepts* that describe groups of alphabets
 * by their properties and can be used to *constrain* templates so that they only work with certain alphabet types.
 * See the \link core_concepts Tutorial on Concepts \endlink for a gentle introduction to the topic.
 *
 * # The alphabet concepts
 *
 * ### alphabet size
 *
 * All alphabets in BioC++ have a fixed size. It
 * can be queried via the bio::alphabet::size type trait and *optionally* also the `alphabet_size` static
 * member of the alphabet (see below for "members VS free/global functions").
 *
 * In some areas we provide alphabets types with different sizes for the same purpose, e.g. bio::alphabet::dna4
 * ('A', 'C', 'G', 'T'), bio::alphabet::dna5 (plus 'N') and bio::alphabet::dna15 (plus ambiguous characters defined by
 * IUPAC). By convention most of our alphabets carry their size in their name (bio::alphabet::dna4 has size 4 a.s.o.).
 *
 * A main reason for choosing a smaller alphabet over a bigger one is the possibility of **optimising for
 * space efficiency**. Note, however, that a single letter by itself can never be smaller than a byte for
 * architectural reasons. Actual space improvements are realised via secondary structures, e.g. when
 * using a `bio::ranges::bitcompressed_vector<bio::alphabet::dna4>` instead of `std::vector<bio::alphabet::dna4>`. Also
 * the single letter quality composite `bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>` fits into one byte, because
 * the product of the alphabet sizes (4 * 42) is smaller than 256; whereas the same composite
 * with bio::alphabet::dna15 requires two bytes per letter (15 * 42 > 256).
 *
 * ### Assigning and retrieving values
 *
 * As mentioned above, we typically think of alphabets in their character representation, but we also
 * require them in "rank representation" as programmers. In C and C++ it is quite difficult to
 * cleanly differentiate between these, because the `char` type is considered an integral type and can
 * be used to index an array (e.g. <tt>my_array['A']</tt> translates to `my_array[65]`). Moreover the sign of `char` is
 * implementation defined and on many platforms the smallest integer types `int8_t` and `uint8_t` are literally the
 * same types as `signed char` and `unsigned char` respectively.
 *
 * This leads to ambiguity when assigning and retrieving values:
 *
 * \include test/snippet/alphabet/all_ambiguous.cpp
 *
 * To solve this problem, alphabets in BioC++ define two interfaces:
 *
 *   1. a **rank based interface** with
 *     * bio::alphabet::to_rank() to produce the numerical representation;
 *     * bio::alphabet::assign_rank_to() to assign from the numerical representation;
 *     * the numerical representation is an unsigned integral type like `size_t`; the exact type can be retrieved via
 *       the bio::alphabet::rank_t.
 *   2. a **character based interface** with
 *     * bio::alphabet::to_char() to produce the visual representation;
 *     * bio::alphabet::assign_char_to() to assign from the visual representation;
 *     * bio::alphabet::char_is_valid_for() that checks whether a character value has a one-to-one mapping to an alphabet value;
 *     * the visual representation is a character type (almost always `char`, but could be `char16_t` or `char32_t`,
 *       as well); the exact type can be retrieved via bio::alphabet::char_t.
 *
 * To prevent the aforementioned ambiguity, you can neither assign from rank or char representation via `operator=`,
 * nor can you cast the alphabet to either of it's representation forms, **you need to explicitly use the
 * interfaces**.
 *
 * For efficiency, the representation saved internally is normally the rank representation, and the character
 * representation
 * is generated via conversion tables. This is, however, not required as long as both interfaces are provided
 * and all functions operate in constant time.
 *
 * The same applies for printing characters although we provide overloads for the {fmt}-library in
 * `<bio/alphabet/fmt.hpp>`.
 *
 * Here is an example of explicit assignment of a rank and char, and how it can be printed via std::cout and
 * {fmt}:
 * \include test/snippet/alphabet/all_nonambiguous.cpp
 *
 * To reduce the burden of calling `assign_char` often, most alphabets in BioC++ provide custom literals for
 * the alphabet and sequences over the alphabet:
 *
 * \include test/snippet/alphabet/all_literal.cpp
 *
 * Note, however, that literals **are not** required by the concept.
 *
 * ### Different concepts
 *
 * All types that have valid implementations of the functions/functors described above model the concept
 * bio::alphabet::writable_alphabet. This is the strongest (i.e. most refined) *general case* concept.
 * There are more refined concepts for specific biological applications (like bio::alphabet::nucleotide_alphabet), and there are
 * less refined concepts that only model part of an alphabet:
 *
 *   * bio::alphabet::semialphabet and derived concepts only require the rank interface;
 *   * bio::alphabet::alphabet (without `writable*`) and derived concepts only require readability and not assignability.
 *
 * Typically you will use bio::alphabet::alphabet in "read-only" situations (e.g. `const` parameters) and
 * bio::alphabet::writable_alphabet whenever the values might be changed.
 * Semi-alphabets are less useful in application code.
 *
 * |                                | [semialphabet](\ref bio::alphabet::semialphabet) | [writable_semialphabet](\ref bio::alphabet::writable_semialphabet) | [alphabet](\ref bio::alphabet::alphabet) | [writable_alphabet](\ref bio::alphabet::writable_alphabet) | Aux |
 * |-------------------------------:|:------------------------------------:|:------------------------------------------------------:|:----------------------------:|:----------------------------------------------:|:---:|
 * | bio::alphabet::size             | ✅                                    | ✅                                                      | ✅                            | ✅                                               |     |
 * | bio::alphabet::to_rank()                 | ✅                                    | ✅                                                      | ✅                            | ✅                                               |     |
 * | bio::alphabet::rank_t           | ✅                                    | ✅                                                      | ✅                            | ✅                                               |  🔗  |
 * | bio::alphabet::assign_rank_to()          |                                      | ✅                                                      |                              | ✅                                               |     |
 * | bio::alphabet::to_char()                 |                                      |                                                        | ✅                            | ✅                                               |     |
 * | bio::alphabet::char_t           |                                      |                                                        | ✅                            | ✅                                               |  🔗  |
 * | bio::alphabet::assign_char_to()          |                                      |                                                        |                              | ✅                                               |     |
 * | bio::alphabet::char_is_valid_for()       |                                      |                                                        |                              | ✅                                               |     |
 * | bio::alphabet::assign_char_strictly_to() |                                      |                                                        |                              | ✅                                               |  🔗  |
 *
 * The above table shows all alphabet concepts and related functions and type traits.
 * The entities marked as "auxiliary" provide shortcuts to the other "essential" entities.
 * This difference is only relevant if you want to create your own alphabet (you do not need to provide an
 * implementation for the "auxiliary" entities, they are provided automatically).
 *
 * ### Members VS free/global functions
 *
 * The alphabet concept (as most concepts in BioC++) looks for free/global functions, i.e. you need to be able
 * to call `bio::alphabet::to_rank(my_letter)`, however *most* alphabets also provide a member function, i.e.
 * `my_letter.to_rank()`. The same is true for the type trait bio::alphabet::size vs the static data member
 * `alphabet_size`.
 *
 * Members are provided for convenience and if you are an application developer who works with a single concrete
 * alphabet type you are fine with using the member functions. If you, however, implement a generic function
 * that accepts different alphabet types, you need to use the free function / type trait interface, because
 * it is the only interface guaranteed to exist (member functions are **not** required/enforced by the concept).
 *
 * # Containers over alphabets
 *
 * In BioC++ it is recommended you use the STL container classes like std::vector for storing sequence data,
 * but you can use other class templates if they satisfy the respective bio::ranges::detail::container, e.g. `std::deque` or
 * <a href="https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md" target="_blank">
 * <tt>folly::fbvector</tt></a> or even <a href="https://doc.qt.io/qt-5/qvector.html" target="_blank">
 * <tt>Qt::QVector</tt></a>.
 *
 * `std::basic_string` is also supported, however, we recommend against using it,
 * because it is not safe (and not useful) to call certain members like `.c_str()` if our alphabets are used as
 * value type.
 *
 * We provide specialised containers with certain properties in the \ref range module.
 *
 * A container over an bio::alphabet::alphabet automatically models the bio::alphabet::sequence concept.
 */

#pragma once

#include <bio/alphabet/adaptation/all.hpp>
#include <bio/alphabet/aminoacid/all.hpp>
#include <bio/alphabet/composite/all.hpp>
#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/gap/all.hpp>
#include <bio/alphabet/mask/all.hpp>
#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/alphabet/quality/all.hpp>
// #include <bio/alphabet/fmt.hpp>
