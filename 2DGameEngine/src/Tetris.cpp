#include "Tetris.h"
#include <SDL_timer.h>

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
	tetrominoes[0] = Tetromino(4,
							  { 0, 1, 0, 0,
							   0, 1, 0, 0,
							   0, 1, 0, 0,
							   0, 1, 0, 0 }
							  );

	tetrominoes[1] = Tetromino(3,
							  {0, 1, 0, 0,
							   1, 1, 1, 0,
							   0, 0, 0, 0,
							   0, 0, 0, 0 });

	tetrominoes[2] = Tetromino(2,
							  {1, 1, 0, 0,
							   1, 1, 0, 0,
							   0, 0, 0, 0,
							   0, 0, 0, 0 });

	tetrominoes[3] = Tetromino(3,
							  {0, 1, 0, 0,
							   0, 1, 0, 0,
							   1, 1, 0, 0,
							   0, 0, 0, 0 });

	tetrominoes[4] = Tetromino(3,
							  {1, 0, 0, 0,
							   1, 0, 0, 0,
							   1, 1, 0, 0,
							   0, 0, 0, 0 });

	tetrominoes[5] = Tetromino(3,
							  {1, 0, 0, 0,
							   1, 1, 0, 0,
							   0, 1, 0, 0,
							   0, 0, 0, 0 });

	tetrominoes[6] = Tetromino(3,
							  {0, 1, 0, 0,
							   1, 1, 0, 0,
							   1, 0, 0, 0,
							   0, 0, 0, 0 });

	// seed the random generator
	time_t theTime;
	srand((unsigned int)time(&theTime));

	// initialize the first tetris piece
	int randomPick = rand() % 7;
	m_nextPiece = tetrominoes[randomPick];
	m_nextPiece.type = randomPick;

	LoadNextPiece(); // spawns the first tetris piece
}

void Tetris::Update()
{
	// check for game over by seeing if a piece at the top of the screen does not fit
	m_gameOver = (m_currentPiece.posY == 0 &&
		!PieceFits(&m_currentPiece.tiles, m_currentPiece.posX, m_currentPiece.posY));

	// Game Timing
	SDL_Delay(50); // Game Tick
	m_timer++;

	// make piece fall down if timer reach the time limit for speed
	if (m_timer == m_fallTime)
	{
		// if the the piece fits in the space below and move it down if it does
		if (PieceFits(&m_currentPiece.tiles, m_currentPiece.posX, m_currentPiece.posY + 1))
		{
			m_currentPiece.posY++; // It can so do it!
		}
		else // piece cannot move down any further
		{
			AddPieceToBoard();

			m_pieceCount++;

			// if 10 pieces have been place since the start of the speed increases
			if (m_pieceCount % 10 == 0)
			{
				// reduces the the time delay by 1 if it is atleast 10
				if (m_fallTime >= 0)
				{
					m_fallTime -= 2;
					m_level++;
				}
			}

			m_score += 25;

			CheckForLines();

			//delete m_currentPiece;

			if (m_linesFound.empty())
				LoadNextPiece();
		}
		m_timer = 0;
	}
}

void Tetris::LoadNextPiece()
{
	m_currentPiece = m_nextPiece;
	m_currentPiece.posX = FIELD_WIDTH / 2 - 1;
	m_currentPiece.posY = 0;

	int randomPick = rand() % 7;

	m_nextPiece = tetrominoes[randomPick];
	m_nextPiece.type = randomPick;
}

void Tetris::MovePieceLeft()
{
	if (PieceFits(&m_currentPiece.tiles, m_currentPiece.posX - 1, m_currentPiece.posY))
		m_currentPiece.posX--;
}

void Tetris::MovePieceRight()
{
	if (PieceFits(&m_currentPiece.tiles, m_currentPiece.posX + 1, m_currentPiece.posY))
		m_currentPiece.posX++;
}

void Tetris::MovePieceDown()
{
	if (PieceFits(&m_currentPiece.tiles, m_currentPiece.posX, m_currentPiece.posY + 1))
		m_currentPiece.posY++;
}

void Tetris::RotateCurrentPiece()
{
	PieceType newRotation;

	// counter clockwise	
	for (int y = 0; y < m_currentPiece.size; y++)
	{
		for (int x = 0; x < m_currentPiece.size; x++)		
		{
			newRotation[m_currentPiece.size - x - 1][y] = m_currentPiece.tiles[y][x];
		}
	}

	// TODO: make piece rotate clockwise
	

	if (PieceFits(&newRotation, m_currentPiece.posX, m_currentPiece.posY))
		m_currentPiece.tiles = newRotation;
}

bool Tetris::PieceFits(PieceType* tetromino, int posX, int posY)
{
	// itereate though all of the 16 spaces in the tetromino
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			// check the piece coordinates are not out of bounds
			if (posX + px >= 0 && posX + px < FIELD_WIDTH)
			{
				if (posY + py >= 0 && posY + py < FIELD_HEIGHT)
				{
					// checks if a tile in the tetromino is colliding with something by
					// seeing if the index in the tetromino has a tile and 
					// the position on the field at the coordinates space has a tile				
					if ((*tetromino)[py][px] == true && m_playField[posY + py][posX + px] != 0)
						return false; // fail on first hit
				}
			}
		}
	}

	return true;
}

void Tetris::AddPieceToBoard()
{
	// Add the current piece into the field
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			// if value in the index is a tile then set the matching space in the field to that tile
			if (m_currentPiece.tiles[py][px] == true)
				m_playField[m_currentPiece.posY + py][m_currentPiece.posX + px] = m_currentPiece.type + 1;
		}
	}
}

void Tetris::CheckForLines()
{
	int tilesInRow = 0;

	// check have we got any lines
	for (int py = 0; py < 4; py++) // iterates through the four rows the last peice occupied
	{
		if (m_currentPiece.posY + py < FIELD_HEIGHT - 1) // ensure y is not out of bounds
		{
			int fy = m_currentPiece.posY + py;

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

	// if a line was formed incremed the score by 100 doubled by the number of lines formed
	if (!m_linesFound.empty()) 
		m_score += (1 << m_linesFound.size()) * 100;
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

		m_linesFound.clear();

		LoadNextPiece();
	}
}