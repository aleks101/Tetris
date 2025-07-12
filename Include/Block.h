#pragma once

#include <SDL.h>
#include <vector>
#include "Colors.h"
#include <Vec2.h>

#define BLOCK_MOVE_LEFT 0x1
#define BLOCK_MOVE_RIGHT 0x2
#define BLOCK_MOVE_DOWN 0x3
#define BLOCK_PLACE 0x4
#define BLOCK_ROTATE_RIGHT 0x5
#define BLOCK_ROTATE_LEFT -1


#define matrix std::vector<bool>
enum class BlockTypes {
	Block_I,
	Block_O,
	Block_T,
	Block_S,
	Block_Z,
	Block_L,
	Block_J,
	NONE
};
namespace SHAPES {
	namespace SHAPE_I {
		static const matrix SHAPE_0 = {
			1,1,1,1,
		};
		static const matrix SHAPE_1 = {
			1,
			1,
			1,
			1
		};
	}
	namespace SHAPE_O {
		static const matrix SHAPE_0 = {
			1,1,
			1,1
		};
	}
	namespace SHAPE_T {
		static const matrix SHAPE_0 = {
			0,1,0,
			1,1,1
		};
		static const matrix SHAPE_1 = {
			1,0,
			1,1,
			1,0
		};
		static const matrix SHAPE_2 = {
			1,1,1,
			0,1,0
		};
		static const matrix SHAPE_3 = {
			0,1,
			1,1,
			0,1
		};
	}
	namespace SHAPE_S {
		static const matrix SHAPE_0 = {
			0,1,1,
			1,1,0
		};
		static const matrix SHAPE_1 = {
			1,0,
			1,1,
			0,1
		};
	}
	namespace SHAPE_Z {
		static const matrix SHAPE_0 = {
			1,1,0,
			0,1,1
		};
		static const matrix SHAPE_1 = {
			0,1,
			1,1,
			1,0
		};
	}
	namespace SHAPE_L {
		static const matrix SHAPE_0 = {
			0,0,1,
			1,1,1
		};
		static const matrix SHAPE_1 = {
			1,0,
			1,0,
			1,1
		};
		static const matrix SHAPE_2 = {
			1,1,1,
			1,0,0
		};
		static const matrix SHAPE_3 = {
			1,1,
			0,1,
			0,1
		};
	}
	namespace SHAPE_J {
		static const matrix SHAPE_0 = {
			1,0,0,
			1,1,1,
		};
		static const matrix SHAPE_1 = {
			1,1,
			1,0,
			1,0
		};
		static const matrix SHAPE_2 = {
			1,1,1,
			0,0,1
		};
		static const matrix SHAPE_3 = {
			0,1,
			0,1,
			1,1
		};
	}
}

class Block
{
public:
	virtual void Rotate(int direction) = 0;
	void Move(int x, int y) {
		m_position.x = x;
		m_position.y = y;
	}
	void Move(Vec2 pos) {
		m_position = pos;
	}
	Vec2 GetPosition() {
		return m_position;
	}
	SDL_Color GetColor() {
		return m_color;
	}
	Vec2 GetDimensions() {
		return m_dimensions;
	}
	int GetUnit(Vec2 pos) {
		if (pos.x < m_dimensions.x && pos.y < m_dimensions.y) {
			if (pos.x >= 0 && pos.y >= 0) {
				return shape[pos.x + m_dimensions.x * pos.y];
			}
		}
		return 0;
	}
	int GetUnit(int x, int y) {
		if (x < m_dimensions.x && y < m_dimensions.y) {
			if (x >= 0 && y >= 0) {
				return shape[x + m_dimensions.x * y];
			}
		}
		return 0;
	}
	static int S_GetUnit(Vec2 position, Vec2 dimensions, matrix shape) {
		if (position.x < dimensions.x && position.y < dimensions.y) {
			if (position.x >= 0 && position.y >= 0) {
				return shape[position.x + dimensions.x * position.y];
			}
		}
		return 0;
	}

	matrix shape;
private:

protected:
	Vec2 m_position = Vec2(3,0);
	Vec2 m_dimensions;
	SDL_Color m_color;
};
class IBlock : public Block {
public:
	IBlock() {
		m_dimensions = Vec2(4, 1);
		m_color = Colors::cyan;
		shape = SHAPES::SHAPE_I::SHAPE_0;
	}
	void Rotate(int direction) {
		(direction > 0) ? m_currRot++ : m_currRot--;
		if (m_currRot >= m_rotations)
			m_currRot = 0;
		else if (m_currRot < 0)
			m_currRot = m_rotations - 1;

		if (m_currRot == 0) {
			shape = SHAPES::SHAPE_I::SHAPE_0;
			m_dimensions = Vec2(4, 1);
		}
		else if (m_currRot == 1) {
			shape = SHAPES::SHAPE_I::SHAPE_1;
			m_dimensions = Vec2(1, 4);
		}
	}

private:
	int m_rotations = 2;
	int m_currRot=0;
};
class OBlock : public Block {
public:
	OBlock() {
		m_dimensions = Vec2(2,2);
		m_color = Colors::yellow;
		shape = SHAPES::SHAPE_O::SHAPE_0;
	}
	void Rotate(int direction) {
		
	}
private:
	int m_rotations = 0;
	int m_currRot = 0;
};
class TBlock : public Block {
public:
	TBlock() {
		m_dimensions = Vec2(3,2);
		m_color = Colors::purple;
		shape = SHAPES::SHAPE_T::SHAPE_0;
	}
	void Rotate(int direction) {
		(direction > 0) ? m_currRot++ : m_currRot--;
		if (m_currRot >= m_rotations)
			m_currRot = 0;
		else if (m_currRot < 0)
			m_currRot = m_rotations - 1;

		if (m_currRot == 0) {
			shape = SHAPES::SHAPE_T::SHAPE_0;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 1) {
			shape = SHAPES::SHAPE_T::SHAPE_1;
			m_dimensions = Vec2(2, 3);
		}
		else if (m_currRot == 2) {
			shape = SHAPES::SHAPE_T::SHAPE_2;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 3) {
			shape = SHAPES::SHAPE_T::SHAPE_3;
			m_dimensions = Vec2(2, 3);
		}
	}
private:
	int m_rotations = 4;
	int m_currRot = 0;
};
class SBlock : public Block {
public:
	SBlock() {
		m_dimensions = Vec2(3, 2);
		m_color = Colors::green;
		shape = SHAPES::SHAPE_S::SHAPE_0;
	}
	void Rotate(int direction) {
		(direction > 0) ? m_currRot++ : m_currRot--;
		if (m_currRot >= m_rotations)
			m_currRot = 0;
		else if (m_currRot < 0)
			m_currRot = m_rotations - 1;

		if (m_currRot == 0) {
			shape = SHAPES::SHAPE_S::SHAPE_0;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 1) {
			shape = SHAPES::SHAPE_S::SHAPE_1;
			m_dimensions = Vec2(2, 3);
		}
	}
private:
	int m_rotations = 2;
	int m_currRot = 0;
};
class ZBlock : public Block {
public:
	ZBlock() {
		m_dimensions = Vec2(3, 2);
		m_color = Colors::red;
		shape = SHAPES::SHAPE_Z::SHAPE_0;
	}
	void Rotate(int direction) {
		(direction > 0) ? m_currRot++ : m_currRot--;
		if (m_currRot >= m_rotations)
			m_currRot = 0;
		else if (m_currRot < 0)
			m_currRot = m_rotations - 1;

		if (m_currRot == 0) {
			shape = SHAPES::SHAPE_Z::SHAPE_0;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 1) {
			shape = SHAPES::SHAPE_Z::SHAPE_1;
			m_dimensions = Vec2(2, 3);
		}
	}
private:
	int m_rotations = 2;
	int m_currRot = 0;
};
class LBlock : public Block {
public:
	LBlock() {
		m_dimensions = Vec2(3, 2);
		m_color = Colors::orange;
		shape = SHAPES::SHAPE_L::SHAPE_0;
	}
	void Rotate(int direction) {
		(direction > 0) ? m_currRot++ : m_currRot--;
		if (m_currRot >= m_rotations)
			m_currRot = 0;
		else if (m_currRot < 0)
			m_currRot = m_rotations - 1;

		if (m_currRot == 0) {
			shape = SHAPES::SHAPE_L::SHAPE_0;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 1) {
			shape = SHAPES::SHAPE_L::SHAPE_1;
			m_dimensions = Vec2(2, 3);
		}
		else if (m_currRot == 2) {
			shape = SHAPES::SHAPE_L::SHAPE_2;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 3) {
			shape = SHAPES::SHAPE_L::SHAPE_3;
			m_dimensions = Vec2(2, 3);
		}
	}
private:
	int m_rotations = 4;
	int m_currRot = 0;
};
class JBlock : public Block {
public:
	JBlock() {
		m_dimensions = Vec2(3, 2);
		m_color = Colors::blue;
		shape = SHAPES::SHAPE_J::SHAPE_0;
	}
	void Rotate(int direction) {
		(direction > 0) ? m_currRot++ : m_currRot--;
		if (m_currRot >= m_rotations)
			m_currRot = 0;
		else if (m_currRot < 0)
			m_currRot = m_rotations - 1;

		if (m_currRot == 0) {
			shape = SHAPES::SHAPE_J::SHAPE_0;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 1) {
			shape = SHAPES::SHAPE_J::SHAPE_1;
			m_dimensions = Vec2(2, 3);
		}
		else if (m_currRot == 2) {
			shape = SHAPES::SHAPE_J::SHAPE_2;
			m_dimensions = Vec2(3, 2);
		}
		else if (m_currRot == 3) {
			shape = SHAPES::SHAPE_J::SHAPE_3;
			m_dimensions = Vec2(2, 3);
		}
	}
private:
	int m_rotations = 4;
	int m_currRot = 0;
};