// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/nucleotide/rna15.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/nucleotide/rna5.hpp>
#include <bio/meta/type_list/function.hpp>
#include <bio/meta/type_list/type_list.hpp>

template <typename T>
using nucleotide_conversion = ::testing::Test;

using nucleotide_types       = bio::meta::type_list<bio::alphabet::dna4,
                                              bio::alphabet::dna5,
                                              bio::alphabet::dna15,
                                              bio::alphabet::rna4,
                                              bio::alphabet::rna5,
                                              bio::alphabet::rna15>; // needed for some tests
using nucleotide_gtest_types = bio::meta::transfer_template_args_onto_t<nucleotide_types, ::testing::Types>;

TYPED_TEST_SUITE(nucleotide_conversion, nucleotide_gtest_types, );

// conversion to any other nucleotide type
TYPED_TEST(nucleotide_conversion, explicit_conversion)
{
    bio::meta::detail::for_each<nucleotide_types>([&](auto nucl) constexpr {
        using out_type = std::decay_t<typename decltype(nucl)::type>;
        EXPECT_EQ(static_cast<out_type>(TypeParam{}.assign_char('A')), out_type{}.assign_char('A'));
        EXPECT_EQ(static_cast<out_type>(TypeParam{}.assign_char('C')), out_type{}.assign_char('C'));
        EXPECT_EQ(static_cast<out_type>(TypeParam{}.assign_char('G')), out_type{}.assign_char('G'));
        EXPECT_EQ(static_cast<out_type>(TypeParam{}.assign_char('T')), out_type{}.assign_char('T'));
        EXPECT_EQ(static_cast<out_type>(TypeParam{}.assign_char('U')), out_type{}.assign_char('U'));
        EXPECT_EQ(static_cast<out_type>(TypeParam{}.assign_char('T')), out_type{}.assign_char('U'));
    });
}

// conversion to rna/dna of same size
TYPED_TEST(nucleotide_conversion, implicit_conversion)
{
    using other_type = std::conditional_t<
      std::is_same_v<TypeParam, bio::alphabet::rna4>,
      bio::alphabet::dna4,
      std::conditional_t<
        std::is_same_v<TypeParam, bio::alphabet::dna4>,
        bio::alphabet::rna4,
        std::conditional_t<
          std::is_same_v<TypeParam, bio::alphabet::rna5>,
          bio::alphabet::dna5,
          std::conditional_t<std::is_same_v<TypeParam, bio::alphabet::dna5>,
                             bio::alphabet::rna5,
                             std::conditional_t<std::is_same_v<TypeParam, bio::alphabet::dna15>,
                                                bio::alphabet::rna15,
                                                /* must be bio::alphabet::rna15 */ bio::alphabet::dna15>>>>>;

    // construct
    EXPECT_EQ(other_type{TypeParam{}.assign_char('C')}, other_type{}.assign_char('C'));
    // assign
    other_type l{};
    l = TypeParam{}.assign_char('C');
    EXPECT_EQ(l, other_type{}.assign_char('C'));
}
