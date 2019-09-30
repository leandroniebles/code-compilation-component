#ifndef SHAPESBAR_H
#define SHAPESBAR_H
#include "Textures.h"

class ShapesBar {
	Texture m_texture_retracted;
	Texture m_texture_extended;
	bool m_visible, m_extended;
	bool m_fixed; // si queda fija siempre visible y extendida
	int m_width; // ancho para dibujo de la shapebar
	bool m_has_mouse; // indica si el mouse esta sobre la barra de formas
	int m_current_selection; // indica cual tipo de entidad esta seleccionada (1...n, 0 es ninguna)
	int shapebar_size_min, shapebar_size_max;
public:
	ShapesBar();
	void Draw();
	void Show() { m_visible = true; }
	void Hide() { m_visible = false; }
	void ToggleFixed() { m_fixed = !m_fixed; }
	void ProcessMotion(int x, int y);
	bool ProcessMouse(int button, int state, int x, int y); // true si le correspond�a ese click
	void ProcessIdle();
	int GetWidth() const;
	static void Initialize();
};

extern ShapesBar *shapes_bar;

#endif

