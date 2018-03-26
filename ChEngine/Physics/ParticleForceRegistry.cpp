#include "ParticleForceRegistry.h"
#include "ForceGenerator.h"
#include "Particle.h"

ParticleForceRegistry::ParticleForceRegistry()
{

}

void ParticleForceRegistry::Add(Particle* particle, ForceGenerator* FG)
{
	ParticleForceRegistration* registration = new ParticleForceRegistration(particle, FG);
	registrations.push_back(*registration);
}

void ParticleForceRegistry::Remove(Particle* particle, ForceGenerator* FG)
{
	auto it = registrations.begin();
	for (auto registration : registrations)
	{
		if (registration.particle == particle && registration.FG == FG)
		{
			registrations.erase(it);
		}
		++it;
	}
}

void ParticleForceRegistry::Clear()
{
	for (auto registration : registrations)
	{
		registration.DestroyFG();
		//delete &registration;
	}
	registrations.clear();
}

void ParticleForceRegistry::UpdateForces(float duration)
{
	for (auto registration : registrations)
	{
		registration.FG->UpdateForce(registration.particle, duration);
	}
}

void ParticleForceRegistry::ParticleForceRegistration::DestroyFG()
{
	delete FG;
}
