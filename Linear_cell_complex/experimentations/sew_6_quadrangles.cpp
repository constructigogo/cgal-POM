#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/draw_linear_cell_complex.h>

typedef CGAL::Linear_cell_complex_for_combinatorial_map<3> LCC3;

typedef LCC3::Point Point;
typedef LCC3::Dart_descriptor Dart_descriptor;

int main()
{
    for(int i = 0; i <= 3; i++)
    {
        LCC3 lcc;

        Dart_descriptor dh1 = lcc.make_quadrangle(Point(0, 0, 0), Point(1, 0, 0), Point(1, 1, 0), Point(0, 1, 0));
        //Dart_descriptor dh2 = lcc.make_quadrangle(Point(0, 1, 0), Point(1, 1, 0), Point(1, 1, 1), Point(0, 1, 1));
        //Dart_descriptor dh3 = lcc.make_quadrangle(Point(0, 0, 1), Point(1, 0, 1), Point(1, 1, 1), Point(0, 1, 1));
        Dart_descriptor dh4 = lcc.make_quadrangle(Point(0, 0, 0), Point(1, 0, 0), Point(1, 0, 1), Point(0, 0, 1));
        //Dart_descriptor dh5 = lcc.make_quadrangle(Point(0, 0, 0), Point(0, 1, 0), Point(0, 1, 1), Point(0, 0, 1));
        //Dart_descriptor dh6 = lcc.make_quadrangle(Point(1, 0, 0), Point(1, 1, 0), Point(1, 1, 1), Point(1, 0, 1));

        if(i == 0)
            lcc.sew<2>(dh1, dh4);
        else if(i == 1)
            lcc.sew<2>(dh1, lcc.beta(dh4, 1));
        else if(i == 2)
            lcc.sew<2>(dh1, lcc.beta(dh4, 1, 1));
        else if(i == 3)
            lcc.sew<2>(dh1, lcc.beta(dh4, 1, 1, 1));
        //lcc.sew<2>(lcc.beta(dh4, 1, 1), dh3);
        //lcc.sew<2>(lcc.beta(dh3, 1, 1), lcc.beta(dh2, 1, 1));
        //lcc.sew<2>(lcc.beta(dh2, 1), lcc.beta(dh6, 1));
        //lcc.sew<2>(lcc.beta(dh2, 1, 1, 1), lcc.beta(dh5, 1));

        CGAL::draw(lcc);
    }
}
