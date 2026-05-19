#pragma once
#include "raylib.h"
class GameObject{
public:
        Vector2 position;
        Color color;
	float rotation;
	GameObject() : position({0,0}),  color(WHITE), rotation(0){}
        virtual void Update()=0;
        virtual void Draw()=0;
        virtual ~GameObject(){};
};
