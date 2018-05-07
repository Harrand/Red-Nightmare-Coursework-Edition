#include "header.h"
#include "templates.h"
#include "SineObject.h"


SineObject::SineObject(BaseEngine* engine) : DisplayableObject(engine)
{
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = 100;
	m_iCurrentScreenY = m_iPreviousScreenY = 100;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = 100;
	m_iDrawHeight = 50;
	// And make it visible
	SetVisible(true);
}


SineObject::~SineObject() {}

void SineObject::Draw()
{
	GetEngine()->DrawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0xccccff);
	// This will store the position at which the object was drawn
	// so that the background can be drawn over the top.
	// This will then remove the object from the screen.
	StoreLastScreenPositionForUndraw();
}

void SineObject::DoUpdate(int iCurrentTime)
{
	// Change position if player presses a key
	static int counter = 0;
	m_iCurrentScreenX = m_iStartDrawPosX + 500 + 250 * std::sin((++counter) / 10.0);
	// Ensure that the object gets redrawn on the display, if something changed
	RedrawObjects();
}