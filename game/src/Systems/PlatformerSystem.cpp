#include "Systems/PlatformerSystem.hpp"

#include <limits>
#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Components/ColliderComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/PlatformComponent.hpp"
#include "Components/PlatformerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/PlayerSystem.hpp"
#include "Tools/Polygon.hpp"

namespace c = components;
namespace e = entityx;

#define NO_EXCEPTIONS std::vector<entityx::Entity>()


namespace systems
{

PlatformerSystem::PlatformerSystem(HitboxUpdaterSystem::InfiniteQuadTreesGrid& quadtreesGrid) :
    entityx::System<PlatformerSystem>(),
    m_quadtreesGrid(quadtreesGrid)
{

}

namespace
{

void MovePolygon(tools::Polygon &poly, float dx, float dy)
{
    poly.SetOrigin(poly.GetOrigin() + sf::Vector2f(dx, dy));
    poly.ComputeGlobalVertices();
    poly.ComputeGlobalEdges();
}

std::vector<e::Entity> GetPotentialObstacles(e::EntityManager& es, entityx::Entity object, float maxMoveLength, int types = c::PlatformComponent::All)
{
    std::vector<e::Entity> potentialObstacles;

    e::ComponentHandle<c::PositionComponent> cBox = object.component<c::PositionComponent>();
    e::ComponentHandle<c::PlatformerComponent> cPhysic = object.component<c::PlatformerComponent>();

    if(!cBox || !object.component<c::HitboxComponent>())
        return potentialObstacles;

    sf::FloatRect objectBoundingBox;
    objectBoundingBox.left = cBox->x - maxMoveLength;
    objectBoundingBox.top = cBox->y - maxMoveLength;
    objectBoundingBox.width = cBox->width + 2*maxMoveLength;
    objectBoundingBox.height = cBox->height + 2*maxMoveLength;

    for(e::Entity obstacle : es.entities_with_components<c::PlatformComponent, c::PositionComponent, c::HitboxComponent>())
    {
        entityx::ComponentHandle<c::PositionComponent> obstacleCBox = obstacle.component<c::PositionComponent>();
        entityx::ComponentHandle<c::PlatformComponent> obstacleCObs = obstacle.component<c::PlatformComponent>();
        sf::FloatRect obstacleBoundingBox(obstacleCBox->x, obstacleCBox->y, obstacleCBox->width, obstacleCBox->height);

        if((obstacle.id() != object.id()) && (cPhysic->layer == obstacleCObs->layer) && ((types & obstacleCObs->platformType) != 0) && objectBoundingBox.intersects(obstacleBoundingBox))
            potentialObstacles.push_back(obstacle);
    }

    return potentialObstacles;
}

bool IsCollidingObstacle(tools::Polygon polygon, std::vector<e::Entity> potentialObstacles, std::vector<e::Entity> except, int onlyOfType = c::PlatformComponent::All)
{
    for(e::Entity& obstacle : potentialObstacles)
    {
        //Ignore the obstacle if it's an exception
        if(std::find(except.begin(), except.end(), obstacle) != except.end())
        {
            continue;
        }

        //Get the collision polygon
        entityx::ComponentHandle<c::HitboxComponent> obstacleCPolygon = obstacle.component<c::HitboxComponent>();
        entityx::ComponentHandle<c::PlatformComponent> obstacleCO = obstacle.component<c::PlatformComponent>();

        if(!obstacleCPolygon || !obstacleCO || ((onlyOfType & obstacleCO->platformType) == 0) || !obstacleCO->activated)
        {
            continue;
        }

        //Test if there is a collision
        if(tools::PolygonCollision(polygon, obstacleCPolygon->getHitbox()))
        {
            return true;
        }
    }

    return false;
}

bool IsCollidingObstacle(tools::Polygon polygon, e::Entity obstacle)
{
    //Get the collision polygon
    entityx::ComponentHandle<c::HitboxComponent> obstacleCPolygon = obstacle.component<c::HitboxComponent>();
    entityx::ComponentHandle<c::PlatformComponent> obstacleCO = obstacle.component<c::PlatformComponent>();

    if(!obstacleCPolygon || !obstacleCO || !obstacleCO->activated)
    {
        return false;
    }

    //Test if there is a collision
    if(tools::PolygonCollision(polygon, obstacleCPolygon->getHitbox()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<e::Entity> GetCollidingObstacles(tools::Polygon polygon, std::vector<e::Entity> potentialObstacles, std::vector<e::Entity> except, int types = c::PlatformComponent::All)
{
    std::vector<e::Entity> collidingObstacles;

    for(e::Entity& obstacle : potentialObstacles)
    {
        //Ignore the obstacle if it's an exception
        if(std::find(except.begin(), except.end(), obstacle) != except.end())
        {
            continue;
        }

        //Get the collision polygon
        entityx::ComponentHandle<c::HitboxComponent> obstacleCPolygon = obstacle.component<c::HitboxComponent>();
        entityx::ComponentHandle<c::PlatformComponent> obstacleCO = obstacle.component<c::PlatformComponent>();

        if(!obstacleCPolygon || !obstacleCO || ((types & obstacleCO->platformType) == 0) || !obstacleCO->activated)
        {
            continue;
        }

        //Test if there is a collision
        if(tools::PolygonCollision(polygon, obstacleCPolygon->getHitbox()))
        {
            collidingObstacles.push_back(obstacle);
        }
    }

    return collidingObstacles;
}

bool IsOnFloor(tools::Polygon polygon, std::vector<e::Entity> potentialFloors, std::vector<e::Entity> except = NO_EXCEPTIONS)
{
    MovePolygon(polygon, 0, 5.f);
    return IsCollidingObstacle(polygon, potentialFloors, except);
}

e::Entity GetFloor(tools::Polygon polygon, std::vector<e::Entity> potentialFloors, std::vector<e::Entity> except = NO_EXCEPTIONS)
{
    MovePolygon(polygon, 0, 5.f);
    std::vector<e::Entity> floors = GetCollidingObstacles(polygon, potentialFloors, except);

    if(floors.size() == 0)
        return e::Entity();
    else
        return floors[0];
}

void ResetPolygonPosition(e::Entity entity, tools::Polygon &poly)
{
    if(!entity.component<c::PositionComponent>())
        return;

    poly.SetOrigin(sf::Vector2f(entity.component<c::PositionComponent>()->x,
                                entity.component<c::PositionComponent>()->y));

    poly.ComputeGlobalVertices();
    poly.ComputeGlobalEdges();
}

}

void PlatformerSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<c::PositionComponent, c::PlatformerComponent, c::HitboxComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position,
        c::PlatformerComponent& platformer,
        c::HitboxComponent& hitbox)
    {
        float oldX = position.x;
        float oldY = position.y;

        float requestedXMove(0.f), requestedYMove(0.f);

        //Watch received events to see the requested moves
        bool wantsToJump(platformer.wantsToJump);
        bool wantsToGoLeft(platformer.wantsToGoLeft);
        bool wantsToGoRight(platformer.wantsToGoRight);
        //TODO: Put the previous variables as attributes of PlatformerComponent
        //and the future PlayerSystem will set them to true or false depending
        //of the pressed keys (according to the config)

        tools::Polygon polygon = hitbox.getHitbox();
        polygon.ComputeGlobalVertices();
        polygon.ComputeGlobalEdges();

        //Update moving speed according to inputs
        if(wantsToGoLeft && !wantsToGoRight)
        {
            platformer.currentSpeed -= platformer.acceleration * dt;
            if(platformer.currentSpeed < -platformer.maxSpeed)
                platformer.currentSpeed = -platformer.maxSpeed;

            requestedXMove += platformer.currentSpeed * dt;
        }
        else if(wantsToGoRight && !wantsToGoLeft)
        {
            platformer.currentSpeed += platformer.acceleration * dt;
            if(platformer.currentSpeed > platformer.maxSpeed)
                platformer.currentSpeed = +platformer.maxSpeed;

            requestedXMove += platformer.currentSpeed * dt;
        }
        else
        {
            bool isPositive = platformer.currentSpeed > 0.f;
            if(isPositive)
            {
                platformer.currentSpeed -= platformer.deceleration * dt;
                if(platformer.currentSpeed < 0)
                    platformer.currentSpeed = 0;
            }
            else
            {
                platformer.currentSpeed += platformer.deceleration * dt;
                if(platformer.currentSpeed > 0)
                    platformer.currentSpeed = 0;
            }

            requestedXMove += platformer.currentSpeed * dt;
        }
        MovePolygon(polygon, requestedXMove, 0.f);

        //Get all potential obstacle
        std::vector<entityx::Entity> potentialObstacles = GetPotentialObstacles(es, entity, std::max(platformer.currentSpeed * dt, platformer.maxFallingSpeed * dt));

        //Update position according to the floor movements
        std::vector<entityx::Entity> overlappingJumpthrus = GetCollidingObstacles(polygon, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Jumpthru);
        std::vector<entityx::Entity>::iterator ground = std::find(overlappingJumpthrus.begin(), overlappingJumpthrus.end(), platformer.groundEntity); //Test the current ground (even if it's a jumpthru)
        if(ground != overlappingJumpthrus.end())
            overlappingJumpthrus.erase(ground);

        float requestedXFloorMove(0.f), requestedYFloorMove(0.f);
        if(IsOnFloor(polygon, potentialObstacles, overlappingJumpthrus) && platformer.groundEntity && platformer.groundEntity == GetFloor(polygon, potentialObstacles, overlappingJumpthrus))
        {
            e::ComponentHandle<c::PositionComponent> floorCPos = platformer.groundEntity.component<c::PositionComponent>();

            requestedXMove += floorCPos->x - platformer.oldFloorPosX;
            requestedYMove += floorCPos->y - platformer.oldFloorPosY;
            requestedXFloorMove = floorCPos->x - platformer.oldFloorPosX;
            requestedYFloorMove = floorCPos->y - platformer.oldFloorPosY;
        }

        //Move the player's hitbox if the floor has moved
        MovePolygon(polygon, requestedXFloorMove, requestedYFloorMove);

        //Get all colliding platforms and give them to CollisionSystem (it will filter those that have a CollidableComponent)
        //(after the X change)
        if(entity.has_component<c::ColliderComponent>())
        {
            auto collidedPlatforms = GetCollidingObstacles(polygon, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::All);
            for(auto& platform : collidedPlatforms)
            {
                emit<ExtraSystemCollisionMessage>(entity, platform);
            }
        }

        //Detect collision on the X-axis
        bool reqXPositive = requestedXMove > 0.f;
        while(IsCollidingObstacle(polygon, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform))
        {
            //Try to move the object on Y-axis to support slopes
            MovePolygon(polygon, 0.f, -1-ceil(abs(requestedXMove)));
            if(!IsCollidingObstacle(polygon, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform))
            {
                requestedYMove += -1-ceil(abs(requestedXMove));

                //Drop the object onto the obstacle
                while(!IsCollidingObstacle(polygon, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform))
                {
                    requestedYMove++;
                    MovePolygon(polygon, 0.f, 1.f);
                }
                requestedYMove--;
                MovePolygon(polygon, 0.f, -1.f);

                break;
            }

            ResetPolygonPosition(entity, polygon);
            MovePolygon(polygon, 0.f, requestedYMove);

            //Push the object back on X-axis
            requestedXMove += (reqXPositive ? -1 : 1);
            if(reqXPositive && requestedXMove < 0.f)
                requestedXMove = 0.f;
            else if(!reqXPositive && requestedXMove > 0.f)
                requestedXMove = 0.f;

            MovePolygon(polygon, requestedXMove, 0.f);
        }

        overlappingJumpthrus = GetCollidingObstacles(polygon, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Jumpthru); //Test the current ground (even if it's a jumpthru)
        ground = std::find(overlappingJumpthrus.begin(), overlappingJumpthrus.end(), platformer.groundEntity);
        if(ground != overlappingJumpthrus.end())
            overlappingJumpthrus.erase(ground);

        //Move the object on Y-axis
        // - Falling
        if(platformer.fallingSpeed < platformer.maxFallingSpeed)
        {
            platformer.fallingSpeed += platformer.gravity * dt;
        }
        else
        {
            platformer.fallingSpeed = platformer.maxFallingSpeed;
        }
        requestedYMove += platformer.fallingSpeed * dt;
        float requestedFall = platformer.fallingSpeed * dt;

        // - Jumping
        if(wantsToJump && ((IsOnFloor(polygon, potentialObstacles) && platformer.jumpingSpeed <= 0.f) || platformer.canJumpAgain))
        {
            platformer.jumpingSpeed = platformer.maxJumpingSpeed;
            platformer.fallingSpeed = 0.f;
            platformer.canJumpAgain = false;

            //TODO: Call lua function like onJumping here !
            //events->emit<SoundEvent>(entity, "jump", SoundEvent::Play);
        }
        if(platformer.jumpingSpeed > 0.f)
        {
            platformer.jumpingSpeed -= platformer.gravity * dt;
        }
        else if(platformer.jumpingSpeed < 0.f)
        {
            platformer.jumpingSpeed = 0.f;
        }

        requestedYMove -= platformer.jumpingSpeed * dt;
        requestedFall -= platformer.jumpingSpeed * dt;

        //Detect collision on Y-axis
        bool reqYPositive = requestedFall > 0.f;
        MovePolygon(polygon, 0.f, requestedFall);

        //Get all colliding platforms and give them to CollisionSystem (it will filter those that have a CollidableComponent)
        //(after the Y change)
        if(entity.has_component<c::ColliderComponent>())
        {
            auto collidedPlatforms = GetCollidingObstacles(polygon, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::All);
            for(auto& platform : collidedPlatforms)
            {
                emit<ExtraSystemCollisionMessage>(entity, platform);
            }
        }

        //Delete Jumpthru which are under or overlapping the player
        std::vector<entityx::Entity> allCollidingObstacles = GetCollidingObstacles(polygon, potentialObstacles, overlappingJumpthrus);

        while((IsCollidingObstacle(polygon, allCollidingObstacles, NO_EXCEPTIONS) && requestedYMove >= requestedYFloorMove) ||
              (IsCollidingObstacle(polygon, allCollidingObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform) && requestedYMove < requestedYFloorMove))
        {
            platformer.fallingSpeed = 0.f;
            platformer.jumpingSpeed = 0.f;

            if(reqYPositive)
            {
                requestedFall--;
                requestedYMove--;
                MovePolygon(polygon, 0.f, -1.f);
            }
            else
            {
                requestedFall++;
                requestedYMove++;
                MovePolygon(polygon, 0.f, +1.f);
            }

            //Ignore micro movement to avoid the "shaking" effect
            if(abs(requestedFall) < 1.f)
            {
                requestedYMove -= requestedFall;
                MovePolygon(polygon, 0.f, -requestedFall);
                requestedFall = 0.f;
            }
        }

        //Update current floor
        if(IsOnFloor(polygon, potentialObstacles, overlappingJumpthrus))
        {
            platformer.groundEntity = GetFloor(polygon, potentialObstacles, overlappingJumpthrus);

            e::ComponentHandle<c::PositionComponent> floorCPos = platformer.groundEntity.component<c::PositionComponent>();
            platformer.oldFloorPosX = floorCPos->x;
            platformer.oldFloorPosY = floorCPos->y;
        }
        else
        {
            platformer.groundEntity = entityx::Entity();
        }

        position.x += requestedXMove;
        position.y += requestedYMove;

        if(IsOnFloor(polygon, potentialObstacles, overlappingJumpthrus))
        {
            if(abs(position.x - oldX - requestedXFloorMove) > 0.1f)
                platformer.movementStateCallbacks.setState(c::PlatformerComponent::Walking);
            else
                platformer.movementStateCallbacks.setState(c::PlatformerComponent::Idle);
        }
        else
        {
            if(position.y - oldY < 0)
                platformer.movementStateCallbacks.setState(c::PlatformerComponent::Jumping);
            else
                platformer.movementStateCallbacks.setState(c::PlatformerComponent::Falling);
        }
        platformer.movementStateCallbacks.callIfNeeded(lua::EntityHandle(entity));

        if(position.x - oldX > requestedXFloorMove + 0.01f)
        {
            platformer.directionStateCallbacks.setState(c::PlatformerComponent::Right);
        }
        else if(position.x - oldX < requestedXFloorMove - 0.01f)
        {
            platformer.directionStateCallbacks.setState(c::PlatformerComponent::Left);
        }
        platformer.directionStateCallbacks.callIfNeeded(lua::EntityHandle(entity));
    });
}

}
