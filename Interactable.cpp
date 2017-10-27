//Interactable.cpp
#include "Interactable.h"
#include <AtTools/AtTools.h>

namespace AtPhys {
    float Interactable::CollisionHeight() {
        if (CollisionSize.Y()) return CollisionSize.Y(); else return Size.Y();
    }

    float Interactable::CollisionWidth() {
        if (CollisionSize.X()) return CollisionSize.X(); else return Size.X();
    }

    void Interactable::DropAt(float X, float Y) {
        Position = Vector2(X, Y);
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
            CollisionSize = Size;
        }

        if (CollisionMap != "") {
            int Tiles = AtTools::Strings::FindTotalOf(';', CollisionMap);

            for (int i=0; i<Tiles; i++) {
                int Tile = AtTools::Strings::StringTo<int>(CollisionMap, ';', i);
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
        } else return Size.Y();
    }

    float Interactable::InputHeight() {
        if (InputSize.Y()) return InputSize.Y(); else return Size.Y();
    }

    float Interactable::InputWidth() {
        if (InputSize.X()) return InputSize.X(); else return Size.X();
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
        Shape = Shapes::Rectangle;
        Interactive = true;
        Collision = Movable = Resizable = false;
    }

    void Interactable::MoveBy(float X, float Y) {
        if (TargetPosition == Vector2(0, 0)) TargetPosition = Position;
        TargetPosition += Vector2(X, Y);
    }

    void Interactable::MoveTo(float X, float Y) {
        TargetPosition = Vector2(X, Y);
    }

    void Interactable::NudgeBy(float X, float Y) {
        Position += Vector2(X, Y);
    }

    float Interactable::Radius() {
        return (Size.X()+Size.Y())/2.f;
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
        } else return Size.X();
    }

    float Interactable::X(int State) {
        if (State == 2 && TargetPosition.X() != 0) {
            return TargetPosition.X();
        } else return Position.X();
    }

    float Interactable::Y(int State) {
        if (State == 2 && TargetPosition.Y() != 0) {
            return TargetPosition.Y();
        } else return Position.Y();
    }

    Interactable::~Interactable() {
        Sector = 0.f;
        Shape = 0;
        Collision = Interactive = Movable = Resizable = false;
    }
}
