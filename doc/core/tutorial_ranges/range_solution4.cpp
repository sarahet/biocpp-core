#include <vector>

#include <sharg/all.hpp>                                      // Optional: the sharg-parser

#include <bio/alphabet/fmt.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>      // include bitcompressed vector

using namespace bio::alphabet::literals;

int main(int argc, char ** argv)
{
    sharg::argument_parser myparser("Vector-implementations-comparison", argc, argv);
    size_t size{};
    bool use_bitvector{};
    myparser.add_positional_option(size, "Size of vector");
    myparser.add_flag(use_bitvector, 'b', "bitvector", "Use bitvector instead of vector");

    try
    {
         myparser.parse();
    }
    catch (sharg::argument_parser_error const & ext)                     // catch user errors
    {
        fmt::print(stderr, "[Error] {}\n", ext.what());
        return -1;
    }

    if (use_bitvector)
    {
        bio::ranges::bitcompressed_vector<bio::alphabet::dna4> vector;
        vector.resize(size, 'A'_dna4);
        fmt::print("Allocated bio::ranges::bitcompressed_vector<bio::alphabet::dna4> of size {}\n", vector.size());
    }
    else
    {
        std::vector<bio::alphabet::dna4> vector{size};
        fmt::print("Allocated std::vector<bio::alphabet::dna4> of size {}\n", vector.size());
    }
}
