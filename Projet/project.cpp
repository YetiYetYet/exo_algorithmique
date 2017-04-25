#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/graphviz.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>           
#include <list>
#include <queue>
#include <string>

using namespace boost;
using namespace std;

typedef struct etablissement{
    string nom;
    int capacite;
} etablissement;

typedef struct eleve{
    string nom;
    string assigne;
    int moyenne;
    int rang;
    list<string> choix;
} eleve;

typedef struct my_e_property{
    bool oui;
    int dist;
    int signature;
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

typedef adjacency_list<listS, vecS, bidirectionalS, point, my_e_property> Graph;

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
            istringstream sstream(line);
            getline(sstream, Veleve.Peleve.nom, ';');
            getline(sstream, acc, ';');
            Veleve.Peleve.moyenne = stoi(acc);
            while(getline(sstream, acc, ';')){
                Veleve.Peleve.choix.push_back(acc);
            }
            Veleve.Peleve.assigne = "";
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
    int signature = 0;

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
                    add_edge(*VecEleve[i], *VecEtablissement[j], {0, dist, signature}, g);
                    add_edge(*VecEtablissement[j], *VecEleve[i], {0, g[*VecEleve[i]].Peleve.rang, -signature}, g);
                    signature++;
                }
            }
            dist++;
        }
    }
}

void doTheThing(Graph &g){
    int signature;
    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
        if(g[*pair_it.first].actual == 0){
            vector<graph_traits<Graph>::out_edge_iterator> VecEdge;

            for(auto pair_itae = out_edges(*pair_it.first, g);
             pair_itae.first != pair_itae.second;
             ++pair_itae.first){
                VecEdge.push_back(pair_itae.first);
            }

            sort(VecEdge.begin(), VecEdge.end(), ([&g] (const graph_traits<Graph>::out_edge_iterator edge1, const graph_traits<Graph>::out_edge_iterator edge2) -> bool {return (g[*edge1].dist < g[*edge2].dist);}));

            for(unsigned int i = 0; (int)i < g[*pair_it.first].Petablissement.capacite && i < VecEdge.size(); i++){
                    g[*VecEdge[i]].oui = 1;
            }
        }
    }
    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
            if(g[*pair_it.first].actual == 1){
            vector<graph_traits<Graph>::out_edge_iterator> VecEdge;

            for(auto pair_itae = out_edges(*pair_it.first, g);
             pair_itae.first != pair_itae.second;
             ++pair_itae.first){
                VecEdge.push_back(pair_itae.first);
            }

            sort(VecEdge.begin(), VecEdge.end(), ([&g] (const graph_traits<Graph>::out_edge_iterator edge1, const graph_traits<Graph>::out_edge_iterator edge2) -> bool {return (g[*edge1].dist < g[*edge2].dist);}));
            if(!VecEdge.empty())
                g[*VecEdge[0]].oui = 1;
        }
    }


    bool ugh;
    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
        if(g[*pair_it.first].actual == 0){
            for(auto pair_itae = out_edges(*pair_it.first, g);
                 pair_itae.first != pair_itae.second;){
                auto acc1 = *pair_itae.first;
                ugh = 1;
                if(g[*pair_itae.first].oui == 1){
                    signature = g[*pair_itae.first].signature;
                    for(auto pair_itae2 = out_edges(target(*pair_itae.first, g), g);
                         pair_itae2.first != pair_itae2.second;){
                        auto acc2 = *pair_itae2.first;
                        if(g[*pair_itae2.first].signature == -signature && g[*pair_itae2.first].oui == 1){
                            ugh = 0;
                            pair_itae.first++;
                            pair_itae2.first++;
                            g[source(acc2, g)].Peleve.assigne = g[target(acc2, g)].Petablissement.nom;
                            g[target(acc2, g)].Petablissement.capacite--;
                            remove_edge(acc1, g);
                            remove_edge(acc2, g);
                        }else{
                            pair_itae2.first++;
                        }
                    }
                }
                if (ugh)
                    pair_itae.first++;
            }
        }
    }

    bool incomprehension;
    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
        if(g[*pair_it.first].actual == 1){
            if(g[*pair_it.first].Peleve.assigne != ""){
                auto pair_itae = out_edges(*pair_it.first, g);
                while(pair_itae.first != pair_itae.second){
                    ugh = 1;
                    auto acc1 = *pair_itae.first;
                    auto pair_itae2 = out_edges(target(acc1, g), g);
                    incomprehension = 0;
                    while(pair_itae2.first != pair_itae2.second){
                        auto acc2 = *pair_itae2.first;
                        if(g[*pair_itae2.first].signature == -g[*pair_itae.first].signature && (!incomprehension)){
                            incomprehension = 1;
                            ugh = 0;
                            pair_itae.first++;
                            pair_itae2.first++;
                            remove_edge(acc1, g);
                            remove_edge(acc2, g);
                        }
                        pair_itae2.first++;
                    }
                    if (ugh)
                        pair_itae.first++;
                }
            } 
        }
    }

    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
        if(g[*pair_it.first].actual == 0){
            if(g[*pair_it.first].Petablissement.capacite == 0){
                auto pair_itae = out_edges(*pair_it.first, g);
                while(pair_itae.first != pair_itae.second){
                    ugh = 1;
                    auto acc1 = *pair_itae.first;
                    auto pair_itae2 = out_edges(target(acc1, g), g);
                    incomprehension = 0;
                    while(pair_itae2.first != pair_itae2.second){
                        auto acc2 = *pair_itae2.first;
                        if(g[*pair_itae2.first].signature == -g[*pair_itae.first].signature && (!incomprehension)){
                            incomprehension = 1;
                            ugh = 0;
                            pair_itae.first++;
                            pair_itae2.first++;
                            remove_edge(acc1, g);
                            remove_edge(acc2, g);
                        }
                        pair_itae2.first++;
                    }
                    if (ugh)
                        pair_itae.first++;
                }
            } 
        }
    }
}


int main(){
    Graph g;
    createPointAndList(g);
    int sec = 100;

    createEdge(g);
    write_graphviz(std::cout, g);
    while(sec--){
        auto pair_it = edges(g);
        if(distance(pair_it.first,pair_it.second)==0){
            sec = 0;
        }else
            doTheThing(g);
            //write_graphviz(std::cout, g);
    }
    //write_graphviz(std::cout, g);
    for (auto pair_it = vertices(g);
     pair_it.first != pair_it.second;
     ++pair_it.first) {
        if(g[*pair_it.first].actual)
            cout << g[*pair_it.first].Peleve.nom << " : " << g[*pair_it.first].Peleve.assigne << endl;
    }
    return 0;
}