
// Ne fonctionne pas

#include <iostream>
#include <vector>
#include "boost/graph/adjacency_list.hpp"
#include <limits>
#include <list>
#include <vector>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property> Graph;
typedef map<int, int> iimap;

Graph 		createGraph(){
	Graph g;

	int f 	= add_vertex(g);
	int b 	= add_vertex(g);
	int nl 	= add_vertex(g);
	int l 	= add_vertex(g);
	int d 	= add_vertex(g);
	int ch 	= add_vertex(g);
	int i 	= add_vertex(g);

	//France
	add_edge(f, b, g);
	add_edge(f, l, g);
	add_edge(f, d, g);
	add_edge(f, ch, g);
	add_edge(f, i, g);

	//Allemagne
	add_edge(d, nl, g);
	add_edge(d, b, g);
	add_edge(d, l, g);
	add_edge(d, ch, g);

	//Belgique
	add_edge(b, nl, g);
	add_edge(b, l, g);

	//Italie
	add_edge(i, ch, g);

	return g;
}

list<int>	sort(vector<int> myvector, int nb){
	list<int> mylist;
	int acc1;
	acc1 = 0;
	int acc2;
	int i;
	i = 0;
	acc2 = 0;
	while(nb){
		for(auto it = myvector.begin(); it != myvector.end(); ++it){
			if(acc1 < *it){
				acc1 = *it;
				acc2 = i;
				//cout << "test" << i <<  endl;
			}
			i++;
		}
		myvector[acc2]=INT_MIN;
		i=0;
		mylist.push_front(acc2);
		acc1 = 0;
		--nb;
	}
    // cout << "mylist countains :";
    // for(auto it = mylist.begin(); it!=mylist.end(); ++it){
    // 	cout << ' ' << *it;
    // }
    // cout << endl;

	return mylist;
}

bool 		allNotColor(vector <int>, nb){
	int i;
	i = 0;
	while(nb){
		if(vector[i] == 0){
			return true;
		}
		i++;
		nb--;
	}
	return false;
}

vector<int>	isAdjency(Graph g, int nb, int vertices){
	vector<int> adjency;
	for(auto pair_it =adjacent_vertices(vertices, g);
	 pair_it.first !=pair_it.second;
	 ++pair_it.first){
		adjency[*pair_it.first]=1;
	}
}
 
void 		coloring(Graph g, int nb){
	vector<int> color (nb);
	vector<int> adjency;
	list<int> mylist;
	int actualColor = 1;

	vector<int> nbdegree (nb);
	int i;
	i = 0;
    for (auto pair_it = vertices(g);
         pair_it.first != pair_it.second;
         ++pair_it.first) {

        auto sommet = *pair_it.first;
        cout << sommet << ": ";
        for (auto pair_it = adjacent_vertices(sommet, g);
             pair_it.first != pair_it.second;
             ++pair_it.first) {

            cout << *pair_it.first << ' ';
        	nbdegree[i]++;

        }
        cout << '\n';
        i++;

    }
    cout << '\n';
    mylist = sort(nbdegree, nb);

    while(allNotColor(color)){
    	color[mylist.front()] = actualColor;
    	adjency=isAdjency(g, nb, mylist.front());
    	for(auto it = mylist.begin(); it != mylist.end(); ++it){
    		if(adjency[*it] = 0 && color[*it]==0){
    			color[*it] = actualColor;
    		}
    	}


    }


}

int			main(int argc, char *argv[]){
	Graph g = createGraph();
	auto pair_it = vertices(g);
	int p = pair_it.second - pair_it.first;
	cout << p << endl;
	coloring(g, p);
	
}