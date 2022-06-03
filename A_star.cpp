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
	float sumDandH;	// suma d i h, używana do wybierania węzłów do sprawdzenia
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
	if (!a.nodeProcessed && (a.sumDandH < b.sumDandH))
		return true;
	return false;
}

void SetColor(int value) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
}

void green_collor() {
	SetColor(10);
}
void red_collor() {
	SetColor(12);
	
}
void white_collor() {
	SetColor(7);
}



// Tylko sprawdzamy, czy węzeł o podanych współrzędnych znajduje się na wskazanej liście
bool czy_jest(list<Node>* Q, int x, int y) {
	for (auto it = Q->begin(); it != Q->end(); it++) {
		if (it->x == x)
			if (it->y == y)
				return true;
	}
	return false;
}

// Szukamy węzeła o podanych współrzędnych na wskazanej liście i zwracamy do niego iterator
bool find(list<Node>* toBeProcessed, int x, int y, list<Node>::iterator* wezel) {
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
float h(int x, int y, int kx, int ky) {
	return sqrt((kx - x) * (kx - x) + (ky - y) * (ky - y));
}

// Funkcja która ma dodać węzeł o podanych współrze∂nych do listy węzłów oczekujących na sprawdzenie
// kx - x dolecelowe - last_x
// ky - y docelowe   - last_y
bool addNode(vector<vector<int>>* graf, int x, int y, int last_x, int last_y, int bx, int by, list<Node>* toBeProcessed, list<Node>* przetworzone) {
	Node w;
	list<Node>::iterator it;
	float d = 0;
	if (czy_jest(przetworzone, x, y))
		return false;	// nie interesują nas węzły będące na liśczie przetworzonych
	if (!toBeProcessed->empty())
		d = toBeProcessed->front().distanceFromFirstNode;	// pobieramy drogę przebytą do węzła z którego tu trafiliśmy
	if (is_xy_reachable(graf, x, y)) {
		d = d + h(x, y, bx, by);	// obliczamy drogę do węzła który dodajemy przez węzeł z którego tu trafiliśmy
		if (find(toBeProcessed, x, y, &it)) {	// sprawdzamy, czy dodawany węzeł nie jest aby już na liście do sprawdzenia
			if (d < it->distanceFromFirstNode) {	// relaksacja - sprawdzamy, czy aby nie znaleźliśmy lepszej (krótszej) drogi
				it->distanceFromFirstNode = d;		// tak, znaleźliśmy,
				it->sumDandH = d + it->h;	// więc wszystko przeliczamy
				it->earlier_x = bx;	// i zazanaczemy, że prowadzi ona przez
				it->earlier_y = by;	// inny węzeł, niż ta poprzednia droga

				// dla wyswietlania drogi
				graf->at(x).at(y) = 9;
			}
		}
		else {	// ale jeśli tego węzła nie ma jeszcze na liście do sprawdzenia, to trzeba go dodać
			w.x = x;
			w.y = y;
			w.distanceFromFirstNode = d;
			w.h = h(x, y, last_x, last_y);
			w.sumDandH = w.distanceFromFirstNode + w.h;
			w.earlier_x = bx;
			w.earlier_y = by;
			w.nodeProcessed = false;
			toBeProcessed->push_back(w);
		}
		return true;
	}
	return false;
}

// sx sy - startowe wspolrzedne - start_x, start_y
// kx ky - wspolrzedne dolecowe
bool A_star(vector<vector<int>>* graph, int start_x, int start_y, int last_x, int last_y, list<Node>* toBeProcessed, list<Node>* processed) {
	if (toBeProcessed->empty())	// czy mamy z czym pracować?
		return false;	// Nieee... Tu nic nie ma... :(
	list<Road> passedRoad;
	// będzemy tak długo szukać, aż dojdziemy do węzła końcowego LUB lista Q będzie pusta
	while (!((toBeProcessed->front().x == last_x && toBeProcessed->front().y == last_y) || toBeProcessed->empty())) {
		addNode(graph, toBeProcessed->front().x - 1, toBeProcessed->front().y - 1, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x - 1, toBeProcessed->front().y, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x - 1, toBeProcessed->front().y + 1, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x, toBeProcessed->front().y - 1, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x, toBeProcessed->front().y + 1, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x + 1, toBeProcessed->front().y - 1, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x + 1, toBeProcessed->front().y, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);
		addNode(graph, toBeProcessed->front().x + 1, toBeProcessed->front().y + 1, last_x, last_y, toBeProcessed->front().x, toBeProcessed->front().y, toBeProcessed, processed);

		toBeProcessed->front().nodeProcessed = true;
		processed->push_back(toBeProcessed->front());
		toBeProcessed->pop_front();
		toBeProcessed->sort();

		// tym zobaczysz wszystkie przetworzone 
		//graf->at(do_przetworzenia->front().x).at(do_przetworzenia->front().y) = 3;
	}
	if (toBeProcessed->empty())	// lista Q jest pusta, więc nie znaleźliśmy drogi...
		return false;
	processed->push_back(toBeProcessed->front());	// droga została znaleziona, więc dodajmy węzeł końcowy do listy sprzwdzonych

	list<Node>::iterator toLastNode;

	for (toLastNode = processed->begin(); toLastNode != processed->end(); ) {
		toLastNode++;
	}
	toLastNode--; // found x,y of last node 
	passedRoad.push_back({ toLastNode->x, toLastNode->y });
	int temp_x = last_x;
	int temp_y = last_y;
	list<Node>::iterator toFirstNode = toLastNode;
	// find road using earlier_x and earlier_y from each Node ago, starting from last Node
	while (temp_x != start_x && temp_y != start_y) {
		int earlier_x = toFirstNode->earlier_x;
		int earlier_y = toFirstNode->earlier_y;
		temp_x = earlier_x;
		temp_y = earlier_y;
		passedRoad.push_back({ earlier_x, earlier_y });
		// find one node before
		for (list<Node>::iterator itt = processed->begin(); itt != processed->end(); itt++) {
			if (itt->x == earlier_x && itt->y == earlier_y) {
				toFirstNode = itt;
			}
		}
	}
	// mark the passed road
	for (list<Road>::iterator markNode = passedRoad.begin(); markNode != passedRoad.end(); markNode++) {
		graph->at(markNode->x).at(markNode->y) = 3;
	}
	//show road parameters (x,y of each road Node) 
	for (list<Road>::iterator it = passedRoad.begin(); it != passedRoad.end(); it++) {
		cout << "(" << it->x << "," << it->y << ")" << endl;
	}
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

