#include "SuperGame.hpp"

bool SuperGame::playNextTurn(unsigned int column){
	// Does the same at first finds the disc inserted position and look connect 4 combos
	// If found increases the score of the corresponding player and falls down the discs
	if(gameStatus == GS_IN_PROGRESS){
		bool playerOneCombo = false;
		bool playerTwoCombo = false;
		if(nextPlayer() == playerOne){
			if(board->insertDisc(column, Grid::GC_PLAYER_ONE)){

				int j = -1;
				for(unsigned int i = 0; i < board->rowCount(); i++){
					if(board->cellAt(i, column) == Grid::GC_EMPTY){
						j++;
					}
				}
				j++;	// recent disc inserted

				if(checkForWinner(column, j, Grid::GC_PLAYER_ONE)){
					playerOne->increaseScore();
					board->fallDown();
					// Goes through a nested loop to see if any more connect 4s been made at any row and column of the grid
					for(int i = 0; i < board->rowCount(); i++){
						for(int j = 0; j< board->columnCount(); j++){
							if(board->cellAt(i, j) != Grid::GC_EMPTY){		// ignores the empty cells
								Grid::Cell comboCell = board->cellAt(i, j);	// saves the cell
								if(checkForWinner(j, i, board->cellAt(i, j))){	// if theres combo

									if(comboCell == Grid::GC_PLAYER_ONE){		// if combocell was player one therefore there is player one combos
										playerOneCombo = true;					// sets the bool playeronecombo as true
									} else {
										playerTwoCombo = true;					// vice versa for player two
									}

								}
							}
							// Check for every row and combo if it has gone through every row and combo
							// which would make i as rowcount -1 and j as column cout - 1 on the last iteration
							// Also need to check if there were any combo for player one OR player two
							// If there were player one combos in the turn then increment score for player one
							// make the bool playerone combo as false and fall down the cells
							// reset i and j to see if any more combo have been made after falling down
							// increment turn and vice versa for player 2
							// Check if theres more moves which means there is a tie and game is complete
							if(i == board->rowCount() - 1 && j == board->columnCount() - 1 && (playerOneCombo || playerTwoCombo)){
								if(playerOneCombo){
									playerOneCombo = false;
									playerOne->increaseScore();
								}
								if(playerTwoCombo){
									playerTwoCombo = false;
									playerTwo->increaseScore();
								}
								board->fallDown();
								i = 0; j = 0;
							}
						}
					}
				}
				turn++;
				if(board->noMoreMoves()){
					gameStatus = GS_COMPLETE;
					if(playerOne->getScore() > playerTwo->getScore()){
						playerOne->increaseWins();
					} else if(playerOne->getScore() < playerTwo->getScore()){
						playerOne->increaseWins();
					}
				}
				return true;
			}
		} else {
			if(board->insertDisc(column, Grid::GC_PLAYER_TWO )){
				int j = -1;
				for(unsigned int i = 0; i < board->rowCount(); i++){
					if(board->cellAt(i, column) == Grid::GC_EMPTY){
						j++;
					}
				}
				j++;	// recent disc inserted

				if(checkForWinner(column, j, Grid::GC_PLAYER_TWO)){
					playerTwo->increaseScore();
					//playerTwoCombo = true;

					board->fallDown();

					for(int i = 0; i < board->rowCount(); i++){
						for(int j = 0; j< board->columnCount(); j++){
							Grid::Cell comboCell = board->cellAt(i, j);
							if(board->cellAt(i, j) != Grid::GC_EMPTY){
								if(checkForWinner(j, i, board->cellAt(i, j))){

									if(comboCell == Grid::GC_PLAYER_ONE){
										playerOneCombo = true;
									} else {
										playerTwoCombo = true;
									}
								}
							}
							if(i == board->rowCount() - 1 && j == board->columnCount() - 1 && (playerOneCombo || playerTwoCombo)){
								if(playerOneCombo){
									playerOne->increaseScore();
									playerOneCombo = false;
								}
								if(playerTwoCombo){
									playerTwo->increaseScore();
									playerTwoCombo = false;
								}
								board->fallDown();
								i = 0; j = 0;
							}

						}
					}
				}
				turn++;
				if(board->noMoreMoves()){
					gameStatus = GS_COMPLETE;
					if(playerOne->getScore() > playerTwo->getScore()){
						playerOne->increaseWins();
					} else if(playerOne->getScore() < playerTwo->getScore()){
						playerTwo->increaseWins();
					}
				}
				return true;
			}
		}
	} else	{
		return false;
	}

}

bool SuperGame::checkForWinner(unsigned int column, int j, Grid::Cell disc){
	// Check for combos in all directions
	// make all the cell excluding the one we're checking as empty so we can check other directions with that cell as well
	bool combo = false;
	if (check_diagonal_combo_SW_NE(column,j,disc )) {
		for(int i = 0; i < xIndex.size(); i++){
			board->makeEmptyCell(xIndex[i],yIndex[i]);
		}
		combo = true;
	}
	if (check_diagonal_combo_NW_SE(column,j,disc )){
		for(int i = 0; i < xIndex.size(); i++){
			board->makeEmptyCell(xIndex[i],yIndex[i]);
		}
		combo = true;
	}

	if (check_vertical_combo(column,j,disc)){
		for(int i = 0; i < xIndex.size(); i++){
			board->makeEmptyCell(xIndex[i],yIndex[i]);
		}
		combo = true;
	}
	if (check_horizontal_combo(column,j,disc )){
		for(int i = 0; i < xIndex.size(); i++){
			board->makeEmptyCell(xIndex[i],yIndex[i]);
		}
		combo = true;
	}
	// If there were combos then make the cell we were looking for combos as empty too
	if(combo == true){
		board->makeEmptyCell(column,j);
	}
	return combo;
}

bool SuperGame::check_diagonal_combo_SW_NE(int x, int y, Grid::Cell player){
	yIndex.clear();
	xIndex.clear();
	int score = 1;
	int count = 1;

	while((y-count >= 0) && (x+count < board->columnCount() )){
		if (board->cellAt(y-count , x+count) == player){	  //Check SW to NE
			yIndex.push_back(y-count);
			xIndex.push_back(x+count);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	count = 1;
	while((y+count < board->rowCount() ) && (x-count >=0)){
		if (board->cellAt(y+count, x-count) == player){	//Check NE to SW
			yIndex.push_back(y+count);
			xIndex.push_back(x-count);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	if (score >= 4) return true;
	else return false;
}

bool SuperGame::check_diagonal_combo_NW_SE(int x, int y, Grid::Cell player){
	yIndex.clear();
	xIndex.clear();
	int score = 1;
	int count = 1;

	while((y+count < board->rowCount() ) && (x+count < board->columnCount() )){
		if (board->cellAt(y+count, x+count) == player){	 //Check NW to SE
			yIndex.push_back(y+count);
			xIndex.push_back(x+count);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	count = 1;
	while((y-count >=0 ) && (x-count >=0)){
		if (board->cellAt(y-count, x-count) == player){	  //Check SE to NW
			yIndex.push_back(y-count);
			xIndex.push_back(x-count);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	if (score >= 4) return true;
	else return false;
}

bool SuperGame::check_vertical_combo(int x, int y, Grid::Cell player){
	yIndex.clear();
	xIndex.clear();
	int score = 1;
	int count = 1;

	while(y+count < board->rowCount() ){				// Going south
		if (board->cellAt(y+count, x) == player){
			yIndex.push_back(y+count);
			xIndex.push_back(x);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}
	count = 1;

	while(y-count >= 0 ){							// going north
		if (board->cellAt(y-count, x) == player){
			yIndex.push_back(y-count);
			xIndex.push_back(x);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	if (score >= 4) return true;
	else return false;
}

bool SuperGame::check_horizontal_combo(int x, int y, Grid::Cell player){
	yIndex.clear();
	xIndex.clear();
	int score = 1;
	int count = 1;

	while(x+count < board->columnCount() ){
		if (board->cellAt(y, x+count) == player){
			yIndex.push_back(y);
			xIndex.push_back(x+count);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	count = 1;
	while(x-count >=0){
		if (board->cellAt(y, x-count) == player){
			yIndex.push_back(y);
			xIndex.push_back(x-count);
			score++;
			count++;
		}
		else break;  //If no combo is detected break from the loop
	}

	if (score >= 4) return true;
	else return false;
}

const Player* SuperGame::winner() const{
	// If game status is complete whoever has the most score is the winner
	if(gameStatus == GS_COMPLETE){
		if(playerOne->getScore() > playerTwo->getScore()){
			return playerOne;
		} else if(playerOne->getScore() < playerTwo->getScore()){
			return playerTwo;
		}
	}
	return 0;
}
