#include "Side.h"
#include "Board.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
	HumanPlayer hp("human");
	BadPlayer bp("Bad AI");
	SmartPlayer sp("Smart AI");
	SmartPlayer sp2("Smart AI2");
	Board b(6, 4);
	Game g(b, &hp, &sp);
	g.play();
	system("pause");
}