#define CGAL_NO_ASSERTIONS

#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Polyhedron_3_to_lcc.h>

#include <thread>

using namespace CGAL;

struct Concurrent_items : public Linear_cell_complex_min_items
{
  typedef CGAL::Tag_true Use_concurrent_container;
};

typedef Linear_cell_complex_for_combinatorial_map<3, 3, Linear_cell_complex_traits<3>, Concurrent_items> LCC;

void import_bunny_into_lcc(LCC& lcc) {
    std::ifstream inputFile("../data/bunny00.off");

    CGAL::import_from_polyhedron_3_flux<LCC>(lcc, inputFile);
}

void benchmark_one_dart_per_cell_iterator()
{

}

BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(My_has_concurrent_tag, Use_concurrent_container, true)

int main()
{
    LCC lcc;
    import_bunny_into_lcc(lcc);

    benchmark_one_dart_per_cell_iterator();
}
