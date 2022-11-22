#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/draw_linear_cell_complex.h>

typedef CGAL::Linear_cell_complex_for_combinatorial_map<2> LCC;
typedef LCC::Dart_descriptor Dart_descriptor;
typedef LCC::Point           Point;
typedef LCC::Vertex_attribute_range Vertex_attribute_range;
typedef LCC::Vertex_attribute Vertex_attribute;

int main()
{
    LCC lcc;

    Dart_descriptor dh1 = lcc.make_quadrangle(Point(0, 0),
                                          Point(1, 0), 
                                          Point(1, 1),
                                          Point(0, 1));

    Vertex_attribute_range vertex_range = lcc.vertex_attributes();

    for(Vertex_attribute_range::iterator start = vertex_range.begin(), end = vertex_range.end(); start != end ;start++)
        std::cout << lcc.point_of_vertex_attribute(start) << std::endl;

    CGAL::draw(lcc);
}