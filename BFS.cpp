#include <iostream>
#include <string>
#include <list>

using namespace std;

struct Node {
	string nazwa;
	int id;
	list<int> sasiedzi;
};

int BFS(list<Node>* graf, list<int>* odwiedzone, string nazwa_wezla) {
	list<int> do_odwiedzenia;
	for (auto it = graf->begin(); it != graf->end(); it++) {
		if (it->nazwa == nazwa_wezla) {	// znajdźmy węzeł o wskazanej nazwie
			do_odwiedzenia.push_back(it->id);	// i dodajmy go na listę do sprawdzenia
			break;	// skoro znaleźliśmy, to nie ma sensu szukać dalej
		}
	}

	while (do_odwiedzenia.size()) {	// algorytm działa, póki ma co sprawdzać
		bool rob_dalej = true;
		for (auto it = odwiedzone->begin(); it != odwiedzone->end(); it++)
			if (*it == do_odwiedzenia.front()) { // sprawdzamy, czy tego węzła nie odwiedziliśmy wcześniej
				rob_dalej = false;
				do_odwiedzenia.pop_front();	// jeżli go odqiedziliśmy, to już nas nie interesuje
				break;
			}
		if (!rob_dalej)
			continue;	// ten węzeł nas nie interesuje, sprawdźmy następny

		odwiedzone->push_back(do_odwiedzenia.front());	// jednak nas interesuje - zbadajmy go
		for (auto it = graf->begin(); it != graf->end(); it++) {	// najpierw znajdźmy go w grafie
			if (it->id == do_odwiedzenia.front()) {
				for (auto it2 = it->sasiedzi.begin(); it2 != it->sasiedzi.end(); it2++)
					do_odwiedzenia.push_back(*it2);	// a potem przepiszmy jego sąsiadów jako potencjalne węzły, które trzeba zbadać
				break;
			}
		}
		do_odwiedzenia.pop_front();
	}

	return 0;
}


int main()
{
	list<Node> graf;
	list<int>odwiedzone;
	//Node t;
	//t.nazwa = "A";
	//t.id = 1;
	//t.sasiedzi.push_back(2);
	//t.sasiedzi.push_back(3);
	//t.sasiedzi.push_back(5);
	//graf.push_back(t);

	graf.push_back({ "A", 1,{2,7} });
	graf.push_back({ "B", 2,{1,3} });
	graf.push_back({ "C", 3,{2,4} });
	graf.push_back({ "D", 4,{3,5} });
	graf.push_back({ "E", 5,{4,6,7} });
	graf.push_back({ "F", 6,{5} });
	graf.push_back({ "G", 7,{1,5,8,9} });
	graf.push_back({ "H", 8,{7} });
	graf.push_back({ "I", 9,{7,10} });
	graf.push_back({ "J", 10,{9,11} });
	graf.push_back({ "K", 11,{10} });
	//graf.push_back({ "A", 1,{2,3,5}});
	//graf.push_back({ "B",2,{ 1,3 } });
	//graf.push_back({ "C",3,{ 1,2,4 }});
	//graf.push_back({ "D",4,{ 3,5 } });
	//graf.push_back({ "E",5,{ 1,4 } });
	BFS(&graf, &odwiedzone, "K");
	cout << "Odwiedzone wezly: \n ";
	for (list<int>::iterator it = odwiedzone.begin(); it != odwiedzone.end(); it++) {
		cout << *it << "\t";
	}
	cout << "\n" << "  ";
	char nazwy[] = {'0','A','B','C','D','E','F','G','H','I','J','K' }; // A->id:1
	for (list<int>::iterator it = odwiedzone.begin(); it != odwiedzone.end(); it++) {
		cout << nazwy[*it] << "\t";
	}
	return 0;
}

