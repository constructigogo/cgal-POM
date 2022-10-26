#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Generalized_map.h>
#include <CGAL/Linear_cell_complex_constructors.h>

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
#define ITERATIONS 10

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

void display_volumes_darts(LCC& lcc, int threadId) {
    One_dart_per_cell_range<3> volumes_range = lcc.one_dart_per_cell<3>();
    for(One_dart_per_cell_range<3>::iterator 
        start = volumes_range.begin(), 
        end = volumes_range.end(); 
        start != end;
        start++)
    {
        //Affichage désactivé pour plus de clarté
        //std::cout << "Thread [" << threadId << "]: " << lcc.point_of_vertex_attribute(lcc.vertex_attribute(start)) << std::endl;
    }
}

void display_vertices_darts(LCC& lcc, int threadId) {
    One_dart_per_cell_range<1> vertices_range = lcc.one_dart_per_cell<1>();
    for(One_dart_per_cell_range<1>::iterator 
        start = vertices_range.begin(), 
        end = vertices_range.end(); 
        start != end;
        start++)
    {
        //Affichage désactivé pour plus de clarté
        //std::cout << "Thread [" << threadId << "]: " << lcc.point_of_vertex_attribute(lcc.vertex_attribute(start)) << std::endl;
    }
}

/*!
* @param lcc La LCC sur laquelle on va itérer
* @param threadId Le numéro du thread qui exécute cette
* fonction. L'id est utilisé pour pouvoir générer un 
* nom de fichier unique en fonction du thread afin 
* que le thread puisse écrire ses résultats dans son 
* propre fichier à lui
*/
void vertices_darts_to_file(LCC& lcc, int threadId) {
    //Génération du nom de fichier en fonction du threadId
    std::string outputFileName = "out";
    outputFileName += std::to_string(threadId);
    outputFileName += ".txt";

    std::string outputFilePath = "outFiles/";
    outputFilePath += outputFileName;

    //Ouverture du fichier qui va accueillir les 
    //résultats du thread (plutôt que de tout écrire 
    //dans la console en vrac avec tous les threads 
    //qui écrivent dans tous les sens en même temps)
    std::ofstream outputFile;
    outputFile.open(outputFilePath);

    One_dart_per_cell_range<0> vertices_range = lcc.one_dart_per_cell<0>();
    for(One_dart_per_cell_range<0>::iterator start = vertices_range.begin(), end = vertices_range.end(); start != end;)
    {
        outputFile << lcc.point_of_vertex_attribute(lcc.vertex_attribute(start)) << std::endl;

        start++;
    }
}

int main()
{
    //Tableau qui va stocker tous les handles des threads qu'on va créer
    std::array<std::thread, NB_THREADS> threads;

    LCC lcc;

    //Création de deux cubes pour remplir la LCC
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

    for(int iter = 0; iter < ITERATIONS; iter++)
    {
        for(int i = 0; i < NB_THREADS; i++)
        {
            threads[i] = std::thread(display_vertices_darts, std::ref(lcc), i + 1);

            //Si on décommente cette ligne, les deux
            //threads ne vont pas s'exécuter en parallèle 
            //(puisque le main thread va attendre la fin 
            //du thread 1 avant de lancer le deuxième thread) 
            //et donc on ne va pas avoir de race condition

            //thread[i].join();
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
