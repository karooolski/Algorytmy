#include <vector>
#include <iostream>
#include <string>
using namespace std;

struct Node {
	string nazwa;
	int id;
};

// Przeszukiwanie "w głąb":
// 1. wybieramy pierwszego sąsiada do którego możemy pójść
//    !! Pułapka: żeby nie cofnąć się do węzła, z którego przyszliśmy
// 2. idziemy do wybranego sąsiada
//    !! Pułapka: a co, jeśli sąsiad jest punktem startowym?
// 3. Jeśli węzeł w którym jesteśmy jest końcowytm - HURA!, zwracamy sukces
// 4. Jeśli nie, to idziemy do 1.

int DFS(vector<vector<int>> graph_connections, int ID_start, int ID_end, vector<int>* seen, int X_ID, int earlier_ID) {
	for (int Y_ID = 0; Y_ID < graph_connections[X_ID].size(); Y_ID++) {
		if (Y_ID != ID_start && Y_ID != earlier_ID && graph_connections[X_ID][Y_ID] == 1) {
			seen->push_back(Y_ID);
			if (Y_ID == ID_end)
				return 1;
			else
				return DFS(graph_connections, ID_start, ID_end, seen, Y_ID, X_ID);
		}
	}
	return 0;
}


int main()
{
	vector <Node> graph_nodes; // graf_wierzcholki // node info
	graph_nodes.push_back({ "A",0 });
	graph_nodes.push_back({ "B",1 });
	graph_nodes.push_back({ "C",2 });
	graph_nodes.push_back({ "D",3 });
	graph_nodes.push_back({ "E",4 });
	graph_nodes.push_back({ "F",5 });
	graph_nodes.push_back({ "G",6 });
	graph_nodes.push_back({ "H",7 });
	graph_nodes.push_back({ "I",8 });
	graph_nodes.push_back({ "J",9 });
	graph_nodes.push_back({ "K",10});
	graph_nodes.push_back({ "L",11});
	graph_nodes.push_back({ "M",12});
	/*
	connections:

	{A,B,C,D,E}   A with
	{A,B,C,D,E}   B with etc, 0-not connected, 1 - connected

	*/

	vector<vector<int>> graph_connections; //graf_krawedzie
	graph_connections = {
	   /*A,B,C,D,E,F,G,H,I,J,K,L,M*/
		{0,1,0,1,0,1,0,0,0,0,0,0,0}, /*A*/
		{1,0,1,0,0,0,0,0,0,0,0,0,0},
		{0,1,0,1,0,0,0,0,0,0,0,0,0},
		{1,0,1,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,0,0},
		{1,0,0,1,0,0,1,1,1,1,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0,0,0}, /*G*/
		{0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1,0}
	};

	// Znajdźmy drogę międzu wierzchołkami A i D
	vector<int> seenNodesIDs;
	int ID_start = 0;
	int ID_end = 12;

	DFS(graph_connections, ID_start, ID_end, &seenNodesIDs, ID_start, ID_start);

	cout << "Droga z wezla " << graph_nodes[ID_start].nazwa << " do wezla " << graph_nodes[ID_end].nazwa << " prowadzi przez:" << endl;
	for (auto it = seenNodesIDs.begin(); it != seenNodesIDs.end(); it++)
		cout << graph_nodes[*it].nazwa << " ";
	cout << endl;

	return 0;
}

/*
graf poczatkowy

	vector<vector<int>> graph_connections; //graf_krawedzie
	graph_connections = {
		{0,1,1,0,1},
		{1,0,1,0,0},
		{1,1,0,1,0},
		{0,0,1,0,1},
		{1,0,0,1,0}
	};

*/
