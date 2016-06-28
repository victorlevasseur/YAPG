#include "Systems/PlatformerSystem.hpp"

#include <limits>
#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Collision/Polygon.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/PlatformerHitboxComponent.hpp"
#include "Components/PlatformComponent.hpp"
#include "Components/PlatformerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/PlayerSystem.hpp"

namespace c = components;
namespace e = entityx;

#define NO_EXCEPTIONS std::vector<entityx::Entity>()


namespace systems
{

PlatformerSystem::PlatformerSystem(collision::EntitySpatialGrid& quadtreesGrid) :
    entityx::System<PlatformerSystem>(),
    m_quadtreesGrid(quadtreesGrid)
{

}

namespace
{

std::vector<e::Entity> GetPotentialObstacles(e::EntityManager& es, const collision::EntitySpatialGrid& quadtreesGrid, entityx::Entity object, float maxMoveLength, int types = c::PlatformComponent::All)
{
    std::vector<e::Entity> potentialObstacles;

    e::ComponentHandle<c::PositionComponent> cBox = object.component<c::PositionComponent>();
    e::ComponentHandle<c::PlatformerComponent> cPhysic = object.component<c::PlatformerComponent>();

    if(!cBox || !object.component<c::PlatformerHitboxComponent>())
        return potentialObstacles;

    sf::FloatRect objectBoundingBox;
    objectBoundingBox.left = cBox->x - maxMoveLength;
    objectBoundingBox.top = cBox->y - maxMoveLength;
    objectBoundingBox.width = cBox->width + 2*maxMoveLength;
    objectBoundingBox.height = cBox->height + 2*maxMoveLength;

    auto collisionCandidates = quadtreesGrid.getEntitiesIntersectingAABB(objectBoundingBox);
    for(e::Entity obstacle : collisionCandidates)
    {
        if(!obstacle)
            continue;
        if(!obstacle.has_component<c::PositionComponent>() || !obstacle.has_component<c::PlatformerHitboxComponent>() || !obstacle.has_component<c::PlatformComponent>())
            continue;

        entityx::ComponentHandle<c::PlatformComponent> obstacleCObs = obstacle.component<c::PlatformComponent>();

        if((obstacle.id() != object.id()) && (cPhysic->layer == obstacleCObs->layer) && ((types & obstacleCObs->platformType) != 0))
            potentialObstacles.push_back(obstacle);
    }

    return potentialObstacles;
}

bool IsCollidingObstacle(collision::Polygon polygon, sf::Transform transform, std::vector<e::Entity> potentialObstacles, std::vector<e::Entity> except, int onlyOfType = c::PlatformComponent::All)
{
    for(e::Entity& obstacle : potentialObstacles)
    {
        //Ignore the obstacle if it's an exception
        if(std::find(except.begin(), except.end(), obstacle) != except.end())
        {
            continue;
        }

        //Get the collision polygon
        entityx::ComponentHandle<c::PositionComponent> obstacleCPosition = obstacle.component<c::PositionComponent>();
        entityx::ComponentHandle<c::PlatformerHitboxComponent> obstacleCPolygon = obstacle.component<c::PlatformerHitboxComponent>();
        entityx::ComponentHandle<c::PlatformComponent> obstacleCO = obstacle.component<c::PlatformComponent>();

        if(!obstacleCPosition || !obstacleCPolygon || !obstacleCO || ((onlyOfType & obstacleCO->platformType) == 0) || !obstacleCO->activated)
        {
            continue;
        }

        //Test if there is a collision
        sf::Transform obstacleTransform;
        obstacleTransform.translate(obstacleCPosition->x, obstacleCPosition->y);
        if(collision::Polygon::collides(polygon, obstacleCPolygon->getHitbox(), transform, obstacleTransform))
        {
            return true;
        }
    }

    return false;
}

bool IsCollidingObstacle(collision::Polygon polygon, sf::Transform transform, e::Entity obstacle)
{
    //Get the collision polygon
    entityx::ComponentHandle<c::PositionComponent> obstacleCPosition = obstacle.component<c::PositionComponent>();
    entityx::ComponentHandle<c::PlatformerHitboxComponent> obstacleCPolygon = obstacle.component<c::PlatformerHitboxComponent>();
    entityx::ComponentHandle<c::PlatformComponent> obstacleCO = obstacle.component<c::PlatformComponent>();

    if(!obstacleCPosition || !obstacleCPolygon || !obstacleCO || !obstacleCO->activated)
    {
        return false;
    }

    //Test if there is a collision
    sf::Transform obstacleTransform;
    obstacleTransform.translate(obstacleCPosition->x, obstacleCPosition->y);
    return collision::Polygon::collides(polygon, obstacleCPolygon->getHitbox(), transform, obstacleTransform);
}

std::vector<e::Entity> GetCollidingObstacles(collision::Polygon polygon, sf::Transform transform, std::vector<e::Entity> potentialObstacles, std::vector<e::Entity> except, int types = c::PlatformComponent::All)
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
        entityx::ComponentHandle<c::PositionComponent> obstacleCPosition = obstacle.component<c::PositionComponent>();
        entityx::ComponentHandle<c::PlatformerHitboxComponent> obstacleCPolygon = obstacle.component<c::PlatformerHitboxComponent>();
        entityx::ComponentHandle<c::PlatformComponent> obstacleCO = obstacle.component<c::PlatformComponent>();

        if(!obstacleCPosition || !obstacleCPolygon || !obstacleCO || ((types & obstacleCO->platformType) == 0) || !obstacleCO->activated)
        {
            continue;
        }

        //Test if there is a collision
        sf::Transform obstacleTransform;
        obstacleTransform.translate(obstacleCPosition->x, obstacleCPosition->y);
        if(collision::Polygon::collides(polygon, obstacleCPolygon->getHitbox(), transform, obstacleTransform))
        {
            collidingObstacles.push_back(obstacle);
        }
    }

    return collidingObstacles;
}

bool IsOnFloor(collision::Polygon polygon, sf::Transform transform, std::vector<e::Entity> potentialFloors, std::vector<e::Entity> except = NO_EXCEPTIONS)
{
    transform.translate(0.f, 5.f);
    return IsCollidingObstacle(polygon, transform, potentialFloors, except);
}

e::Entity GetFloor(collision::Polygon polygon, sf::Transform transform, std::vector<e::Entity> potentialFloors, std::vector<e::Entity> except = NO_EXCEPTIONS)
{
    transform.translate(0.f, 5.f);
    std::vector<e::Entity> floors = GetCollidingObstacles(polygon, transform, potentialFloors, except);

    if(floors.size() == 0)
        return e::Entity();
    else
        return floors[0];
}

}

void PlatformerSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<c::PositionComponent, c::PlatformerComponent, c::PlatformerHitboxComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position,
        c::PlatformerComponent& platformer,
        c::PlatformerHitboxComponent& hitbox)
    {
        float oldX = position.x;
        float oldY = position.y;

        float requestedXMove(0.f), requestedYMove(0.f);

        //Watch received events to see the requested moves
        bool wantsToJump(platformer.wantsToJump);
        bool wantsToGoLeft(platformer.wantsToGoLeft);
        bool wantsToGoRight(platformer.wantsToGoRight);

        std::set<entityx::Entity> entitiesHit; //< Store the entities hit by the platformer object

        collision::Polygon polygon = hitbox.getHitbox();
        sf::Transform polygonTransform = position.getPositionTransform();

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
        polygonTransform.translate(requestedXMove, 0.f);

        //Get all potential obstacle
        std::vector<entityx::Entity> potentialObstacles = GetPotentialObstacles(es, m_quadtreesGrid, entity, std::max(platformer.currentSpeed * dt, platformer.maxFallingSpeed * dt));

        //Update position according to the floor movements
        std::vector<entityx::Entity> overlappingJumpthrus = GetCollidingObstacles(polygon, polygonTransform, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Jumpthru);
        std::vector<entityx::Entity>::iterator ground = std::find(overlappingJumpthrus.begin(), overlappingJumpthrus.end(), platformer.groundEntity); //Test the current ground (even if it's a jumpthru)
        if(ground != overlappingJumpthrus.end())
            overlappingJumpthrus.erase(ground);

        float requestedXFloorMove(0.f), requestedYFloorMove(0.f);
        if(IsOnFloor(polygon, polygonTransform, potentialObstacles, overlappingJumpthrus) &&
            platformer.groundEntity &&
            platformer.groundEntity == GetFloor(polygon, polygonTransform, potentialObstacles, overlappingJumpthrus))
        {
            e::ComponentHandle<c::PositionComponent> floorCPos = platformer.groundEntity.component<c::PositionComponent>();

            requestedXMove += floorCPos->x - platformer.oldFloorPosX;
            requestedYMove += floorCPos->y - platformer.oldFloorPosY;
            requestedXFloorMove = floorCPos->x - platformer.oldFloorPosX;
            requestedYFloorMove = floorCPos->y - platformer.oldFloorPosY;
        }

        //Move the player's hitbox if the floor has moved
        polygonTransform.translate(requestedXFloorMove, requestedYFloorMove);

        //Store the entities collided by the platformer entity
        std::vector<entityx::Entity> collidedPlatforms = GetCollidingObstacles(polygon, polygonTransform, potentialObstacles, NO_EXCEPTIONS);
        for(entityx::Entity& collidedPlatform : collidedPlatforms)
        {
            entitiesHit.insert(collidedPlatform);
        }
        //////////////////////////////////////////////////////

        //Detect collision on the X-axis
        bool reqXPositive = requestedXMove > 0.f;
        while(IsCollidingObstacle(polygon, polygonTransform, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform))
        {
            //Try to move the object on Y-axis to support slopes
            polygonTransform.translate(0.f, -1-ceil(abs(requestedXMove)));
            if(!IsCollidingObstacle(polygon, polygonTransform, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform))
            {
                requestedYMove += -1-ceil(abs(requestedXMove));

                //Drop the object onto the obstacle
                while(!IsCollidingObstacle(polygon, polygonTransform, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform))
                {
                    requestedYMove++;
                    polygonTransform.translate(0.f, 1.f);
                }
                requestedYMove--;
                polygonTransform.translate(0.f, -1.f);

                break;
            }

            polygonTransform = position.getPositionTransform();
            polygonTransform.translate(0.f, requestedYMove);

            //Push the object back on X-axis
            requestedXMove += (reqXPositive ? -1 : 1);
            if(reqXPositive && requestedXMove < 0.f)
                requestedXMove = 0.f;
            else if(!reqXPositive && requestedXMove > 0.f)
                requestedXMove = 0.f;

            polygonTransform.translate(requestedXMove, 0.f);
        }

        overlappingJumpthrus = GetCollidingObstacles(polygon, polygonTransform, potentialObstacles, NO_EXCEPTIONS, c::PlatformComponent::Jumpthru); //Test the current ground (even if it's a jumpthru)
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
        if(wantsToJump && ((IsOnFloor(polygon, polygonTransform, potentialObstacles) && platformer.jumpingSpeed <= 0.f) || platformer.canJumpAgain))
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
        polygonTransform.translate(0.f, requestedFall);

        //Store the entities collided by the platformer entity
        std::vector<entityx::Entity> collidedPlatforms2 = GetCollidingObstacles(polygon, polygonTransform, potentialObstacles, NO_EXCEPTIONS);
        for(entityx::Entity& collidedPlatform : collidedPlatforms2)
        {
            entitiesHit.insert(collidedPlatform);
        }
        //////////////////////////////////////////////////////

        //Delete Jumpthru which are under or overlapping the player
        std::vector<entityx::Entity> allCollidingObstacles = GetCollidingObstacles(polygon, polygonTransform, potentialObstacles, overlappingJumpthrus);

        while((IsCollidingObstacle(polygon, polygonTransform, allCollidingObstacles, NO_EXCEPTIONS) && requestedYMove >= requestedYFloorMove) ||
              (IsCollidingObstacle(polygon, polygonTransform, allCollidingObstacles, NO_EXCEPTIONS, c::PlatformComponent::Platform) && requestedYMove < requestedYFloorMove))
        {
            platformer.fallingSpeed = 0.f;
            platformer.jumpingSpeed = 0.f;

            if(reqYPositive)
            {
                requestedFall--;
                requestedYMove--;
                polygonTransform.translate(0.f, -1.f);
            }
            else
            {
                requestedFall++;
                requestedYMove++;
                polygonTransform.translate(0.f, +1.f);
            }

            //Ignore micro movement to avoid the "shaking" effect
            if(abs(requestedFall) < 1.f)
            {
                requestedYMove -= requestedFall;
                polygonTransform.translate(0.f, -requestedFall);
                requestedFall = 0.f;
            }
        }

        //Update current floor
        if(IsOnFloor(polygon, polygonTransform, potentialObstacles, overlappingJumpthrus))
        {
            platformer.groundEntity = GetFloor(polygon, polygonTransform, potentialObstacles, overlappingJumpthrus);

            e::ComponentHandle<c::PositionComponent> floorCPos = platformer.groundEntity.component<c::PositionComponent>();
            platformer.oldFloorPosX = floorCPos->x;
            platformer.oldFloorPosY = floorCPos->y;
        }
        else
        {
            platformer.groundEntity = entityx::Entity();
        }

        //Move the entity
        position.x += requestedXMove;
        position.y += requestedYMove;
        polygonTransform = position.getPositionTransform();

        //Call on_hit callback on all platform hit by the platformer
        for(entityx::Entity collidedPlatform: entitiesHit)
        {
            if(collidedPlatform.component<c::PlatformComponent>()->onHitFunc.valid())
            {
                collidedPlatform.component<c::PlatformComponent>()->onHitFunc.call(lua::EntityHandle(collidedPlatform), lua::EntityHandle(entity));
            }
        }

        //Call the movement callbacks
        if(IsOnFloor(polygon, polygonTransform, potentialObstacles, overlappingJumpthrus))
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
