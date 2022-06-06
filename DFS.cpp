#include <vector>
#include <iostream>
#include <string>
#include<windows.h>
using namespace std;

struct Node {
	string nazwa;
	int id;
};
struct nodeID {
	int x;
	int y;
};
bool operator == (nodeID a, nodeID b) {
	if (a.x == b.x && a.y == b.y) return true;
	return false;
}

void setColor(int value) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
}

void green_collor() {
	setColor(10);
}
void red_collor() {
	setColor(12);
}
void white_collor() {
	setColor(7);
}

void showGraph(vector<vector<int>>graph) {
	char A_M[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M'};
	for (int i = 0; i < graph.size(); i++) 
		cout << A_M[i]<<" ";
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			if (j == 0) cout << endl;
			if (graph[i][j] == 2) {
				red_collor();
				cout << graph[i][j] << " ";
				white_collor();
			}
			else cout << graph[i][j] << " ";
			if (j == graph[i].size() - 1) cout <<" " <<A_M[i]<<" connect with";
		}
	}
}

// Przeszukiwanie "w głąb":
// 1. wybieramy pierwszego sąsiada do którego możemy pójść
//    !! Pułapka: żeby nie cofnąć się do węzła, z którego przyszliśmy
// 2. idziemy do wybranego sąsiada
//    !! Pułapka: a co, jeśli sąsiad jest punktem startowym?
// 3. Jeśli węzeł w którym jesteśmy jest końcowytm - HURA!, zwracamy sukces
// 4. Jeśli nie, to idziemy do 1.

bool isNotDiscovered(vector<int>* discovered,int y) {
	for (vector<int>::iterator it = discovered->begin(); it != discovered->end(); it++) {
		if (*(it) == y) {
			return false;
		}
	}
	return true; // is not discovered
}
char A_M[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M' };
typedef vector<vector<int>> graph_connections;
int DFS(graph_connections*gc, int y_start, int y_end, vector<int>* discovered, int y_now, int y_earlier) {
	for (int x = 0; x < gc[y_now].size(); x++) {
		cout << A_M[x] <<": [" << y_now << "," << x << "] \n";
		if (x != y_start && x != y_earlier && gc->at(y_now).at(x) == 1 && isNotDiscovered(discovered,x)) {
			gc->at(y_now).at(x) = 2;
			discovered->push_back(x);
			cout << A_M[y_now] << "->" << A_M[x] <<" ";
			cout <<" [" << y_now << "," << x << "] \n";
			return DFS(gc, y_start, y_end, discovered, x, y_now);
		}

		if (x == y_end)
			return 1;
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
		{1,0,1,0,0,0,0,0,0,0,0,0,0}, /*B*/
		{0,1,0,1,0,0,0,0,0,0,0,0,0}, /*C*/
		{1,0,1,0,1,1,0,0,0,0,0,0,0}, /*D*/
		{0,0,0,1,0,0,1,0,0,0,0,0,0}, /*E*/
		{1,0,0,1,0,0,1,1,1,1,0,0,0}, /*F*/
		{0,0,0,0,1,1,0,0,0,0,0,0,0}, /*G*/
		{0,0,0,0,0,1,0,0,0,0,0,0,0}, /*H*/
		{0,0,0,0,0,1,0,0,0,0,0,0,0}, /*I*/
		{0,0,0,0,0,1,0,0,0,0,1,0,0}, /*J*/
		{0,0,0,0,0,0,0,0,0,1,0,1,0}, /*K*/
		{0,0,0,0,0,0,0,0,0,0,1,0,1}, /*L*/
		{1,0,0,0,0,0,0,0,0,0,0,1,0}  /*M*/
	};

	// Znajdźmy drogę międzu wierzchołkami A i D
	vector<int> seenNodesIDs;
	int y_start = 0;
	int y_end = 12;
	showGraph(graph_connections);
	cout << "\n \n";
	DFS(&graph_connections, y_start, y_end, &seenNodesIDs, y_start, y_start);
	//DFSx(&graph_connections, ID_start, ID_end, &seenNodesIDs, ID_start, ID_start);
	//DFSr(&graph_connections, &seenNodesIDs, ID_start, ID_end, ID_start, ID_start);
	//DFSrr(&graph_connections, &seenNodesIDs, ID_start, ID_end, ID_start, ID_start);
	showGraph(graph_connections);

	cout << "Droga z wezla " << graph_nodes[y_start].nazwa << " do wezla " << graph_nodes[y_end].nazwa << " prowadzi przez:" << endl;
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


	a tutaj ten wiekszy 

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

		vector<vector<int>> graph_connections; //graf_krawedzie
	graph_connections = {
	   /*A,B,C,D,E,F,G,H,I,J,K,L,M*/
//{0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, /*A*/
//{ 1,0,1,0,0,0,0,0,0,0,0,0,0 }, /*B*/
//{ 0,1,0,1,0,0,0,0,0,0,0,0,0 }, /*C*/
//{ 1,0,1,0,1,1,0,0,0,0,0,0,0 }, /*D*/
//{ 0,0,0,1,0,0,0,0,0,0,0,0,0 }, /*E*/
//{ 1,0,0,1,0,0,1,1,1,1,0,0,0 }, /*F*/
//{ 0,0,0,0,0,1,0,0,0,0,0,0,0 }, /*G*/
//{ 0,0,0,0,0,1,0,0,0,0,0,0,0 }, /*H*/
//{ 0,0,0,0,0,1,0,0,0,0,0,0,0 }, /*I*/
//{ 0,0,0,0,0,1,0,0,0,0,1,0,0 }, /*J*/
//{ 0,0,0,0,0,0,0,0,0,1,0,1,0 }, /*K*/
//{ 0,0,0,0,0,0,0,0,0,0,1,0,1 }, /*L*/
//{ 1,0,0,0,0,0,0,0,0,0,0,1,0 }  /*M*/
//	};



//*/
