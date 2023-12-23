#pragma once

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
};

struct Grid
{
    int width{50}, height{30};
    std::vector<std::vector<RigidBody*>> cells;

    Grid() { cells.resize(width * height); }

    void assignToCell(RigidBody* obj, float x, float y) { cells[floor(x + 50) + floor(-y + 50) * width].push_back(obj); }
};

namespace Physics
{
    constexpr uint32_t subSteps = 8;
    constexpr float rad = 1.0f; // Circle Radius

    constexpr glm::vec2 gravity = { 0.0f, -50.0f };

    constexpr float boundsX{50.0f}, boundsY{30.0f};

    inline void SolveCollisions(RigidBody* body1, RigidBody* body2)
    {
        const glm::vec2 collisionAxis = body1->currPos - body2->currPos;
        const float squareDistance = collisionAxis.x * collisionAxis.x + collisionAxis.y * collisionAxis.y;

        if (squareDistance < (rad + rad) * (rad + rad))
        {
            const float distance =  sqrtf(squareDistance);
            const glm::vec2 n = collisionAxis / distance;
            const float delta = rad + rad - distance;
            body1->currPos += 0.5f * delta * n;
            body2->currPos -= 0.5f * delta * n;
        }
    }

    inline void Update(std::vector<RigidBody>& rBodies, Grid& grid, const float deltaTime)
    {
        const float subDeltaTime = deltaTime * 0.125f;

        for (uint32_t ss = subSteps; ss--;)
        {
            // Bounding Box & Acceleration
            for (RigidBody& body : rBodies)
            {
                body.accelerate(gravity);

                if (fabs(body.currPos.x) > boundsX - rad)
                    body.currPos.x > 0 ? body.currPos.x *= (boundsX - rad) / body.currPos.x : body.currPos.x *= (boundsX - rad) / -body.currPos.x;
                if (fabs(body.currPos.y) > boundsY - rad)
                    body.currPos.y > 0 ? body.currPos.y *= (boundsY - rad) / body.currPos.y : body.currPos.y *= (boundsY - rad) / -body.currPos.y;
            }

            // Body Collision Solver
            for (RigidBody& body1 : rBodies)
                for (RigidBody& body2 : rBodies)
                    SolveCollisions(&body1, &body2);

            // Apply Updated Position
            for (RigidBody& body : rBodies) body.updatePos(subDeltaTime);
        }
    }
}
