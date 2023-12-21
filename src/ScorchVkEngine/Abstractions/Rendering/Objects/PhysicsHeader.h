#pragma once
#include <Windows.h>
#include <glm/glm.hpp>

struct RigidBody
{
    glm::vec2      currPos;
    glm::vec2      prevPos;
    glm::vec2 acceleration;
    glm::vec2     velocity;

    void updatePos(const float dt)
    {
        velocity = currPos - prevPos;
        prevPos = currPos;

        currPos += velocity + acceleration * dt * dt;

        acceleration = {};
    }

    void accelerate(const glm::vec2 acc){ acceleration += acc; }

    bool operator==(const RigidBody& body2) const { return this->currPos == body2.currPos; }
};

namespace Physics
{
    constexpr uint32_t subSteps = 8;
    constexpr float rad = 1.0f; // Circle Radius

    constexpr glm::vec2 gravity = { 0.0f, -50.0f };

    constexpr float boundsX{66.0f}, boundsY{34.0f};

    inline void Update(std::vector<RigidBody>& rBodies, const float deltaTime)
    {
        const float subDeltaTime = deltaTime * 0.125f; // [ deltaTime / 8 ] Can Be Written As [ deltaTime * 0.125f ]

        for (uint32_t ss = subSteps; ss--;)
        {
            for (RigidBody& body : rBodies)
            {
                body.accelerate(gravity);

                // Set Bounding Box
                if (fabs(body.currPos.x) > boundsX - rad)
                    body.currPos.x > 0 ? body.currPos.x *= (boundsX - rad) / body.currPos.x : body.currPos.x *= (boundsX - rad) / -body.currPos.x;
                if (fabs(body.currPos.y) > boundsY - rad)
                    body.currPos.y > 0 ? body.currPos.y *= (boundsY - rad) / body.currPos.y : body.currPos.y *= (boundsY - rad) / -body.currPos.y;
            }

            // Body Collision Solver
            const uint32_t bodyCount = rBodies.size();
            for (uint32_t i = 0; i < bodyCount; ++i)
            {
                RigidBody& body1 = rBodies[i];
                for (uint32_t k = i + 1; k < bodyCount; ++k)
                {
                    RigidBody& body2 = rBodies[k];
                    const glm::vec2 collisionAxis = body1.currPos - body2.currPos;
                    const float squareDistance = collisionAxis.x * collisionAxis.x + collisionAxis.y * collisionAxis.y;

                    if (squareDistance < (rad + rad) * (rad + rad))
                    {
                        const float distance =  sqrtf(squareDistance);
                        const glm::vec2 n = collisionAxis / distance;
                        const float delta = rad + rad - distance;
                        body1.currPos += 0.5f * delta * n;
                        body2.currPos -= 0.5f * delta * n;
                    }
                }
            }

            // Apply Updated Position
            for (RigidBody& body : rBodies) body.updatePos(subDeltaTime);
        }
    }
}
