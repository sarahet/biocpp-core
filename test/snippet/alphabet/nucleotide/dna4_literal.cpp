#include <bio/alphabet/nucleotide/dna4.hpp>

int main()
{
    using bio::alphabet::operator""_dna4;

    bio::alphabet::dna4_vector foo{"ACGTTA"_dna4};
    bio::alphabet::dna4_vector bar = "ACGTTA"_dna4;
    auto bax = "ACGTTA"_dna4;
}
