// ============================================================================
//                 SeqAn - The Library for Sequence Analysis
// ============================================================================
//
// Copyright (c) 2006-2018, Knut Reinert & Freie Universitaet Berlin
// Copyright (c) 2016-2018, Knut Reinert & MPI Molekulare Genetik
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ============================================================================

/*!\file
 * \brief Provides seqan3::align_result.
 * \author Jörg Winkler <j.winkler AT fu-berlin.de>
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 */

#pragma once

namespace seqan3
{

namespace detail
{

/*!
 * \brief A struct that contains the actual alignment result data.
 * \tparam id_t         The type for the alignment identifier.
 * \tparam score_t      The type for the resulting score.
 * \tparam coordinate_t The type for both the begin and end coordinates, can be omitted.
 * \tparam trace_t      The type for the alignment trace, can be omitted.
 */
template <typename id_t, typename score_t, typename coordinate_t = void *, typename trace_t = void *>
struct align_result_value_type
{
    //! \brief The alignment identifier.
    id_t id{};
    //! \brief The alignment score.
    score_t score{};
    //! \brief The end coordinate of the alignment.
    coordinate_t end_coordinate{};
    //! \brief The begin coordinate of the alignment.
    coordinate_t begin_coordinate{};
    //! \brief The alignment trace, i.e. the actual base pair matching.
    trace_t trace{};
};

//! \brief Type deduction for id and score only.
template <typename id_t, typename score_t>
align_result_value_type(id_t, score_t) -> align_result_value_type<id_t, score_t>;

//! \brief Type deduction for id, score and end coordinate.
template <typename id_t, typename score_t, typename coordinate_t>
align_result_value_type(id_t, score_t, coordinate_t) -> align_result_value_type<id_t, score_t, coordinate_t>;

//! \brief Type deduction without the trace.
template <typename id_t, typename score_t, typename coordinate_t>
align_result_value_type(id_t, score_t, coordinate_t, coordinate_t) -> align_result_value_type<id_t, score_t,
                                                                                              coordinate_t>;

//! \brief Type deduction with all available fields.
template <typename id_t, typename score_t, typename coordinate_t, typename trace_t>
align_result_value_type(id_t, score_t, coordinate_t, coordinate_t, trace_t) -> align_result_value_type<id_t,
                                                                                                       score_t,
                                                                                                       coordinate_t,
                                                                                                       trace_t>;

} // namespace detail

/*!\brief Stores the alignment results and gives access to score, traceback and the begin and end coordinates.
 * \ingroup pairwise
 * \tparam align_result_traits The type of the traits object.
 *
 * \details
 * Objects of this class are the result of an alignment computation.
 * It always contains and alignment identifier and the resulting score.
 * Optionally – if the user requests – also the begin and end positions within
 * the sequences and the trace can be calculated. When accessing a field that
 * has not been calculated an std::bad_optional_access exception is thrown.
 */
template <typename align_result_traits>
class align_result
{
private:
    //! \brief Traits object that contains the actual alignment result data.
    align_result_traits data;

    using id_t = decltype(data.id);
    using score_t = decltype(data.score);
    using coord_t = decltype(data.end_coordinate);
    using trace_t = decltype(data.trace);

public:
    /*!\brief Constructor to pass the alignment result traits.
     * \param[in] traits The alignment results.
     */
    align_result(align_result_traits traits) : data(traits) {};

    //! \brief Default constructor.
    align_result() {}
    //! \brief Default copy constructor.
    align_result(align_result const &) = default;
    //! \brief Default move constructor.
    align_result(align_result &&) = default;
    //! \brief Default copy assignment.
    align_result & operator=(align_result const &) = default;
    //! \brief Default move assignment.
    align_result & operator=(align_result &&) = default;
    //! \brief Default destructor.
    ~align_result() = default;

    /*!\name Access functions
     * \brief Functions to access elements of the alignment result type.
     * \{
     */

    /*!\brief Returns the alignment identifier.
     * \return The id field.
     */
    constexpr id_t get_id() const noexcept
    {
        return data.id;
    }

    /*!\brief Returns the alignment score.
     * \return The score field.
     */
    constexpr score_t get_score() const noexcept
    {
        return data.score;
    }

    /*!\brief Returns the end coordinate of the alignment.
     * \return A pair of positions in the respective sequences, where the calculated alignment ends.
     */
    constexpr coord_t get_end_coordinate() const noexcept
    {
        static_assert(!std::is_same_v<coord_t, void *>,
                      "Trying to access the end coordinate, although it has not been computed.");
        return data.end_coordinate;
    }

    /*!\brief Returns the begin coordinate of the alignment.
     * \return  A pair of positions in the respective sequences, where the calculated alignment starts.
     * \details Guaranteed to be smaller than or equal to `get_end_coordinate()`.
     */
    constexpr coord_t get_begin_coordinate() const noexcept
    {
        // TODO Problem: Assertion does not fail, if end_coordinate exists, because the coordinate type is set.
        static_assert(!std::is_same_v<coord_t, void *>,
                      "Trying to access the begin coordinate, although it has not been computed.");
        return data.begin_coordinate;
    }

    /*!\brief Returns the traceback of the alignment.
     * \return At least two gapped sequences, which represent the alignment.
     */
    constexpr trace_t get_trace() const noexcept
    {
        static_assert(!std::is_same_v<trace_t, void *>,
                      "Trying to access the trace, although it has not been computed.");
        return data.trace;
    }
    //!\}
};

} // namespace seqan3
