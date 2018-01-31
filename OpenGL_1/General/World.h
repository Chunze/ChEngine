#ifndef WORLD_H
#define WORLD_H
#pragma once

class World
{
public:

	virtual void LoadWorld(const char* fileName) = 0;
	virtual void Update(float Delta) = 0;
};

#endif
