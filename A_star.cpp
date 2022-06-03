#include <vector>
#include <list>
using namespace std;
//Program znajdujący drogę w grafie na płaszczyźnie, korzystający z algorytmu A*.
struct wezel {
	int x;		// współrzędna x położenia węzła na płaszczyźnie
	int y;		// współrzędna y położenia węzła na płaszczyźnie
	float d;	// odległość przebyta od węzła startowego do tego węzła
	float h;	// heurystyka - ile zostało w linii prostej do węzła końcowego
	float deha;	// suma d i h, używana do wybierania węzłów do sprawdzenia
	int px;		// współrzędna x położenia węzła poprzedzającego bieżący na najkrótszej drodze
	int py;		// współrzędna y położenia węzła poprzedzającego bieżący na najkrótszej drodze
	bool p;		// czy węzeł został już sprawdzony?
};

// Przeciążony operator porównania, używany podczas sortowania list
bool operator < (wezel a, wezel b) {
	if (!a.p && (a.deha < b.deha))
		return true;
	return false;
}

// Tylko sprawdzamy, czy węzeł o podanych współrzędnych znajduje się na wskazanej liście
bool czy_jest(list<wezel>* Q, int x, int y) {
	for (auto it = Q->begin(); it != Q->end(); it++) {
		if (it->x == x)
			if (it->y == y)
				return true;
	}
	return false;
}

// Szukamy węzeła o podanych współrzędnych na wskazanej liście i zwracamy do niego iterator
bool znajdz(list<wezel>* Q, int x, int y, list<wezel>::iterator* w) {
	for (auto it = Q->begin(); it != Q->end(); it++) {
		if (it->x == x)
			if (it->y == y) {
				*w = it;
				return true;
			}
	}
	return false;
}

// Sprawdzamy czy węzeł o podanych współrzędnych w ogóle jest węzłem we wskazanym grafie, a jeśli jest, to czy jest osiągalny
bool czy_mozna(vector<vector<int>>* graf, int x, int y) {
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
bool dodaj_wezel(vector<vector<int>>* graf, int x, int y, int kx, int ky, int bx, int by, list<wezel>* Q, list<wezel>* D) {
	wezel w;
	list<wezel>::iterator it;
	float d = 0;
	if (czy_jest(D, x, y))
		return false;	// nie interesują nas węzły będące na liśczie przetworzonych
	if (!Q->empty())
		d = Q->front().d;	// pobieramy drogę przebytą do węzła z którego tu trafiliśmy
	if (czy_mozna(graf, x, y)) {
		d = d + h(x, y, bx, by);	// obliczamy drogę do węzła który dodajemy przez węzeł z którego tu trafiliśmy
		if (znajdz(Q, x, y, &it)) {	// sprawdzamy, czy dodawany węzeł nie jest aby już na liście do sprawdzenia
			if (d < it->d) {	// relaksacja - sprawdzamy, czy aby nie znaleźliśmy lepszej (krótszej) drogi
				it->d = d;		// tak, znaleźliśmy,
				it->deha = d + it->h;	// więc wszystko przeliczamy
				it->px = bx;	// i zazanaczemy, że prowadzi ona przez
				it->py = by;	// inny węzeł, niż ta poprzednia droga
			}
		}
		else {	// ale jeśli tego węzła nie ma jeszcze na liście do sprawdzenia, to trzeba go dodać
			w.x = x;
			w.y = y;
			w.d = d;
			w.h = h(x, y, kx, ky);
			w.deha = w.d + w.h;
			w.px = bx;
			w.py = by;
			w.p = false;
			Q->push_back(w);
		}
		return true;
	}
	return false;
}

bool A_star(vector<vector<int>>* graf, int sx, int sy, int kx, int ky, list<wezel>* Q, list<wezel>* D) {
	if (Q->empty())	// czy mamy z czym pracować?
		return false;	// Nieee... Tu nic nie ma... :(
	// będzemy tak długo szukać, aż dojdziemy do węzła końcowego LUB lista Q będzie pusta
	while (!((Q->front().x == kx && Q->front().y == ky) || Q->empty())) {
		dodaj_wezel(graf, Q->front().x - 1, Q->front().y - 1, kx, ky, Q->front().x, Q->front().y, Q, D);
		dodaj_wezel(graf, Q->front().x - 1, Q->front().y, kx, ky, Q->front().x, Q->front().y, Q, D);
		dodaj_wezel(graf, Q->front().x - 1, Q->front().y + 1, kx, ky, Q->front().x, Q->front().y, Q, D);
		dodaj_wezel(graf, Q->front().x, Q->front().y - 1, kx, ky, Q->front().x, Q->front().y, Q, D);
		dodaj_wezel(graf, Q->front().x, Q->front().y + 1, kx, ky, Q->front().x, Q->front().y, Q, D);
		dodaj_wezel(graf, Q->front().x + 1, Q->front().y - 1, kx, ky, Q->front().x, Q->front().y, Q, D);
		dodaj_wezel(graf, Q->front().x + 1, Q->front().y, kx, ky, Q->front().x, Q->front().y, Q, D);
		dodaj_wezel(graf, Q->front().x + 1, Q->front().y + 1, kx, ky, Q->front().x, Q->front().y, Q, D);

		Q->front().p = true;
		D->push_back(Q->front());
		Q->pop_front();
		Q->sort();
	}
	if (Q->empty())	// lista Q jest pusta, więc nie znaleźliśmy drogi...
		return false;
	D->push_back(Q->front());	// droga została znaleziona, więc dodajmy węzeł końcowy do listy sprzwdzonych
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

	list<wezel> Q;	// lista węzłów do sprawdzenia
	list<wezel> D;	// lista sprawdzonych węzłów
	dodaj_wezel(&graf, 14, 0, 0, 14, 14, 0, &Q, &D);	// dodajmy węzełstartowy
	A_star(&graf, 14, 0, 0, 14, &Q, &D);				// i poszukajmy drogi w grafie

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

