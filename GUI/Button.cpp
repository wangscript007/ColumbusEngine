#include <GUI/Button.h>
#include <iostream>

namespace Columbus
{

	namespace GUI
	{

		void C_Button::update()
		{
			mState = 0;

			if (mIO.mouse.enabled == false)
				return;

			C_Vector2 m = mIO.mouse.coords;

			if (m.x > mPos.x && m.x < (mPos.x + mSize.x))
				if (m.y > mPos.y && m.y < (mPos.y + mSize.y))
					mState = 1;
		}

		void C_Button::draw()
		{
			C_Vector2 pos = mPos / mIO.screen.size;
			C_Vector2 size = mSize / mIO.screen.size * 2;

			//C_DisableDepthMaskOpenGL();

			glColor4f(1, 0, 0, 1);

			glBegin(GL_QUADS);
				glVertex2f(pos.x, pos.y);
				glVertex2f(pos.x + size.x, pos.y);
				glVertex2f(pos.x + size.x, pos.y + size.y);
				glVertex2f(pos.x, pos.y + size.y);
			glEnd();

			//C_EnableDepthMaskOpenGL();
		}

	}

}