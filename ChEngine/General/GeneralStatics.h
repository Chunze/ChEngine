#ifndef GENERAL_STATICS_H
#define GENERAL_STATICS_H
#pragma once
#include <memory>
#include "glm.h"

namespace ChEngine
{
	namespace GeneralStatics
	{
		template <class T>
		static std::shared_ptr<T> NewGameObject(GameContext* gameContext, World* world, vec3 Location = vec3(), quat Rotation = quat())
		{
			auto Object = make_shared<T>(gameContext, world, Location, Rotation);

			world->RegisterGameObject(Object);

			return Object;
		}

		template <class T>
		static std::shared_ptr<T> NewComponent(GameContext* gameContext, World* world)
		{
			auto Component = make_shared<T>(gameContext, world);

			world->RegisterComponent(Component);

			return Component;
		}

	}
}
#endif
