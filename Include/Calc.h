#pragma once

#include <Grid.h>
#include "Cell.h"
#include "Block.h"

namespace Calculations {
	enum FailType {
		OUT_OF_BOUNDS,
		SPACE_OCCUPIED,
	};
	struct TestInfo {
		bool success;
		int failType;
	};

	bool TestMoveRight(Grid<Cell>* grid, Block* block);
	bool TestMoveLeft(Grid<Cell>* grid, Block* block);
	bool TestMoveDown(Grid<Cell>* grid, Block* block);
	bool TestMoveDown(Grid<Cell>* grid, Vec2 position, Vec2 dimension, matrix shape, int(*GetUnit)(Vec2 position, Vec2 dimensions, matrix shape));
	bool TestMoveUp(Grid<Cell>* grid, Block* block);
	TestInfo TestRotation(Grid<Cell>* grid, Block* block);
}