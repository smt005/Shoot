#include "EffectObject.h"

EffectObject::EffectObject()
{

}

void EffectObject::action()
{
	_scale += 0.05;
	_alpha -= 0.05;

	if (_alpha <= 0.0f)
		_live = false;
}