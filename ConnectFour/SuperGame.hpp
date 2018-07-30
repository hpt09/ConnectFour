#ifndef SUPERGAME_HPP
#define SUPERGAME_HPP

#include "Game.hpp"

class SuperGame : public Game {
public:
	// Plays next turn on a column
	bool playNextTurn(unsigned int column);

	// Inherited from Game class and modified. *Doesn't check for winner*
	// checks for combo and disappears it
	bool checkForWinner(unsigned int column, int j, Grid::Cell disc);

	// checks horizontal combo around a particular cell and stores it in vector
	bool check_horizontal_combo(int x, int y, Grid::Cell player);

	// checks vertical combo around a particular cell and stores it in vector

	bool check_vertical_combo(int x, int y, Grid::Cell player);

	// checks diagonal combo (\) around a particular cell and stores it in vector

	bool check_diagonal_combo_NW_SE(int x, int y, Grid::Cell player);

	// checks diagonal combo (/) around a particular cell and stores it in vector

	bool check_diagonal_combo_SW_NE(int x, int y, Grid::Cell player);

	// Returns the winner

	virtual const Player* winner() const;

private:
	std::vector <int>  xIndex;
	std::vector <int>  yIndex;
};

#endif /* end of include guard: SUPERGAME_HPP */
