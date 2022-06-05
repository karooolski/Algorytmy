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
typedef vector<vector<int>> graph_connections;
int DFS(graph_connections*gc, int y_start, int y_end, vector<int>* seen, int y, int y_earlier) {
	for (int x = 0; x < gc[x].size(); x++) {
		cout << " [" << y << "," << x << "] \n";
		if (x != y_start && x != y_earlier && gc->at(y).at(x) == 1) {
			gc->at(y).at(x) = 2;
			seen->push_back(x);
			return DFS(gc, y_start, y_end, seen, x, y);
		}
		//blad : else if(gc->at(x).at(y+1)!=1)return DFS(gc, y_start, y_end, seen, y, x+1);
		//else {if (Y_ID!=ID_end)  }
		//else {gc->at(X_ID).at(Y_ID) = 3;}
		//else { if(!Y_ID==0&&!X_ID==0)Y_ID--; X_ID ++; }
		//else { if (gc->at(x).at(y++) != 1)x++; }
		else gc->at(y).at(x) = 3;
		if (x == y_end)
			return 1;
	}
	return 0;
}

//void DFS(int k)
//{
//	cout << "Odwiedzono " << k << " wierzcholek\n";
//	w[k].odwiedzony = 1;
//	for (int i = 0; i < w[k].polaczenia.size(); i++)
//		if (!w[w[k].polaczenia[i]].odwiedzony)
//			DFS(w[k].polaczenia[i]);
//}

// close but not what i am loking for :(
void DFSx(graph_connections* gc, int ID_start, int ID_end, vector<int>* seen, int X_ID, int earlier_ID) {
	bool changed = false;
	for (int i = X_ID; i < gc->size(); i++)
	{
		for (int j = 0; j < gc[i].size(); j++)
		{
			if (gc->at(i).at(j) == 1) {
				gc->at(i).at(j) = 2;
				seen->push_back(j);
				i++;
				changed = true;
				break;
			}
		}
	}
}
/// <summary>
/// </summary>
/// <param name="gc"></param>
/// <param name="seen"></param>
/// <param name="ID_start"></param>
/// <param name="ID_end"></param>
/// <param name="X_ID"></param>
/// <param name="earlier_ID"></param>
// jumps through some nodes but it is really close though
void DFSr(graph_connections* gc, vector<int>*seen, int ID_start, int ID_end,  int X_ID, int earlier_ID) {
	vector<nodeID>node_ids;
	bool changed = false;
	int x = 0;
	int last_correct_x = 0;
	for (int y = X_ID; y < gc->size(); y++)
	{
		if (changed == true){
			if(y-1>0)
				y--; // undo y++
			changed = false;
		}
		if(!(y<0))
		for (; x < gc[y].size(); x++)
		{
			if (x >= gc->at(y).size()) { x = 0; }
			if (gc->at(y).at(x) == 1) {
				last_correct_x = x;
				cout << " [" << x << "," << y << "] \n";
				gc->at(y).at(x) = 2;
				seen->push_back(y); // push only j,  i mean A,B...M
				y++;
				changed = true;
				break;
			}
			else {
				gc->at(y).at(x) = 3;
			}
		}
	}
}
// it marks '2' from left-upper corner to right-down corner
void DFSrr(graph_connections* gc, vector<int>* seen, int ID_start, int ID_end, int X_ID, int earlier_ID){
	while (ID_start != ID_end+1) {
		gc->at(ID_start).at(X_ID) = 2;
		cout << " [" << X_ID << "," << ID_start << "] \n";
		ID_start++;
		X_ID++;
		//DFSr(gc, seen, ID_start++, ID_end, X_ID, ID_start);
		//DFSr(gc, seen, ID_start, ID_end, X_ID++, ID_start);
	}
	
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
		{0,0,0,1,0,0,0,0,0,0,0,0,0}, /*E*/
		{1,0,0,1,0,0,1,1,1,1,0,0,0}, /*F*/
		{0,0,0,0,0,1,0,0,0,0,0,0,0}, /*G*/
		{0,0,0,0,0,1,0,0,0,0,0,0,0}, /*H*/
		{0,0,0,0,0,1,0,0,0,0,0,0,0}, /*I*/
		{0,0,0,0,0,1,0,0,0,0,1,0,0}, /*J*/
		{0,0,0,0,0,0,0,0,0,1,0,1,0}, /*K*/
		{0,0,0,0,0,0,0,0,0,0,1,0,1}, /*L*/
		{1,0,0,0,0,0,0,0,0,0,0,1,0}  /*M*/
	};

	// Znajdźmy drogę międzu wierzchołkami A i D
	vector<int> seenNodesIDs;
	int ID_start = 0;
	int ID_end = 12;
	showGraph(graph_connections);
	cout << "\n \n";
	DFS(&graph_connections, ID_start, ID_end, &seenNodesIDs, ID_start, ID_start);
	//DFSx(&graph_connections, ID_start, ID_end, &seenNodesIDs, ID_start, ID_start);
	//DFSr(&graph_connections, &seenNodesIDs, ID_start, ID_end, ID_start, ID_start);
	//DFSrr(&graph_connections, &seenNodesIDs, ID_start, ID_end, ID_start, ID_start);
	showGraph(graph_connections);

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
