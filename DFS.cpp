#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

struct Node{
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

int DFS(vector<vector<int>> graf, int start, int koniec, vector<int> *odwiedzone, int wezel, int poprzedni) {
	for (int i = 0; i < graf[wezel].size(); i++) {
		if (i != start&&i != poprzedni&&graf[wezel][i]) {
			odwiedzone->push_back(i);
			if (i == koniec)
				return 1;
			else
				return DFS(graf, start, koniec, odwiedzone, i, wezel);
		}
	}
	return 0;
}


int main()
{
	vector <Node> graf_wierzcholki;
	graf_wierzcholki.push_back({ "A",1 });
	graf_wierzcholki.push_back({ "B",2 });
	graf_wierzcholki.push_back({ "C",3 });
	graf_wierzcholki.push_back({ "D",4 });
	graf_wierzcholki.push_back({ "E",5 });

	vector<vector<int>> graf_krawedzie;
	graf_krawedzie = {
		{0,1,1,0,1},
		{1,0,1,0,0},
		{1,1,0,1,0},
		{0,0,1,0,1},
		{1,0,0,1,0}
	};

	// Znajdźmy drogę międzu wierzchołkami A i D
	vector<int> odwiedzone;
	int start = 0;
	int koniec = 3;

	DFS(graf_krawedzie, start, koniec, &odwiedzone, start, start);

	cout << "Droga z wezla " << graf_wierzcholki[start].nazwa << " do wezla " << graf_wierzcholki[koniec].nazwa << " prowadzi przez:" << endl;
	for (auto it = odwiedzone.begin(); it != odwiedzone.end(); it++)
		cout << graf_wierzcholki[*it].nazwa << " ";
	cout << endl;

	return 0;
}

