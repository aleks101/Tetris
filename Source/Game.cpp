#include "Game.h"
#include <Log.h>
#include <Assets.h>
#include <Button.h>
#include "Block.h"

#include <Windows.h>

#ifdef _DEBUG
#define SHOWCONSOLE ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif // _DEBUG
#ifdef NDEBUG
#define SHOWCONSOLE ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

Game::Game(){
	srand(time(NULL));

	application = App::GetInstance();
	application->Init("Tetris", 500, 600);
	ev = application->GetEvent();
	m_manager.ChangeFile("Files/Save/highscore.txt");

	//SHOWCONSOLE;
}
Game::~Game() {
	Save();

	delete grid;
	delete smallGrid;
	delete holdGrid;

	delete tetrad;

	scoreText->CleanTexture();
	highscoreText->CleanTexture();
	nextText->CleanTexture();
	pauseText->CleanTexture();
	levelText->CleanTexture();
	nameInput->CleanTexture();
	gameOverText->CleanTexture();
	restartText->CleanTexture();
	showLeaderbord->CleanTexture();
	swapText->CleanTexture();
	turnMusic->CleanTexture();
	delete turnMusic;
	delete swapText;
	delete restartText;
	delete showLeaderbord;
	delete gameOverText;
	delete nameInput;
	delete levelText;
	delete scoreText;
	delete highscoreText;
	delete nextText;
	delete pauseText;
	delete musicPlayer;

	Assets::CleanFonts();
	Assets::CleanSounds();

	application->Quit();
}
void Game::Start() {
	if (MIX_INIT_MP3 != Mix_Init(MIX_INIT_MP3)) {
		LOGln("SDL_Mixer failed to initialize!");
		return;
	}

	Assets::AddFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf", 20);
	Assets::AddSound("Files/Audio/move.mp3");
	Assets::AddSound("Files/Audio/place.wav");
	Assets::AddSound("Files/Audio/Background/1. Palm Tree Shade.wav");
	Assets::AddSound("Files/Audio/Background/2. Bim Bom Bomp.wav");
	Assets::AddSound("Files/Audio/Background/3. Funk Pump.wav");
	Assets::AddSound("Files/Audio/Background/6. Chilling Monkey.wav");
	Assets::AddSound("Files/Audio/Background/15. Star Rock.wav");

	musicPlayer = new Sound<Mix_Chunk>(Assets::GetSound("Files/Audio/Background/1. Palm Tree Shade.wav"), 50);
	musicPlayer->Play();

	tracks[0] = { "Files/Audio/Background/1. Palm Tree Shade.wav", 0 };
	tracks[1] = { "Files/Audio/Background/2. Bim Bom Bomp.wav", 1 };
	tracks[2] = { "Files/Audio/Background/3. Funk Pump.wav", 2 };
	tracks[3] = { "Files/Audio/Background/6. Chilling Monkey.wav", 3 };
	tracks[4] = { "Files/Audio/Background/15. Star Rock.wav", 4 };

	SDL_Renderer* ren = application->GetRenderer();
	scoreText = new Text(ren, { 10 * CELL_SIZE + 10,0 }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Score: 0", { 255,255,255 });
	scoreText->ChangeSizeFor(8);
	highscoreText = new Text(ren, { 10 * CELL_SIZE + 10,15 }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Highscore: 0", { 255,255,255 });
	highscoreText->ChangeSizeFor(8);
	levelText = new Text(ren, { 10 * CELL_SIZE + 10, 30 }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Level: 0", { 255,255,255 });
	levelText->ChangeSizeFor(8);
	swapText = new Text(ren, { 10 * CELL_SIZE + 10, 75 + 3 * CELL_SIZE }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Swap", { 255,255,255 });

	nameInput = new TextInput(ren, Vec2(90, 180), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "ENTER NAME", { 255, 255, 255, 255 }, {255, 0, 0, 255}, application->GetEvent());

	gameOverText = new Text(ren, { 90, 60 }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "GAME OVER", { 255, 255, 255, 255 });
	nextText = new Text(ren, { 10 * CELL_SIZE + 10,60}, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Next ", { 255,255,255 });
	pauseText = new Text(ren, { 90, 60 }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Paused", { 255,255,255 });
	restartText = new Button(ren, ev, { (float)(90 + gameOverText->GetDest()->w+ 10), 60}, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "TRY AGAIN", {255,255,255}, {255, 0, 0,0});
	showLeaderbord = new Button(ren, ev, { 90, 90 }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Show leaderbord", { 255, 255, 255, 255 }, { 255, 0, 0, 255 });
	turnMusic = new Button(ren, ev, { 90, 105 }, Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), "Mute music", { 255, 255, 255, 255 }, { 255, 0, 0, 255 });
	
	scoreText->SetNoChangeText("Score: ");
	highscoreText->SetNoChangeText("Highscore: ");
	levelText->SetNoChangeText("Level: ");

	gameOverRect = { 75, 170, 240, 420 };
	gamePausedRect = { 75, 45, 240, 120 };


	grid = new Grid<Cell>(10, 20);
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			*grid->AccessElement(i, j) = Cell(application->GetRenderer(), { 113,121,126,255 }, { 20, 20, 20, 255 }, { CELL_SIZE * i, CELL_SIZE * j, CELL_SIZE, CELL_SIZE }, j, i);
	smallGrid = new Grid<Cell>(2, 4);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			*smallGrid->AccessElement(i, j) = Cell(application->GetRenderer(), { 0,0,0,0 }, { 0,0,0,0 }, { 10 * CELL_SIZE + 10 + CELL_SIZE * j, 75 + CELL_SIZE * i, CELL_SIZE, CELL_SIZE }, j, i);
			smallGrid->AccessElement(i, j)->ShowBorder(false);
			smallGrid->AccessElement(i, j)->DrawBorderFirst(true);
		}
	}
	holdGrid = new Grid<Cell>(2, 4);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			*holdGrid->AccessElement(i, j) = Cell(application->GetRenderer(), { 0,0,0,0 }, { 0,0,0,0 }, { 10 * CELL_SIZE + 10 + CELL_SIZE * j, 90 + 3 * CELL_SIZE + CELL_SIZE * i,CELL_SIZE, CELL_SIZE }, j, i);
			holdGrid->AccessElement(i, j)->ShowBorder(false);
			holdGrid->AccessElement(i, j)->DrawBorderFirst(true);
		}
	}

	tetrad = new Tetrad(grid);

	Load();

	MainLoop();
}
void Game::MainLoop() {
	while (isRunning) {
		TimeD::CalcDeltaTime();

		if (SDL_PollEvent(ev))
			HandleEvents();
		if (isPaused || initiatingResume)
			PausedLoop();
		else
			GameLoop();

		if (muteMusic == false && musicPlayer->CheckIfAudioIsPlaying() == false) {
			currentTrack++;
			if (currentTrack >= 5)
				currentTrack = 0;
			musicPlayer->ChangeAudio(Assets::GetSound(tracks[currentTrack].name.c_str()));
			musicPlayer->Play();
			LOGln("Music Playing -> " + tracks[currentTrack].name);
		}
		turnMusic->Update();
		if (turnMusic->CheckMouseClick()) {
			//MUTE
			muteMusic = !muteMusic;
			if (muteMusic == false) {
				musicPlayer->ChangeVolume(0);
			}
			else {
				musicPlayer->ChangeVolume(50);
			}
		}

		nextBlock = tetrad->GetNextBlock();
		if (nextBlock != nullptr) {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if (nextBlock->GetUnit(j, i) == 1) {
						smallGrid->AccessElement(i, j)->ChangeColor(nextBlock->GetColor());
						smallGrid->AccessElement(i, j)->Fill(true);
					}
					else
						smallGrid->AccessElement(i, j)->Fill(false);
				}
			}
			delete nextBlock;
			nextBlock = nullptr;
		}
		if (tetrad->GetSwapBlock() != nullptr) {
			Block* block = tetrad->GetSwapBlock();
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 4; j++) {
					if (block->GetUnit(j, i) == 1) {
						holdGrid->AccessElement(i, j)->ChangeColor(block->GetColor());
						holdGrid->AccessElement(i, j)->Fill(true);
					}
					else
						holdGrid->AccessElement(i, j)->Fill(false);
				}
			}
		}
		Rendering();
	}
}
void Game::Rendering() {
	tetrad->RenderBlock();
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			grid->AccessElement(i, j)->Render();
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 4; j++)
			smallGrid->AccessElement(i, j)->Render();
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 4; j++)
			holdGrid->AccessElement(i, j)->Render();

	scoreText->Render();
	highscoreText->Render();
	nextText->Render();
	levelText->Render();
	swapText->Render();
	if (tetrad->IsGameOver() == true) {
		SDL_Rect outline = gamePausedRect;
		outline.x--;
		outline.y--;
		outline.w += 2;
		outline.h += 2;
		SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(application->GetRenderer(), &outline);
		SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);
		SDL_RenderFillRect(application->GetRenderer(), &gamePausedRect);
		SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);

		SDL_RenderDrawLine(application->GetRenderer(), outline.x, outline.y + 35, outline.x + outline.w, outline.y + 35);
		SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);

		turnMusic->Render();

		outline = gameOverRect;
		outline.x--;
		outline.y--;
		outline.w += 2;
		outline.h += 2;
		SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(application->GetRenderer(), &outline);
		SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);
		SDL_RenderFillRect(application->GetRenderer(), &gameOverRect);
		SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);

		SDL_RenderDrawLine(application->GetRenderer(), outline.x, outline.y + 35, outline.x + outline.w, outline.y + 35);
		SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);

		restartText->Render();
		gameOverText->Render();
		nameInput->Render();
		for (int i = 0; i < playerScores.size(); i++) {
			playerScores[i].Render();
		}
	}
	else if (isPaused || initiatingResume) {

		SDL_Rect outline = gamePausedRect;
		outline.x--;
		outline.y--;
		outline.w += 2;
		outline.h += 2;
		SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(application->GetRenderer(), &outline);
		SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);
		SDL_RenderFillRect(application->GetRenderer(), &gamePausedRect);
		SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);

		SDL_RenderDrawLine(application->GetRenderer(), outline.x, outline.y + 35, outline.x + outline.w, outline.y + 35);
		SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);

		pauseText->Render();
		showLeaderbord->Render();
		turnMusic->Render();


		if (leaderbordShown) {
			outline = gameOverRect;

			outline.x--;
			outline.y--;
			outline.w += 2;
			outline.h += 2;
			SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);
			SDL_RenderDrawRect(application->GetRenderer(), &outline);
			SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);
			SDL_RenderFillRect(application->GetRenderer(), &gameOverRect);
			SDL_SetRenderDrawColor(application->GetRenderer(), 255, 255, 255, 255);

			SDL_RenderDrawLine(application->GetRenderer(), outline.x, outline.y + 35, outline.x + outline.w, outline.y + 35);
			SDL_SetRenderDrawColor(application->GetRenderer(), 0, 0, 0, 0);
			for (int i = 0; i < playerScores.size(); i++) {
				playerScores[i].Render();
			}
		}

	}
	application->Render();
}
void Game::HandleEvents() {
	if (ev->type == SDL_QUIT) {
		isRunning = false;
		return;
	}
	else if (ev->type == SDL_KEYDOWN) {
		if (tetrad->IsGameOver() == false) {
			if (ev->key.keysym.sym == SDLK_ESCAPE) {
				isPaused = !isPaused;
				if (!isPaused && !initiatingResume) {
					timer = 4;
					initiatingResume = true;
				}
			}
			if (!isPaused && !initiatingResume) {
				SDL_Keycode sym = ev->key.keysym.sym;
				if (sym == SDLK_SPACE) {
					tetrad->Move(BLOCK_PLACE);
				}
				if (sym == SDLK_RIGHT || sym == SDLK_d) {
					tetrad->Move(BLOCK_MOVE_RIGHT);
				}
				if (sym == SDLK_LEFT || sym == SDLK_a) {
					tetrad->Move(BLOCK_MOVE_LEFT);
				}
				if (sym == SDLK_DOWN || sym == SDLK_s) {
					tetrad->Move(BLOCK_MOVE_DOWN);
				}
				if (sym == SDLK_UP || sym == SDLK_w || sym == SDLK_e) {
					tetrad->Rotate(BLOCK_ROTATE_RIGHT);
				}
				if (sym == SDLK_q) {
					tetrad->Rotate(BLOCK_ROTATE_LEFT);
				}
				if (sym == SDLK_TAB) {
					tetrad->Swap();
				}
			}
		}
	}
}

void Game::GameLoop() {
	timer += TimeD::DeltaTime();
	tetrad->Update();
	if(tetrad->IsGameOver() == false) {
		scoreText->ChangeText(tetrad->GetScore());
		levelText->ChangeText(tetrad->GetLevel());
		score = tetrad->GetScore();
		if (highscore < score) {
			highscore = score;
			highscoreText->ChangeText(highscore);
		}
	}
	else {
		restartText->Update();
		if (restartText->CheckMouseClick()) {
			Save();

			tetrad->Reset();
		}
		nameInput->Update();
		nameInput->Input();
	}
}
void Game::PausedLoop() {
	pauseText->ChangeText("Paused");
	if (initiatingResume) {
		timer -= TimeD::DeltaTime();
		if (timer <= 0) {
			initiatingResume = false;
			isPaused = false;
			timer = 4;
		}
		pauseText->ChangeText("Resume in " + std::to_string((int)timer));
	}
	showLeaderbord->Update();
	if (showLeaderbord->CheckMouseClick()) {
		leaderbordShown = !leaderbordShown;
	}

	//pauseText->Render();
}
void Game::Save() {
	std::string txt = nameInput->GetText();
	size_t pos = txt.find("ENTER NAME");
	if (pos == std::string::npos) {
		m_manager.WriteAppend(nameInput->GetText() + " " + std::to_string(tetrad->GetScore()) + "\n");
	}
}
#include <string>
#include <algorithm>

void Game::Load() {
	std::ifstream data;
	data.open("Files/Save/highscore.txt");
	std::string line;
	int cnt = 0;

	std::vector<std::string> players;
	while (std::getline(data, line)) {
		players.push_back(line);
	}
	data.close();
	std::vector<std::pair<int, std::string>> scores;
	for (int i = 0; i < players.size(); i++) {
		int j = players[i].length();
		int l = players[i].length();
		while (players[i][j] != ' ') {
			j--;
		}
		j++;
		std::string scoreStr;
		for (int k = 0; k + j < l; k++) {
			if (players[i][k + j] == '.')
				break;
			scoreStr.push_back(players[i][k + j]);
		}
		std::string name;
		for (int k = 0; k < j-1; k++) {
			name.push_back(players[i][k]);
		}
		std::pair<int, std::string> par = std::make_pair(std::stoi(scoreStr), name);
		scores.push_back(par);
	}
	std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
		return a.first > b.first; // Sort by int in descending order
		});
	playerScores.push_back(Text(application->GetRenderer(), Vec2(90, 210), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), scores[cnt].second + " " + std::to_string(scores[cnt].first), {255,255,255,255}));
	cnt++;
	while (cnt < scores.size()) {
		playerScores.push_back(Text(application->GetRenderer(), Vec2(90, playerScores[cnt-1].GetDest()->y + playerScores[cnt - 1].GetDest()->h), Assets::GetFont("Files/Fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf"), scores[cnt].second + " " + std::to_string(scores[cnt].first), {255,255,255,255}));
		cnt++;
	}
	highscoreText->ChangeText(scores[0].first);
}