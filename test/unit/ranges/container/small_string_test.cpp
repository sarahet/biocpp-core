// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <ranges>
#include <string>
#include <utility>

#include <gtest/gtest.h>

#include <bio/ranges/container/concept.hpp>
#include <bio/ranges/container/small_string.hpp>

using namespace std::literals;

// standard construction.
TEST(small_string, standard_construction)
{
    EXPECT_TRUE((std::is_default_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_nothrow_default_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_copy_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_trivially_copy_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_nothrow_copy_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_move_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_trivially_move_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_nothrow_move_constructible_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_copy_assignable_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_trivially_copy_assignable_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_nothrow_copy_assignable_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_move_assignable_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_trivially_move_assignable_v<bio::ranges::small_string<4>>));
    EXPECT_TRUE((std::is_nothrow_move_assignable_v<bio::ranges::small_string<4>>));
}

TEST(small_string, container)
{
    EXPECT_TRUE(bio::ranges::detail::container<bio::ranges::small_string<4>>);
    EXPECT_TRUE(std::ranges::random_access_range<bio::ranges::small_string<4>>);
}

// construction from literal.
TEST(small_string, construct_from_literal)
{
    EXPECT_TRUE((std::is_same_v<bio::ranges::small_string<5>, decltype(bio::ranges::small_string{"hello"})>));
}

// construction from char.
TEST(small_string, construct_from_char)
{
    EXPECT_TRUE((std::is_same_v<bio::ranges::small_string<1>, decltype(bio::ranges::small_string{'h'})>));
}

// construction from array.
TEST(small_string, construct_from_array)
{
    EXPECT_TRUE((std::is_same_v<bio::ranges::small_string<5>,
                                decltype(bio::ranges::small_string{
                                  std::array{'h', 'e', 'l', 'l', 'o'}
    })>));
}

// assignment from literal.
TEST(small_string, assign_from_literal)
{
    bio::ranges::small_string<20> em{};
    em = "hello";
    EXPECT_EQ(em, bio::ranges::small_string<20>{"hello"});

    em.assign("boo");
    EXPECT_EQ(em, bio::ranges::small_string<20>{"boo"});
}

TEST(small_string, capacity)
{
    constexpr bio::ranges::small_string em{"hello"};

    EXPECT_EQ(em.max_size(), 5u);
    constexpr auto msize = em.max_size();
    EXPECT_EQ(msize, 5u);

    EXPECT_EQ(em.capacity(), 5u);
    constexpr auto cap = em.capacity();
    EXPECT_EQ(cap, 5u);
}

TEST(small_string, c_str)
{
    {
        bio::ranges::small_string em{"hello"};
        EXPECT_EQ(std::string{em.c_str()}, "hello"s);
    }

    {
        bio::ranges::small_string em{'x'};
        EXPECT_EQ(std::string{em.c_str()}, "x"s);
    }
}

TEST(small_string, string)
{
    bio::ranges::small_string em{"hello"};
    EXPECT_EQ(em.str(), "hello"s); // explicit
}

TEST(small_string, view_conversion)
{
    bio::ranges::small_string em{"hello"};
    std::string_view          str = em;
    EXPECT_EQ(str, "hello"s); // explicit
}

TEST(small_string, string_conversion)
{
    bio::ranges::small_string em{"hello"};
    std::string               str = static_cast<std::string>(em);
    EXPECT_EQ(str, "hello"s); // explicit
}

constexpr bool erase_test()
{
    bio::ranges::small_string em{"hello"};
    em.erase();
    bool res = em.empty();

    bio::ranges::small_string em1{"hello"};
    em1.erase(2);
    res = res && (em1 == bio::ranges::small_string<5>{"he"});

    bio::ranges::small_string em2{"hello"};
    em2.erase(2, 2);
    res = res && (em2 == bio::ranges::small_string<5>{"heo"});

    return res;
}

TEST(small_string, erase)
{
    constexpr bool res = erase_test();
    EXPECT_TRUE(res);
}

TEST(small_string, concat)
{
    {
        constexpr bio::ranges::small_string em =
          bio::ranges::small_string{"hello"} + bio::ranges::small_string{' '} + bio::ranges::small_string{"world"};
        constexpr auto size = em.size();

        EXPECT_EQ(size, 11u);
        EXPECT_EQ(em.str(), "hello world"s);
    }

    {
        static constexpr char const a[] = "hello";
        static constexpr char const b[] = " ";
        static constexpr char const c[] = "world";
        constexpr auto em = bio::ranges::small_string{a} + bio::ranges::small_string{b} + bio::ranges::small_string{c};
        constexpr auto size = em.size();

        EXPECT_EQ(size, 11u);
        EXPECT_EQ(em.str(), "hello world"s);
    }
}

TEST(small_string, begin)
{
    bio::ranges::small_string s{"hello"};
    EXPECT_EQ(*s.begin(), 'h');
    EXPECT_TRUE((std::is_same_v<decltype(s)::iterator, decltype(s.begin())>));

    bio::ranges::small_string<5> const cs{s};
    EXPECT_EQ(*cs.begin(), 'h');
    EXPECT_TRUE((std::is_same_v<decltype(cs)::const_iterator, decltype(cs.begin())>));
}

TEST(small_string, cbegin)
{
    bio::ranges::small_string s{"hello"};
    EXPECT_EQ(*s.begin(), 'h');
    EXPECT_TRUE((std::is_same_v<decltype(s)::const_iterator, decltype(s.cbegin())>));
}

TEST(small_string, end)
{
    bio::ranges::small_string s{"hello"};
    EXPECT_EQ(*(s.end() - 1), 'o');
    EXPECT_TRUE((std::is_same_v<decltype(s)::iterator, decltype(s.end())>));

    bio::ranges::small_string<5> const cs{s};
    static_assert(std::is_same_v<bio::ranges::small_string<5> const, decltype(cs)>);
    EXPECT_EQ(*(cs.end() - 1), 'o');
    EXPECT_TRUE((std::is_same_v<decltype(cs)::const_iterator, decltype(cs.end())>));
}

TEST(small_string, cend)
{
    bio::ranges::small_string s{"hello"};
    EXPECT_EQ(*(s.cend() - 1), 'o');
    EXPECT_TRUE((std::is_same_v<decltype(s)::const_iterator, decltype(s.cend())>));
}

TEST(small_string, swap)
{
    bio::ranges::small_string s1{"hello"};
    bio::ranges::small_string s2{"olleh"};
    { // global function.
        std::swap(s1, s2);
        EXPECT_EQ(s1, bio::ranges::small_string{"olleh"});
        EXPECT_EQ(s2, bio::ranges::small_string{"hello"});
    }

    { // member function
        s1.swap(s2);
        EXPECT_EQ(s1, bio::ranges::small_string{"hello"});
        EXPECT_EQ(s2, bio::ranges::small_string{"olleh"});
    }
}

TEST(small_string, modifying)
{
    bio::ranges::small_string<50> s1{"hello"};
    EXPECT_EQ(std::string{s1.c_str()}, "hello"s);

    s1.pop_back();
    EXPECT_EQ(std::string{s1.c_str()}, "hell"s);

    s1.insert(s1.end(), {'o', 'o', 'o', 'o', 'o'});
    EXPECT_EQ(std::string{s1.c_str()}, "hellooooo"s);

    s1.assign("moooo");
    EXPECT_EQ(std::string{s1.c_str()}, "moooo"s);

    s1.resize(3);
    EXPECT_EQ(std::string{s1.c_str()}, "moo"s);

    s1.push_back('s');
    EXPECT_EQ(std::string{s1.c_str()}, "moos"s);

    s1.resize(10, 'a');
    EXPECT_EQ(std::string{s1.c_str()}, "moosaaaaaa"s);

    s1.resize(2, 'x');
    EXPECT_EQ(std::string{s1.c_str()}, "mo"s);

    s1.clear();
    EXPECT_EQ(std::string{s1.c_str()}, ""s);
}

TEST(small_string, equality)
{
    constexpr bool cmp1 = bio::ranges::small_string{"hello"} == bio::ranges::small_string{"hello"};
    constexpr bool cmp2 = bio::ranges::small_string{"hello"} == bio::ranges::small_string{"hell"};
    constexpr bool cmp3 = bio::ranges::small_string{"hell"} == bio::ranges::small_string{"hello"};
    constexpr bool cmp4 = bio::ranges::small_string{"hella"} == bio::ranges::small_string{"hello"};

    EXPECT_TRUE(cmp1);
    EXPECT_FALSE(cmp2);
    EXPECT_FALSE(cmp3);
    EXPECT_FALSE(cmp4);
}

TEST(small_string, inequality)
{
    constexpr bool cmp1 = bio::ranges::small_string{"hello"} != bio::ranges::small_string{"hello"};
    constexpr bool cmp2 = bio::ranges::small_string{"hello"} != bio::ranges::small_string{"hell"};
    constexpr bool cmp3 = bio::ranges::small_string{"hell"} != bio::ranges::small_string{"hello"};
    constexpr bool cmp4 = bio::ranges::small_string{"hella"} != bio::ranges::small_string{"hello"};

    EXPECT_FALSE(cmp1);
    EXPECT_TRUE(cmp2);
    EXPECT_TRUE(cmp3);
    EXPECT_TRUE(cmp4);
}

TEST(small_string, less)
{
    constexpr bool cmp1 = bio::ranges::small_string{"hello"} < bio::ranges::small_string{"hello"};
    constexpr bool cmp2 = bio::ranges::small_string{"hello"} < bio::ranges::small_string{"hell"};
    constexpr bool cmp3 = bio::ranges::small_string{"hell"} < bio::ranges::small_string{"hello"};
    constexpr bool cmp4 = bio::ranges::small_string{"hella"} < bio::ranges::small_string{"hello"};

    EXPECT_FALSE(cmp1);
    EXPECT_FALSE(cmp2);
    EXPECT_TRUE(cmp3);
    EXPECT_TRUE(cmp4);
}

TEST(small_string, less_equal)
{
    constexpr bool cmp1 = bio::ranges::small_string{"hello"} <= bio::ranges::small_string{"hello"};
    constexpr bool cmp2 = bio::ranges::small_string{"hello"} <= bio::ranges::small_string{"hell"};
    constexpr bool cmp3 = bio::ranges::small_string{"hell"} <= bio::ranges::small_string{"hello"};
    constexpr bool cmp4 = bio::ranges::small_string{"hella"} <= bio::ranges::small_string{"hello"};

    EXPECT_TRUE(cmp1);
    EXPECT_FALSE(cmp2);
    EXPECT_TRUE(cmp3);
    EXPECT_TRUE(cmp4);
}

TEST(small_string, greater)
{
    constexpr bool cmp1 = bio::ranges::small_string{"hello"} > bio::ranges::small_string{"hello"};
    constexpr bool cmp2 = bio::ranges::small_string{"hello"} > bio::ranges::small_string{"hell"};
    constexpr bool cmp3 = bio::ranges::small_string{"hell"} > bio::ranges::small_string{"hello"};
    constexpr bool cmp4 = bio::ranges::small_string{"hella"} > bio::ranges::small_string{"hello"};

    EXPECT_FALSE(cmp1);
    EXPECT_TRUE(cmp2);
    EXPECT_FALSE(cmp3);
    EXPECT_FALSE(cmp4);
}

TEST(small_string, greater_equal)
{
    constexpr bool cmp1 = bio::ranges::small_string{"hello"} >= bio::ranges::small_string{"hello"};
    constexpr bool cmp2 = bio::ranges::small_string{"hello"} >= bio::ranges::small_string{"hell"};
    constexpr bool cmp3 = bio::ranges::small_string{"hell"} >= bio::ranges::small_string{"hello"};
    constexpr bool cmp4 = bio::ranges::small_string{"hella"} >= bio::ranges::small_string{"hello"};

    EXPECT_TRUE(cmp1);
    EXPECT_TRUE(cmp2);
    EXPECT_FALSE(cmp3);
    EXPECT_FALSE(cmp4);
}

template <std::size_t N>
constexpr bio::ranges::small_string<N> fill_small_string(bio::ranges::small_string<N> s, char const val)
{
    s.resize(N);

    for (size_t i = 0; i < N; ++i)
    {
        s[i] = val;
    }

    return s;
}

TEST(small_string, compile_time_fill)
{
    constexpr bool cmp = fill_small_string(bio::ranges::small_string<4>{}, 'x') == bio::ranges::small_string{"xxxx"};
    EXPECT_TRUE(cmp);
}

TEST(small_string, output)
{
    bio::ranges::small_string em{"hello"};
    std::ostringstream        os;
    os << em;
    EXPECT_EQ(os.str(), "hello"s);
}
