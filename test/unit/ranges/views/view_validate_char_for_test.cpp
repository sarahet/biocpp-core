// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/validate_char_for.hpp>
#include <bio/test/expect_range_eq.hpp>

using std::literals::string_view_literals::operator""sv;

TEST(view_validate_char_for, basic)
{
    std::string vec{"ACTTTGATA"};
    std::string cmp{"ACTTTGATA"};

    // pipe notation
    EXPECT_RANGE_EQ(cmp, vec | bio::ranges::views::validate_char_for<bio::alphabet::dna5>);

    // function notation
    EXPECT_RANGE_EQ(cmp, bio::ranges::views::validate_char_for<bio::alphabet::dna5>(vec));

    // combinability
    std::string cmp2{"ATAGTTTCA"};
    EXPECT_RANGE_EQ(cmp2, vec | bio::ranges::views::validate_char_for<bio::alphabet::dna5> | std::views::reverse);
}

TEST(view_validate_char_for, deep_view)
{
    std::vector<std::string> foo{"ACGTA", "TGCAT"};

    auto v = foo | bio::ranges::views::validate_char_for<bio::alphabet::dna5>;

    ASSERT_EQ(std::ranges::size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "ACGTA"sv);
    EXPECT_RANGE_EQ(v[1], "TGCAT"sv);
}

TEST(view_validate_char_for, concepts)
{
    std::string vec{"ACTTTGATA"};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::contiguous_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), char>));

    auto v1 = vec | bio::ranges::views::validate_char_for<bio::alphabet::dna5>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::alphabet::dna5>));
    EXPECT_TRUE((std::ranges::output_range<decltype(v1), char>));
}

TEST(view_validate_char_for, exception)
{
    std::string foo = "ACGPTA";

    auto v = foo | bio::ranges::views::validate_char_for<bio::alphabet::dna5>;
    EXPECT_THROW((std::ranges::equal(v, "ACGNTA"sv)), bio::alphabet::invalid_char_assignment);
}
