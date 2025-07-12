#include "Cell.h"

Cell::Cell(SDL_Renderer* ren, SDL_Color color, SDL_Color backgroundColor, SDL_Rect dest, int y, int x) :
	Object(dest), m_color(color), m_y(y), m_x(x), m_emptyColor(backgroundColor) {
	//m_borderColor = { 113,121,126,255 };
	m_ren = ren;
}
Cell::Cell() : Object({0,0,CELL_SIZE,CELL_SIZE }) {
	//m_borderColor = { 113,121,126,255 };
	m_ren = nullptr;
	m_y = 0;
	m_x = 0;
}
Cell::Cell(const Cell& obj) : Object({ 0,0,CELL_SIZE,CELL_SIZE }) {
	//m_borderColor = { 113,121,126,255 };
	m_color = obj.m_color;
	m_dest = obj.m_dest;
	m_ren = obj.m_ren;
	m_y = obj.m_y;
	m_x = obj.m_x;
	m_isFilled = obj.m_isFilled;
	m_emptyColor = obj.m_emptyColor;
}
Cell& Cell::operator=(const Cell& obj) {
	//m_borderColor = { 113,121,126,255 };
	m_color = obj.m_color;
	m_emptyColor = obj.m_emptyColor;

	m_dest = obj.m_dest;
	m_ren = obj.m_ren;
	m_y = obj.m_y;
	m_x = obj.m_x;
	m_isFilled = obj.m_isFilled;
	return *this;
}
void Cell::Render() {
	if (m_drawBorderFirst) {
		if (m_showBorder) {
			SDL_SetRenderDrawColor(m_ren, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
		}
		else {
			SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
		}
		SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
		SDL_RenderDrawRect(m_ren, &m_dest);
		SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
		if (m_isFilled) {
			SDL_SetRenderDrawColor(m_ren, m_color.r, m_color.g, m_color.b, m_color.a);
			SDL_RenderFillRect(m_ren, &m_dest);
		}
	}
	else {
		if (m_isFilled) {
			SDL_SetRenderDrawColor(m_ren, m_color.r, m_color.g, m_color.b, m_color.a);
			SDL_RenderFillRect(m_ren, &m_dest);
		}
		else if(!m_forcePaint){
			SDL_SetRenderDrawColor(m_ren, m_emptyColor.r, m_emptyColor.g, m_emptyColor.b, m_emptyColor.a);
			SDL_RenderFillRect(m_ren, &m_dest);
		}
		if (m_showBorder) {
			SDL_SetRenderDrawColor(m_ren, m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
		}
		else {
			SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
		}
		SDL_RenderDrawRect(m_ren, &m_dest);
		SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
	
	}
	m_forcePaint = false;
	m_showBorder = false;
}
void Cell::Update() {

}
void Cell::ChangeColor(SDL_Color color) {
	m_color = color;
}
void Cell::ChangeBorderColor(SDL_Color color) {
	m_borderColor = color;
}
void Cell::ForcePaint(SDL_Color color) {
	m_color = color;
	SDL_SetRenderDrawColor(m_ren, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(m_ren, &m_dest);
	SDL_RenderDrawRect(m_ren, &m_dest);
	SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0);
	m_forcePaint = true;
}
void Cell::ShowBorder(bool yesOrNo) {
	m_showBorder = yesOrNo;
}
void Cell::DrawBorderFirst(bool yesOrNo) {
	m_drawBorderFirst = yesOrNo;
}