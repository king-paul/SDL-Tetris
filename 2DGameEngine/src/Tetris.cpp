#include "Tetris.h"
#include <iostream>

Tetris::Tetris(int fieldWidth, int fieldHeight) : nFieldWidth(fieldWidth), nFieldHeight(fieldHeight)
{
	m_pieceCount = 0;

	// iterates through the field placing zeros on vacant spaces and 9s on the bounadary walls
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			// checks if the x and y coordinates are outside the range if the field
			if (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1)
			if (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1)
				m_playField[y][x] = 9;
			else
				m_playField[y][x] = 0;
		}
	}

	// Create the tetrominoes

	/* I Block
	 * . . . .
	 * # # # #
	 * . . . .
	 * . . . . */
	tetrominoes[0] = Tetromino({-1, 0}, {0, 0}, {1, 0}, {2, 0}, 0, 4);

	/* T block
	 * . # . 
	 * # # # 
	 * . . . */
	tetrominoes[1] = Tetromino({ -1, 0 }, { 0, 0 }, { 0, -1 }, { 1, 0 }, 1 , 3);

	/* O block
	 * . . . .
	 * . # # .
	 * . # # .
	 * . . . .*/
	tetrominoes[2] = Tetromino({0, 0}, {1, 0}, {0, 1}, {1, 1} , 2, 2);

	/* L block
	 * # . . 
	 * # # #
	 * . . . */
	tetrominoes[3] = Tetromino({-1 ,-1}, {-1 , 0}, { 0, 0}, { 1, 0}, 3, 3);
	
	/* J Block
	* . . # 
	* # # # 
	* . . .	*/
	tetrominoes[4] = Tetromino({-1, 0}, {0, 0}, {1, 0}, {1, -1}, 4), 3;

	/* S Block
	* . # #
	* # # .
	* . . . */
	tetrominoes[5] = Tetromino({-1 ,0 }, { 0, 0}, { 0, -1}, { 1, -1}, 5, 3);

	/* Z Block
	* # # .
	* . # #
	* . . . */
	tetrominoes[6] = Tetromino({-1, -1}, {0, -1}, {0, 0}, {1, 0}, 6, 3);

	// seed the random generator
	time_t theTime;
	std::srand((unsigned int)time(&theTime));

	// initialize the first tetris piece
	int randomPick = 0;// rand() % 7;
	m_nextPiece = new Tetromino(tetrominoes[randomPick]);
	m_nextPiece->type = randomPick;

	LoadNextPiece(); // spawns the first tetris piece
}

void Tetris::Update(float deltaTime)
{
	if (m_event == Event::GameOver)
		return;

	m_event = Null; // resets event on start of next frame

	// check for game over by seeing if a piece at the top of the screen does not fit
	if (!m_gameOver && m_currentPiece->posY == 0 &&
		!PieceFits(m_currentPiece->positions, m_currentPiece->posX, m_currentPiece->posY))
	{
		m_gameOver = true;
		m_event = Event::GameOver;
	}

	// Game Timing
	m_timer += deltaTime;

	// make piece fall down if timer reach the time limit for speed
	if (m_timer >= (m_fallTime/10) || m_sendToBottom)
	{
		// if the the piece fits in the space below and move it down if it does
		if (PieceFits(m_currentPiece->positions, m_currentPiece->posX, m_currentPiece->posY + 1))
		{
			m_currentPiece->posY++; // It can so do it!
		}
		else // piece cannot move down any further
		{
			AddPieceToBoard();

			m_event = Event::PieceLanded;
			m_sendToBottom = false;
			m_pieceCount++;

			CheckForLines();

			// clear current tetromino from heap
			delete m_currentPiece;
			m_currentPiece = nullptr;

			if (m_linesFound.empty())
				LoadNextPiece();
		}
		m_timer = 0;
	}
}

Tetris::~Tetris()
{
	if (m_currentPiece != nullptr)
		delete m_currentPiece;
}

void Tetris::IncreaseLevel()
{
	if (m_fallTime > 1)
	{
		m_fallTime--;
		m_level++;
	}
	else if (m_fallTime > 0.5f)
	{
		m_fallTime -= 0.1f;
		m_level++;
	}

	m_event = Event::LevelUp;
}

void Tetris::LoadNextPiece()
{
	m_currentPiece = m_nextPiece;
	m_currentPiece->posX = FIELD_WIDTH / 2 - 1;
	m_currentPiece->posY = 1;

	int randomPick = rand() % 7;

	m_nextPiece = new Tetromino(tetrominoes[randomPick]);
	m_nextPiece->type = randomPick;
}

void Tetris::MovePieceLeft()
{
	if (m_currentPiece != nullptr && 
		PieceFits(m_currentPiece->positions, m_currentPiece->posX - 1, m_currentPiece->posY))
	{
		m_currentPiece->posX--;
		m_event = Event::MovePiece;
	}
}

void Tetris::MovePieceRight()
{
	if (m_currentPiece != nullptr && 
		PieceFits(m_currentPiece->positions, m_currentPiece->posX + 1, m_currentPiece->posY))
	{
		m_currentPiece->posX++;
		m_event = Event::MovePiece;
	}
}

void Tetris::MovePieceDown()
{
	if (m_currentPiece != nullptr && 
		PieceFits(m_currentPiece->positions, m_currentPiece->posX, m_currentPiece->posY + 1))
	{
		m_currentPiece->posY++;
		m_event = Event::MovePiece;
	}
}

void Tetris::RotateCurrentPiece(bool clockwise)
{
	if(m_currentPiece == nullptr)
		return;

	PieceType newRotation;

	if (m_currentPiece->type == 0) // I block
	{
		newRotation = Rotate2dArray(clockwise);
	}
	else if (m_currentPiece->type == 2) // O block
	{
		newRotation = m_currentPiece->positions;
	}
	else
	{
		for (int i = 0; i < m_currentPiece->positions.size(); i++)
		{
			if (clockwise)
			{
				newRotation[i].x = m_currentPiece->positions[i].y;
				newRotation[i].y = -m_currentPiece->positions[i].x;
			}
			else
			{
				newRotation[i].x = -m_currentPiece->positions[i].y;
				newRotation[i].y = m_currentPiece->positions[i].x;
			}

		}
	}

	if (PieceFits(newRotation, m_currentPiece->posX, m_currentPiece->posY))
	{
		m_currentPiece->positions = newRotation;
		m_event = Event::RotatePiece;
	}
}

bool Tetris::PieceFits(PieceType tetromino, int posX, int posY)
{
	for (Coord tile : tetromino)
	{
		// check the piece coordinates are not out of bounds
		if (posX + tile.x < 0 || posX + tile.x > FIELD_WIDTH ||
			posY + tile.y < 0 || posY + tile.y > FIELD_HEIGHT)
			return false;

		// checks if a tile in the tetromino is colliding with something
		if (m_playField[posY + tile.y][posX + tile.x] != 0)
			return false;		
	}

	return true; // all spaces were vacant
}

void Tetris::AddPieceToBoard()
{
	for (Coord tile : m_currentPiece->positions)
	{
		m_playField[m_currentPiece->posY + tile.y][m_currentPiece->posX + tile.x] = m_currentPiece->type + 1;
	}
}

void Tetris::CheckForLines()
{
	int tilesInRow = 0;

	// check have we got any lines
	for (int py = 0; py < 4; py++) // iterates through the four rows the last peice occupied
	{
		if (m_currentPiece->posY + py < FIELD_HEIGHT - 1) // ensure y is not out of bounds
		{
			int fy = m_currentPiece->posY + py;

			// iterate through all positions in the row
			for (int px = 1; px < FIELD_WIDTH - 1; px++)
			{
				// check if there is a tile in the space
				if (m_playField[fy][px] != 0)
					tilesInRow++;
			}

			if (tilesInRow == FIELD_WIDTH-2) // if a line was found
			{
				// Remove the line by setting all of the values to 8
				for (int fx = 1; fx < FIELD_WIDTH - 1; fx++)
					m_playField[fy][fx] = 8; // 8 is equals character

				m_linesFound.push_back(fy); // add line to lines vector
				m_linesFormed++;
			}
		}

		tilesInRow = 0;
	}

	// if one or more lines was formed apply actions
	if (m_linesFound.size() > 0)
	{
		if (m_linesFound.size() == 4)
			m_event = Event::Tetris4Lines;
		else
			m_event = Event::LineClear;
	}

}

void Tetris::ClearLinesFound()
{
	if (!m_linesFound.empty())
	{
		for (int rowNum : m_linesFound)
		{
			// iterate through all lines above where the line was formed
			for (int py = rowNum; py > 0; py--)
			{
				// iterate through all tiles in row
				for (int px = 1; px < FIELD_WIDTH - 1; px++)
				{
					// move everything in row above down one row
					m_playField[py][px] = m_playField[py-1][px];
				}
			}
		}	


		// Update the score if 1 or more lines is formed lines is formed
		switch (m_linesFound.size())
		{
			case 1: m_score += SINGLE_LINE_POINTS * m_level; break;
			case 2: m_score += DOUBLE_LINE_POINTS * m_level; break;
			case 3: m_score += TRIPLE_LINE_POINTS * m_level; break;
			case 4: m_score += QUADROUPLE_LINE_POINTS * m_level; break;
			default: break;
		}

		// if 10 lines have been formed since the start of the level then
		// increase the level
		if (m_linesFormed % 10 == 0)
		{
			IncreaseLevel();
		}
		
		m_linesFound.clear();		

		LoadNextPiece();
	}
}

PieceType Tetris::Rotate2dArray(bool clockwise)
{
	bool tetrominoArray[4][4];
	bool rotatedArray[4][4];

	// initialize 2d arrays
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			tetrominoArray[y][x] = false;
			rotatedArray[y][x] = false;
		}
	}

	// convert coordinates into array indexes
	for (Coord tile : m_currentPiece->positions)
	{
		tetrominoArray[tile.y + 1] [tile.x + 1] = true;
	}

	// iterate through all cells/tiles in tetromino
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			// rotate each tile left or right depending on value of clockwise
			if (clockwise)
				rotatedArray[x][4 - y - 1] = tetrominoArray[y][x];
			else
				rotatedArray[4 - x - 1][y] = tetrominoArray[y][x];
		}
	}

	PieceType newRotation;
	int pieceIndex = 0;

	// convert array back into coordinates
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (rotatedArray[y][x] == true)
			{
				newRotation[pieceIndex] = { x - 1, y - 1 };
				pieceIndex++;
			}
		}		
	}

	return newRotation;
}