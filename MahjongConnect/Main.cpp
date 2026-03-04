#include "Engine/Engine.h"
#include "Level/GameLevel.h"

using namespace Wanted;
int main()
{
	Engine engine;
	engine.SetNewLevel(new GameLevel());
	engine.Run();

	return 0;
}