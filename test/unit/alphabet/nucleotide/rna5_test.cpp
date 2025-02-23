// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/rna5.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(rna5, alphabet, bio::alphabet::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, semi_alphabet_test, bio::alphabet::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, alphabet_constexpr, bio::alphabet::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, semi_alphabet_constexpr, bio::alphabet::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, nucleotide, bio::alphabet::rna5, );

TEST(rna5, to_char_assign_char)
{
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('U')), 'U');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('T')), 'U');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('R')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('Y')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('S')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('W')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('K')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('M')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('B')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('D')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('H')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('V')), 'N');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('N')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna5{}.assign_char('!')), 'N');
}

TEST(rna5, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_rna5), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_rna5), 'C');
    EXPECT_EQ(bio::alphabet::to_char('G'_rna5), 'G');

    EXPECT_EQ(bio::alphabet::to_char('U'_rna5), 'U');
    EXPECT_EQ(bio::alphabet::to_char('T'_rna5), 'U');

    EXPECT_EQ(bio::alphabet::to_char('R'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('Y'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('S'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('W'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('K'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('M'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('B'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('D'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('H'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('V'_rna5), 'N');

    EXPECT_EQ(bio::alphabet::to_char('N'_rna5), 'N');
    EXPECT_EQ(bio::alphabet::to_char('!'_rna5), 'N');
}

TEST(rna5, string_literal)
{
    bio::alphabet::rna5_vector v;
    v.resize(5, 'A'_rna5);
    EXPECT_EQ(v, "AAAAA"_rna5);

    std::vector<bio::alphabet::rna5> w{'A'_rna5, 'C'_rna5, 'G'_rna5, 'T'_rna5, 'U'_rna5, 'N'_rna5};
    EXPECT_EQ(w, "ACGUUN"_rna5);
}

TEST(rna5, char_is_valid)
{
    constexpr auto validator = [](char const c)
    {
        return c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'a' || c == 'c' || c == 'g' ||
               c == 't' || c == 'u' || c == 'N' || c == 'n';
    };
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::alphabet::rna5::char_is_valid(c), validator(c));
}
