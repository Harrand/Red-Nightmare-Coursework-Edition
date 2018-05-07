#ifndef SINE_OBJECT_H
#define SINE_OBJECT_H
#include "DisplayableObject.h"
#include <functional>
class SineObject : public DisplayableObject
{
public:
	SineObject(BaseEngine* engine);
	~SineObject();

	virtual void Draw() override;
	virtual void DoUpdate(int iCurrentTime) override;
};

#endif

