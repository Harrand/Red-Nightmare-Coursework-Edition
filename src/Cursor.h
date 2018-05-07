#ifndef CURSOR_HPP
#define CURSOR_HPP
#include "DisplayableObject.h"
#include "Utility.h"
#include "JPGImage.h"
#include <utility>

class Cursor : public DisplayableObject
{
public:
	Cursor(BaseEngine* engine);
	~Cursor();
	virtual void Draw() override;
	virtual void DoUpdate(int iCurrentTime) override;
	BoundingBox2i GetBoundingBox() const;
	static Vector2i GetMousePosition(const BaseEngine& engine);
	static bool MouseLeftClicked(const BaseEngine& engine);
	static bool MouseRightClicked(const BaseEngine& engine);
	/// Returns true if the player is currently casting with it.
	bool IsSpecial() const;
private:
	const ImageData& GetCursorImage() const;

	ImageData cursor_image_off, cursor_image_on;
	bool just_paused;
};

#endif