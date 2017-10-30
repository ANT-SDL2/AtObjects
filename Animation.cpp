//Animation.cpp
#include "Animation.h"
#include <iostream>
#include <sstream>
#include <math.h>

namespace AtObjects {
    void Animation::AddTile(int Tile, float Weight) {
        Tiles.push_back(Tile);
        Weights.push_back(Weight);
    }

    Animation::Animation() {
        Next = Previous = NULL;
        Peak = 0.0f;
        Duration = Modifier = State = 0;
    }

    std::string Animation::GetAnimationset() {
        return Animationset;
    }

    int Animation::GetDuration() {
        return Duration;
    }

    std::string Animation::GetID() {
        std::ostringstream Stream;
        Stream << Animationset << ":" << State;
        return Stream.str();
    }

    int Animation::GetModifier() {
        return Modifier;
    }

    float Animation::GetPeak() {
        return Peak;
    }

    int Animation::GetState() {
        return State;
    }

    int Animation::GetTile(unsigned int Index) {
        if (Index < Tiles.size()) return Tiles[Index]; else return 0;
    }

    int Animation::GetTiles() {
        return Tiles.size();
    }

    float Animation::GetWeight(unsigned int Index) {
        if (Index < Weights.size()) return Weights[Index]; else return 0.f;
    }

    void Animation::SetAnimationset(std::string Animationset) {
        this->Animationset = Animationset;
    }

    void Animation::SetDuration(int Duration) {
        this->Duration = Duration;
    }

    void Animation::SetModifier(int Modifier) {
        this->Modifier = Modifier;
    }

    void Animation::SetPeak(float Peak) {
        this->Peak = Peak;
    }

    void Animation::SetState(int State) {
        this->State = State;
    }

    void Animation::Who() {
        std::cout << Animationset << ":"<< State << std::endl;
    }

    Animation::~Animation() {
        Next = Previous = NULL;
        Peak = 0.0f;
        Duration = Modifier = State = 0;
    }
}
