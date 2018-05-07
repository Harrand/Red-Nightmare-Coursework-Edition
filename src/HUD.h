#ifndef HUD_H
#define HUD_H
#include "DisplayableObject.h"

class HUD : public DisplayableObject
{
public:
	HUD(BaseEngine* engine);
	~HUD() = default;
	virtual void Draw() override;
};

#endif