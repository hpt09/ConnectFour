#include "Grid.hpp"

Grid::Grid(unsigned int rows, unsigned int columns){
	// Adjusting for boundaries
	if(rows < 4){
		noOfRows = 4;
	} else{
		noOfRows = rows;
	}
	if(columns < 4){
		noOfColumns = 4;
	} else {
		noOfColumns = columns;
	}
	// Initialisation
	board.resize(noOfRows, std::vector<Cell>(noOfColumns, GC_EMPTY));

}

Grid::~Grid(){

}

bool Grid::insertDisc(unsigned int column, Cell disc){
	// Empty disc can't be inserted
	if(disc == GC_EMPTY){
		return false;
	}
	// Column can't be out of bounds
	else if(column > (noOfColumns - 1)){
		return false;
	}
	else {
		// find the row of the column (j) which is empty
		int j = -1;
		for(unsigned int i = 0; i < noOfRows; i++){
			if(board[i][column] == GC_EMPTY){
				j++;
			}
		}
		// no empty rows
		if(j == -1){
			return false;
		} else {
			// inserting disc at (j) row and (column) column
			board[j][column] = disc;
			return true;
		}
	}
}

Grid::Cell Grid::cellAt(unsigned int row, unsigned int column) const{
	// Adjusting for boundaries
	if(row > noOfRows){
		return GC_EMPTY;
	} else if (column > noOfColumns){
		return GC_EMPTY;
	} else {
		// Returning cell at particular row and column
		return board[row][column];
	}
}

void Grid::reset(){
	// Resetting all the rows and columns to empty by assign function
	board.assign(noOfRows, std::vector<Cell>(noOfColumns, GC_EMPTY));
}

unsigned int Grid::rowCount() const{
	return noOfRows;
}

unsigned int Grid::columnCount() const{
	return noOfColumns;
}


void Grid::printBoard(){
	// Used for debugging. Prints out the grid for you
	cout << "*******************" << endl;

	for(int i = 0; i < noOfRows; i++){
		for(int j = 0; j< noOfColumns; j++){
			cout << board[i][j];
		}
		cout << endl;
	}
	cout << "********************" ;

}

void Grid::makeEmptyCell(int x, int y){
	// Making a particular cell empty
	board[y][x] = GC_EMPTY;
}

bool Grid::noMoreMoves(){
	// To find if any more moves can be made in the meaning if the grid is full hence the game is tie
	// find the row of the column (j) which is empty
	unsigned int k;
	for(k = 0; k < noOfColumns; k++){
		int j = -1;
		for(unsigned int i = 0; i < noOfRows; i++){
			if(board[i][k] == GC_EMPTY){
				j++;
			}
		}
		// If (j) equals to -1 means no row is empty
		// hence if (j) does not equal to -1 for any column means there is still moves left
		if(j != -1){
			return false;
		}
	}
	// if (k) column is the last column and there was no empty row since we checked at every column then return true
	if(k == noOfColumns - 1){
		return true;
	}
}

void Grid::fallDown(){
	// Method for all the cells to fall down when a combo is disappeared
	for(unsigned int l = 0 ; l < noOfColumns; l++){
		// Running a loop through all columns and finding the first row which is not empty
		int j = -1;
		int noOfBreaks = 0;
		std::vector<int> breakIndexes;
		for(unsigned int i = 0; i < noOfRows; i++){
			if(board[i][l] == GC_EMPTY){
				j++;
			} else {
				break;
			}
		}
		if(j == noOfRows - 1){	// for empty columns do nothing

		} else {
			j++;				// index of row of first disc

			for(int i = j; i < noOfRows; i++){
				if(board[i][l] == GC_EMPTY){
					// If there are empty spaces after the first non-empty row, store the indexes of the empty spaces in a vector
					breakIndexes.push_back(i);
					noOfBreaks++;
				}
			}
			if(noOfBreaks > 0){	// if there are breaks in columns
				for(int i = 0; i < noOfBreaks; i++){
					int k = breakIndexes[i];
					for(k ; k > j; k-- ){
						// start from the first break and keep moving 1 index down until where the first non-empty row was
						board[k][l] = board[k - 1][l];
					}
					// make the last row as empty as all the cells are moved downwards
					board[k][l] = GC_EMPTY;
				}
			}
		}
	}
}

