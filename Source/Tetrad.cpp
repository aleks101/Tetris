#include "Tetrad.h"
#include "Calc.h"

#include <Assets.h>
#include <TimeD.h>
#include <ctime>
#include <string>

#define TOI(num) std::to_string(num)

Tetrad::Tetrad() : m_sound(Assets::GetSound("Files/Audio/move.mp3"), 80) {

}
Tetrad::Tetrad(Grid<Cell>* grid) : m_sound(Assets::GetSound("Files/Audio/move.mp3"), 80){
	m_tetradType = (BlockTypes)(rand() % 7);
	m_grid = grid;
	Start();
}
Tetrad::Tetrad(const Tetrad& obj) : m_sound(Assets::GetSound("Files/Audio/move.mp3"), 80) {
	m_grid = obj.m_grid;
	m_tetradType = obj.m_tetradType;
	m_block = obj.m_block;
}
Tetrad& Tetrad::operator=(const Tetrad& obj) {
	m_grid = obj.m_grid;
	m_tetradType = obj.m_tetradType;
	m_block = obj.m_block;
	return *this;
}
void Tetrad::Rotate(int direction) {
	Calculations::TestInfo info = Calculations::TestRotation(m_grid, m_block);

	if (info.success == true) {
		m_block->Rotate(direction);
		m_sound.ChangeAudio(Assets::GetSound("Files/Audio/move.mp3"));
		m_sound.Play();
	}
	else {
		if (info.failType == Calculations::FailType::OUT_OF_BOUNDS) {
			LOGln("OUT OF BOUNDS ERROR");

			Vec2 position = m_block->GetPosition();
			Vec2 dimension = m_block->GetDimensions();
			int offset = 0;
			bool stopTesting = false;
			int k = 0;
			//CHECK FOR GROUND
			if ((position.y + dimension.x) > (m_grid->GetColumnCount())) {
				m_block->Rotate(direction);
				position = m_block->GetPosition();
				dimension = m_block->GetDimensions();
				offset = (position.y + dimension.y) - (m_grid->GetColumnCount());
				m_block->Rotate(direction);
				m_block->Rotate(direction);
				m_block->Rotate(direction);
				for (int i = 0; i < offset; i++) {
					if (Calculations::TestMoveUp(m_grid, m_block))
						m_block->Move(Vec2(m_block->GetPosition().x, m_block->GetPosition().y - 1));
					else {
						stopTesting = true;
						for (int j = 0; j < i; j++)
							m_block->Move(Vec2(m_block->GetPosition().x, m_block->GetPosition().y + 1));
						break;
					}
					k = i;
				}
				if (!stopTesting) {
					if (Calculations::TestRotation(m_grid, m_block).success == true) {
						m_block->Rotate(direction);
						m_sound.Play();
						LOGln("ROTATED between bounds");
						return;
					}
					else {
						for (int j = 0; j < k; j++)
							m_block->Move(Vec2(m_block->GetPosition().x, m_block->GetPosition().y + 1));
					}
				}
			}
			//CHECKING FOR LEFT BORDER
			if ((position.x + dimension.y) > m_grid->GetRowCount()) {
				offset = (position.x + dimension.y) - m_grid->GetRowCount();
				stopTesting = false;
				k = 0;
				for (int i = 0; i < offset; i++) {
					if (Calculations::TestMoveLeft(m_grid, m_block))
						m_block->Move(Vec2(m_block->GetPosition().x - 1, m_block->GetPosition().y));
					else {
						stopTesting = true;
						for (int j = 0; j < i; j++)
							m_block->Move(Vec2(m_block->GetPosition().x + 1, m_block->GetPosition().y));
						break;
					}
					k = i;
				}
				if (!stopTesting) {
					if (Calculations::TestRotation(m_grid, m_block).success == true) {
						m_block->Rotate(direction);
						m_sound.Play();
						LOGln("ROTATED between bounds");
						return;
					}
					else {
						for (int j = 0; j < k; j++)
							m_block->Move(Vec2(m_block->GetPosition().x + 1, m_block->GetPosition().y));
					}
				}
			}
			goto checkOCCUPIED;
		}
		if (info.failType == Calculations::FailType::SPACE_OCCUPIED) {
			checkOCCUPIED:
			LOGln("SPACE OCCUPIED ERROR");

			m_block->Rotate(direction);
			Vec2 origPos = m_block->GetPosition();
			Vec2 dim = m_block->GetDimensions();

			m_block->Rotate(direction);
			m_block->Rotate(direction);
			m_block->Rotate(direction);
			//CHECK Y
			for (int i = 1; i < dim.y; i++) {
				//DOWN
				m_block->Move(origPos.x, origPos.y + i);
				Calculations::TestInfo info = Calculations::TestRotation(m_grid, m_block);
				if (info.success == false && (info.failType == Calculations::FailType::OUT_OF_BOUNDS || info.failType == Calculations::FailType::SPACE_OCCUPIED)) {
					m_block->Move(origPos.x, origPos.y);
				}
				else if (info.success == true) {
					m_block->Rotate(direction);
					LOGln("ROTATED, between blocks");
					return;
				}
				//UP
				m_block->Move(origPos.x, origPos.y - i);
				info = Calculations::TestRotation(m_grid, m_block);
				if (info.success == false && (info.failType == Calculations::FailType::OUT_OF_BOUNDS || info.failType == Calculations::FailType::SPACE_OCCUPIED)) {
					m_block->Move(origPos.x, origPos.y);
				}
				else if (info.success == true) {
					m_block->Rotate(direction);
					LOGln("ROTATED, between blocks");
					return;
				}
			}
			//CHECK X
			for (int i = 1; i <= dim.x; i++) {
				//RIGHT
				m_block->Move(origPos.x + i, origPos.y);
				Calculations::TestInfo info = Calculations::TestRotation(m_grid, m_block);
				if (info.success == false && (info.failType == Calculations::FailType::OUT_OF_BOUNDS || info.failType == Calculations::FailType::SPACE_OCCUPIED)) {
					m_block->Move(origPos.x, origPos.y);
				}
				else if (info.success == true) {
					m_block->Rotate(direction);
					LOGln("ROTATED, between blocks");
					return;
				}
				//LEFT
				m_block->Move(origPos.x - i, origPos.y);
				info = Calculations::TestRotation(m_grid, m_block);
				if (info.success == false && (info.failType == Calculations::FailType::OUT_OF_BOUNDS || info.failType == Calculations::FailType::SPACE_OCCUPIED)) {
					m_block->Move(origPos.x, origPos.y);
				}
				else if (info.success == true) {
					m_block->Rotate(direction);
					LOGln("ROTATED, between blocks");
					return;
				}
			}

		}
	}
}
void Tetrad::Move(int direction) {
	m_sound.ChangeAudio(Assets::GetSound("Files/Audio/move.mp3"));
	Vec2 pos = m_block->GetPosition();
	if (direction == BLOCK_MOVE_RIGHT) {
		//MOVE RIGHT
		if (Calculations::TestMoveRight(m_grid, m_block)) {
			m_block->Move(Vec2(m_block->GetPosition().x + 1, m_block->GetPosition().y));
				m_sound.Play();
		}
	}
	else if (direction == BLOCK_MOVE_LEFT) {
		//MOVE LEFT
		if (Calculations::TestMoveLeft(m_grid, m_block)) {
			m_block->Move(Vec2(m_block->GetPosition().x - 1, m_block->GetPosition().y));
				m_sound.Play();
		}
	}
	if (direction == BLOCK_MOVE_DOWN) {
		//MOVE DOWN
		if (Calculations::TestMoveDown(m_grid, m_block)) {
			m_block->Move(Vec2(m_block->GetPosition().x, m_block->GetPosition().y + 1));
				m_sound.Play();
		}
	}
	if (direction == BLOCK_PLACE) {
		bool x = true;
		while (x) {
			if (Calculations::TestMoveDown(m_grid, m_block)) {
				m_block->Move(Vec2(m_block->GetPosition().x, m_block->GetPosition().y + 1));
			}
			else {
				x = false;
				m_sound.ChangeAudio(Assets::GetSound("Files/Audio/place.wav"));
				m_sound.Play();
				Place();
			}
		}
	}
}
void Tetrad::Swap() {
	if (m_alreadySwapped == false) {
		if (m_swapBlock == nullptr) {
			m_swapBlock = m_block;
			if (m_swapBlock->GetDimensions().x < m_swapBlock->GetDimensions().y)
				m_swapBlock->Rotate(BLOCK_ROTATE_RIGHT);
			m_block = nullptr;
			Start();
		}
		else {
			Block* tmp = m_swapBlock;
			m_swapBlock = m_block;
			if (m_swapBlock->GetDimensions().x < m_swapBlock->GetDimensions().y)
				m_swapBlock->Rotate(BLOCK_ROTATE_RIGHT);
			m_block = tmp;
			m_block->Move(3, 0);
		}
	}
	m_alreadySwapped = true;
}
void Tetrad::Start() {
	if(m_block !=nullptr)
		delete m_block;

	if (m_tetradType == BlockTypes::Block_O) {
		m_block = new OBlock();
	}
	else if (m_tetradType == BlockTypes::Block_I) {
		m_block = new IBlock();
	}
	else if (m_tetradType == BlockTypes::Block_J) {
		m_block = new JBlock();
	}
	else if (m_tetradType == BlockTypes::Block_L) {
		m_block = new LBlock();
	}
	else if (m_tetradType == BlockTypes::Block_T) {
		m_block = new TBlock();
	}
	else if (m_tetradType == BlockTypes::Block_S) {
		m_block = new SBlock();
	}
	else if (m_tetradType == BlockTypes::Block_Z) {
		m_block = new ZBlock();
	}

	m_tetradType = (BlockTypes)(rand() % 7);
}
void Tetrad::Place() {
	Vec2 pos = m_block->GetPosition();
	Vec2 dim = m_block->GetDimensions();
	for (int i = 0; i < m_block->GetDimensions().x; i++) {
		for (int j = 0; j < m_block->GetDimensions().y; j++) {
			if (m_block->GetUnit(i,j) == 1) {
				m_grid->AccessElement(i + pos.x, j + pos.y)->Fill(true);
				m_grid->AccessElement(i + pos.x, j + pos.y)->ChangeColor(m_block->GetColor());
			}
		}
	}

	delete m_block;
	m_block = nullptr;
	m_timer = 0;
	m_alreadySwapped = false;
	Start();
}
void Tetrad::RemoveFullRows() {
	int adder = 15;
	for (int i = m_grid->GetColumnCount() - 1; i >= 0; i--) {
		bool rowFull = true;
		for (int j = 0; j < m_grid->GetRowCount(); j++) {
			if (m_grid->AccessElement(j, i)->IsEmpty()) {
				rowFull = false;
			}
		}
		if (rowFull) {
			m_rowsCleared++;
			m_score += adder * m_scoreMultiplyer;
			adder *= 2;
			for (int y = i; y > 0; y--) {
				for (int j = 0; j < m_grid->GetRowCount(); j++) {
					m_grid->AccessElement(j, y)->ChangeColor(m_grid->AccessElement(j, y - 1)->GetColor());
					m_grid->AccessElement(j, y)->Fill(!m_grid->AccessElement(j, y - 1)->IsEmpty());
				}
			}
			i++;
		}
	}
}
void Tetrad::Update() {
	m_time += TimeD::DeltaTime();

	if (!m_grid->AccessElement(3, 0)->IsEmpty()) {
		m_gameover = true;
		LOGln("Game over!");
	}
	if (m_gameover == false) {
		if (m_rowsCleared >= 10) {
			m_level++;
			m_rowsCleared -= 10;
			m_moveTime -= 0.05f;
			m_groundTime -= 0.03f;
			if (m_moveTime <= 0.10f)
				m_moveTime = 0.10f;
			if (m_groundTime <= 0.30f)
				m_groundTime = 0.30f;
			m_scoreMultiplyer = 1.35f * m_level;
		}
		if (m_time >= m_moveTime) {
			m_time = 0;
			if (m_block != nullptr) {
				if (Calculations::TestMoveDown(m_grid, m_block)) {
					m_block->Move(Vec2(m_block->GetPosition().x, m_block->GetPosition().y + 1));
					m_hasMovedDown = true;
				}
				else {
					m_hasMovedDown = false;
				}
			}
			else
				LOGln("block is null");
		}
		if (m_hasMovedDown == false) {
			m_timer += TimeD::DeltaTime();
			m_groundTime -= TimeD::DeltaTime();
			if (m_timer >= m_groundTime) {
				Place();
				m_hasMovedDown = true;
			}
		}
		Preshow();
		RemoveFullRows();
	}
}
void Tetrad::Reset() {
	m_gameover = false;
	m_score = 0;
	m_level = 1;
	m_moveTime = 1.0f;
	m_groundTime = 1.0f;
	for (int i = 0; i < m_grid->GetRowCount(); i++) {
		for (int j = 0; j < m_grid->GetColumnCount(); j++) {
			m_grid->AccessElement(i, j)->Fill(false);
		}
	}
	Start();
}
void Tetrad::RenderBlock() {
	if (m_block != nullptr) {
		Vec2 pos = m_block->GetPosition();
		for (int i = pos.x; i < m_block->GetDimensions().x + pos.x; i++) {
			for (int j = pos.y; j < m_block->GetDimensions().y + pos.y; j++) {
				if (m_block->GetUnit(Vec2(i - pos.x, j - pos.y))) {
					m_grid->AccessElement(i, j)->ForcePaint(m_block->GetColor());
				}
			}
		}
	}
}
Block* Tetrad::GetSwapBlock() {
	return m_swapBlock;
}
Block* Tetrad::GetNextBlock() {
	if (m_tetradType == BlockTypes::Block_O) {
		return new OBlock();
	}
	else if (m_tetradType == BlockTypes::Block_I) {
		return new IBlock();
	}
	else if (m_tetradType == BlockTypes::Block_J) {
		return new JBlock();
	}
	else if (m_tetradType == BlockTypes::Block_L) {
		return new LBlock();
	}
	else if (m_tetradType == BlockTypes::Block_T) {
		return new TBlock();
	}
	else if (m_tetradType == BlockTypes::Block_S) {
		return new SBlock();
	}
	else if (m_tetradType == BlockTypes::Block_Z) {
		return new ZBlock();
	}
}
BlockTypes Tetrad::GetNextBlockType() {
	return m_tetradType;
}
float Tetrad::GetScore() {
	return m_score;
}
void Tetrad::Preshow() {
	int x = true;
	Vec2 position = m_block->GetPosition();
	Vec2 dimension = m_block->GetDimensions();
	while (x) {
		if (Calculations::TestMoveDown(m_grid, position, dimension, m_block->shape, Block::S_GetUnit)) {
			position.y++;
		}
		else {
			x = false;
		}
	}
	for (int i = 0; i < m_grid->GetColumnCount(); i++) {
		for (int j = 0; j < m_grid->GetRowCount(); j++) {
			m_grid->AccessElement(j, i)->ChangeBorderColor({ 0,0,0,0 });
		}
	}
	for (int i = 0; i < dimension.x; i++) {
		for (int j = 0; j < dimension.y; j++) {
			if (m_block->GetUnit(i, j) == 1) {
				m_grid->AccessElement(position.x + i, position.y + j)->ChangeBorderColor(m_block->GetColor());
				m_grid->AccessElement(position.x + i, position.y + j)->ShowBorder(true);
			}
		}
	}
}