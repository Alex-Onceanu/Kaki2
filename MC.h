#pragma once


#include "WorldEntity.h"
#include "Texture.h"
#include "Event.h"

#include <vector>
#include <memory>
#include <unordered_map>


using namespace std;

class MC :
    public WorldEntity,
    public Listener
{
public:
    MC(TextureManager* tm_);
    ~MC();

    void ProcessCollision();

    void ProcessInput() override;
    void Update() override;
    void Draw() override;

    virtual bool CanProcess(Event* e) override;
    virtual void OnEvent(Event* e) override;

    

private:
    void LoadAllImages();
    void UpdateRectCoordinates();
    void InitEventToFunction();
    
    void MoveUp();
    void MoveDown();
    void MoveRight();
    void MoveLeft();

    void StopUp();
    void StopDown();
    void StopRight();
    void StopLeft();


protected:
    enum animationsIndex
    {
        INDEX_UP, INDEX_DOWN, INDEX_LEFT
    };

    unique_ptr<unordered_map<EventEnum, void (MC::*)()>> eventToFunction;

    unique_ptr<vector<vector<shared_ptr<Texture>>>> allAnimations;
    unique_ptr<vector<shared_ptr<Texture>>> currentAnimation;

    int frameCount;
    int animCount;

    const int SPEED_REF = 5;
    Position speed;
};

