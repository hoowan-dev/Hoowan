#pragma once

#include <functional>

#include "Hoowan/Core/Timestep.h"

#include "box2d/b2_world.h"

namespace Hoowan
{
	class Physics2DWorld
	{
	public:
		// Box2D is stepped at a fixed rate, independent of the framerate
		static constexpr float FixedTimeStep = 1.0f / 60.0f;

		// Upper bound on catch-up steps per frame, so a long hitch (or a breakpoint)
		// doesn't make the simulation fall further and further behind
		static constexpr uint32_t MaxStepsPerFrame = 5;

		Physics2DWorld(float gravity);
		~Physics2DWorld();

		// Advances the simulation by one fixed step
		void SimulateFrame();

		// Accumulates frame time and runs as many fixed steps as fit into it.
		// beforeStep runs before each step, so callers can snapshot body state
		// for interpolation.
		void Update(Timestep ts, const std::function<void()>& beforeStep);

		// How far (0..1) the current frame is between the last two fixed steps
		float GetInterpolationAlpha() const { return m_Accumulator / FixedTimeStep; }

		Ref<b2World> GetWorldReference();

	private:
		Ref<b2World> m_World;
		float m_Accumulator = 0.0f;
	};
}
