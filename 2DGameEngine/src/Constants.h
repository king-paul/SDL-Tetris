#ifndef CONSTANTS_H
#define CONSTANTS_H

// SDL Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const unsigned int FPS = 120;
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;

// audio mixer constants
const unsigned int AUDIO_FREQUENCY = 441000;
const unsigned int AUDIO_CHANNELS = 2;
const unsigned int AUDIO_CHUNK_SIZE = 1025;

// Tetris Layout Constants
const int TILE_SIZE = 32;
const int COLS = 12;
const int ROWS = 18;
const int GRID_OFFSET_X = 6;
const int GRID_OFFSET_Y = 12;

// right panel margins
const int MARGIN_LEFT = WINDOW_WIDTH/2 + 10;
const int MARGIN_TOP = 12;

#endif