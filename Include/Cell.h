#pragma once

#include <Object.h>

#define CELL_SIZE 30

class Cell : public Object
{
public:
	Cell(SDL_Renderer* ren, SDL_Color color, SDL_Color backgroundColor, SDL_Rect dest, int y, int x);
	Cell();
	Cell(const Cell& obj);
	Cell& operator=(const Cell& obj);

	void Render();
	void Update();
	
	void ChangeColor(SDL_Color color);
	void ChangeBorderColor(SDL_Color color);
	void ForcePaint(SDL_Color color);
	void ShowBorder(bool yesOrNo);
	void DrawBorderFirst(bool yesOrNo);
	bool IsEmpty() const { return !m_isFilled; }
	void Fill(bool fill) {
		m_isFilled = fill;
	}
	SDL_Color GetColor() {
		return m_color;
	}

	SDL_Rect* GetDest() {
		return &m_dest;
	}
	SDL_Rect* GetScreen() {
		return &m_screen;
	}
	SDL_Rect* GetSrc() {
		return& m_src;
	}
private:
	SDL_Color m_borderColor;
	SDL_Color m_color;
	SDL_Color m_emptyColor;
	int m_x, m_y;
	bool m_isFilled = false;
	bool m_showBorder = true;
	bool m_drawBorderFirst = false;
	bool m_forcePaint = false;
};

