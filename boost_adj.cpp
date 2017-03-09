#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp> 
#include <iostream>

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property> Graph;


int main() {
    Graph g;

    auto a = add_vertex(g); 
    auto b = add_vertex(g); 
    auto c = add_vertex(g); 
    add_edge(a, b, g);
    add_edge(c, b, g);
    add_edge(a, c, g);

    // Parcours des sommets
    for (auto pair_it = vertices(g);
         pair_it.first != pair_it.second;
         ++pair_it.first)
        std::cout << *pair_it.first << ' ';
    std::cout << '\n';

    // Parcours des arêtes
    for (auto pair_it = edges(g);
         pair_it.first != pair_it.second;
         ++pair_it.first)
        std::cout << *pair_it.first << ' ';
    std::cout << '\n';

    // Parcours des sommets et des adjacents
    for (auto pair_it = vertices(g);
         pair_it.first != pair_it.second;
         ++pair_it.first) {
        auto sommet = *pair_it.first;
        std::cout << sommet << ": ";
        for (auto pair_it = adjacent_vertices(sommet, g);
             pair_it.first != pair_it.second;
             ++pair_it.first) {
            std::cout << *pair_it.first << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    std::cout << is_bipartite(g) << '\n';

    return 0;
}
