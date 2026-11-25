#pragma once

#include "glm/glm.hpp"

#include "box2d/b2_body.h"
#include "box2d/b2_world.h"

namespace Hoowan
{
	class CollisionBody
	{
	public:
		static void Init(Ref<b2World> world);

		CollisionBody(bool isDynamic);
		CollisionBody(bool isDynamic, glm::vec2& position);
		CollisionBody(bool isDynamic, glm::mat4& transform);
		CollisionBody(bool isDynamic, glm::vec2& position, glm::vec2& scale);

		~CollisionBody();

		// A CollisionBody owns its b2Body, so it can be moved but not copied
		CollisionBody(const CollisionBody&) = delete;
		CollisionBody& operator=(const CollisionBody&) = delete;
		CollisionBody(CollisionBody&& other) noexcept;
		CollisionBody& operator=(CollisionBody&& other) noexcept;

		// Methods
		void SetLinearVelocity(glm::vec2& velocity);
		void ApplyForce(glm::vec2& force);

		// Records the current position/angle as the "previous" physics state.
		// Call right before each world step.
		void SavePreviousState();

		// Getters
		// alpha blends between the previous and current physics state (0 = previous,
		// 1 = current), smoothing motion when the framerate differs from the physics rate
		glm::mat4 GetTransform(float alpha = 1.0f) const;

	private:
		static Ref<b2World> s_World;

		void DestroyBody();

		b2Body* m_Body = nullptr;
		glm::vec2 m_PreviousPosition = { 0.0f, 0.0f };
		float m_PreviousAngle = 0.0f;	// radians
	};
}