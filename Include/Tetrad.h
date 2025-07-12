#pragma once

#include <Grid.h>
#include <Sound.h>
#include "Cell.h"
#include "Block.h"
#include <Particle.h>

class Tetrad
{
public:
	Tetrad();
	Tetrad(Grid<Cell>* grid);
	Tetrad(const Tetrad& obj);
	Tetrad& operator=(const Tetrad& obj);

	void Rotate(int direction);
	void Move(int direction);
	void Swap();
	void Place();
	void Start();

	void RenderBlock();
	void RemoveFullRows();
	void Preshow();

	Block* GetNextBlock();
	Block* GetSwapBlock();
	BlockTypes GetNextBlockType();
	float GetScore();
	int GetLevel() {
		return m_level;
	}
	bool IsGameOver() {
		return m_gameover;
	}

	void Update();
	void Reset();
private:
	BlockTypes m_tetradType = BlockTypes::Block_O;
	Block* m_block = nullptr;
	Block* m_swapBlock = nullptr;
	Grid<Cell>* m_grid = nullptr;
	Sound<Mix_Chunk> m_sound;

	bool m_alreadySwapped = false;
	bool m_hasMovedDown = true;
	float m_timer = 0;
	float m_time = 0;
	float m_moveTime = 1.0f;
	float m_groundTime = 1.0f;
	float m_score = 0;
	float m_scoreMultiplyer = 1.35f;
	int m_level = 1;
	bool m_gameover = false;
	int m_rowsCleared = 0;
};