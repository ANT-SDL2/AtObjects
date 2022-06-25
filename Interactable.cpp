//Interactable.cpp
#include "Interactable.h"
#include <sdlUtility/Strings.h>

using namespace sdlUtility;

namespace sdlObjects {
    void Interactable::AcquireSize(float Width, float Height) {
        if (Width < 0) Width = AcquiredSize.X();
        if (Height < 0) Height = AcquiredSize.Y();
        AcquiredSize = Vector2(Width, Height);
    }

    float Interactable::CollisionHeight() {
        if (CollisionSize.Y()) return CollisionSize.Y(); else return AcquiredSize.Y();
    }

    float Interactable::CollisionWidth() {
        if (CollisionSize.X()) return CollisionSize.X(); else return AcquiredSize.X();
    }

    void Interactable::DropAt(float X, float Y) {
        AcquiredPosition = Vector2(X, Y);
    }

    float Interactable::dHeight() {
        return Height(2)-Height(1);
    }

    float Interactable::dWidth() {
        return Width(2)-Width(1);
    }

    float Interactable::dX() {
        return X(2)-X(1);
    }

    float Interactable::dY() {
        return Y(2)-Y(1);
    }

    void Interactable::EnableCollision(bool State, std::string CollisionMap) {
        Collision = State;

        if (!CollisionSize.X() && !CollisionSize.Y()) {
            CollisionSize = AcquiredSize;
        }

        if (CollisionMap != "") {
            int Tiles = Strings::FindTotalOf(';', CollisionMap);

            for (int i=0; i<Tiles; i++) {
                int Tile = Strings::StringTo<int>(CollisionMap, ';', i);
                this->CollisionMap.push_back(Tile);
            }
        }
    }

    void Interactable::EnableInteraction(bool State) {
        Interactive = State;
    }

    void Interactable::EnableMoving(bool State) {
        Movable = State;
    }

    void Interactable::EnableResizing(bool State) {
        Resizable = State;
    }

    float Interactable::Height(int State) {
        if (State == 2 && TargetSize.Y() != 0) {
            return TargetSize.Y();
        } else if (State == 3) {
            return Size.Y();
        } else {
            if (Size.Y() < 0) return AcquiredSize.Y(); else return Size.Y();
        }
    }

    float Interactable::InputHeight() {
        if (InputSize.Y()) return InputSize.Y(); else return AcquiredSize.Y();
    }

    float Interactable::InputWidth() {
        if (InputSize.X()) return InputSize.X(); else return AcquiredSize.X();
    }

    bool Interactable::IsCollidable(int Tile) {
        if (Tile > 0) return CollisionMap[Tile-1]; else return Collision;
    }

    bool Interactable::IsDriven() {
        return (TargetPosition.X() || TargetPosition.Y());
    }

    bool Interactable::IsInteractive() {
        return Interactive;
    }

    bool Interactable::IsMovable() {
        return Movable;
    }

    bool Interactable::IsResizable() {
        return Resizable;
    }

    Interactable::Interactable() {
        Sector = 0.f;
        Position = Vector2(-1, -1);
        Size = Vector2(-1, -1);
        Shape = Shapes::Rectangle;
        Interactive = true;
        Collision = Movable = Resizable = false;
    }

    void Interactable::MoveBy(float X, float Y) {
        if (TargetPosition == Vector2(0, 0)) TargetPosition = AcquiredPosition;
        TargetPosition += Vector2(X, Y);
    }

    void Interactable::MoveTo(float X, float Y) {
        TargetPosition = Vector2(X, Y);
    }

    void Interactable::NudgeBy(float X, float Y) {
        AcquiredPosition += Vector2(X, Y);
    }

    float Interactable::Radius() {
        return (AcquiredSize.X()+AcquiredSize.Y())/2.f;
    }

    void Interactable::Relax(int Axis) {
        if (Axis == Axis::X) {
            TargetPosition = Vector2(0, TargetPosition.Y());
        } else if (Axis == Axis::Y) {
            TargetPosition = Vector2(TargetPosition.X(), 0);
        } else if (Axis == Axis::XY) {
            TargetPosition = Vector2(0, 0);
        }
    }

    void Interactable::RelaxSize() {
        TargetSize = Vector2(0, 0);
    }

    void Interactable::ResizeCollisionTo(float Width, float Height) {
        CollisionSize = Vector2(Width, Height);
    }

    void Interactable::ResizeInputTo(float Width, float Height) {
        InputSize = Vector2(Width, Height);
    }

    void Interactable::ResizeTo(float Width, float Height) {
        if (Width < 0) Width = Size.X();
        if (Height < 0) Height = Size.Y();
        Size = Vector2(Width, Height);
    }

    float Interactable::Section() {
        return Sector;
    }

    void Interactable::Section(float Sector) {
        this->Sector = Sector;
    }

    void Interactable::StretchBy(float Width, float Height) {
        TargetSize += Vector2(Width, Height);
    }

    void Interactable::StretchTo(float Width, float Height) {
        if (Width < 0) Width = TargetSize.X();
        if (Height < 0) Height = TargetSize.Y();
        TargetSize = Vector2(Width, Height);
    }

    float Interactable::Width(int State) {
        if (State == 2 && TargetSize.X() != 0) {
            return TargetSize.X();
        } else if (State == 3) {
            return Size.X();
        } else {
            if (Size.X() < 0) return AcquiredSize.X(); else return Size.X();
        }
    }

    float Interactable::X(int State) {
        if (State == 2 && TargetPosition.X() != 0) {
            return TargetPosition.X();
        } else return AcquiredPosition.X();
    }

    float Interactable::Y(int State) {
        if (State == 2 && TargetPosition.Y() != 0) {
            return TargetPosition.Y();
        } else return AcquiredPosition.Y();
    }

    Interactable::~Interactable() {
        Sector = 0.f;
        Shape = 0;
        Collision = Interactive = Movable = Resizable = false;
    }
}
