#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

/*

    Ecs.h

    header, Ecs for SpaceWip2

    2021

*/

// list of components, please add more

struct Position
{
    uint32_t entityId;
    int x, y;
    //Vector3 pos;
};

struct Transform
{
    uint32_t entityId;
    //Vector3 pos, rot, scale;
};

struct Health
{
    uint32_t entityId;
    int curHealth;
    int maxHealth;
};


namespace Ecs {

    enum ComponentType{posType, transType, healthType};

    struct ComponentList_
    {
        std::vector<std::unordered_map<uint32_t, Position>> mapList;
    };

    struct ComponentList
    {
        std::unordered_map<uint32_t, Position> position;
        std::unordered_map<uint32_t, Health> health;
    };

    // Scene-class declaration

    class Scene {

        private:
            uint32_t numEntities = 0;

        public:
            ComponentList component;
            ComponentList_ component_;

            Scene();
            int  createEntity(int x, int y);
            void removeEntity(uint32_t entityId);

            template <typename T>
            void addComponent(uint32_t entityId);
            void removeComponent(ComponentType type, uint32_t entityId);
            bool hasComponent(ComponentType type, uint32_t entityId);

            void updatePosition(Position *p);
            void updateHealth(Health *h);

    };


}
