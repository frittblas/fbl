#include <typeinfo>

#include "ecs.h"

/*

    Ecs.cpp

    Ecs for SpaceWip2

    2021

*/

namespace Ecs {


    // Scene-class implementation

    Scene::Scene() {

        // do something here maybe

    }

    int Scene::createEntity(int x, int y)
    {
        int id = numEntities++; // total number of entities

        // all entities must have a position
        //component.position[id].entityId = id;
        //component.position[id].x = x;
        //component.position[id].y = y;

        Position tmpPos;

        tmpPos.entityId = id;
        tmpPos.x = x;
        tmpPos.y = y;

        auto tmpPair = std::pair<int32_t, Position>{id, tmpPos};

        component.position.insert(tmpPair);

        //std::cout << component.position[id].x <<std::endl;
        
        return id;

    }

    void Scene::removeEntity(uint32_t entityId) {

        // remove all components that refer to the entityId
        component.position.erase(entityId);
        component.health.erase(entityId);

    }

    template <typename T>
    void Scene::addComponent(uint32_t entityId) {

        if(typeid(T) == typeid(Position)) component.position[entityId].entityId = entityId;
        else if(typeid(T) == typeid(Health)) component.health[entityId].entityId = entityId;

        //std::unordered_map<uint32_t, T> tempMap;

        //component_.mapList.push_back(tempMap);
     
    }

    void Scene::removeComponent(ComponentType type, uint32_t entityId) {

        switch(type) {

            case posType :
                component.position.erase(entityId);
            break;
            case transType :
            break;
            case healthType :
                component.health.erase(entityId);
            break;

        }


    }

    bool Scene::hasComponent(ComponentType type, uint32_t entityId) {

        // find the fastest way to check if the entity has the component, try std::bitset
        // 

        return true;
    }

    void Scene::updatePosition(Position *p)
    {
        if(p->x > 0)
        { 

            Position* newP = &component.position[p->entityId];
            newP->x++;

        }
    }

    void Scene::updateHealth(Health *h)
    {

        h->curHealth++;

        if(h->curHealth <= 0)
        {
            std::cout<<"Entity is dead!"<<std::endl;
            removeEntity(h->entityId);
        }
    }

 

}

int main() {

    std::cout<<"Testing fblecs.."<<std::endl;

    Ecs::Scene scene;

    uint32_t ship1 = scene.createEntity(10, 10);
    //int ship2 = scene1.create_entity();


    scene.addComponent<Health>(ship1);

    //Position *shipPos = &scene.component.position[ship1];

    int counter = 0;

    while(counter < 20)
    {
        // update all systems

        //std::unordered_map<uint32_t, Ecs::Position>:: iterator itr;
    
        for (auto itr = scene.component.position.begin(); itr != scene.component.position.end(); itr++) {

            scene.updatePosition(&scene.component.position[itr->first]);

        }

        for (auto itr = scene.component.health.begin(); itr != scene.component.health.end(); itr++) {

            scene.updateHealth(&scene.component.health[itr->first]);

        }

        std::cout<<"ship1.x = "<< scene.component.position[ship1].x<<std::endl;

        counter++;

    }

    return 0;

}
