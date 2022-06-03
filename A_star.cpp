#include<iostream>
#include <vector>
#include <list>
#include<windows.h> /*set color*/ 
using namespace std;
//Program znajdujący drogę w grafie na płaszczyźnie, korzystający z algorytmu A*.
struct Node {
	int x;		// współrzędna x położenia węzła na płaszczyźnie
	int y;		// współrzędna y położenia węzła na płaszczyźnie
	float distanceFromFirstNode;	// odległość przebyta od węzła startowego do tego węzła
	float h;	// heurystyka - ile zostało w linii prostej do węzła końcowego
	float sum_D_H;	// suma d i h, używana do wybierania węzłów do sprawdzenia
	int earlier_x;		// współrzędna x położenia węzła poprzedzającego bieżący na najkrótszej drodze
	int earlier_y;		// współrzędna y położenia węzła poprzedzającego bieżący na najkrótszej drodze
	bool nodeProcessed;		// czy węzeł został już sprawdzony?
};

struct Road {
	int x;
	int y;
};

// Przeciążony operator porównania, używany podczas sortowania list
bool operator < (Node a, Node b) {
	if (!a.nodeProcessed && (a.sum_D_H < b.sum_D_H))
		return true;
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

// Tylko sprawdzamy, czy węzeł o podanych współrzędnych znajduje się na wskazanej liście
bool isNodeInList(list<Node>* Q, int x, int y) {
	for (auto it = Q->begin(); it != Q->end(); it++) {
		if (it->x == x)
			if (it->y == y)
				return true;
	}
	return false;
}

// Szukamy węzeła o podanych współrzędnych na wskazanej liście i zwracamy do niego iterator
bool findNode(list<Node>* toBeProcessed, int x, int y, list<Node>::iterator* wezel) {
	for (auto it = toBeProcessed->begin(); it != toBeProcessed->end(); it++) {
		if (it->x == x)
			if (it->y == y) {
				*wezel = it;
				return true;
			}
	}
	return false;
}

// Sprawdzamy czy węzeł o podanych współrzędnych w ogóle jest węzłem we wskazanym grafie, a jeśli jest, to czy jest osiągalny
bool is_xy_reachable(vector<vector<int>>* graf, int x, int y) {
	if (x >= graf->size() || x < 0)
		return false;
	if (y >= graf->at(x).size() || y < 0)
		return false;
	if (graf->at(x).at(y) > 0)
		return true;
	return false;
}

// Funkcja licząca odległość między dwoma wskazanymi węzłami
// heurystka h
float distanceBitweenNodes(int x, int y, int other_x, int other_y) {
	return sqrt((other_x - x) * (other_x - x) + (other_y - y) * (other_y - y));
}

// Funkcja która ma dodać węzeł o podanych współrze∂nych do listy węzłów oczekujących na sprawdzenie
// kx - x dolecelowe -> last_x   // ky - y docelowe   -> last_y   // bx,by -> earlier_x, earlier_y
bool addNode(vector<vector<int>>* graph, int x, int y, int last_x, int last_y, int earlier_x, int earlier_y, list<Node>* toBeProcessed, list<Node>* przetworzone) {
	Node node;
	list<Node>::iterator it;
	float d = 0;
	if (isNodeInList(przetworzone, x, y))
		return false;	// nie interesują nas węzły będące na liśczie przetworzonych
	if (!toBeProcessed->empty())
		d = toBeProcessed->front().distanceFromFirstNode;	// pobieramy drogę przebytą do węzła z którego tu trafiliśmy
	if (is_xy_reachable(graph, x, y)) {
		d = d + distanceBitweenNodes(x, y, earlier_x, earlier_y);	// obliczamy drogę do węzła który dodajemy przez węzeł z którego tu trafiliśmy
		if (findNode(toBeProcessed, x, y, &it)) {	// sprawdzamy, czy dodawany węzeł nie jest aby już na liście do sprawdzenia
			if (d < it->distanceFromFirstNode) {	// relaksacja - sprawdzamy, czy aby nie znaleźliśmy lepszej (krótszej) drogi
				it->distanceFromFirstNode = d;		// tak, znaleźliśmy,
				it->sum_D_H = d + it->h;	// więc wszystko przeliczamy
				it->earlier_x = earlier_x;	// i zazanaczemy, że prowadzi ona przez
				it->earlier_y = earlier_y;	// inny węzeł, niż ta poprzednia droga
				//graf->at(x).at(y) = 9; // dla wyswietlania drogi
			}
		}
		else {	// ale jeśli tego węzła nie ma jeszcze na liście do sprawdzenia, to trzeba go dodać
			node.x = x;
			node.y = y;
			node.distanceFromFirstNode = d;
			node.h = distanceBitweenNodes(x, y, last_x, last_y);
			node.sum_D_H = node.distanceFromFirstNode + node.h;
			node.earlier_x = earlier_x;
			node.earlier_y = earlier_y;
			node.nodeProcessed = false;
			toBeProcessed->push_back(node);
		}
		return true;
	}
	return false;
}

// sx sy - startowe wspolrzedne - start_x, start_y // kx ky - wspolrzedne dolecowe
bool A_star(vector<vector<int>>* graph, int start_x, int start_y, int last_x, int last_y, list<Node>* toBeProcessed, list<Node>* processed) {
	if (toBeProcessed->empty())	// czy mamy z czym pracować?
		return false;	// Nieee... Tu nic nie ma... :(
	list<Road> passedRoad;
	// będzemy tak długo szukać, aż dojdziemy do węzła końcowego LUB lista Q będzie pusta
	while (!((toBeProcessed->front().x == last_x && toBeProcessed->front().y == last_y) || toBeProcessed->empty())) {
		addNode(graph, toBeProcessed->front().x - 1,toBeProcessed->front().y - 1,	last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x - 1,toBeProcessed->front().y,		last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x - 1,toBeProcessed->front().y + 1,	last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x,	toBeProcessed->front().y - 1,	last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x,	toBeProcessed->front().y + 1,	last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x + 1,toBeProcessed->front().y - 1,	last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x + 1,toBeProcessed->front().y,		last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x + 1,toBeProcessed->front().y + 1,	last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		toBeProcessed->front().nodeProcessed = true;
		processed->push_back(toBeProcessed->front());
		toBeProcessed->pop_front();
		toBeProcessed->sort();
		//graf->at(toBeProcessed->front().x).at(toBeProcessed->front().y) = 3; // tym zobaczysz wszystkie przetworzone 
	}
	if (toBeProcessed->empty())	// lista Q jest pusta, więc nie znaleźliśmy drogi...
		return false;
	processed->push_back(toBeProcessed->front());	// droga została znaleziona, więc dodajmy węzeł końcowy do listy sprzwdzonych

	// finding last node coordinates
	list<Node>::iterator toLastNode;
	for (toLastNode = processed->begin(); toLastNode != processed->end(); ) {
		toLastNode++;
	}
	toLastNode--; // found x,y of last node 
	passedRoad.push_front({ toLastNode->x, toLastNode->y });
	int temp_x = last_x;
	int temp_y = last_y;
	list<Node>::iterator toFirstNode = toLastNode;
	// find road using earlier_x and earlier_y from each Node ago, starting from last Node
	while (temp_x != start_x && temp_y != start_y) {
		int earlier_x = toFirstNode->earlier_x;
		int earlier_y = toFirstNode->earlier_y;
		temp_x = earlier_x;
		temp_y = earlier_y;
		passedRoad.push_front({ earlier_x, earlier_y });
		// find a node before, we know its x,y, but need an object to find another node
		for (list<Node>::iterator itt = processed->begin(); itt != processed->end(); itt++) {
			if (itt->x == earlier_x && itt->y == earlier_y) {
				toFirstNode = itt; // found node before
			}
		}
	}
	// mark the passed road
	for (list<Road>::iterator markNode = passedRoad.begin(); markNode != passedRoad.end(); markNode++) {
		graph->at(markNode->x).at(markNode->y) = 3;
	}
	cout << "\n";
	//show road parameters (x,y of each road Node) from end do start
	cout << "poczatek\n";
	for (list<Road>::iterator it = passedRoad.begin(); it != passedRoad.end(); it++) {
		cout << "(" << it->x << "," << it->y << ")" << endl;
	}
	cout << "koniec\n";
	return true;
}

/* stara wersja funkcji */
/*bool A_star(vector<vector<int>> *graf, int sx, int sy, int kx, int ky, list<wezel> *Q) {
	if (Q->empty())
		return false;

	while (!(Q->front().x == kx && Q->front().y == ky)) {
		if (!czy_jest(Q, Q->front().x - 1, Q->front().y - 1))
			dodaj_wezel(graf, Q->front().x - 1, Q->front().y - 1, kx, ky, Q->front().x, Q->front().y, Q);
		if (!czy_jest(Q, Q->front().x - 1, Q->front().y))
			dodaj_wezel(graf, Q->front().x - 1, Q->front().y, kx, ky, Q->front().x, Q->front().y, Q);
		if (!czy_jest(Q, Q->front().x - 1, Q->front().y + 1))
			dodaj_wezel(graf, Q->front().x - 1, Q->front().y + 1, kx, ky, Q->front().x, Q->front().y, Q);
		if (!czy_jest(Q, Q->front().x, Q->front().y - 1))
			dodaj_wezel(graf, Q->front().x, Q->front().y - 1, kx, ky, Q->front().x, Q->front().y, Q);
		if (!czy_jest(Q, Q->front().x, Q->front().y + 1))
			dodaj_wezel(graf, Q->front().x, Q->front().y + 1, kx, ky, Q->front().x, Q->front().y, Q);
		if (!czy_jest(Q, Q->front().x + 1, Q->front().y - 1))
			dodaj_wezel(graf, Q->front().x + 1, Q->front().y - 1, kx, ky, Q->front().x, Q->front().y, Q);
		if (!czy_jest(Q, Q->front().x + 1, Q->front().y))
			dodaj_wezel(graf, Q->front().x + 1, Q->front().y, kx, ky, Q->front().x, Q->front().y, Q);
		if (!czy_jest(Q, Q->front().x + 1, Q->front().y + 1))
			dodaj_wezel(graf, Q->front().x + 1, Q->front().y + 1, kx, ky, Q->front().x, Q->front().y, Q);
		Q->front().p = true;
		Q->sort();
	}
	return true;
}
*/

void showGraph(vector<vector<int>>graf) {
	for (int i = 0; i < graf.size(); i++)
	{
		for (int j = 0; j < graf[i].size(); j++)
		{
			if (j == 0) cout << endl;
			if (graf[i][j] == 3) {
				green_collor();
				cout << graf[i][j]<<" ";
				white_collor();
			}
			else cout << graf[i][j]<<" ";
		}
	}
}

int main()
{
	// A zdfiniujmy sobie taki graf:
	vector<vector<int>> graf = {
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } ,
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,0,1,1,1,1,1 },
	{ 1,1,1,1,0,1,1,1,1,0,1,1,1,1,1 },
	{ 1,1,1,1,0,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,0,0,0,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,0,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,0,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,0,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,0,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,0,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};
	showGraph(graf);
	list<Node> Q;	// lista węzłów do sprawdzenia
	list<Node> D;	// lista sprawdzonych węzłów
	addNode(&graf, 14, 0, 0, 14, 14, 0, &Q, &D);	// dodajmy węzełstartowy
	A_star(&graf, 14, 0, 0, 14, &Q, &D);				// i poszukajmy drogi w grafie
	cout << "po Astar" << endl;
	showGraph(graf);

	// Po wyjściu z funkcji, lista D zawiera wszystkie zbadane węzły.
	// Na jej końcu jest węzeł końcowy, który zawiera namiary na swojego poprzednika
	// na najkrótszej drodze. Teraz trzeba go odnaleźć, a potem jeszcze jego poprzednika,
	// i jeszcze wcześniejszego... Tak długo aż dotrzemy do węzłą startowego.
	// Znaleziona w ten sposób droga będzie rozwiązaniem zadania.
	//
	// Czyli:
	// Trzeba napisać funkcję odnajdującą na liście D znalezioną przez program drogę,
	// a następnie ją wyświetlić.
	// Wystarczy wyświetlić listę współrzednych węzłów od punktu startowego do końcowego.

	return 0;
}
