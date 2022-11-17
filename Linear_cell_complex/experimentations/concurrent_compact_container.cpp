#define CGAL_NO_ASSERTIONS

#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Polyhedron_3_to_lcc.h>

#include <thread>

//Nombre de threads qui vont parcourir la LCC en parallèle
#define NB_THREADS 2

//Combien de fois on va répéter l'itération sur 
//la LCC. Ce paramètre sert uniquement à avoir
//plus de chance de trouver une race condition
//en lançant le programme 1 fois. Si on a que
//une seule itération, même avec 10 threads, 
//on est pas sûr que le programme crash. 
//Si on répète 100 fois les itérations, on a 
//beaucoup plus de chance de faire crasher 
//le programme
#define ITERATIONS 1

using namespace CGAL;

struct Concurrent_items : public Linear_cell_complex_min_items
{
  typedef CGAL::Tag_true Use_concurrent_container;
};

typedef Linear_cell_complex_for_combinatorial_map<3, 3, Linear_cell_complex_traits<3>, Concurrent_items> LCC;

void import_bunny_into_lcc(LCC& lcc, int thread_id) {
    std::ifstream inputFile("../data/bunny00.off");

    CGAL::import_from_polyhedron_3_flux<LCC>(lcc, inputFile);
}

void construct_lcc_crash() {
    LCC lcc;

    std::array<std::thread, NB_THREADS> threads;

    for(int iter = 0; iter < ITERATIONS; iter++)
    {
        for(int i = 0; i < NB_THREADS; i++)
        {
            threads[i] = std::thread(import_bunny_into_lcc, std::ref(lcc), i + 1);

            //threads[i].join();
        }

        //On join tous les threads à la fin de chaque itération
        for(int i = 0; i < NB_THREADS; i++)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }

    for(int i = 0; i < NB_THREADS; i++)
    {
        if(threads[i].joinable())
            threads[i].join();
    }
}

BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(My_has_concurrent_tag, Use_concurrent_container, true)

int main()
{
    std::cout << "Concurrent items ? : " << My_has_concurrent_tag<Concurrent_items>::value << std::endl;

    construct_lcc_crash();
}
