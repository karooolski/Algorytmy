#include <vector>
#include <iostream>
#include <string>
#include<windows.h>
using namespace std;

struct Node {
	string nazwa;
	int id;
};
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
typedef vector<vector<int>> graph_connections;
void showGraph(graph_connections graph) {
	char A_M[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M'};
	for (int i = 0; i < graph.size(); i++)
		cout << i << " ";
	cout << "(x)";
	cout << "\n";
	for (int i = 0; i < graph.size(); i++) 
		cout << A_M[i]<<" ";
	for (int i = 0; i < graph.size(); i++){
		for (int j = 0; j < graph[i].size(); j++){
			if (j == 0) cout << endl;
			if (graph[i][j] == 2) {
				red_collor();
				cout << graph[i][j] << " ";
				white_collor();
			} else cout << graph[i][j] << " ";
			if (j == graph[i].size() - 1) cout <<" " <<A_M[i]<<" <-connect with,  ("<<i<<"):y";
		}
	}
}

char A_M[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M' };

bool isLeaf(graph_connections *graph,int y) {
	int count = 0;
	int actual = graph->at(y).at(0);
	for (int i = 0; i < graph->at(y).size(); i++) {
		if (graph->at(y).at(i) == 1) {
			actual = graph->at(y).at(i);
			count++;
		}	
	}
	if (count == 1)
		return true;
	return false;
}

bool noOnesHere(graph_connections *graph, int y) { 
	for (int i = 0; i < graph->size(); i++) {
		if (graph->at(y).at(i) == 1) {
			return false;
		}
	}
	return true; // there is no '1' in single lane (x) of (y:height) because '1' is marked as '2'-> to be collored, its marked leaf btw.
}

int DFS(graph_connections *graph, vector<int>*discovered,int x,int y,int y_end,int earlier_x,int earlier_y) {
	system("CLS");
	showGraph(graph[0]);
	if (y >= y_end || y >= graph->size()-1) return 0;
	if (noOnesHere(graph, y)) { // it means only one '1' is marked as '2' and other are zeros
		// sometimes is need to jump over marked leafs (MARKED as '2', red collor) 
		return DFS(graph, discovered,x,y + 1, y_end, earlier_x, earlier_y);
		return DFS(graph, discovered,x+1, y , y_end, earlier_x, earlier_y);
	}
	if (isLeaf(graph, y)) { // logic when i am in a leaf 
		int temp_x = 0;
		int temp_wynik = graph->at(y).at(temp_x);
		while (temp_wynik != 1) { // find '1' in leaf and mark it, then return where you were
			temp_wynik = graph->at(y).at(temp_x);
			temp_x++;
		}
		temp_x--;
		graph->at(y).at(temp_x) = 2;
		discovered->push_back(y);
		system("CLS");
		showGraph(graph[0]);
		return DFS(graph, discovered, earlier_x + 1, earlier_y, y_end, earlier_x, earlier_y);
	}
	for (int i = x; i < graph->size(); i++) { // iterate trough whole (x) and find '1'
		if (graph->at(y).at(i) == 1) {
				discovered->push_back(y);
				graph->at(y).at(i) = 2; earlier_x = x; earlier_y = y;
				system("CLS"); 
				showGraph(graph[0]); cout << "\n";
				return DFS(graph, discovered, x + 1, y + 1, y_end,earlier_x,earlier_y); // go right and down
		}
	}
}

int main()
{
	vector <Node> graph_abc; // graf_wierzcholki // node info
	graph_abc.push_back({ "A",0 });
	graph_abc.push_back({ "B",1 });
	graph_abc.push_back({ "C",2 });
	graph_abc.push_back({ "D",3 });
	graph_abc.push_back({ "E",4 });
	graph_abc.push_back({ "F",5 });
	graph_abc.push_back({ "G",6 });
	graph_abc.push_back({ "H",7 });
	graph_abc.push_back({ "I",8 });
	graph_abc.push_back({ "J",9 });
	graph_abc.push_back({ "K",10});
	graph_abc.push_back({ "L",11});
	graph_abc.push_back({ "M",12});
	/* How to read: Example: 
	connections:

	 A,B,C,D,E  
	 0,1,1,0,1		A with	
	 1,0,1,0,0		B with 

	 A is connected with B,C,E
	 B is connected with A,C
	 0-not connected, 1 - connected
	*/
	graph_connections graph; //graf_krawedzie

	graph_connections graph_first = {
	{0,1,1,0,1},
	{1,0,1,0,0},
	{1,1,0,1,0},
	{0,0,1,0,1},
	{1,0,0,1,0}
	};

	graph = {
		/*0,1,2,3,4,5,6,7,8,9,10,11,12*/
		/*A,B,C,D,E,F,G,H,I,J,K,L,M*/
		 {0,1,0,1,0,1,1,0,0,0,0,0,0}, /*A  0*/
		 {1,0,1,0,0,0,0,0,0,0,0,0,0}, /*B  1*/
		 {0,1,0,1,0,0,0,0,0,0,0,0,0}, /*C  2*/
		 {1,0,1,0,1,1,1,0,0,0,0,0,0}, /*D  3*/
		 {0,0,0,1,0,1,1,0,0,0,0,0,0}, /*E  4*/
		 {0,0,0,1,1,0,1,0,0,0,0,0,0}, /*F  5*/
		 {1,0,0,1,0,0,1,1,1,1,0,0,0}, /*G  6*/
		 {0,0,0,0,0,1,0,0,0,0,0,0,0}, /*H  7*/
		 {0,0,0,0,0,1,0,0,0,0,0,0,0}, /*I  8*/
		 {0,0,0,0,0,1,0,0,0,0,1,0,0}, /*J  9*/
		 {0,0,0,0,0,0,0,0,0,1,0,1,0}, /*K 10*/
		 {0,0,0,0,0,0,0,0,0,0,1,0,1}, /*L 11*/
		 {1,0,0,0,0,0,0,0,0,0,0,1,0}  /*M 12*/
	};

	// Znajdźmy drogę międzu wierzchołkami A i M
	vector<int> discovered;
	int y_start = 0;
	int y_end = 12;
	showGraph(graph);
	cout << "\n \n"; 
	DFS(&graph, &discovered, 0, 0, y_end,0,0);
	// showGraph(graph); //to juz sie wyswietla w funkcji
	cout << "\nDroga z wezla " << graph_abc[y_start].nazwa << " do wezla " << graph_abc[y_end].nazwa << " prowadzi przez:\n";
	for (auto it = discovered.begin(); it != discovered.end(); it++)
		cout << graph_abc[*it].nazwa << " ";
	cout << endl;

	return 0;
}

//bool yIsDiscovered(vector<int>* discovered,int y) {
//	for (int i = 0; i < discovered->size(); i++) {
//		if (discovered->at(i) == y)
//			return true;
//	}
//	return false;
//}

// Przeszukiwanie "w głąb":
// 1. wybieramy pierwszego sąsiada do którego możemy pójść
//    !! Pułapka: żeby nie cofnąć się do węzła, z którego przyszliśmy
// 2. idziemy do wybranego sąsiada
//    !! Pułapka: a co, jeśli sąsiad jest punktem startowym?
// 3. Jeśli węzeł w którym jesteśmy jest końcowytm - HURA!, zwracamy sukces
// 4. Jeśli nie, to idziemy do 1.

//bool isNotDiscovered(vector<int>* discovered,int y) {
//	for (vector<int>::iterator it = discovered->begin(); it != discovered->end(); it++) {
//		if (*(it) == y) {
//			return false;
//		}
//	}
//	return true; // is not discovered
//}

// kod z zajec
//int DFS(graph_connections*gc, int y_start, int y_end, vector<int>* discovered, int y_now, int y_earlier) {
//	for (int x = 0; x < gc[y_now].size(); x++) {
//		cout << A_M[x] <<": [" << y_now << "," << x << "] \n";
//		if (x != y_start && x != y_earlier && gc->at(y_now).at(x) == 1 && isNotDiscovered(discovered,x)) {
//			gc->at(y_now).at(x) = 2;
//			discovered->push_back(x);
//			cout << A_M[y_now] << "->" << A_M[x] <<" ";
//			cout <<" [" << y_now << "," << x << "] \n";
//			return DFS(gc, y_start, y_end, discovered, x, y_now);
//		}
//
//		if (x == y_end)
//			return 1;
//	}
//	return 0;
//}



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



		//DFS(&graph_connections, y_start, y_end, &seenNodesIDs, y_start, y_start);
	//DFSx(&graph_connections, ID_start, ID_end, &seenNodesIDs, ID_start, ID_start);
	//DFSr(&graph_connections, &seenNodesIDs, ID_start, ID_end, ID_start, ID_start);
	//DFSrr(&graph_connections, &seenNodesIDs, ID_start, ID_end, ID_start, ID_start);
	//DFSx(&graph, &seenNodesIDs, 0,0,y_end,0);


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





//bool forward_zeros(graph_connections* graph, int y, int x) {
//	for (int i = x; i < graph->size(); i++) {
//		if (graph->at(y).at(i) == 1 )
//		return false;
//	}
//	return true;
//}
// // nie do konca dziala
//int DFSx(graph_connections *graph,vector<int> *discovered, int y,int x, int y_end, int przesuniecie){
//	if (forward_zeros(graph, y, x))
//		if(graph->at(y).at(x) != 1)
//			if (!(x - 1 < 0))
//				return DFSx(graph, discovered, y, x - 1, y_end, 1);
//	if (graph->at(y).at(x) == 0)
//		if (x + 1 < graph->at(y).size()) {
//			przesuniecie++;
//			return DFSx(graph, discovered, y, x + 1, y_end,przesuniecie);
//		}
//	if (graph->at(y).at(x) == 1)
//		if (y + przesuniecie < graph->size()) {
//			int tmp = przesuniecie;
//			przesuniecie = 1;
//			discovered->push_back(y);
//			graph->at(y).at(x) = 2;
//			return DFSx(graph, discovered, y + tmp, x+1, y_end,przesuniecie);
//		}
//	
//	if (isLeaf(graph, y))
//		if (y - 1 < graph->size())
//			if (x + 1 < graph->at(y).size())
//				return DFSx(graph, discovered, y - 1, x + 1, y_end,przesuniecie);
//	if (y == y_end) {
//		cout << y; 
//		return 0;
//	}
//		
//}

//struct nodeID {
//	int x;
//	int y;
//};
//bool operator == (nodeID a, nodeID b) {
//	if (a.x == b.x && a.y == b.y) return true;
//	return false;
//}




//*/
