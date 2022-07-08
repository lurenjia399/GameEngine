#include "CoreMinimalObject.h"

vector<CCoreMinimalObject*> ObjectPool;

CCoreMinimalObject::CCoreMinimalObject()
{
	isCanTick = true;
	ObjectPool.emplace_back(this);
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator it = ObjectPool.begin(); it != ObjectPool.end(); it++)
	{
		if (*it == this)
		{
			ObjectPool.erase(it);
			break;
		}
	}
}
