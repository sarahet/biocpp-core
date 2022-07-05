#include <bio/alphabet/structure/structured_rna.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/structure/dot_bracket3.hpp>
#include <bio/alphabet/fmt.hpp>

int main()
{
    using bio::alphabet::operator""_rna4;
    using bio::alphabet::operator""_db3;

    bio::alphabet::structured_rna<bio::alphabet::rna4, bio::alphabet::dot_bracket3> letter{'G'_rna4, '('_db3};
    fmt::print("{} {} {}\n",
               bio::alphabet::to_rank(letter),
               bio::alphabet::to_rank(get<0>(letter)),
               bio::alphabet::to_rank(get<1>(letter)));
    // 6 2 1

    fmt::print("{} {} {}\n",
               bio::alphabet::to_char(letter),
               bio::alphabet::to_char(get<0>(letter)),
               bio::alphabet::to_char(get<1>(letter)));
    // G G (

    // modify via structured bindings and references:
    auto & [ seq_l, structure_l ] = letter;
    seq_l = 'U'_rna4;
    fmt::print("{}\n", bio::alphabet::to_char(letter));
    // U
}
