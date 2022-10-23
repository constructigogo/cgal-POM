#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>

#include <thread>

//Nombre de threads qui vont parcourir la LCC en parallèle
#define NB_THREADS 640

using namespace CGAL;

typedef Linear_cell_complex_for_combinatorial_map<3> LCC;
typedef Linear_cell_complex_for_combinatorial_map<2> LCC_2;

template <unsigned int d>
using One_dart_per_cell_range = LCC::One_dart_per_cell_range<d>;

typedef LCC::Dart_range Dart_range;
typedef LCC::Dart_handle Dart_handle;
typedef LCC::Dart_descriptor Dart_descriptor;
typedef LCC::Dart Dart;
typedef LCC::Point Point;

void display_volume_darts(LCC lcc) {
    std::cout << "Volume darts: " << std::endl;
    One_dart_per_cell_range<3> volumes_range = lcc.one_dart_per_cell<3>();
    for(One_dart_per_cell_range<3>::iterator start = volumes_range.begin(), end = volumes_range.end(); start != end; start++)
    {
        std::cout << lcc.point_of_vertex_attribute(lcc.vertex_attribute(start)) << std::endl;
    }
}

void display_vertices_darts(LCC lcc, int threadId) {
    std::string outputFileName = "out";
    outputFileName += std::to_string(threadId);
    outputFileName += ".txt";

    std::string outputFilePath = "outFiles/";
    outputFilePath += outputFileName;

    std::cout << outputFilePath;
    std::ofstream outputFile;
    outputFile.open(outputFilePath);

    One_dart_per_cell_range<0> vertices_range = lcc.one_dart_per_cell<0>();
    for(One_dart_per_cell_range<0>::iterator start = vertices_range.begin(), end = vertices_range.end(); start != end; start++)
    {
        outputFile << lcc.point_of_vertex_attribute(lcc.vertex_attribute(start)) << std::endl;
    }
}

void display_edges_darts_lcc2(LCC_2 lcc, int threadId) {
    std::string outputFileName = "out";
    outputFileName += std::to_string(threadId);
    outputFileName += ".txt";

    std::string outputFilePath = "outFiles/";
    outputFilePath += outputFileName;

    std::cout << outputFilePath;
    std::ofstream outputFile;
    outputFile.open(outputFilePath);

    LCC_2::One_dart_per_cell_range<1> edges_range = lcc.one_dart_per_cell<1>();
    for(LCC_2::One_dart_per_cell_range<1>::iterator start = edges_range.begin(), end = edges_range.end(); start != end; start++)
    {
        outputFile << lcc.point_of_vertex_attribute(lcc.vertex_attribute(start)) << std::endl;
    }
}

int main()
{
    std::array<std::thread, NB_THREADS> threads;

    LCC lcc;

    std::string filename = "data/graph.txt";

    LCC_2 lcc2;

    std::ifstream is(filename.c_str());
    std::cout<<"Importing plane graph from "<<filename<<std::endl;
    CGAL::import_from_plane_graph(lcc2, is);

    for(int i = 0; i < NB_THREADS; i++)
        threads[i] = std::thread(display_edges_darts_lcc2, lcc2, i);

    for(int i = 0; i < NB_THREADS; i++)
        threads[i].join();

    return EXIT_SUCCESS;

    /* Dart_descriptor d1=
    lcc.make_hexahedron(Point(0,0,0), Point(5,0,0),
                            Point(5,5,0), Point(0,5,0),
                            Point(0,5,4), Point(0,0,4),
                            Point(5,0,4), Point(5,5,4));
    Dart_descriptor d2=
    lcc.make_hexahedron(Point(5,0,0), Point(10,0,0),
                            Point(10,5,0), Point(5,5,0),
                            Point(5,5,4), Point(5,0,4),
                            Point(10,0,4), Point(10,5,4));


    //En lançant deux threads qui vont itérer sur les même darts,
    //on va avoir des problèmes de concurrence sur la gestion des 
    //marks en interne et ça va exploser
    std::thread thread(display_vertices_darts, lcc, 0);
    thread.join();

    /* for(int i = 0; i < NB_THREADS; i++)
        threads[i] = std::thread(display_vertices_darts, lcc, i);

    for(int i = 0; i < NB_THREADS; i++)
        threads[i].join(); */

    //Cette ligne est là seulement pour
    //que le main thread attende la fin du thread d'affichage. 
    //Si le main thread n'attend pas le thread d'affichage, 
    //l'instance locale de LCC va être détruite et le thread 
    //d'affichage, qui est toujours en train d'utiliser LCC 
    //pour afficher les points des darts va planter */
}
