#pragma once
#include <array>
#include <vector>
#include <time.h>
#include <thread>
#include <windows.h>

using namespace std;

#define ASCII_VALUE 65
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18

typedef array<array <bool, 4>, 4> PieceType;

struct Tetromino
{	
	Tetromino() {} // default constructor

	Tetromino(int size, PieceType tiles) // paremeter constructor
	{
		this->size = size;
		this->tiles = tiles;

		type = 0;
		posX = 0;
		posY = 0;
	}	
	
	Tetromino(const Tetromino& ref) // copy constructor
	{
		size = ref.size;
		tiles = ref.tiles;
		type = ref.type;
		posX = ref.posX;
		posY = ref.posY;
	}
	
	PieceType tiles;
	int size;

	int type;
	int posX;
	int posY;
	
};

class Tetris
{
public:
	Tetris(int fieldWidth = 12, int fieldHeight = 18);
	~Tetris();
	
	void Update(float deltaTime);
	void LoadNextPiece();	
	void RotateCurrentPiece(bool clockwise);
	void MovePieceLeft();
	void MovePieceRight();
	void MovePieceDown();
	void ClearLinesFound();	

	void IncreaseLevel();

	// getters
	unsigned int GetBoardValue(int x, int y){ return m_playField[y][x]; }
	array<array<int, FIELD_WIDTH>, FIELD_HEIGHT>* GetField() { return &m_playField; }
	Tetromino* GetCurrentPiece() { return m_currentPiece; }
	Tetromino* GetNextPiece() { return m_nextPiece; }
	bool FormedLines() { return !m_linesFound.empty(); }
	vector<int> GetLines() { return m_linesFound; }
	int Score() { return m_score; }
	int PiecesPlaced(){ return m_pieceCount; }
	int LinesFormed() { return m_linesFormed; }
	int Level() { return m_level; }
	bool GameOver() { return m_gameOver; }

	// setters
	void SendPieceToBottom() { m_sendToBottom = true; }

	/// <summary>
	/// Checks whether or not a tetromino with a specified orientation will fit
	/// at a specified coordinate on the field
	/// </summary>
	/// <param name="tetromino">The 2d array of tiles within the piece</param>
	/// <param name="posX">The x position of the piece's top left</param>
	/// <param name="posY">The y position of the piece's top left</param>
	/// <returns>true of false based on whether the piece fits</returns>
	bool PieceFits(PieceType* tetromino, int posX, int posY);

private:	

	void AddPieceToBoard();
	void CheckForLines();

	Tetromino* m_currentPiece;
	Tetromino* m_nextPiece;

	Tetromino tetrominoes[7];

	int nFieldWidth; // the width of the board
	int nFieldHeight; // the height of the board	

	array<array<int, FIELD_WIDTH>, FIELD_HEIGHT> m_playField; // stores the game board

	float m_fallTime = 10; // the speed in which tetrominos fall
	float m_timer = 0; // the time between each descent of tetromino
	int m_pieceCount = 0; // how many pieces have fallen in the game
	int m_score = 0; // the player's current score
	int m_linesFormed = 0; // the total number of lines the player has formed
	int m_level = 1; // the level of the game based on number of times the speed has increased

	bool m_sendToBottom = false;
	bool m_gameOver = false;

	vector<int> m_linesFound; // stores row numbers where lines have been formed
};


