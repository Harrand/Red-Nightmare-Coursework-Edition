#include "header.h"
#include "templates.h"
#include "Cursor.h"
#include "Utility.h"
#include "PsyhshEngine.h"

Cursor::Cursor(BaseEngine* engine): DisplayableObject(engine), cursor_image_off(), cursor_image_on(), just_paused(false)
{
	this->cursor_image_off.LoadImage("resources/rune_off.png");
	this->cursor_image_on.LoadImage("resources/rune_on.png");
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = 100;
	m_iCurrentScreenY = m_iPreviousScreenY = 100;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = this->GetCursorImage().GetWidth();
	m_iDrawHeight = this->GetCursorImage().GetHeight();
	// And make it visible
	SetVisible(true);
}

Cursor::~Cursor()
{
}

void Cursor::DoUpdate(int iCurrentTime)
{
	DisplayableObject::DoUpdate(iCurrentTime);
	this->GetEngine()->UpdateMouseInfo();
	Vector2i mousePosition = Cursor::GetMousePosition(*(this->m_pEngine));
	this->m_iCurrentScreenX = utility::clamp<float>(mousePosition.first - (m_iDrawWidth / 2.0f), 0.0f, this->m_pEngine->GetWindowWidth() - m_iDrawWidth);
	this->m_iCurrentScreenY = utility::clamp<float>(mousePosition.second - (m_iDrawHeight / 2.0f), 0.0f, this->m_pEngine->GetWindowHeight() - m_iDrawHeight);

	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	if (engine->IsKeyPressed(SDLK_ESCAPE) && !this->just_paused)
	{
		engine->SetPaused(!engine->IsPaused());
		this->just_paused = true;
	}
	else if(!engine->IsKeyPressed(SDLK_ESCAPE))
		this->just_paused = false;
}

BoundingBox2i Cursor::GetBoundingBox() const
{
	return { {this->m_iCurrentScreenX, this->m_iCurrentScreenY + this->m_iDrawHeight}, {this->m_iCurrentScreenX + this->m_iDrawWidth, this->m_iCurrentScreenY} };
}

void Cursor::Draw()
{
	/*GetEngine()->DrawForegroundOval(m_iCurrentScreenX, m_iCurrentScreenY, m_iCurrentScreenX + m_iDrawWidth - 1, m_iCurrentScreenY + m_iDrawHeight - 1,
		this->IsSpecial() ? 0xff0000 : 0x4444ff);
		*/
	this->GetCursorImage().RenderImageWithMask(this->m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, this->GetCursorImage().GetWidth(), this->GetCursorImage().GetHeight());
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionForUndraw();
	/* draws the bounding box for debug purposes.
	utility::draw_bounding_box(this->GetBoundingBox(), this->m_pEngine);
	StoreLastScreenPositionForUndraw();
	*/

}

Vector2i Cursor::GetMousePosition(const BaseEngine& engine)
{
	return {engine.GetCurrentMouseX(), engine.GetCurrentMouseY()};
}

bool Cursor::MouseLeftClicked(const BaseEngine& engine)
{
	return engine.GetCurrentButtonStates() & SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool Cursor::MouseRightClicked(const BaseEngine& engine)
{
	return engine.GetCurrentButtonStates() & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

bool Cursor::IsSpecial() const
{
	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	return engine != nullptr && engine->GetPlayer().IsCasting();
}

const ImageData& Cursor::GetCursorImage() const
{
	if (this->IsSpecial())
		return this->cursor_image_on;
	else
		return this->cursor_image_off;
}
