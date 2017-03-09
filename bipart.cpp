#include <iostream>
#include <map>
#include <vector>
#include <queue>

using namespace std;

//typedef vector<char> next;
typedef map<char, vector<char>> graph;

graph 	creategraph()
{
	graph vertex; 
	vertex['a'] = {'a', 'h'};
	vertex['b'] = {'a', 'h', 'c'};
	vertex['c'] = {'i', 'f', 'd'};
	vertex['d'] = {'c', 'f', 'e'};
	vertex['e'] = {'d', 'f'};
	vertex['f'] = {'g', 'c', 'd', 'e'};
	vertex['g'] = {'f', 'i', 'h'};
	vertex['h'] = {'a', 'b', 'i','g'};
	vertex['i'] = {'c','h','g'};
	return vertex;
}


graph 	creategraphbipart()
{
	graph vertex;
	vertex['a'] = {'e','f'};
	vertex['e'] = {'a'};
	vertex['f'] = {'a','b','d'};
	vertex['b'] = {'f'};
	vertex['d'] = {'g','f'};
	vertex['g'] = {'c','d'};
	vertex['c'] = {'g'};
	return vertex;	
}


void 	breadth_First_Search(graph ugh, char root, int nbVertex)
{
	map<char, bool> seen;
	queue<char> myqueue;
	int i;
	char acc;
	acc = root;
	for(i = 0; i < nbVertex; i++)
	{
		seen[acc] = 0;
		acc++;
	}
	myqueue.push(root);
	while(!myqueue.empty())
	{
		if(seen[ myqueue.front() ] == 0)
		{
			seen[ myqueue.front() ] = 1;
			cout << myqueue.front() << endl;
			for(i = 0; i < ugh[myqueue.front()].size(); i++)
			{
				if(seen[ugh[myqueue.front()][i]] != 1)
					myqueue.push(ugh[myqueue.front()][i]);
			}
		}
		myqueue.pop();
	}
}


bool	bipart(graph ugh, char root, int nbVertex)
{
	map<char, int> color;
	map<char, bool> seen;
	queue<char> myqueue;
	int i;
	char acc;
	acc = root;
	for(i = 0; i < nbVertex; i++)
	{
		color[acc] = 0;
		acc++;
	}
	acc = root;
	for(i = 0; i < nbVertex; i++)
	{
		seen[acc] = 0;
		acc++;
	}
	myqueue.push(root);
	// red = 1, green = 2
	color[myqueue.front()] = 1;
	while(!myqueue.empty())
	{
		if(seen[myqueue.front()]==0)
		{
			seen[myqueue.front()]=1;
			//cout << myqueue.front() << endl;
			for(i=0; i < ugh[myqueue.front()].size(); i++)
			{
				if(seen[ugh[myqueue.front()][i]] != 1)
				{
					myqueue.push(ugh[myqueue.front()][i]);
					if(color[myqueue.front()] == 1)
					{
						if(color[ugh[myqueue.front()][i]] == 1)
							return 0;
						else if(color[ugh[myqueue.front()][i]] == 0)
							color[ugh[myqueue.front()][i]] = 2;
						
					}
					if(color[myqueue.front()] == 2)
					{
						if(color[ugh[myqueue.front()][i]] == 2)
							return 0;
						else if(color[ugh[myqueue.front()][i]] == 0)
							color[ugh[myqueue.front()][i]] = 1;
					}
				}
			}
		}
		myqueue.pop();		
	}
	return 1;
}


int 	main()
{
	graph ugh = creategraph();
	graph ugh2 = creategraphbipart();
	//breadth_First_Search(ugh, 'a', 9);
	cout << "Graph non bipart : " << bipart(ugh, 'a', 9) << endl;

	cout << "Graph Bipart : " << bipart(ugh2, 'a', 7) << endl;
}



