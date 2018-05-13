#ifndef __EFFEC_OBJECT_H__
#define __EFFEC_OBJECT_H__

#include "../Object/Object.h"

class EffectObject : public Object
{
public:
	EffectObject();

public:
	bool _live = true;
	float _scale = 0.0f;
	float _alpha = 1.0f;

public:
	void action() override;
};

#endif	// __EFFEC_OBJECT_H__