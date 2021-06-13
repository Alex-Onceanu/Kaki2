#pragma once


#include "WorldEntity.h"
#include "Texture.h"

#include <vector>
#include <memory>

using namespace std;

class MC :
    public WorldEntity
{
public:
    MC(TextureManager* tm_);
    ~MC();

    void ProcessCollision();

    void ProcessInput() override;
    void Update() override;
    void Draw() override;

private:
    void LoadAllImages();
    void UpdateRectCoordinates();

protected:
    enum animationsIndex
    {
        INDEX_UP, INDEX_DOWN, INDEX_RIGHT, INDEX_LEFT
    };
    
    unique_ptr<vector<vector<shared_ptr<Texture>>>> allAnimations;
    unique_ptr<vector<shared_ptr<Texture>>> currentAnimation;

    int frameCount;
    int animCount;
};

