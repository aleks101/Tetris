#include "Calc.h"

namespace Calculations {
	bool TestMoveRight(Grid<Cell>* grid, Block* block) {
		//CHECK FOR BORDER
		if (grid != nullptr && block != nullptr) {
			Vec2 position = block->GetPosition();
			Vec2 dimension = block->GetDimensions();

			if (position.x + dimension.x < grid->GetRowCount()) {
				for (int j = 0; j < dimension.y; j++) {
					for (int i = 0; i < dimension.x; i++) {
						if (block->GetUnit(i, j) == 1) {
							if (!grid->AccessElement(position.x + i + 1, position.y + j)->IsEmpty())
								return false;
						}
					}
				}
			}
			position.x++;
			if (position.x + dimension.x > grid->GetRowCount())
				return false;
			return true;
		}
		return false;
	}
	bool TestMoveLeft(Grid<Cell>* grid, Block* block) {
		//CHECK FOR BORDER
		if (grid != nullptr && block != nullptr) {
			Vec2 position = block->GetPosition();
			Vec2 dimension = block->GetDimensions();

			if (position.x - 1 > 0) {
				for (int j = 0; j < dimension.y; j++) {
					for (int i = 0; i < dimension.x; i++) {
						if (block->GetUnit(i, j) == 1) {
							if (grid->AccessElement(position.x - 1 + i, position.y + j)->IsEmpty() == false)
								return false;
						}
					}
				}
			}
			position.x--;

			if (position.x < 0)
				return false;
			return true;
		}
		return false;
	}
	bool TestMoveDown(Grid<Cell>* grid, Block* block) {
		//CHECK FOR BORDER
		if (grid != nullptr && block != nullptr) {
			Vec2 position = block->GetPosition();
			Vec2 dimension = block->GetDimensions();
		
			if (position.y + dimension.y + 1 > grid->GetColumnCount())
				return false;
			if (position.y + dimension.y < grid->GetColumnCount()) {

				for (int i = 0; i < dimension.x; i++) {
					for (int j = 0; j < dimension.y; j++) {
						if (block->GetUnit(i, j) == 1) {
							if (!grid->AccessElement(position.x + i, position.y + j + 1)->IsEmpty()) {
								return false;
							}
						}
					}
				}
			}	
	

			return true;
		}
		return false;
	}
	bool TestMoveDown(Grid<Cell>* grid, Vec2 position, Vec2 dimension, matrix shape, int(*GetUnit)(Vec2 position, Vec2 dimensions, matrix shape)) {
		//CHECK FOR BORDER
		if (grid != nullptr && GetUnit != nullptr) {
			if (position.y + dimension.y + 1 > grid->GetColumnCount())
				return false;
			if (position.y + dimension.y < grid->GetColumnCount()) {
				for (int i = 0; i < dimension.x; i++) {
					for (int j = 0; j < dimension.y; j++) {
						if (GetUnit(Vec2(i,j), dimension, shape) == 1) {
							if (!grid->AccessElement(position.x + i, position.y + j + 1)->IsEmpty()) {
								return false;
							}
						}
					}
				}
			}
			return true;
		}
		return false;
	}
	bool TestMoveUp(Grid<Cell>* grid, Block* block) {
		//CHECK FOR BORDER
		if (grid != nullptr && block != nullptr) {
			Vec2 position = block->GetPosition();
			Vec2 dimension = block->GetDimensions();
			position.y--;

			if (position.y < 0)
				return false;
			return true;
		}
		return false;
	}
	TestInfo TestRotation(Grid<Cell>* grid, Block* block) {
		//CHECK FOR BORDER
		TestInfo info;
		info.success = false;
		info.failType = FailType::OUT_OF_BOUNDS;
		
		if (grid != nullptr && block != nullptr) {
			block->Rotate(BLOCK_ROTATE_RIGHT);
			Vec2 position = block->GetPosition();
			Vec2 dimension = block->GetDimensions();
			block->Rotate(BLOCK_ROTATE_RIGHT);
			block->Rotate(BLOCK_ROTATE_RIGHT);
			block->Rotate(BLOCK_ROTATE_RIGHT);


			if (position.x + dimension.x > grid->GetRowCount()) {
				info.failType = FailType::OUT_OF_BOUNDS;
				return info;
			}
			if (position.y + dimension.y > grid->GetColumnCount()) {
				info.failType = FailType::OUT_OF_BOUNDS;
				return info;
			}
			block->Rotate(BLOCK_ROTATE_RIGHT);
			for (int i = 0; i < dimension.x; i++) {
				for (int j = 0; j < dimension.y; j++) {
					if ((!grid->AccessElement(i + position.x, j + position.y)->IsEmpty())) {
						if (block->GetUnit(i, j) == 1) {
							block->Rotate(BLOCK_ROTATE_RIGHT);
							block->Rotate(BLOCK_ROTATE_RIGHT);
							block->Rotate(BLOCK_ROTATE_RIGHT);

							info.failType = FailType::SPACE_OCCUPIED;
							return info;
						}
					}
				}
			}
			block->Rotate(BLOCK_ROTATE_RIGHT);
			block->Rotate(BLOCK_ROTATE_RIGHT);
			block->Rotate(BLOCK_ROTATE_RIGHT);
			info.success = true;
			return info;
		}
		return info;
	}
}