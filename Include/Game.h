#pragma once

#include <ctime>
#include <App.h>
#include <Grid.h>
#include <Text.h>
#include <Button.h>
#include <TimeD.h>
#include <Data.h>
#include <TextInput.h>

#include "Cell.h"
#include "Tetrad.h"

struct Music {
	std::string name;
	int id;
};

class Game: public Data
{
public:
	Game();
	~Game();
	void Start();
private:
	void MainLoop();
	void GameLoop();
	void PausedLoop();
	void HandleEvents();
	void Rendering();

	void Save();
	void Load();

	App* application = nullptr;
	SDL_Event* ev = nullptr;
	bool isRunning = true;
	bool isPaused = true;

	Grid<Cell>* grid;
	Grid<Cell>* smallGrid;
	Grid<Cell>* holdGrid;
	Text* scoreText, * highscoreText, * nextText, * pauseText, *levelText, *swapText;
	TextInput* nameInput;
	Text* gameOverText;
	std::vector<Text> playerScores;
	Button *restartText, *showLeaderbord, *turnMusic;
	Tetrad* tetrad;

	int score = 0;
	int highscore = 0;
	float timer = 4;
	bool initiatingResume = false;
	bool leaderbordShown = false;

	Music tracks[5];
	int currentTrack = 4;
	bool muteMusic = false;
	Sound<Mix_Chunk>* musicPlayer = nullptr;

	Block* nextBlock = nullptr;
	BlockTypes nextBlockType = BlockTypes::NONE;

	SDL_Rect gameOverRect;
	SDL_Rect gamePausedRect;
};

