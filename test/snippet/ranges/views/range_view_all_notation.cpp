#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/complement.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::dna4_vector vec{"ACGGTC"_dna4};

    // these are synonymous:
    auto vec_view1 = vec | bio::views::complement;
    auto vec_view2 = bio::views::complement(vec);

    // both views "behave" like a collection of the elements 'T', 'G', 'C', 'C', 'A', 'G'
    // but can be copied cheaply et cetera
}
