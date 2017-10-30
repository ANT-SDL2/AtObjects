//Object.cpp
#include "Object.h"
#include "Physics.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

namespace AtObjects {
    void Object::AccelerateTowards(float Direction, float Velocity) {
        Acceleration = Vector2(cosf(Direction)*Velocity, sinf(Direction)*Velocity);
        Interactable.Relax();
    }

    bool Object::AllowsAutoSize(int Axis) {
        if (Axis == Axis::X) {
            return AutoSize.X();
        } else if (Axis == Axis::Y) {
            return AutoSize.Y();
        } else return (AutoSize.X() || AutoSize.Y());
    }

    bool Object::AllowsOtherInput() {
        return AllowOtherInput;
    }

    void Object::AnchorAt(float AnchorX, float AnchorY) {
        Anchor = Vector2(AnchorX, AnchorY);
    }

    int Object::AnchorX(int State) {
        int AnchorX = 0;

        if (BaseParent) {
            if (Scaled) BaseParent->EnableScaling(true);

            float ParentWidth = BaseParent->Width(State), Width = this->Width(State);
            AnchorX = ParentWidth*Anchor.X()-Width/2.f;

            if (Anchor.X() != 0.5) {
                if (AnchorX < 0) AnchorX = 0; else if (Anchor.X() != 0 && AnchorX+Width > ParentWidth) AnchorX = ParentWidth-Width;
            }

            if (Anchor.X() != 0.f) AnchorX -= BaseParent->ChildrenOffsetX();

            if (Scaled) BaseParent->EnableScaling(false);
        }

        return AnchorX;
    }

    int Object::AnchorY(int State) {
        int AnchorY = 0;

        if (BaseParent) {
            if (Scaled) BaseParent->EnableScaling(true);

            float ParentHeight = BaseParent->Height(State), Height = this->Height(State);
            AnchorY = ParentHeight*Anchor.Y()-Height/2.f;;

            if (Anchor.Y() != 0.5) {
                if (AnchorY < 0) AnchorY = 0; else if (Anchor.Y() && AnchorY+Height > ParentHeight) AnchorY = ParentHeight-Height;
            }

            if (Anchor.Y() != 0.f) AnchorY -= BaseParent->ChildrenOffsetY();

            if (Scaled) BaseParent->EnableScaling(false);
        }

        return AnchorY;
    }

    void Object::Block(int Axis) {
        if (abs(Axis) == Axis::X) {
            if (Axis > 0) BlockX = 1; else if (Axis < 0) BlockX = -1;
        } else if (abs(Axis) == Axis::Y) {
            if (Axis > 0) BlockY = 1; else if (Axis < 0) BlockY = -1;
        }
    }

    float Object::ChildrenOffsetX() {
        float Scale = Scaled?this->Scale.X():1;
        return Scale*ChildrenOffset.X();
    }

    float Object::ChildrenOffsetY() {
        float Scale = Scaled?this->Scale.Y():1;
        return Scale*ChildrenOffset.Y();
    }

    float Object::ClosestCollisionX() {
        float ClosestCollisionX = CorrectionLimits.X();

        if (ClosestCollisionX != 0.f) {
            if (Interactable.Shape == Shapes::Disk) {
                if (ClosestCollisionX < 0) ClosestCollisionX += CollisionWidth()/2.f; else if (ClosestCollisionX > 0) ClosestCollisionX -= CollisionWidth()/2.f;
            } else if (Interactable.Shape == Shapes::Rectangle) {
                ClosestCollisionX += CollisionWidth();
            }
        }

        return ClosestCollisionX;
    }

    float Object::ClosestCollisionY() {
        float ClosestCollisionY = CorrectionLimits.Y();

        if (ClosestCollisionY != 0.f) {
            if (Interactable.Shape == Shapes::Disk) {
                ClosestCollisionY += CollisionWidth()/2.f;
            } else if (Interactable.Shape == Shapes::Rectangle) {
                ClosestCollisionY += CollisionHeight();
            }
        }

        return fabs(ClosestCollisionY);
    }

    float Object::CollisionHeight() {
        float Scale = Scaled?this->Scale.Y():1;
        return Scale*(Interactable.CollisionHeight());
    }

    float Object::CollisionWidth() {
        float Scale = Scaled?this->Scale.X():1;
        return Scale*(Interactable.CollisionWidth());
    }

    float Object::ClipHeight() {
        return ClipSize.Y();
    }

    float Object::ClipWidth() {
        return ClipSize.X();
    }

    float Object::ClipX() {
        return ClipPosition.X();
    }

    float Object::ClipY() {
        return ClipPosition.Y();
    }

    float Object::DistanceFrom(Object *Object) {
        float Distance = 0;
        if (Object) {
            float CenterX = Object->X(Reference::Origin, Position::Center);
            float CenterY = Object->Y(Reference::Origin, Position::Center);

            Distance = sqrtf(powf(CenterX-X(Reference::Origin, Position::Center), 2)+powf(CenterY-Y(Reference::Origin, Position::Center), 2));
        }
        return Distance;
    }

    void Object::DropAt(float X, float Y) {
        Interactable.DropAt(X, Y);
        Interactable.Relax();
        //TargetInstance.SetPosition(X, Y);
        //ProcessVelocity();
    }

    void Object::Enable(bool State) {
        this->State = State;
    }

    void Object::EnableAutoSize(bool State) {
        AutoSize = State;
    }

    void Object::EnableOtherInput(bool State) {
        this->AllowOtherInput = State;
    }

    void Object::EnableScaling(bool State) {
        Scaled = State;
    }

    int Object::EventHandler() {
        int Event = Events::Idle;

        if (EventQueue.size() > 0) {
            Event = EventQueue[0];
            EventQueue.erase(EventQueue.begin());

            switch (Event) {
                case Events::Drag:
                    OnDrag();


                    if (DragRequest.X() || DragRequest.Y()) {
                        if (Resistance > 0) {
                            Scaled = true;
                            if ((fabs(DragRequest.X()-X()) > Resistance || fabs(DragRequest.Y()-Y()) > Resistance)) {
                                Scaled = false;
                                Resistance = -Resistance;
                                DropAt(DragRequest.X()/Scale.X()-ParentX()-AnchorX(), DragRequest.Y()/Scale.Y()-ParentY()-AnchorY());
                                EventQueue.push_back(Events::Move);
                                DragRequest = Vector2(0, 0);
                            }
                        } else {
                            MoveTo(DragRequest.X()/Scale.X(), DragRequest.Y()/Scale.Y());
                            DragRequest = Vector2(0, 0);
                        }
                    }
                    break;
                case Events::Resize:
                    OnResize();

                    if (ResizeRequest.X() || ResizeRequest.Y()) {
                        ResizeTo(ResizeRequest.X()/Scale.X(), ResizeRequest.Y()/Scale.Y());
                        ResizeRequest = Vector2(0, 0);
                    }
                    break;
                case Events::Hide:
                    OnHide();
                    break;
                case Events::LeftButtonDown:
                    OnLeftButtonDown();
                    break;
                case Events::LeftButtonUp:
                    OnLeftButtonUp();

                    DragReference = Vector2(0, 0);
                    ResizeReference = Vector2(0, 0);
                    if (Resistance < 0) Resistance = -Resistance;
                    DragRequest = Vector2(0, 0);

                    break;
                case Events::Load:
                    OnLoad();
                    break;
                case Events::MouseIn:
                    OnMouseIn();
                    break;
                case Events::MouseOut:
                    OnMouseOut();
                    break;
                case Events::Move:
                    OnMove();
                    break;
                case Events::RightButtonDown:
                    OnRightButtonDown();
                    break;
                case Events::RightButtonUp:
                    OnRightButtonUp();
                    break;
                case Events::Show:
                    OnShow();
                    break;
                case Events::Stop:
                    OnStop();
                    break;
                default:
                    break;
            }
        }

        return Event;
    }

    float Object::GetAnchor(int Axis) {
        if (Axis == Axis::X) return Anchor.X(); else return Anchor.Y();
    }

    float Object::GetDirection() {
        return Direction;
    }

    std::string Object::GetID() {
        return Index+"."+Name;
    }

    std::string Object::GetIndex() {
        return Index;
    }

    std::string Object::GetName() {
        return Name;
    }

    Interactable *Object::GetInteractable() {
        return &Interactable;
    }

    std::string Object::GetValue() {
        return Value;
    }

    float Object::Height(int State) {
        float Scale = Scaled?this->Scale.Y():1.f;
        float BaseHeight = Scale*Interactable.Height(State);

        float Height = BaseHeight;

        if (!BaseHeight) {
            Height = InheritedHeight(State);
        }

        //if (Name == "Testy") std::cout << Width() << " " << Height << " " << BaseHeight << " " << AnchorX() << std::endl;

        return Height;
    }

    void Object::Hide() {
        if (Shown) {
            Shown = false;
            ResetInput();
            EventQueue.push_back(Events::Hide);
        }
    }

    int Object::Input(SDL_Event &Event, bool Debug) {
        int Input = 0;

        if ((State && IsShown(true) && Interactable.IsInteractive()) || (Debug&&BaseParent&&BaseParent->IsShown(true))) {
            if (Event.type == SDL_MOUSEMOTION || Event.type == SDL_USEREVENT) {
                //Mouse in and out events
                if (SDL_ShowCursor(-1)) {
                    Scaled = true;

                    //Width/Height may be inherited and not defined in InputWidth
                    float InputWidth = Interactable.InputWidth()?Interactable.InputWidth()*Scale.X():Width();
                    float InputHeight = Interactable.InputHeight()?Interactable.InputHeight()*Scale.Y():Height();

                    bool HoverCheck = false;
                    int Shape = Interactable.Shape;
                    if (Shape == Shapes::Rectangle) {
                        float InputClipX = ClipPosition.X(), InputClipY = ClipPosition.Y();
                        float InputClipHeight = ClipSize.Y(), InputClipWidth = ClipSize.X();

                        if (InputClipWidth > 1) InputClipWidth = 1; else if (InputClipWidth < 0) InputClipWidth = 0;
                        if (InputClipHeight > 1) InputClipHeight = 1; else if (InputClipHeight < 0) InputClipHeight = 0;
                        if (InputClipX <= 0) InputClipX = 0; else if (InputClipX > 1) InputClipX = 1;
                        if (InputClipY <= 0) InputClipY = 0; else if (InputClipY > 1) InputClipY = 1;

                        float InputWidthClipped = InputClipWidth*(1-InputClipX)*InputWidth, InputHeightClipped = InputClipHeight*(1-InputClipY)*InputHeight;
                        HoverCheck = PointInRectangle(Event.motion.x, Event.motion.y, X(Reference::Origin, InputClipX), Y(Reference::Origin, InputClipY), InputWidthClipped, InputHeightClipped, Direction);
                        //if (Name == "MainScreenUI") std::cout << HoverCheck << " " << Event.motion.x << " " << X(Reference::Origin, InputClipX) << " " << Event.motion.y << " " << Y(Reference::Origin, InputClipY) << " " << InputWidthClipped << " " << InputHeightClipped << " " << ClipPosition.X() << " " << ClipPosition.Y() << std::endl;
                    } else if (Shape == Shapes::Disk) {
                        HoverCheck = PointInDisk(Event.motion.x, Event.motion.y, X(Reference::Origin, Position::Center), Y(Reference::Origin, Position::Center), InputWidth/2.f, InputHeight/2.f, Interactable.Section()*2.f*M_PI, Direction);
                    } else if (Shape == Shapes::Triangle) {
                        HoverCheck = PointInTriangle(Event.motion.x, Event.motion.y, X(), Y(), InputWidth, InputHeight, Direction);
                    }

                    if (HoverCheck) {
                        if (!Hovered) {
                            //if (!SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT) || (AllowOtherInput && Event.type == SDL_MOUSEMOTION)) {
                            if (true) {
                                EventQueue.push_back(Events::MouseIn);
                                Hovered = true;
                                Input = Events::MouseIn;
                            }
                        } else Input = -Events::MouseIn;
                    } else {
                        if (Hovered) {
                            if (!((Sticky||Interactable.IsMovable()) && IsClicked(SDL_BUTTON_LEFT))) {
                                EventQueue.push_back(Events::MouseOut);
                                Hovered = false;
                                if (IsClicked(SDL_BUTTON_LEFT) && !(Interactable.IsMovable())) Clicked = Events::Idle;
                                Input = Events::MouseOut;
                            } else {
                                Input = -Events::MouseIn;
                            }
                        }
                    }

                    //Drag and resize events
                    if (Event.type == SDL_MOUSEMOTION) {
                        if (IsClicked(SDL_BUTTON_LEFT)) {
                            if ((Interactable.IsResizable()) && PointInRectangle(DragReference.X(), DragReference.Y(), ResizeReference.X()-5, ResizeReference.Y()-5, 5, 5, Direction)) {
                                ResizeRequest = Vector2(Event.motion.x-X(), Event.motion.y-Y());
                                Input = Events::Resize;
                                if (EventQueue.back() != Events::Resize) EventQueue.push_back(Events::Resize);
                                //std::cout << Resize.X() << std::endl;
                            } else if (Interactable.IsMovable()) {
                                DragRequest = Vector2(Event.motion.x-DragReference.X(), Event.motion.y-DragReference.Y());
                                Input = Events::Drag;
                                if (EventQueue.back() != Events::Drag) EventQueue.push_back(Events::Drag);
                                //std::cout << Drag.X() << std::endl;
                            } else if (Interactable.IsMovable() || Sticky) DragReference = Vector2(Event.motion.x-X(), Event.motion.y-Y());
                        }
                    }

                    Scaled = false;
                }
            } else if (Event.type == SDL_MOUSEBUTTONDOWN) {
                Scaled = true;
                //Button down events
                if (Hovered && Event.button.button == SDL_BUTTON_LEFT) {
                    if (!IsClicked(SDL_BUTTON_LEFT)) {
                        if (IsClicked(SDL_BUTTON_RIGHT)) EventQueue.push_back(Events::RightButtonUp);
                        EventQueue.push_back(Events::LeftButtonDown);

                        Clicked = SDL_BUTTON_LEFT;
                        Input = Events::LeftButtonDown;

                        if (Interactable.IsMovable() || Sticky || Debug) DragReference = Vector2(Event.motion.x-X(), Event.motion.y-Y());
                        if (Interactable.IsMovable() || Sticky || Debug) ResizeReference = Vector2(Width(), Height());
                    } else Clicked = 0;
                } else if (Hovered && Event.button.button == SDL_BUTTON_RIGHT) {
                    if (!IsClicked(SDL_BUTTON_RIGHT)) {
                        if (IsClicked(SDL_BUTTON_LEFT)) EventQueue.push_back(Events::LeftButtonUp);
                        EventQueue.push_back(Events::RightButtonDown);

                        Clicked = SDL_BUTTON_RIGHT;
                        Input = Events::RightButtonDown;
                    } else Clicked = 0;
                }
                Scaled = false;
            } else if (Event.type == SDL_MOUSEBUTTONUP) {
                //Button up events
                if (Hovered) {
                    if (IsClicked(SDL_BUTTON_LEFT) && Event.button.button == SDL_BUTTON_LEFT) {
                        EventQueue.push_back(Events::LeftButtonUp);

                        Clicked = 0;
                        Input = Events::LeftButtonUp;
                    } else if (IsClicked(SDL_BUTTON_RIGHT) && Event.button.button == SDL_BUTTON_RIGHT) {
                        EventQueue.push_back(Events::RightButtonUp);
                        Clicked = 0;
                        Input = Events::RightButtonUp;
                    }
                } else Clicked = 0;
            }
        }

        if (!Input) Input = OnInput(Event);

        return Input;
    }
    float Object::InterpolateX(float Interpolation) {
        float Scale = Scaled?this->Scale.X():1.f;
        float SpeedX = VelocityX(true);

        //float LeftOverX = X(Reference::Origin, 0, 2)-X();
        //if (X(Reference::Origin, 0, 2) != 0) SpeedX = LeftOverX;

        float MaxVelocity = VelocityLimit();

        if (MaxVelocity && (SpeedX == Velocity.X())) {
            if (fabs(MaxVelocity) < fabs(SpeedX)) {
                if (SpeedX < 0) SpeedX = -MaxVelocity; else SpeedX = MaxVelocity;
            }
        }

        float InterpolateX = Scale*Interpolation*SpeedX;

        if (InterpolateX*BlockX > 0) InterpolateX = 0;

        if (Interactable.IsCollidable()) {
            float ClosestCollision = ClosestCollisionX();
            if (Velocity.X() && ClosestCollision && fabs(InterpolateX) > fabs(ClosestCollision)) {
                InterpolateX = fabs(ClosestCollision);
                if (SpeedX < 0.f) InterpolateX = -InterpolateX;
            }
        }

        //if (fabs(LeftOverX) < fabs(InterpolateX)) InterpolateX = LeftOverX;

        //if (Name == "Neotms") std::cout << InterpolateX << " " << BlockX << " " << CorrectionLimits.X() << std::endl;
        //if (Name == "Neotms" ) { std::cout << Name << " " << PreviousInstance.X() << " " << TargetInstance.X() << " " << InterpolateX << std::endl; } //else std::cout << "0" << std::endl; }
        //if (Name == "Camera01") { std::cout << InterpolateX/SpeedX << " " << " " << Velocity.X() << " " << BaseParent->GetVelocity(Axis::X) << std::endl; }
        //if (Name == "Basement" && InterpolateX) { std::cout << "Area " << SpeedX << " " << MaxVelocity << std::endl; }
        //if (Name == "Camera01" && InterpolateX) { std::cout << "Camera " << SpeedX << " " << MaxVelocity << std::endl; }

        //if (GetID() == "Debug.Dialog") std::cout << TargetInstance.X()-PreviousInstance.X() << " " << SpeedX << std::endl;

        return InterpolateX;
    }

    float Object::InterpolateY(float Interpolation) {
        float Scale = Scaled?this->Scale.Y():1.f;
        float SpeedY = VelocityY(true);

        float MaxVelocity = VelocityLimit();
        if (MaxVelocity && (SpeedY == Velocity.Y())) {
            if (fabs(MaxVelocity) < fabs(SpeedY)) { if (SpeedY < 0) SpeedY = -MaxVelocity; else SpeedY = MaxVelocity; }
        }

        float InterpolateY = Scale*Interpolation*SpeedY;

        if (InterpolateY*BlockY > 0) InterpolateY = 0;

        if (Interactable.IsCollidable()) {
            float ClosestCollision = ClosestCollisionY();
            if (Velocity.Y() && ClosestCollision && fabs(InterpolateY) > fabs(ClosestCollision)) {
                InterpolateY = fabs(ClosestCollision);
                if (SpeedY < 0.f) InterpolateY = -InterpolateY;
            }
        }
        //if (Name == "Camera01") { std::cout << InterpolateY/SpeedY << " " << " " << SpeedY << " " << MaxVelocity << " " << BaseParent->GetVelocity(Axis::Y) << std::endl; }
        //if (Name == "Camera01") { std::cout << SpeedY << " " << PreviousInstance.Y() << " " << TargetInstance.Y() << " " << Velocity.Y() << std::endl; }

        return InterpolateY;
    }

    float Object::InheritedHeight(int State) {
        return 0.f;
    }

    float Object::InheritedWidth(int State) {
        return 0.f;
    }

    bool Object::IsActive() {
        return State;
    }

    int Object::IsBlocked(int Axis) {
        int IsBlocked = 0;

        if (Axis == Axis::X) {
            IsBlocked = BlockX;
        } else if (Axis == Axis::Y) {
            IsBlocked = BlockY;
        } else if (Axis == Axis::XY) {
            IsBlocked = BlockX&&BlockY;
        }

        return IsBlocked;
    }

    bool Object::IsClicked(int Button) {
        if (Clicked == Button) return true; else return false;
    }

    bool Object::IsClipped() {
        return (ClipSize != Vector2(1.f, 1.f) || ClipPosition != Vector2(0.f, 0.f));
    }

    bool Object::IsHovered() {
        return Hovered;
    }

    bool Object::IsMoving(int Axis, bool State) {
        bool Moving = false;

        if (Axis == Axis::XY && (Velocity.X() || Velocity.Y())) {
            Moving = true;
        } else if (Axis == Axis::X && Velocity.X()) {
            Moving = true;
        } else if (Axis == Axis::Y && Velocity.Y()) {
            Moving = true;
        }

        return (Moving or (State && BaseParent && BaseParent->IsMoving(Axis, State) && Shown));
    }

    bool Object::IsShown(bool State) {
        if (BaseParent && State) return (Shown && BaseParent->IsShown(true)); else return Shown;
    }

    void Object::LimitVelocity(float MaxVelocity) {
        this->MaxVelocity = MaxVelocity;
    }

    void Object::LookAt(Object *Object) {
        if (Object) {
            int CenterX = Object->X(Reference::Origin, Position::Center);
            int CenterY = Object->Y(Reference::Origin, Position::Center);

            float Direction = atan2(CenterY-Y(Reference::Origin, Position::Center), CenterX-X(Reference::Origin, Position::Center))+M_PI/2.f;
            LookAt(Direction);
        }
    }

    void Object::LookAt(float Direction) {
        if (Direction > 2*M_PI) Direction -= 2*M_PI; else if (Direction < 0) Direction += 2*M_PI;
        this->Direction = Direction;
    }

    void Object::MoveBy(float X, float Y) {
        if (!Interactable.IsDriven()) EventQueue.push_back(Events::Move);
        if (fabs(X) < 0.01) X = 0.f;
        if (fabs(Y) < 0.01) Y = 0.f;
        Interactable.MoveBy(X, Y);
        if (Interactable.dX()*BlockX < -1) BlockX = 0;
        if (Interactable.dY()*BlockY < -1) BlockY = 0;
    }

    void Object::MoveTo(float X, float Y) {
        if (!Interactable.IsDriven()) EventQueue.push_back(Events::Move);
        Interactable.MoveTo(X-(ParentX()+AnchorX()), Y-(ParentY()+AnchorY()));
        if (Interactable.dX()*BlockX < -1) BlockX = 0;
        if (Interactable.dY()*BlockY < -1) BlockY = 0;
    }

    void Object::MoveToLocal(float X, float Y) {
        if (!Interactable.IsDriven()) EventQueue.push_back(Events::Move);
        Interactable.MoveTo(X, Y);
        if (Interactable.dX()*BlockX < -1) BlockX = 0;
        if (Interactable.dY()*BlockY < -1) BlockY = 0;
    }

    Object::Object() {
        AllowOtherInput = Hovered = false; Shown = true; Scaled = Sticky = false; State = true;
        Direction = 0; MaxVelocity = 0;
        ClipSize = Vector2(1.f, 1.f);
        BlockX = BlockY = Clicked = TimeOfFlight = 0;
        Resistance = 0;
        Scale = Vector2(1.f, 1.f);
        BaseParent = NULL;
    }

    void Object::OnCollision() {
    }

    void Object::OnDrag() {
    }

    void Object::Offset(int Axis, float Position) {
        if (Axis == Axis::X) {
            Interactable.DropAt(Position, Interactable.Y());
            Interactable.Relax(Axis::X);
        } else {
            Interactable.DropAt(Interactable.X(), Position);
            Interactable.Relax(Axis::Y);
        }
    }

    void Object::OffsetChildren(int Axis, float Position) {
        if (Axis == Axis::X) {
            ChildrenOffset = AtObjects::Vector2(Position, ChildrenOffset.Y());
        } else if (Axis == Axis::Y) {
            ChildrenOffset = AtObjects::Vector2(ChildrenOffset.X(), Position);
        }
    }

    void Object::OnHide() {
    }

    int Object::OnInput(SDL_Event &Event) {
        int Input = 0;
        return Input;
    }

    void Object::OnLeftButtonDown() {
    }

    void Object::OnLeftButtonUp() {
    }

    void Object::OnLoad() {
    }

    void Object::OnMouseIn() {
    }

    void Object::OnMouseOut() {
    }

    void Object::OnMove() {
    }

    void Object::OnProcess() {
    }

    void Object::OnResize() {
    }

    void Object::OnRightButtonDown() {
    }

    void Object::OnRightButtonUp() {
    }

    void Object::OnShow() {
    }

    void Object::OnStop() {
    }

    void Object::OnValueChange() {
    }

    float Object::ParentX(int State) {
        if (BaseParent) {
            if (Scaled) BaseParent->EnableScaling(true);
            float X = (BaseParent->X(Reference::Origin, Position::None, State)+BaseParent->ChildrenOffsetX());
            if (Scaled) BaseParent->EnableScaling(false);
            return X;
        } else return 0;
    }

    float Object::ParentY(int State) {
        if (BaseParent) {
            if (Scaled) BaseParent->EnableScaling(true);
            float Y = (BaseParent->Y(Reference::Origin, Position::None, State)+BaseParent->ChildrenOffsetY());
            if (Scaled) BaseParent->EnableScaling(false);
            return Y;
        } else return 0;
    }

    void Object::Process() {
        if (State) {
            Update();

            //if (IsMoving()) std::cout << Name << " " << Velocity.X() << " " << Velocity.Y() << std::endl;
            //if (Name == "Neotms") std::cout << PreviousInstance.Y() << " " << TargetInstance.Y() << std::endl;
        }

        OnProcess();

        CorrectionLimits = Vector2(0.f, 0.f);
    }

    void Object::ProcessCollisions() {
        bool BlockXFlag = false;
        bool BlockYFlag = false;

        float TotalAdjustX = 0, TotalAdjustY = 0;
        float AdjustX = 0, AdjustY = 0;

        int Axis = 0, Adjustment = 0;

        int Step = 0;
        unsigned int i = 0;

        //if (Name == "Neotms") std::cout << "PC: " << CollisionQueue.size() << " " << BlockX << std::endl;

        while (CollisionQueue.size() > 0) {
            //Get next Adjustment
            Axis = CollisionQueue[Adjustment][2];
            AdjustX = CollisionQueue[Adjustment][0];
            AdjustY = CollisionQueue[Adjustment][1];

            //if (Name == "Neotms") std::cout << "C: " <<  i+1 << " " << CollisionQueue[i][0] << " " << CollisionQueue[i][1] << " " << CollisionQueue[i][2] << std::endl;

            //Determine Adjustment (smallest first)
            if (abs(CollisionQueue[i][2]) == Axis::X) {
                BlockXFlag = true;
                if (fabs(CollisionQueue[i][0]) < fabs(AdjustX)) {
                    Adjustment = i;

                    AdjustX = CollisionQueue[i][0];
                    AdjustY = CollisionQueue[i][1];
                    Axis = CollisionQueue[i][2];
                }
            } else if (abs(CollisionQueue[i][2]) == Axis::Y) {
                BlockYFlag = true;
                if (fabs(CollisionQueue[i][1]) < fabs(AdjustY)) {
                    Adjustment = i;
                    AdjustX = CollisionQueue[i][0];
                    AdjustY = CollisionQueue[i][1];
                    Axis = CollisionQueue[i][2];
                }
            } else if (abs(CollisionQueue[i][2]) == Axis::XY) {
                BlockXFlag = true;
                BlockYFlag = true;

                Adjustment = i;
                AdjustX = CollisionQueue[i][0];
                AdjustY = CollisionQueue[i][1];
                Axis = CollisionQueue[i][2];
            }

            i++;

            if (i >= CollisionQueue.size()) {
                //Apply Adjustment
                if (TotalAdjustX*AdjustX > 0) AdjustX -= TotalAdjustX; else if (TotalAdjustX*AdjustX < 0 && Axis == Axis::X) {
                    Axis = Axis::XY;
                    if (AdjustY*Velocity.Y() < 0) AdjustY = -AdjustY;
                    //std::cout << "Conflict" << std::endl;
                } else {
                    /*
                    if (AdjustX*CorrectionLimits.X() > 0 && fabs(AdjustX)+GetCollisionWidth()/2.f > fabs(CorrectionLimits.X()) && fabs(CorrectionLimits.Y()) <= GetCollisionWidth()/2.f) {
                        std::cout << Step << " FY " << CorrectionLimits.Y() << std::endl;
                        Axis = Axis::Y;
                        if (AdjustY*Velocity.Y() < 0) AdjustY = -AdjustY;
                    }
                    */
                }

                if (TotalAdjustY*AdjustY > 0) AdjustY -= TotalAdjustY; else if (TotalAdjustY*AdjustY < 0 && Axis == Axis::Y) {
                    Axis = Axis::XY;
                    if (AdjustX*Velocity.X() < 0) AdjustX = -AdjustX;
                    //std::cout << "Conflict" << std::endl;
                } else {
                    /*
                    if (AdjustY*CorrectionLimits.Y() > 0 && fabs(AdjustY)+GetCollisionWidth()/2.f > fabs(CorrectionLimits.Y()) && fabs(CorrectionLimits.X()) <= GetCollisionWidth()/2.f) {
                        std::cout << Step << " FX " << CorrectionLimits.X() << std::endl;
                        Axis = Axis::X;
                        if (AdjustX*Velocity.X() < 0) AdjustX = -AdjustX;
                    }
                    */
                }

                if (Axis == Axis::X) {
                    if (AdjustX != 0) {
                        //std::cout << Step << " X " << AdjustX << " " << AdjustY << " " << CollisionQueue[Adjustment][2] << " " << CorrectionLimits.X() << std::endl;;
                        Interactable.NudgeBy(-AdjustX, 0);
                        TotalAdjustX += AdjustX;
                        if (AdjustX > 0) BlockX = 1; else if (AdjustX < 0) BlockX = -1;
                    }
                } else if (Axis == Axis::Y) {
                    if (AdjustY != 0) {
                        //std::cout << Step << " Y " << AdjustY << " " << AdjustX << " " << CollisionQueue[Adjustment][2] << " " << CorrectionLimits.Y() << std::endl;;
                        Interactable.NudgeBy(0, -AdjustY);
                        TotalAdjustY += AdjustY;
                        if (AdjustY > 0) BlockY = 1; else if (AdjustY < 0) BlockY = -1;
                    }
                } else if (abs(Axis) == Axis::XY) {
                    //std::cout << Step << " XY " << AdjustX << " " << AdjustY << " " << CollisionQueue[Adjustment][2] << " " << CorrectionLimits.X() << " " << CorrectionLimits.Y() << std::endl;;

                    Interactable.NudgeBy(-AdjustX, -AdjustY);

                    TotalAdjustX += AdjustX;
                    TotalAdjustY += AdjustY;

                    if (AdjustX > 0) BlockX = 1; else if (AdjustX < 0) BlockX = -1; else if (Axis == Axis::XY) BlockX = 1; else if (Axis == -Axis::XY) BlockX = -1;
                    if (AdjustY > 0) BlockY = 1; else if (AdjustY < 0) BlockY = -1; else if (Axis == Axis::XY) BlockY = 1; else if (Axis == -Axis::XY) BlockY = -1;
                }

                CollisionQueue.erase(CollisionQueue.begin()+Adjustment);

                Step++;
                Adjustment = 0;
                AdjustX = AdjustY = 0;
                i = 0;
            }
        }

        if (!BlockXFlag) BlockX = 0;
        if (!BlockYFlag) BlockY = 0;

        //if (Name == "Neotms") std::cout << CorrectionLimits.X() << " " << CorrectionLimits.Y() << std::endl;

        //if (Name == "Neotms") std::cout << "B: " << BlockX << " " << BlockY << " " << ClosestCollisionX() << std::endl << std::endl;

        if (Step) OnCollision();
    }

    void Object::ProcessVelocity() {
        float Vx = 0, Vy = 0, Angle = 0;

        if (Interactable.IsCollidable() && !DragReference) {
            float Vo = 0.f;
            float Acceleration = 0.f;

            //Guided motion
            if (Interactable.IsDriven()) {
                Vo = Velocity.Length();

                Vector2 TargetAcceleration(Interactable.dX(), Interactable.dY());
                Acceleration = TargetAcceleration.Length();
                Angle = TargetAcceleration.Angle();

                if (Acceleration < 1.f) { Interactable.Relax(); Vo = Acceleration = 0.f; }
            //Projectile and accelerated motion
            } else {
                Vo = !ProjectileVelocity?Velocity.Length():ProjectileVelocity.Length();
                float AccelerationFactor = this->Acceleration.Length();

                if (Vo || AccelerationFactor) {
                    if (!!ProjectileVelocity) {
                        Angle = ProjectileVelocity.Angle();
                        ProjectileVelocity = Vector2(0.f, 0.f);
                    } else Angle = !this->Acceleration?Velocity.Angle():this->Acceleration.Angle();

                    float EffectiveArea = fabs(sin(Angle-Direction))*CollisionWidth() + fabs(cos(Angle-Direction))*CollisionHeight();
                    float Drag = CalculateDrag(Vo, 5e-4f*EffectiveArea, EffectiveArea);

                    Acceleration = AccelerationFactor-Drag;

                    if (Acceleration < 0 && fabs(Acceleration) > Vo) Acceleration = -Vo;

                    //std::cout << "Free: " << Vo << " " << Acceleration << " " << Drag << std::endl;
                    //if (Mass && Vector2(Vx, Vy).Length()*powf(Vo, 2) < Drag/Mass) { Vx = 0; Vy = 0; }
                }

            }

            Vx = (Vo + Acceleration)*cos(Angle);
            Vy = (Vo + Acceleration)*sin(Angle);

            if (Interactable.IsDriven()) {
                if (Interactable.dX() && (fabs(Vx) > fabs(Interactable.dX()))) Vx = Interactable.dX();
                if (Interactable.dY() && (fabs(Vy) > fabs(Interactable.dY()))) Vy = Interactable.dY();
            }

            //if (Name == "Neotms") std::cout << Vo << " " << Acceleration << " " << Vx << std::endl;
            //if (Name == "Neotms") std::cout << BlockX << " " << Vx << " " << BlockY << " " << Vy << std::endl;
        } else {
            if (Interactable.IsDriven()) {
                Vx = Interactable.dX();
                Vy = Interactable.dY();
                Angle = Vector2(Vx, Vy).Angle();
            }
        }

        //Apply Blocks
        if (BlockX && BlockX*Vx >= 0) Vx = 0;
        if (BlockY && BlockY*Vy >= 0) Vy = 0;

        //Apply MaxVelocity
        if ((Vx || Vy) && MaxVelocity && !DragReference) {
            float CosA = cosf(Angle);
            float SinA = sinf(Angle);
            if (fabs(Vx) > fabs(MaxVelocity*CosA)) Vx = MaxVelocity*CosA;
            if (fabs(Vy) > fabs(MaxVelocity*SinA)) Vy = MaxVelocity*SinA;
        }

        if (fabs(Vy) < 0.01f) Vy = 0.f;
        if (fabs(Vx) < 0.01f) Vx = 0.f;

        if (TimeOfFlight > 0) {
            TimeOfFlight -= 1;
            if (TimeOfFlight == 0) {
                Vx = Vy = 0;
            }
        }

        Velocity = Vector2(Vx, Vy);

        //if (Name == "Neotms") std::cout << Vx << " " << Interactable.dX() << std::endl;
    }

    void Object::PushCollision(int Axis, float AdjustX, float AdjustY) {
        std::vector<float> Adjust;
        Adjust.push_back(AdjustX);
        Adjust.push_back(AdjustY);
        Adjust.push_back(Axis);

        CollisionQueue.push_back(Adjust);
        //std::cout << "Pushed Collision: " << Axis << " " << AdjustX << " " << AdjustY << std::endl;
    }

    void Object::PushEvent(int Event, bool Force) {
        if (Event == Events::MouseIn) {
            if (!Hovered) {
                EventQueue.push_back(Event);
                Hovered = true;
            }
        } else if (Event == Events::MouseOut) {
            if (Hovered || Force) { //TEMP unwanted
                EventQueue.push_back(Event);
                Hovered = false;
            }
        } else if (Event == Events::LeftButtonDown) {
            if (!Clicked) {
                EventQueue.push_back(Event);
                Clicked = SDL_BUTTON_LEFT;
            }
        } else if (Event == Events::RightButtonDown) {
            if (!Clicked || Force) {
                EventQueue.push_back(Event);
                Clicked = SDL_BUTTON_RIGHT;
            }
        } else if (Event == Events::LeftButtonUp || Event == Events::RightButtonUp) {
            if (Clicked || Force) {
                EventQueue.push_back(Event);
                Clicked = 0;
            }
        } else EventQueue.push_back(Event);
    }

    void Object::ResetInput() {
        if (Hovered) {
            Hovered = false;
            EventQueue.push_back(Events::MouseOut);
        }

        if (Clicked) {
            if (Clicked == SDL_BUTTON_LEFT) {
                EventQueue.push_back(Events::LeftButtonUp);
            } else if (Clicked == SDL_BUTTON_RIGHT) {
                EventQueue.push_back(Events::RightButtonUp);
            }
            Clicked = 0;
        }
    }

    void Object::ResistDrag(int Resistance) {
        this->Resistance = Resistance;
    }

    void Object::Resize(int Axis, float Size) {
        if (Axis == Axis::X) {
            Interactable.ResizeTo(Size, -1);
        } else {
            Interactable.ResizeTo(-1, Size);
        }
    }

    void Object::ResizeTo(float Width, float Height) {
        Interactable.ResizeTo(Width, Height);
    }

    void Object::ResizeBy(float Width, float Height) {
        Interactable.StretchBy(Width, Height);
    }

    void Object::RotateBy(float Angle) {
        Direction += Angle;
        if (Direction > 2.f*M_PI) Direction -= 2.f*M_PI; else if (Direction < 0) Direction += 2.f*M_PI;
    }

    void Object::ScaleTo(float ScaleX, float ScaleY) {
        Scale = Vector2(ScaleX, ScaleY);
    }

    void Object::SetCorrectionLimits(float AdjustX, float AdjustY) {
        if (AdjustX || AdjustY) {
            Vector2 Limit = Vector2(AdjustX, AdjustY);
            if (CorrectionLimits == Vector2(0.f, 0.f) || CorrectionLimits.Length2() > Limit.Length2()) {
                CorrectionLimits = Limit;
            }
        }
    }

    void Object::SetName(std::string Name) {
        if (Name != "") this->Name = Name;
    }

    void Object::SetIndex(std::string Index) {
        this->Index = Index;
    }

    void Object::SetValue(std::string Value) {
        this->Value = Value;
        if (this->Value != Value) EventQueue.push_back(Events::ValueChange);
        OnValueChange();
    }

    void Object::Show() {
        if (!Shown) {
            Shown = true;
            ResetInput();
            EventQueue.push_back(Events::Show);
        }
    }

    void Object::ShootTowards(float Direction, float Velocity) {
        ProjectileVelocity = Vector2(cosf(Direction)*Velocity, sinf(Direction)*Velocity);
        TimeOfFlight = 2*Velocity;
        Interactable.Relax();
    }

    void Object::StopMovement(int Axis) {
        if (Axis == Axis::X) {
            Offset(Axis, Interactable.X()+Velocity.X());
            Velocity = Vector2(0.f, Velocity.Y());
        } else if (Axis == Axis::Y) {
            Offset(Axis, Interactable.Y()+Velocity.Y());
            Velocity = Vector2(Velocity.X(), 0.f);
        } else if (Axis == Axis::XY) {
            Velocity = Vector2(0, 0);
            Interactable.DropAt(Interactable.X(), Interactable.Y());
            Interactable.Relax();
        }
        if (!IsMoving()) EventQueue.push_back(Events::Stop);
    }

    void Object::Toggle() {
        if (Shown) Hide(); else Show();
    }

    void Object::Update() {
        //Update size
        float ScreenWidth = Interactable.Width()+ResizeRate.X();
        float ScreenHeight = Interactable.Height()+ResizeRate.Y();
        Interactable.ResizeTo(ScreenWidth, ScreenHeight);

        //Process ResizeRate
        ResizeRate = Vector2(Interactable.dWidth(), Interactable.dHeight());

        //ProcessVelocity();

        //Get speed
        float SpeedX = Velocity.X();
        float SpeedY = Velocity.Y();

        if (BlockX*SpeedX > 0) SpeedX = 0;
        if (BlockY*SpeedY > 0) SpeedY = 0;

        //if (Name == "Neotms") std::cout << SpeedX << " " << Interactable.dX() << std::endl;

        //Update position
        float ScreenX = Interactable.X()+SpeedX;
        float ScreenY = Interactable.Y()+SpeedY;
        Interactable.DropAt(ScreenX, ScreenY);

        ProcessVelocity();
    }

    float Object::VelocityLimit(bool State) {
        if (!MaxVelocity && State && BaseParent) return BaseParent->VelocityLimit(State); else { if (!DragReference) return MaxVelocity; else return 0; }
    }

    float Object::VelocityX(bool State) {
        float Return = Velocity.X();
        if (State && BaseParent) Return += BaseParent->VelocityX(State);
        return Return;
    }

    float Object::VelocityY(bool State) {
        float Return = Velocity.Y();
        if (State && BaseParent) Return += BaseParent->VelocityY(State);
        return Return;
    }

    void Object::Who() {
        std::cout << Index << "." << Name << ": ";
        if (Anchor.Y() || Anchor.X()) std::cout << Anchor.Y() << " " << Anchor.X() << " ";
        std::cout << "(" << X(Reference::Parent) << ", " << Y(Reference::Parent) << ")";
        if (BaseParent) std::cout << " " << BaseParent->GetName();
        std::cout << std::endl;
    }

    float Object::Width(int State) {
        float Scale = Scaled?this->Scale.X():1.f;
        float BaseWidth = Scale*Interactable.Width(State);

        float Width = BaseWidth;

        if (!BaseWidth) {
            Width = InheritedWidth(State);
        }

        //if (Name == "Paragraph") std::cout << Width << " " << Height() << " " << BaseWidth << std::endl;

        return Width;
    }

    float Object::X(int Reference, float Position, int State) {
        float X = 0.f;

        //Reference
        if (Reference == Reference::Origin) {
            X += ParentX(State)+AnchorX(State);
        } else if (Reference == Reference::Parent) {
            X += AnchorX(State);
        } else if (Reference == Reference::Collision) {
            X += ParentX(State)+AnchorX(State)+(Width(State)-CollisionWidth())/2.f;
        } else if (Reference == Reference::ParentCollision) {
            X += (Width(State)-CollisionWidth())/2.f;
        }

        //Position
        if (Reference == Reference::Collision || Reference == Reference::ParentCollision) {
            X += Position*CollisionWidth();
        } else {
            X += Position*Width(State);
        }

        //State
        float Scale = Scaled?this->Scale.X():1.f;
        X += Scale*Interactable.X(State);

        return X;
    }

    float Object::X(float Interpolation, int Reference, float Position) {
        return X(Reference, Position)+InterpolateX(Interpolation);
    }

    float Object::Y(int Reference, float Position, int State) {
        float Y = 0;

        //Reference
        if (Reference == Reference::Origin) {
            Y += ParentY(State)+AnchorY(State);
        } else if (Reference == Reference::Parent) {
            Y += AnchorY(State);
        } else if (Reference == Reference::Collision) {
            Y += ParentY(State)+AnchorY(State)+(Height(State)-CollisionHeight())/2.f;
        } else if (Reference == Reference::ParentCollision) {
            Y += (Height(State)-CollisionHeight())/2.f;
        }

        //Position
        if (Reference == Reference::Collision || Reference == Reference::ParentCollision) {
            Y += Position*CollisionHeight();
        } else {
            Y += Position*Height(State);
        }

        //State
        //float Scale = (Scaled&&this->Scale.X()!=1.f)?this->Scale.Y():1;
        float Scale = Scaled?this->Scale.Y():1;
        Y += Scale*Interactable.Y(State);

        //if (Name == "TestMessage") std::cout << Y << " " << AnchorY() << " " << ParentY(State) << " " << Scale << std::endl;

        return Y;
    }

    float Object::Y(float Interpolation, int Reference, float Position) {
        return Y(Reference, Position)+InterpolateY(Interpolation);
    }

    Object::~Object() {
        AllowOtherInput = Hovered = Shown = Scaled = State = Sticky = false;
        Direction = 0; MaxVelocity = 0;
        BlockX = BlockY = Clicked = TimeOfFlight = 0;
        Resistance = 0;
        BaseParent = NULL;
    }
}
