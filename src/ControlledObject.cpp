#include "header.h"
#include "templates.h"
#include "ControlledObject.h"
#include "PsyhshEngine.h"
#include <string>


ControlledObject::ControlledObject(BaseEngine* engine): DisplayableObject(engine), can_move(true)
{
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = 100;
	m_iCurrentScreenY = m_iPreviousScreenY = 100;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = 200;
	m_iDrawHeight = 100;
	// And make it visible
	SetVisible(true);
}

void ControlledObject::Draw()
{
	GetEngine()->DrawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x4444ff);
	using namespace std::string_literals;
	GetEngine()->DrawForegroundString(m_iCurrentScreenX, m_iCurrentScreenY, (std::to_string(m_iCurrentScreenX) + ","s + std::to_string(m_iCurrentScreenY)).c_str(), 0x000000);
	// This will store the position at which the object was drawn
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionForUndraw();
}

void ControlledObject::DoUpdate(int iCurrentTime)
{
	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	if (!this->can_move || engine->IsPaused())
		return;
	// Change position if player presses a key
	if (this->IsMovingUp() && m_iCurrentScreenY-2 >= 0)
		m_iCurrentScreenY -= 2;
	if (this->IsMovingDown() && m_iCurrentScreenY+2 + m_iDrawHeight <= 600)
		m_iCurrentScreenY += 2;
	if (this->IsMovingLeft() && m_iCurrentScreenX-2 >= 0)
		m_iCurrentScreenX -= 2;
	if (this->IsMovingRight() && m_iCurrentScreenX+2 + m_iDrawWidth <= 800)
		m_iCurrentScreenX += 2;
	// Ensure that the object gets redrawn on the display, if something changed
	RedrawObjects();
}

bool ControlledObject::IsMovingUp() const
{
	return this->GetEngine()->IsKeyPressed(ControlledObject::UP_KEY);
}

bool ControlledObject::IsMovingDown() const
{
	return this->GetEngine()->IsKeyPressed(ControlledObject::DOWN_KEY);
}

bool ControlledObject::IsMovingRight() const
{
	return this->GetEngine()->IsKeyPressed(ControlledObject::RIGHT_KEY);
}

bool ControlledObject::IsMovingLeft() const
{
	return this->GetEngine()->IsKeyPressed(ControlledObject::LEFT_KEY);
}

bool ControlledObject::IsMoving() const
{
	return this->IsMovingDown() || this->IsMovingUp() || this->IsMovingLeft() || this->IsMovingRight();
}