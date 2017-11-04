//Object.h
#ifndef NEOOBJECTS_OBJECT_H
#define NEOOBJECTS_OBJECT_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Interactable.h"
#include "Renderable.h"
#include <AtUtility/Vector2.h>

using namespace AtUtility;

namespace Reference {
    static const int Origin             = 0;
    static const int Parent             = 1;
    static const int Local              = 2;
    static const int Collision          = 3;
    static const int ParentCollision    = 4;
}

namespace Position {
    static const float None               = 0.f;
    static const float Center             = 0.5f;
    static const float Edge               = 1.0f;
}

namespace AtObjects {
    namespace Events {
        static const int Idle               = 0;
        static const int MouseIn            = 1;
        static const int MouseOut           = 2;
        static const int LeftButtonDown     = 3;
        static const int RightButtonDown    = 4;
        static const int LeftButtonUp       = 5;
        static const int RightButtonUp      = 6;
        static const int Resize             = 7;
        static const int Drag               = 8;
        static const int Show               = 9;
        static const int Hide               = 10;
        static const int Load               = 11;
        static const int Move               = 12;
        static const int Stop               = 13;
        static const int ValueChange        = 14;
        static const int Return             = 15;
        static const int AnimationChange    = 16;
        static const int TextureChange      = 17;
    }

    class Object {
        //Members
        protected:
            bool AllowOtherInput, Hovered, Scaled, Shown, State, Sticky;
            float Direction, MaxVelocity;
            int BlockX, BlockY, Clicked, TimeOfFlight;
            int Resistance;
            Interactable Interactable;
            Renderable Renderable;
            std::string Index, Name, Value;
            std::vector< std::vector<float> > CollisionQueue;
            std::vector<int> EventQueue;
            Object *BaseParent;
            Vector2 Anchor, AutoSize, ChildrenOffset, ClipPosition, ClipSize, CorrectionLimits, DragReference, DragRequest, ResizeReference, ResizeRequest, Scale;
            Vector2 Acceleration, ProjectileVelocity, ResizeRate, Velocity;
        //Access Methods
        public:
            Object();
            virtual ~Object();
            bool AllowsAutoSize(int Axis = Axis::XY);
            bool AllowsOtherInput();
            virtual float ChildrenOffsetX();
            virtual float ChildrenOffsetY();
            float CollisionHeight();
            float CollisionWidth();
            float ClipHeight();
            float ClipWidth();
            float ClipX();
            float ClipY();
            float DistanceFrom(Object *Object);
            float GetAnchor(int Axis);
            float GetDirection();
            std::string GetID();
            std::string GetIndex();
            class Interactable *GetInteractable();
            AtObjects::Renderable *GetRenderable();
            std::string GetName();
            std::string GetValue();
            float Height(int State = 1);
            bool IsActive();
            int IsBlocked(int Axis);
            bool IsClicked(int Button);
            bool IsClipped();
            bool IsHovered();
            bool IsMoving(int Axis = Axis::XY, bool State = false);
            bool IsShown(bool State = false);
            float VelocityLimit(bool State = false);
            float Width(int State = 1);
            float X(int Reference = Reference::Origin, float Position = 0.f, int State = 1);
            float X(float Interpolation, int Reference = Reference::Origin, float Position = 0.f);
            float Y(int Reference = Reference::Origin, float Position = 0.f, int State = 1);
            float Y(float Interpolation, int Reference = Reference::Origin, float Position = 0.f);
        protected:
            int AnchorX(int State = 1);
            int AnchorY(int State = 1);
            float ClosestCollisionX();
            float ClosestCollisionY();
            virtual float InheritedHeight(int State = 1);
            virtual float InheritedWidth(int State = 1);
            float InterpolateX(float Interpolation);
            float InterpolateY(float Interpolation);
            float ParentX(int State = 1);
            float ParentY(int State = 1);
            float VelocityX(bool State = false);
            float VelocityY(bool State = false);
        //Process Methods
        public:
            void AccelerateTowards(float Direction, float Acceleration);
            void AnchorAt(float HorizontalAnchor, float VerticalAnchor);
            void Block(int Axis);
            bool DetectCollisionWith(Object *Object, int &Axis, float &AdjustX, float &AdjustY, float Interpolation);
            void DropAt(float X, float Y);
            void Enable(bool State);
            void EnableAutoSize(bool State);
            void EnableOtherInput(bool State);
            void EnableScaling(bool State);
            int EventHandler();
            void Hide();
            int Input(SDL_Event &Event, bool Debug = false);
            void LimitVelocity(float MaxVelocity);
            void LookAt(Object *Object);
            void LookAt(float Direction);
            void MoveBy(float X, float Y);
            void MoveTo(float X, float Y);
            void MoveToLocal(float X, float Y);
            void Offset(int Axis, float Position);
            virtual void OffsetChildren(int Axis, float Position);
            void Process();
            void ProcessCollisions();
            void PushCollision(int Axis, float AdjustX, float AdjustY);
            void PushEvent(int Event, bool Force = false);
            void RenderShape(float Interpolation, int DebugState);
            void ResistDrag(int Resistance);
            void ResetInput();
            void Resize(int Axis, float Size);
            void ResizeTo(float Width, float Height);
            void ResizeBy(float Width, float Height);
            void RotateBy(float Angle);
            void ScaleTo(float ScaleX, float ScaleY);
            void SetCorrectionLimits(float AdjustX, float AdjustY);
            void SetIndex(std::string Index);
            void SetName(std::string Name);
            void SetValue(std::string Value);
            void ShootTowards(float Direction, float Velocity);
            void Show();
            void StopMovement(int Axis = Axis::XY);
            void Toggle();
            void Who();
        protected:
            virtual void OnAnimationChange(AtObjects::Renderable *Renderable);
            virtual int OnInput(SDL_Event &Event);
            virtual void OnCollision();
            virtual void OnDrag();
            virtual void OnHide();
            virtual void OnLeftButtonDown();
            virtual void OnLeftButtonUp();
            virtual void OnLoad();
            virtual void OnMouseIn();
            virtual void OnMouseOut();
            virtual void OnMove();
            virtual void OnProcess();
            virtual void OnResize();
            virtual void OnRightButtonDown();
            virtual void OnRightButtonUp();
            virtual void OnShow();
            virtual void OnStop();
            virtual void OnTextureChange();
            virtual void OnValueChange();
            void ProcessVelocity();
            void Update();
    };
}

#endif
