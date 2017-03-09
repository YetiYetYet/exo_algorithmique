#include <iostream>
#include <vector>
#include "boost/graph/adjacency_list.hpp"
#include <limits>
#include <list>
#include <vector>
#include <map>
#include <queue>

using namespace std;
using namespace boost;

typedef struct my_e_property{
	bool select;
	unsigned int weight;
} my_e_property;

typedef adjacency_list<vecS, vecS, undirectedS, bool, my_e_property> Graph;
typedef graph_traits<Graph>::edge_descriptor edgeD;

Graph 		createGraph(){
	Graph gr;
	auto a = add_vertex(false, gr);
	auto b = add_vertex(false, gr);
	auto c = add_vertex(false, gr);
	auto d = add_vertex(false, gr);
	auto e = add_vertex(false, gr);
	auto f = add_vertex(false, gr);
	auto g = add_vertex(false, gr);
	auto h = add_vertex(false, gr);
	auto i = add_vertex(false, gr);

	add_edge(a,b, {false, 4}, gr);
	add_edge(a,h, {false, 8}, gr);

	add_edge(b,h, {false, 11}, gr);
	add_edge(b,c, {false, 8}, gr);

	add_edge(c,d, {false, 7}, gr);
	add_edge(c,i, {false, 2}, gr);
	add_edge(c,f, {false, 4}, gr);

	add_edge(d,e, {false, 9}, gr);
	add_edge(d,f, {false, 14}, gr);

	add_edge(e,f, {false, 10}, gr);

	add_edge(f,g, {false, 2}, gr);

	add_edge(g,i, {false, 6}, gr);
	add_edge(g,h, {false, 1}, gr);

	add_edge(h,i, {false, 7}, gr);

	return gr;
}

Graph 		createGraph2(){
	Graph gr;
	auto b = add_vertex(false, gr);
	auto o = add_vertex(false, gr);
	auto e = add_vertex(false, gr);
	auto r = add_vertex(false, gr);
	auto sl = add_vertex(false, gr);
	auto l = add_vertex(false, gr);
	auto sd = add_vertex(false, gr);
	auto c = add_vertex(false, gr);

	add_edge(b, o, {false, 5}, gr);
	add_edge(b, e, {false, 18}, gr);
	add_edge(b, r, {false, 9}, gr);
	add_edge(b, sl, {false, 13}, gr);
	add_edge(b, l, {false, 7}, gr);
	add_edge(b, sd, {false, 38}, gr);
	add_edge(b, c, {false, 22}, gr);

	add_edge(o, e, {false, 17}, gr);
	add_edge(o, r, {false, 11}, gr);
	add_edge(o, sl, {false, 7}, gr);
	add_edge(o, l, {false, 12}, gr);
	add_edge(o, sd, {false, 38}, gr);
	add_edge(o, c, {false, 15}, gr);

	add_edge(e, r, {false, 27}, gr);
	add_edge(e, sl, {false, 23}, gr);
	add_edge(e, l, {false, 15}, gr);
	add_edge(e, sd, {false, 20}, gr);
	add_edge(e, c, {false, 25}, gr);

	add_edge(r, sl, {false, 20}, gr);
	add_edge(r, l, {false, 15}, gr);
	add_edge(r, sd, {false, 40}, gr);
	add_edge(r, c, {false, 25}, gr);

	add_edge(sl, l, {false, 15}, gr);
	add_edge(sl, sd, {false, 40}, gr);
	add_edge(sl, c, {false, 30}, gr);

	add_edge(l, sd, {false, 35}, gr);
	add_edge(l, c, {false, 10}, gr);

	add_edge(sd, c, {false, 45}, gr);

	return gr;

}

bool 		allVertexSelected(Graph &g){
	for(auto pair_it = vertices(g);
		pair_it.first != pair_it.second;
		++pair_it.first){
		if(g[*pair_it.first] != 1)
			return 0;
	}
	return 1;
}

Graph 		Prim(){
	Graph g = createGraph2();
	Graph a;
	bool I_AM_AN_ERROR = false;
	priority_queue<edgeD, vector<edgeD>, function<bool(edgeD, edgeD)>> mypq([&g] (edgeD edge1, edgeD edge2) -> bool {if(g[edge1].weight > g[edge2].weight) return true; else return false;});
	auto pair_itv = vertices(g);
	graph_traits<Graph>::vertex_descriptor acc = *pair_itv.first;
	g[acc]=true;

	for(auto pair_itae = out_edges(acc, g);
		pair_itae.first != pair_itae.second;
		++pair_itae.first){
		mypq.push(*pair_itae.first);
	}


	while(!mypq.empty()){
		if(allVertexSelected(g)){
			break;
		}
		if(g[source(mypq.top(), g)] == true && g[target(mypq.top(), g)] && !mypq.empty() == true){
			mypq.pop();
			I_AM_AN_ERROR = true;
		}

		else if(g[source(mypq.top(), g)] == true){
			g[mypq.top()].select = true;
			acc = target(mypq.top(), g);
			g[acc] = true;
			mypq.pop();
		}

		else if(g[target(mypq.top(), g)] == true){
			g[mypq.top()].select = true;
			acc = source(mypq.top(), g);
			g[acc] = true;
			mypq.pop();
		}

		if(!I_AM_AN_ERROR){
			for(auto pair_itae = out_edges(acc, g);
				pair_itae.first != pair_itae.second;
				++pair_itae.first){
				mypq.push(*pair_itae.first);
			}
		}
		else{
			I_AM_AN_ERROR = false;
		}
	}

    a = g;
    auto pair_it = edges(a);

    while(pair_it.first != pair_it.second){
    	edgeD ugh = *pair_it.first;
       	if(g[*pair_it.first].select == false){
    		++pair_it.first;
    		remove_edge(ugh, a);
    	}
    	else{
    		++pair_it.first;
    	}
    }
	return a;
}

void 		show_me_what_you_got(Graph &g){
    for (auto pair_it = vertices(g);
         pair_it.first != pair_it.second;
         ++pair_it.first)
        std::cout << *pair_it.first << ' ';
    std::cout << '\n';

    for (auto pair_it = edges(g);
         pair_it.first != pair_it.second;
         ++pair_it.first)
        std::cout << *pair_it.first << ' ';
    std::cout << '\n';
}

int			main(/*int argc, char *argv[]*/){
	Graph g = Prim();
	show_me_what_you_got(g);
}