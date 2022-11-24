//#define CGAL_NO_ASSERTIONS

#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Polyhedron_3_to_lcc.h>

#include <thread>

//Nombre de threads qui vont parcourir la LCC en parallèle
#define NB_THREADS 10

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

struct ConcurrentItems : Linear_cell_complex_min_items
{
    typedef CGAL::Tag_true Use_concurrent_container;
};

//typedef Linear_cell_complex_for_combinatorial_map<3> LCC;
typedef Linear_cell_complex_for_combinatorial_map<3, 3, Linear_cell_complex_traits<3>, ConcurrentItems> LCC;

template <unsigned int d>
using One_dart_per_cell_range = LCC::One_dart_per_cell_range<d>;

typedef LCC::Dart_range Dart_range;
typedef LCC::Dart_handle Dart_handle;
typedef LCC::Dart_descriptor Dart_descriptor;
typedef LCC::Dart Dart;
typedef LCC::Point Point;

template <unsigned int d>
void iterate_over_darts(LCC& lcc) {
    One_dart_per_cell_range<d> vertices_range = lcc.one_dart_per_cell<d>();

    for(typename One_dart_per_cell_range<d>::iterator
        start = vertices_range.begin();
        start.cont();
        start++)
    {
        ;
    }
}

//template<unsigned int d>
//void iterate_over_darts_concurrent(ConcurrentItemsLCC& lcc) {
//    ConcurrentItemsLCC::One_dart_per_cell_range<d> vertices_range = lcc.one_dart_per_cell<d>();

//    for(typename ConcurrentItemsLCC::One_dart_per_cell_range<d>::iterator
//        start = vertices_range.begin(),
//        end = vertices_range.end();
//        start != end;
//        start++)
//    {
//        ;
//    }
//}

template<unsigned int d>
void iterate_over_darts_concurrent(LCC& lcc) {
    LCC::One_dart_per_cell_range<d> vertices_range = lcc.one_dart_per_cell<d>();

    for(typename LCC::One_dart_per_cell_range<d>::iterator
        start = vertices_range.begin(), end = vertices_range.end();
        start != end;
        ++start)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "thread ending" << std::endl;
}

template <typename LCC>
void import_bunny_into_lcc(LCC& lcc) {
    std::ifstream inputFile("../data/bunny00.off");

    CGAL::import_from_polyhedron_3_flux<LCC>(lcc, inputFile);
}

void iterator_compact_container() {
    //Tableau qui va stocker tous les handles des threads qu'on va créer
    std::array<std::thread, NB_THREADS> threads;

    LCC lcc;
    import_bunny_into_lcc(lcc);

    //En lançant deux threads qui vont itérer sur les même darts,
    //on va avoir des problèmes de concurrence sur la gestion des
    //marks en interne et ça va exploser
    for(int iter = 0; iter < ITERATIONS; iter++)
    {
        for(int i = 0; i < NB_THREADS; i++)
        {
            threads[i] = std::thread(iterate_over_darts<1>, std::ref(lcc));

            //Si on décommente cette ligne, les deux
            //threads ne vont pas s'exécuter en parallèle
            //(puisque le main thread va attendre la fin
            //du thread 1 avant de lancer le deuxième thread)
            //et donc on ne va pas avoir de race condition

            //threads[i].join();
        }

        //On join tous les threads à la fin de chaque itération
        for(int i = 0; i < NB_THREADS; i++)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }

    //On va attendre la fin de tous les
    //threads pour pas que le main thread
    //finisse son exécution et donc destroy
    //la LCC sur laquelle les threads sont
    //en train d'itérer
    for(int i = 0; i < NB_THREADS; i++)
    {
        if(threads[i].joinable())
            threads[i].join();
    }
}

void iterator_concurrent_compact_container() {
    //Tableau qui va stocker tous les handles des threads qu'on va créer
    std::array<std::thread, NB_THREADS> threads;

    LCC lcc;
    //import_bunny_into_lcc(lcc);
    lcc.make_hexahedron(Point(0, 0, 0), Point(1, 0, 0), Point(1, 0, 1), Point(0, 0, 1), Point(0, 1, 1), Point(0, 1, 0), Point(1, 1, 0), Point(1, 1, 1));

    for(int iter = 0; iter < ITERATIONS; iter++)
    {
        for(int i = 0; i < NB_THREADS; i++)
        {
            threads[i] = std::thread(iterate_over_darts_concurrent<1>, std::ref(lcc));

            //threads[i].join();
        }

        //On join tous les threads à la fin de chaque itération
        for(int i = 0; i < NB_THREADS; i++)
        {
            if(threads[i].joinable())
                threads[i].join();
        }

        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
    }

    for(int i = 0; i < NB_THREADS; i++)
    {
        if(threads[i].joinable())
            threads[i].join();
    }
}

int main(int argc, char* argv[])
{
    //iterator_compact_container();
    iterator_concurrent_compact_container();
}
