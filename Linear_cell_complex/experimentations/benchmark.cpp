#define CGAL_NO_ASSERTIONS

#define LOG 0
#define LOG2 0

#ifdef LOG
    #if LOG
        #define LOG_MESSAGE(x) x;
    #else
        #define LOG_MESSAGE(x)
    #endif
#endif

#ifdef LOG2
    #if LOG2
        #define LOG_MESSAGE2(x) x
    #else
        #define LOG_MESSAGE2(x)
    #endif
#endif

#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Polyhedron_3_to_lcc.h>
#include <CGAL/Timer.h>

#define BENCHMARK_ITERATIONS 1000

using namespace CGAL;

struct Concurrent_items : public Linear_cell_complex_min_items
{
  typedef CGAL::Tag_true Use_concurrent_container;
};

typedef Linear_cell_complex_for_combinatorial_map<3, 3, Linear_cell_complex_traits<3>, Concurrent_items> LCC_Concurrent;
typedef Linear_cell_complex_for_combinatorial_map<3> LCC;

template <unsigned int d>
using One_dart_per_cell_range_concurrent = LCC_Concurrent::One_dart_per_cell_range<d>;

template <unsigned int d>
using One_dart_per_cell_range = LCC::One_dart_per_cell_range<d>;

void import_bunny_into_lcc_concurrent(LCC_Concurrent& lcc) {
    std::ifstream inputFile("../data/bunny00.off");

    CGAL::import_from_polyhedron_3_flux<LCC_Concurrent>(lcc, inputFile);
}

void import_bunny_into_lcc(LCC& lcc) {
    std::ifstream inputFile("../data/bunny00.off");

    CGAL::import_from_polyhedron_3_flux<LCC>(lcc, inputFile);
}

void benchmark_bunny_one_dart_per_cell_iterator_concurrent()
{
    LCC_Concurrent lcc;
    import_bunny_into_lcc_concurrent(lcc);

    One_dart_per_cell_range_concurrent<1> range = lcc.one_dart_per_cell<1>();

    Timer timer;
    timer.start();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        for(One_dart_per_cell_range_concurrent<1>::iterator start = range.begin(), end = range.end();
            start != end; start++) {
            ;
        }
    }
    timer.stop();

    std::cout << "Total duration: " << timer.time() * 1000 << "ms" << std::endl;
    std::cout << "Average duration: " << timer.time() * 1000 / (double)BENCHMARK_ITERATIONS << "ms" << std::endl;
}

void benchmark_bunny_one_dart_per_cell_iterator()
{
    LCC lcc;
    import_bunny_into_lcc(lcc);

    One_dart_per_cell_range<1> range = lcc.one_dart_per_cell<1>();

    Timer timer;
    timer.start();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        for(One_dart_per_cell_range<1>::iterator start = range.begin(), end = range.end();
            start != end; start++)
            ;
    }
    timer.stop();

    std::cout << "Total duration: " << timer.time() * 1000 << "ms" << std::endl;
    std::cout << "Average duration: " << timer.time() * 1000 / (double)BENCHMARK_ITERATIONS << "ms" << std::endl;
}

int main()
{
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Concurrent: " << std::endl;
    benchmark_bunny_one_dart_per_cell_iterator_concurrent();

    std::cout << "Non-concurrent: " << std::endl;
    benchmark_bunny_one_dart_per_cell_iterator();
}
