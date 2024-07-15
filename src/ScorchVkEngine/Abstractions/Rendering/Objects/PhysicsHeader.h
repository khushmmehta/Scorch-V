#pragma once

#include <glm/glm.hpp>
#include <cmath>

constexpr float rad = 0.5f; // Circle Radius
constexpr float diam = 1.0f; // Circle Diameter

struct RigidBody
{
    glm::vec2      currPos;
    glm::vec2      prevPos;
    glm::vec2 acceleration;
    glm::vec2     velocity;

    uint32_t xIndex;

    RigidBody(glm::vec2 cPos, glm::vec2 pPos, uint32_t i)
    : currPos(cPos), prevPos(pPos), acceleration(), velocity(cPos - pPos), xIndex(i)
    {}

    void updatePos(const float dt)
    {
        velocity = currPos - prevPos;
        prevPos = currPos;

        currPos += velocity + acceleration * dt * dt;

        acceleration = {};
    }

    void accelerate(const glm::vec2 acc){ acceleration += acc; }
};

struct ShelfBook
{
    std::vector<RigidBody*> rBodies;

    ShelfBook()
    {
        rBodies.reserve(512);
    }
};

struct Shelf
{
    std::vector<ShelfBook> books{128};

    void write(RigidBody* body) { books[floor(body->currPos.x + 64)].rBodies.emplace_back(body); }
    void erase() { for (ShelfBook book : books) book.rBodies.clear(); }
};

namespace Physics
{
    constexpr uint32_t subSteps = 16;
    constexpr glm::vec2 gravity = { 0.0f, -100.0f };
    constexpr float boundsX{64.0f}, boundsY{36.0f}; // IMPLEMENT AUTOMATIC BOUND UPDATING TO SCREEN WIDTH & HEIGHT

    inline void SolveCollisions(RigidBody* body1, RigidBody* body2)
    {
        const glm::vec2 collisionAxis = body1->currPos - body2->currPos;

        const float squareDistance = collisionAxis.x * collisionAxis.x + collisionAxis.y * collisionAxis.y;

        if (squareDistance < diam * diam)
        {
            const float distance = sqrt(squareDistance);
            const glm::vec2 n = collisionAxis / distance;
            const float delta = diam - distance;
            body1->currPos += 0.5f * delta * n;
            body2->currPos -= 0.5f * delta * n;
        }
    }

    inline void CheckBooksCollision(ShelfBook& book1, ShelfBook& book2)
    {
        /*for (RigidBody* body1 : book1.rBodies)
            for (RigidBody* body2 : book2.rBodies)
                if (body1->xIndex != body2->xIndex)
                    if (fabs(body1->currPos.x - body2->currPos.x < diam))
                        SolveCollisions(body1, body2);*/

        for (uint32_t i = 0; i < book1.rBodies.size(); i++)
        {
            RigidBody* body1 = book1.rBodies[i];

            for (uint32_t j = 0; j < book2.rBodies.size(); j++)
            {
                RigidBody* body2 = book2.rBodies[j];

                if (body1->xIndex == body2->xIndex) break;

                if (fabs(body1->currPos.x - body2->currPos.x < diam)) SolveCollisions(body1, body2);
            }
        }
    }

    inline void Update(std::vector<RigidBody*>& rBodies, Shelf& shelf, const float deltaTime)
    {
        const float subDeltaTime = deltaTime / subSteps;

        for (uint32_t ss = subSteps; ss--;)
        {
            for (RigidBody* body : rBodies)
            {
                body->accelerate(gravity);

                // Set Bounding Box
                if (fabs(body->currPos.x) > boundsX - rad)
                    body->currPos.x > 0 ? body->currPos.x *= (boundsX - rad) / body->currPos.x : body->currPos.x *= (boundsX - rad) / -body->currPos.x;
                if (fabs(body->currPos.y) > boundsY - rad)
                    body->currPos.y > 0 ? body->currPos.y *= (boundsY - rad) / body->currPos.y : body->currPos.y *= (boundsY - rad) / -body->currPos.y;

                shelf.write(body);
            }

            for (int i = 1; i < 127; ++i)
            {
                ShelfBook& currentBook = shelf.books[i];

                for (int j = -1; j < 2; ++j)
                {
                    ShelfBook& otherBook = shelf.books[i + j];
                    CheckBooksCollision(currentBook, otherBook);
                }
            }

            shelf.erase();

            /*for (uint32_t i = 0; i < rBodies.size(); i++)
            {
                RigidBody* body1 = rBodies[i];

                for (uint32_t j = i + 1; j < rBodies.size(); j++)
                {
                    RigidBody* body2 = rBodies[j];
                    if (fabs(body1->currPos.x - body2->currPos.x < diam)) SolveCollisions(body1, body2);
                }
            }*/

            // Apply Updated Position
            for (RigidBody* body : rBodies) body->updatePos(subDeltaTime);
        }
    }
}
