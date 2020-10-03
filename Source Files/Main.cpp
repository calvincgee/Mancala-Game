#include "../Header Files/Side.h"
#include "../Header Files/Board.h"
#include "../Header Files/Player.h"
#include "../Header Files/Game.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

int main() {
	int nHoles;
	int beansPerHole;

	cout << "Welcome to Mancala!" << endl;
	cout << "Input the number of holes on your board." << endl;
	cin >> nHoles;
	while (nHoles <= 1) {
		cout << "That's not enough holes." << endl;
		cin >> nHoles;
	}
	cout << "Now input the number of beans per hole at the start of the game." << endl;
	cin >> beansPerHole;
	while (beansPerHole < 1) {
		cout << "That's not enough beans." << endl;
		cin >> beansPerHole;
	}
	Board b(nHoles, beansPerHole);

	char q1;
	string name1;
	string name2;

	cout << "Now choose who is playing the game." << endl;
	cout << "Would you like a human to play? (y/n)" << endl;
	cin >> q1;

	if (q1 == 'y' || q1 == 'Y') {
		cout << "Please enter Player One's name." << endl;
		cin >> name1;
		HumanPlayer p1(name1);
		char q2;
		cout << "Would you like to play against another human? (y/n)" << endl;
		cin >> q2;
		if (q2 == 'y' || q2 == 'Y') {
			cout << "Please enter Player Two's name." << endl;
			cin >> name2;
			HumanPlayer p2(name2);
			Game g(b, &p1, &p2);
			g.play();
		} else {
			char q2;
			cout << "Would you like to play against the Smart AI? (y/n)" << endl;
			cin >> q2;
			if (q2 == 'y' || q2 == 'Y') {
				name2 = "Smart AI";
				SmartPlayer p2(name2);
				Game g(b, &p1, &p2);
				g.play();
				system("pause");
			} else {
				char q3;
				cout << "Would you like to play against the Bad AI? (y/n)" << endl;
				cin >> q3;
				if (q3 == 'y' || q3 == 'Y') {
					name2 = "Bad AI";
					BadPlayer p2(name2);
					Game g(b, &p1, &p2);
					g.play();
					system("pause");
				} else {
					cout << "Well...you can't play with yourself. That's what the Internet is for! Try again." << endl;
					exit(0);
				}
			}
		}
	} else {
		char q2;
		cout << "Would you like Player One to be the Smart AI? (y/n)" << endl;
		cin >> q2;
		if (q2 == 'y' || q2 == 'Y') {
			name1 = "Smart Player One";
			SmartPlayer p1(name1);
			char q3;
			cout << "Would you like Player Two to be the Smart AI? (y/n)" << endl;
			cin >> q3;
			if (q3 == 'y' || q3 == 'Y') {
				name2 = "Smart Player Two";
				SmartPlayer p2(name2);
				Game g(b, &p1, &p2);
				g.play();
				system("pause");
			} else {
				name2 = "Bad Player Two";
				BadPlayer p2(name2);
				Game g(b, &p1, &p2);
				g.play();
				system("pause");
		}
		} else {
			name1 = "Bad Player One";
			BadPlayer p1(name1);
			char q3;
			cout << "Would you like Player Two to be the Smart AI? (y/n)" << endl;
			cin >> q3;
			if (q3 == 'y' || q3 == 'Y') {
				name2 = "Smart Player Two";
				SmartPlayer p2(name2);
				Game g(b, &p1, &p2);
				g.play();
				system("pause");
			} else {
				name2 = "Bad Player Two";
				BadPlayer p2(name2);
				Game g(b, &p1, &p2);
				g.play();
				system("pause");
			}
		}
		
	}
}