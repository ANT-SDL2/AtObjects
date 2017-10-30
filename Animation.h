//Animation.h
#ifndef NEOOBJECTS_ANIMATION_H
#define NEOOBJECTS_ANIMATION_H

#include "Texture.h"
#include <vector>
namespace AtObjects {
    class Animation {
        //Members
        public:
            Animation *Next, *Previous;
        protected:
            float Peak;
            int Duration, Modifier, State;
            std::string Animationset;
            std::vector<float> Weights;
            std::vector<int> Tiles;
        //Access Methods
        public:
            Animation();
            ~Animation();
            std::string GetAnimationset();
            int GetDuration();
            std::string GetID();
            int GetModifier();
            float GetPeak();
            int GetState();
            int GetTile(unsigned int Index);
            int GetTiles();
            float GetWeight(unsigned int Index);
        //Process Methods
        public:
            void AddTile(int Tile, float Weight);
            void SetAnimationset(std::string Animationset);
            void SetDuration(int Duration);
            void SetModifier(int Modifier);
            void SetPeak(float Peak);
            void SetState(int State);
            void Who();
    };
}

#endif
