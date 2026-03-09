#include "Engine/Engine.h"
#include "Level/MenuLevel.h"

using namespace Wanted;
int main()
{

	Engine engine;
	engine.SetNewLevel(new MenuLevel());
	engine.Run();

	return 0;
}