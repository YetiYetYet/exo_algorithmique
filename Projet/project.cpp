#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/graphviz.hpp>
#include <vector>
#include <random>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <stdlib.h>     
#include <time.h>       
#include <list>
#include <map>
#include <queue>
#include <string>

using namespace boost;
using namespace std;
//using std::vector;

typedef struct etablissement{
    string nom;
    int capacite;
} etablissement;

typedef struct eleve{
    string nom;
    int moyenne;
    int rang;
    list<string> choix;
} eleve;

typedef struct my_e_property{
    bool oui;
    int dist;
} my_e_property;

typedef struct point {
    bool actual;
    etablissement Petablissement;
    eleve Peleve;
    bool operator<(const point& eleve2) const
    {
        if (Peleve.moyenne < eleve2.Peleve.moyenne)
            return false;
        else
            return true;
    }
}point;

typedef adjacency_list<vecS, vecS, bidirectionalS, point, my_e_property> Graph;

void createPointAndList(Graph &g){
    vector<point> prefEtabl;
    string line, acc;
    point Vetablissement;
    point Veleve;
    int rang = 0;
    ifstream myfile ("etablissement.txt");
    if (myfile.is_open()){
        Vetablissement.actual = 0;
        while ( getline (myfile,line) ){
            istringstream sstream(line);
            getline(sstream, Vetablissement.Petablissement.nom, ';');
            //cout << Vetablissement.Petablissement.nom << endl;
            getline(sstream, acc);
            Vetablissement.Petablissement.capacite = stoi(acc);
            add_vertex(Vetablissement, g);
        }
        myfile.close();
    }
    else 
        cout << "impossible d'ouvrir etablissement.txt" << endl;

    myfile.open("eleve.txt");
    if (myfile.is_open()){
        Veleve.actual = 1;
        while ( getline (myfile,line) ){
            //cout << endl;
            istringstream sstream(line);
            getline(sstream, Veleve.Peleve.nom, ';');
            getline(sstream, acc, ';');
            Veleve.Peleve.moyenne = stoi(acc);
            while(getline(sstream, acc, ';')){
                //cout << acc << endl;
                Veleve.Peleve.choix.push_front(acc);
            }
            prefEtabl.emplace_back(Veleve);
            add_vertex(Veleve, g);
            Veleve.Peleve.choix.clear();
        }
        myfile.close();
    }
    else 
        cout << "impossible d'ouvrir eleve.txt" << endl;

    sort(prefEtabl.begin(), prefEtabl.end());

    for(unsigned int i = 0; i < prefEtabl.size(); i++){
        for (auto pair_it = vertices(g);
         pair_it.first != pair_it.second;
         ++pair_it.first) {
            if(g[*pair_it.first].actual == 1){
                if(g[*pair_it.first].Peleve.nom==prefEtabl[i].Peleve.nom){
                    g[*pair_it.first].Peleve.rang = rang;
                } 
            }
        }
        rang++;
    }
}

void createEdge(Graph &g){
    vector<graph_traits<Graph>::vertex_iterator> VecEleve;
    vector<graph_traits<Graph>::vertex_iterator> VecEtablissement;
    int dist;

    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
        if(g[*pair_it.first].actual == 0)
            VecEtablissement.emplace_back(pair_it.first);
        else if (g[*pair_it.first].actual == 1)
            VecEleve.push_back(pair_it.first);
    }

    for(unsigned int i = 0; i < VecEleve.size(); i++){
        dist = 0;
        list<string>::iterator it;
        for(it=g[*VecEleve[i]].Peleve.choix.begin(); it != g[*VecEleve[i]].Peleve.choix.end(); ++it){
            for(unsigned int j = 0; j < VecEtablissement.size(); j++){
                if(*it == g[*VecEtablissement[j]].Petablissement.nom){
                    add_edge(*VecEleve[i], *VecEtablissement[j], {0, dist}, g);
                    add_edge(*VecEtablissement[j], *VecEleve[i], {0, g[*VecEleve[i]].Peleve.rang}, g);
                }
            }
            dist++;
        }
    }
}

void doTheThing(Graph &g){
    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
        if(g[*pair_it.first].actual == 0){
            //g[*pair_it.first].Petablissement.capacite
            vector<graph_traits<Graph>::out_edge_iterator> VecEdge;

            for(auto pair_itae = out_edges(*pair_it.first, g);
             pair_itae.first != pair_itae.second;
             ++pair_itae.first){
                VecEdge.push_back(pair_itae.first);
            }

            sort(VecEdge.begin(), VecEdge.end(), [&g] (graph_traits<Graph>::edge_descriptor edge1, graph_traits<Graph>::edge_descriptor edge2) -> bool {g[edge1].dist < g[edge2].dist});

            for(int i = 0; i < g[*pair_it.first].Petablissement.capacite; i++){
                g[*VecEdge[i]].oui = 1;
            }
        }
    }
}


int main(/*int argc, char *argv[]*/){
    Graph g;
    createPointAndList(g);

    // for (auto pair_it = vertices(g);
    //  pair_it.first != pair_it.second;
    //  ++pair_it.first) {
    //     cout << g[*pair_it.first].Peleve.nom << " :" << endl;
    //     while(!g[*pair_it.first].Peleve.choix.empty()){
    //         cout << g[*pair_it.first].Peleve.choix.front() << endl;
    //         g[*pair_it.first].Peleve.choix.pop_front();
    //     }
    //     cout << endl;
    // }

    // for(unsigned int i = 0; i < prefEtabl.size();i++)
    //     cout << prefEtabl[i].Peleve.nom << endl;

    // for (auto pair_it = vertices(g);
    //  pair_it.first != pair_it.second;
    //  ++pair_it.first) {
    //     cout << g[*pair_it.first].Peleve.nom << " : " << g[*pair_it.first].Peleve.rang << endl;
    // }


    createEdge(g);
    doTheThing(g);
    write_graphviz(std::cout, g);
    return 0;
}