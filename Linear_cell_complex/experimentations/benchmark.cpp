#define CGAL_NO_ASSERTIONS

#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Polyhedron_3_to_lcc.h>

#include <chrono>
#include <thread>

#define BENCHMARK_ITERATIONS 1000

using namespace CGAL;

struct Concurrent_items : public Linear_cell_complex_min_items
{
  typedef CGAL::Tag_true Use_concurrent_container;
};

typedef Linear_cell_complex_for_combinatorial_map<3, 3, Linear_cell_complex_traits<3>, Concurrent_items> LCC;

template <unsigned int d>
using One_dart_per_cell_range = LCC::One_dart_per_cell_range<d>;

void import_bunny_into_lcc(LCC& lcc) {
    std::ifstream inputFile("../data/bunny00.off");

    CGAL::import_from_polyhedron_3_flux<LCC>(lcc, inputFile);
}

void benchmark_bunny_one_dart_per_cell_iterator()
{
    LCC lcc;
    import_bunny_into_lcc(lcc);

    One_dart_per_cell_range<3> range = lcc.one_dart_per_cell<3>();

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        for(One_dart_per_cell_range<3>::iterator start = range.begin(), end = range.end();
            start != end; start++)
            ;
    }
    auto stop= std::chrono::high_resolution_clock::now();

    long long int duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Total duration: " << duration << "ms" << std::endl;
    std::cout << "Average duration: " << duration / (double)BENCHMARK_ITERATIONS << "ms" << std::endl;
}

BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(My_has_concurrent_tag, Use_concurrent_container, true)

int main()
{
    benchmark_bunny_one_dart_per_cell_iterator();
}
