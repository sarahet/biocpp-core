#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/views/translate.hpp>

int main()
{
    using namespace bio::alphabet::literals;

    bio::alphabet::dna5_vector vec{"ACGTACGTACGTA"_dna5};

    // default frame translation
    auto v1 = vec | bio::views::translate;
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T],[Y,V,R,T],[T,Y,V,R],[R,T,Y]]

    // single frame translation
    auto v2 = vec | bio::views::translate(bio::alphabet::translation_frames::FWD_FRAME_0);
    // == [[T,Y,V,R]]

    // reverse translation
    auto v3 = vec | bio::views::translate(bio::alphabet::translation_frames::FWD_REV_0);
    // == [[T,Y,V,R],[Y,V,R,T]]

    // forward frames translation
    auto v4 = vec | bio::views::translate(bio::alphabet::translation_frames::FWD);
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T]]

    // six frame translation
    auto v5 = vec | bio::views::translate(bio::alphabet::translation_frames::SIX_FRAME);
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T],[Y,V,R,T],[T,Y,V,R],[R,T,Y]]

    // function syntax
    auto v6 = bio::views::translate(vec, bio::alphabet::translation_frames::FWD_REV_0);
    // == [[T,Y,V,R],[Y,V,R,T]]

    // combinability
    auto v7 = vec | bio::views::complement | bio::views::translate(bio::alphabet::translation_frames::FWD_REV_0);
    // == [[C,M,H,A],[M,H,A,C]]
}
