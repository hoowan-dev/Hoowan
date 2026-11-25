#include "hwpch.h"
#include "Physics2DWorld.h"

#include "box2d/b2_math.h"

namespace Hoowan
{
	Physics2DWorld::Physics2DWorld(float gravity)
		: m_World(CreateRef<b2World>(b2Vec2(0.0f, gravity)))
	{

	}

	Physics2DWorld::~Physics2DWorld()
	{

	}

	void Physics2DWorld::SimulateFrame()
	{
		m_World->Step(FixedTimeStep, 6, 2);
	}

	void Physics2DWorld::Update(Timestep ts, const std::function<void()>& beforeStep)
	{
		m_Accumulator += ts.GetSeconds();

		uint32_t steps = 0;
		while (m_Accumulator >= FixedTimeStep && steps < MaxStepsPerFrame)
		{
			if (beforeStep)
				beforeStep();

			SimulateFrame();
			m_Accumulator -= FixedTimeStep;
			steps++;
		}

		// Hit the step cap: drop the backlog instead of carrying it into later frames
		if (m_Accumulator >= FixedTimeStep)
			m_Accumulator = 0.0f;
	}

	Ref<b2World> Physics2DWorld::GetWorldReference()
	{
		return m_World;
	}

}