//ObjectManager.h
#ifndef NEOOBJECTS_OBJECTMANAGER_H
#define NEOOBJECTS_OBJECTMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <Lua/lua.hpp>
#include <AtTools/AtTools.h>

#include "Object.h"
#include "List.h"

namespace AtObjects {
    namespace Input {
        static const int Keyboard       = 1;
        static const int Joystick       = 2;
    }

    template <class BaseType>
    class ObjectManager {
        //Members
        protected:
            bool Debug[10], RenderCollision, Render;
            int InputType;
            SDL_Joystick *Joystick;
            std::map<std::string, BaseType*> Objects;
            std::string AssetsDirectory, Location, UserDirectory;
            std::vector<int> EventQueue;
            std::vector<std::string> Scripts;
            lua_State *Lua;
            BaseType *DebugFocus;
            Vector2 Resolution, NativeResolution, Scale;
        //Access Methods
        public:
            ObjectManager();
            ~ObjectManager();
            bool GetDebug(int i);
        //Process Methods
        public:
            void SetDebug(int i, bool State);
        protected:
            template <class IDType, class Type, class InheritedType> Type *MakeObject(List<IDType, Type, InheritedType> &List, std::string Index, std::string ID, InheritedType *Parent);
            template <class IDType, class Type, class InheritedType> void ProcessListCollisions(List<IDType, Type, InheritedType> &List, InheritedType *Area);
            template <class IDType, class Type, class InheritedType> bool RemoveFromList(List<IDType, Type, InheritedType> &List, Type *Target);
            virtual void RemoveRelatedTo(BaseType *Parent);
            template <class IDType, class Type, class InheritedType> void RemoveRelatedToFromList(List<IDType, Type, InheritedType> &List, InheritedType *Parent);
            template <class IDType, class Type, class InheritedType> void ResetList(List<IDType, Type, InheritedType> &List, InheritedType *Parent = NULL);
            virtual void ResetRelatedTo(BaseType *Parent);
            template <class IDType, class Type, class InheritedType> void ScaleList(List<IDType, Type, InheritedType> &List, Vector2 Scale);
    };

    template <class BaseType>
    bool ObjectManager<BaseType>::GetDebug(int i) {
        if (i >= 0 && i < 10) return Debug[i]; else return false;
    }

    template <class BaseType>
    template <class IDType, class Type, class InheritedType>
    Type *ObjectManager<BaseType>::MakeObject(List<IDType, Type, InheritedType> &List, std::string Index, std::string Name, InheritedType *Parent) {
        Type *Object = NULL;

        if (!Objects[Index+"."+Name]) {
            Object = List.NewItem(Index+"."+Name);

            if (Object) {
                Objects[Index+"."+Name] = Object;

                Object->SetParent(Parent);
                Object->SetName(Name);
                Object->SetIndex(Index);
                Object->PushEvent(AtObjects::Events::Load);
                Object->ScaleTo(Scale.X(), Scale.Y());
                /*if (Index == "Settings" && ID == "WindowModeList") {
                    std::cout << Object->ID() << " " << Objects[Index][ID]->ID() << std::endl;
                }*/
            }
        } else std::cerr << "(AtPhys/ObjectManager.h) MakeObject(): Failed to create duplicate element: " << Name << std::endl;

        return Object;
    }

    template <class BaseType>
    ObjectManager<BaseType>::ObjectManager() {
        this->Lua = NULL;
        Debug[0] = Debug[1] = Debug[2] = Debug[3] = Debug[4] = Debug[5] = Debug[6] = Debug[7] = Debug[8] = Debug[9] = false;
        InputType = Input::Keyboard;
        Joystick = NULL;
        Resolution = Vector2(800, 600);
        NativeResolution = Vector2(800, 600);
        Scale = Vector2(1.f, 1.f);
        DebugFocus = NULL;
    }

    template <class BaseType>
    template <class IDType, class Type, class InheritedType>
    void ObjectManager<BaseType>::ProcessListCollisions(List<IDType, Type, InheritedType> &List, InheritedType *Area) {
        Type *Object = List.GetFirst();
        while (Object) {
            if (Object->GetParent() == Area) {
                Object->ProcessCollisions();
            }
            Object = Object->Next;
        }
    }

    template <class BaseType>
    template <class IDType, class Type, class InheritedType>
    bool ObjectManager<BaseType>::RemoveFromList(List<IDType, Type, InheritedType> &List, Type *Target) {
        bool Removed = false;

        if (Target) {
            std::string ObjectID = Target->GetID();
            if (List[ObjectID]) {
                RemoveRelatedTo(Target);

                Removed = List.RemoveItem(List[ObjectID]);
                if (Removed) {
                    Objects.erase(ObjectID);
                    //std::cout << "Removed: " << ObjectID << std::endl;
                }
            }
        }

        return Removed;
    }

    template <class BaseType>
    void ObjectManager<BaseType>::RemoveRelatedTo(BaseType *Parent) {
    }

    template <class BaseType>
    template <class IDType, class Type, class InheritedType>
    void ObjectManager<BaseType>::RemoveRelatedToFromList(List<IDType, Type, InheritedType> &List, InheritedType *Parent) {
        Type *Object = List.GetFirst();
        while (Object) {
            Type *Next = Object->Next;

            if (Object->GetParent() == Parent) {
                if (Next) {
                    InheritedType *NextParent = Next->GetParent();

                    bool Child = false;
                    while (NextParent && !Child) { if (NextParent == Object) Child = true; else NextParent = NextParent->GetParent(); }

                    if (Child) {
                        //std::cout << Next->ID() << " is a child of " << Object->ID() << std::endl;

                        while (Next && Child) {
                            Next = Next->Next;

                            if (Next) {
                                NextParent = Next->GetParent();

                                Child = false;
                                while (NextParent && !Child) if (NextParent == Object) Child = true; else NextParent = NextParent->GetParent();
                            }
                        }

                        //if (Next) std::cout << Next->ID() << " is now Next" << std::endl; else std::cout << "there are no more items on the list" << std::endl;
                    }

                }

                RemoveFromList(List, Object);
            }
            Object = Next;
        }
    }

    template <class BaseType>
    template <class IDType, class Type, class InheritedType>
    void ObjectManager<BaseType>::ResetList(List<IDType, Type, InheritedType> &List, InheritedType *Parent) {
        Type *Object = List.GetFirst();
        while (Object) {
            if (Object->GetParent() == Parent) {
                Object->ResetInput();
                ResetRelatedTo(Object);
            }
            Object = Object->Next;
        }
    }

    template <class BaseType>
    void ObjectManager<BaseType>::ResetRelatedTo(BaseType *Parent) {
    }

    template <class BaseType>
    template <class IDType, class Type, class InheritedType>
    void ObjectManager<BaseType>::ScaleList(List<IDType, Type, InheritedType> &List, Vector2 Scale) {
        Type *Object = List.GetFirst();
        while (Object) {
            Object->ScaleTo(Scale.X(), Scale.Y());
            Object = Object->Next;
        }
    }

    template <class BaseType>
    void ObjectManager<BaseType>::SetDebug(int i, bool State) {
        if (i >= 0 && i < 10) {
            Debug[i] = State;
            DebugFocus = NULL;
        }
    }

    template <class BaseType>
    ObjectManager<BaseType>::~ObjectManager() {
        Lua = NULL;
        Debug[0] = Debug[1] = Debug[2] = Debug[3] = Debug[4] = Debug[5] = Debug[6] = Debug[7] = Debug[8] = Debug[9] = false;
        InputType = 0;
        Joystick = NULL;
        DebugFocus = NULL;
    }
}

#endif
