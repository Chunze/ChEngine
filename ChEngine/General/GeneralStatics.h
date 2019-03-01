#ifndef GENERAL_STATICS_H
#define GENERAL_STATICS_H
#pragma once
#include <memory>

namespace GeneralStatics
{
// 	template <class T>
// 	static std::shared_ptr<T> NewGameObject(GameContext* gameContext, World* world)
// 	{
// 
// 	}

	template <class T>
	static std::shared_ptr<T> NewComponent(GameContext* gameContext, World* world)
	{
		auto Component = make_shared<T>(gameContext, world);

		world->RegisterComponent(Component);

		return Component;
	}

}
#endif
