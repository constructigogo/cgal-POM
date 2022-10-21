#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>

#include <thread>

using namespace CGAL;

typedef Linear_cell_complex_for_combinatorial_map<3> LCC;

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

int main()
{
    std::thread thread_volumes;

    LCC lcc;

    Dart_descriptor d1=
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
    thread_volumes = std::thread(display_volume_darts, lcc);

    //thread_volumes.join();//Si on décommente cette ligne, 
    //le deuxième thread ne démarrera pas avant que le 
    //premier ait terminé donc les deux threads ne vont 
    //pas s'exécuter en parallèle et le programme va 
    //réussir son exécution

    thread_volumes = std::thread(display_volume_darts, lcc);





    thread_volumes.join();//Cette ligne est là seulement pour
    //que le main thread attende la fin du thread d'affichage. 
    //Si le main thread n'attend pas le thread d'affichage, 
    //l'instance locale de LCC va être détruite et le thread 
    //d'affichage, qui est toujours en train d'utiliser LCC 
    //pour afficher les points des darts va planter
}
