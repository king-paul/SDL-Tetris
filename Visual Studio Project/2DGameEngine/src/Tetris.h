#pragma once
#include <array>
#include <vector>

using namespace std;

#define ASCII_VALUE 65
#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18

// Tetris scoring and level
const int SINGLE_LINE_POINTS = 40;
const int DOUBLE_LINE_POINTS = 100;
const int TRIPLE_LINE_POINTS = 300;
const int QUADROUPLE_LINE_POINTS = 1200;
const int LINES_PER_LEVEL = 10;

struct Coord
{
	int x; int y;
};

typedef std::array<Coord, 4> PieceType;

enum Event
{
	Null = -1,
	MovePiece,
	RotatePiece,
	PieceLanded,
	LineClear,
	Tetris4Lines,
	LevelUp,
	GameOver
};

struct Tetromino
{	
	int posX;
	int posY;
	int size;
	int type;

	PieceType positions;

	Tetromino() 
	{
		size = 3;
		posX = 0;
		posY = 0;
		type = 0;

	} // default constructor

	Tetromino(int size, PieceType tiles) // paremeter constructor
	{
		this->size;
		this->positions = tiles;

		posX = 0;
		posY = 0;
	}	

	Tetromino(Coord p1, Coord p2, Coord p3, Coord p4, int type, int size = 4)
	{
		positions[0] = p1;
		positions[1] = p2;
		positions[2] = p3;
		positions[3] = p4;

		this->type = type;
		this->size = size;

		posX = 0;
		posY = 0;
	}
	
	Tetromino(const Tetromino& ref) // copy constructor
	{
		posX = ref.posX;
		posY = ref.posY;

		positions[0] = ref.positions[0];
		positions[1] = ref.positions[1];
		positions[2] = ref.positions[2];
		positions[3] = ref.positions[3];
	}
	
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
	//void TriggerGameOver();

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
	Event GetEvent() { return m_event; }

	// setters
	void SendPieceToBottom() { m_sendToBottom = true; }
	void ResetEvent() { m_event = Null; }

	/// <summary>
	/// Checks whether or not a tetromino with a specified orientation will fit
	/// at a specified coordinate on the field
	/// </summary>
	/// <param name="tetromino">The 2d array of tiles within the piece</param>
	/// <param name="posX">The x position of the piece's top left</param>
	/// <param name="posY">The y position of the piece's top left</param>
	/// <returns>true of false based on whether the piece fits</returns>
	bool PieceFits(PieceType tetromino, int posX, int posY);

private:	

	void AddPieceToBoard();
	void CheckForLines();
	PieceType Rotate2dArray(bool clockwise);

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

	Event m_event = Null; // records last event for sound effects
};