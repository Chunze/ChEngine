#include "DrawList.h"

DrawList::DrawList(GameContext gameContext)
	: BaseClass(gameContext)
{

}

void DrawList::Add(DrawListElement elementToAdd)
{
	bHasNewData = true;
	elements.push_back(elementToAdd);
}

void DrawList::Clear()
{
	bHasNewData = false;
	elements.clear();
}
