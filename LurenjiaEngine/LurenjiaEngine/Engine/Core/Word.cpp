#include "Word.h"
#include "Camera.h"

CWord::CWord()
{
	camera = CreateObject<CCamera>("camera");
}
