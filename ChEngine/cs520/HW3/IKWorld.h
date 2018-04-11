#ifndef IK_WORLD_H
#define IK_WORLD_H

#include "World.h"

class IKWorld : public World
{
public:
	IKWorld(GameContext* gameContext);

	virtual void Update(float Delta) override;

	virtual void InitCamera() override;

	void InputHandler();

	class TailSkel* Tail;
};

#endif
