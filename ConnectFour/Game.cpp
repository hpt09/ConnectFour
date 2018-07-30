#include "Game.hpp"

Game::Game(){
	// Initialising variables
	board = 0;
	playerOne = 0;
	playerTwo = 0;
	gameStatus = GS_INVALID;
	turn = 0;
	playersDisc = Grid::GC_EMPTY;
}

Game::~Game(){
	// deleting all heap allocated things and assigning them to zero
	delete board;
	board = 0;
}

void Game::setGrid(Grid* grid){
	// Setting board as the grid assigned. Game status becomes in progress if player one and player two are assigned
	if(grid != 0){
		delete board;	// delete previously assigned grid. Safe to delete a null pointer
		board = grid;
		if(playerOne != 0 && playerTwo != 0){
			gameStatus = GS_IN_PROGRESS;
		}
	}
}

void Game::setPlayerOne(Player* player){
	// Setting player one. Game status becomes in progress if grid and player two are assigned
	if(player != 0 && player != playerTwo){
		playerOne = player;
		if(board != 0 && playerTwo != 0){
			gameStatus = GS_IN_PROGRESS;
		}
	}
}

void Game::setPlayerTwo(Player* player){
	// Setting player two. Game status becomes in progress if grid and player one are assigned
	if(player != 0 && player != playerOne){
		playerTwo = player;
		if(playerOne != 0 && board != 0){
			gameStatus = GS_IN_PROGRESS;
		}
	}
}

void Game::restart(){
	// making all the cell is the board as empty
	// making all the player's scores as zero
	// making game status as in progress
	// making turns as zero
	if(gameStatus != GS_INVALID){
		board->reset();
		playerOne->resetScore();
		playerTwo->resetScore();
		gameStatus = GS_IN_PROGRESS;
		turn = 0;
	}
}

Game::Status Game::status() const{
	return gameStatus;
}

const Player* Game::winner() const{
	// If the game status is complete, check for ties which means no winner so return 0
	// If no tie then if turn is divisible by 2 return player two as that means it would player two's turn if the game wasn't completed
	// vice versa for player
	if(gameStatus == GS_COMPLETE){
		if(turn == board->rowCount() * board->columnCount()){
			return 0;
		}
		if(turn % 2 == 0){
			return playerTwo;
		} else{
			return playerOne;
		}
	}
	return 0;
}

const Player* Game::nextPlayer() const{
	// if turn is divisble by 2 then return player one as game starts with player one
	if(gameStatus == GS_IN_PROGRESS){
		if(turn % 2 == 0){
			return playerOne;
		} else{
			return playerTwo;
		}
	}
	return 0;
}

bool Game::checkForWinner(unsigned int column, Grid::Cell disc){
	// find the row of the column (j) which is empty
	int j = -1;
	for(unsigned int i = 0; i < board->rowCount(); i++){
		if(board->cellAt(i, column) == Grid::GC_EMPTY){
			j++;
		}
	}
	j++;	// recent disc inserted

	// Checking for any connect 4s on all directions
	if (check_diagonal_combo_SW_NE(column,j,disc )) {
		return true;
	}
	else if (check_diagonal_combo_NW_SE(column,j,disc )) {
		return true;
	}
	else if (check_vertical_combo(column,j,disc)) {
		return true;
	}
	else if (check_horizontal_combo(column,j,disc )){
		return true;
	}
	else {
		return false;
	}
}

bool Game::check_diagonal_combo_SW_NE(int x, int y, Grid::Cell player){
	int score = 1;
	int count = 1;

	while((y-count >= 0) && (x+count < board->columnCount() )){
		if (board->cellAt(y-count , x+count) == player){	  //Check SW to NE
			score++;
			count++;
		}
		else {
			break;  //If no combo is detected break from the loop
		}
	}

	count = 1;
	while((y+count < board->rowCount() ) && (x-count >=0)){
		if (board->cellAt(y+count, x-count) == player){	//Check NE to SW
			score++;
			count++;
		}
		else {
			break;  //If no combo is detected break from the loop
		}
	}

	if (score >= 4) {	// if score is atleast 4 then there is connect 4
		return true;
	}
	else {
		return false;
	}
}

bool Game::check_diagonal_combo_NW_SE(int x, int y, Grid::Cell player){
	int score = 1;
	int count = 1;

	while((y+count < board->rowCount() ) && (x+count < board->columnCount() )){
		if (board->cellAt(y+count, x+count) == player){	 //Check NW to SE
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	count = 1;
	while((y-count >=0 ) && (x-count >=0)){
		if (board->cellAt(y-count, x-count) == player){	  //Check SE to NW
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	if (score >= 4) {	// if score is atleast 4 then there is connect 4
		return true;
	}
	else {
		return false;
	}
}

bool Game::check_vertical_combo(int x, int y, Grid::Cell player){
	int score = 1;
	int count = 1;

	while(y+count < board->rowCount() ){				// Going south
		if (board->cellAt(y+count, x) == player){
			score++;
			count++;
		}
		else {
			break;  //If no combo is detected break from the loop
		}
	}
	count = 1;

	while(y-count >= 0 ){							// going north
		if (board->cellAt(y-count, x) == player){
			score++;
			count++;
		}
		else {
			break;  //If no combo is detected break from the loop
		}
	}

	if (score >= 4) {	// if score is atleast 4 then there is connect 4
		return true;
	}
	else {
		return false;
	}
}

bool Game::check_horizontal_combo(int x, int y, Grid::Cell player)
{
	int score = 1;
	int count = 1;

	while(x+count < board->columnCount() ){			// going right
		if (board->cellAt(y, x+count) == player){
			score++;
			count++;
		}
		else {
			break;  //If no combo is detected break from the loop
		}
	}

	count = 1;
	while(x-count >=0){				// going left
		if (board->cellAt(y, x-count) == player){
			score++;
			count++;
		}
		else {
			break;  //If no combo is detected break from the loop
		}
	}

	if (score >= 4) {		// if score is atleast 4 then there is connect 4
		return true;
	}
	else {
		return false;
	}
}

bool Game::playNextTurn(unsigned int column){
	// Turn is complete if game is in progress and a disc is inserted
	// If disc is inserted every turn it checks for winner
	// If winner is detected it changes game status and increases score and wins for player and increments turn
	// if turn is equal to row count multiplied by column count means all the moves have been played
	// game status is changed to complete but no wins or scores are updated
	if(gameStatus == GS_IN_PROGRESS){
		if(nextPlayer() == playerOne){
			if(board->insertDisc(column, Grid::GC_PLAYER_ONE)){
				if(checkForWinner(column, Grid::GC_PLAYER_ONE)){
					gameStatus = GS_COMPLETE;
					playerOne->increaseScore();
					playerOne->increaseWins();
				}
				turn++;
				if(turn == board->rowCount() * board->columnCount()){
					gameStatus = GS_COMPLETE;
				}
				return true;
			}
		} else {
			if(board->insertDisc(column, Grid::GC_PLAYER_TWO )){
				if(checkForWinner(column, Grid::GC_PLAYER_TWO)){
					gameStatus = GS_COMPLETE;
					playerTwo->increaseScore();
					playerTwo->increaseWins();
				}
				turn++;
				if(turn == board->rowCount() * board->columnCount()){
					gameStatus = GS_COMPLETE;
				}
				return true;
			}
		}
	} else	{
		return false;
	}
}
