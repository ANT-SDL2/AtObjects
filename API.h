//API.h
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef NEOPHYSICS_API_H
#define NEOPHYSICS_API_H

#include <AtTools/AtTools.h>
#include <iostream>

namespace AtPhys {
    template <class StateType, class BaseType>
    class API {
        //Members
        protected:
            static StateType *ObjectManager;
            static std::string ObjectManagerName;
            static const luaL_Reg ObjectMethods[], ObjectManagerMethods[];
        //Access Methods
        protected:
            API();
            ~API();
        //Process Methods
        protected:
        //Common
            static void MakeObjectInstance(lua_State *Lua, std::string ID, std::string Type) {
                lua_createtable(Lua, 0, 1);
                lua_pushstring(Lua, ID.c_str());
                lua_setfield(Lua, -2, "ID");

                luaL_getmetatable(Lua, Type.c_str());
                lua_setmetatable(Lua, -2);
            }

            template <class Type>
            static Type* GetObject(lua_State *Lua) {
                Type *Object = NULL;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments >= 1 and lua_istable(Lua, 1)) {
                        lua_getfield(Lua, 1, "ID");
                        std::string ID = luaL_checkstring(Lua, Arguments+1);

                        ObjectManager->Get(Object, ID);
                    }
                }

                return Object;
            }

        //ObjectManager
            //Access Functions
            static int GetDebug(lua_State *Lua) {
                int Results = 1;
                bool State = false;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        int i = lua_tointeger(Lua, 1);

                        State = ObjectManager->GetDebug(i);
                    } else std::cerr << "(AtPhys/API.h) GetDebug(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                lua_pushboolean(Lua, State);

                return Results;
            }

            //Process Functions
            static int ClearState(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    ObjectManager->ClearState();
                }

                return Results;
            }

            static int LoadState(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string Location = lua_tostring(Lua, 1);
                        ObjectManager->LoadState(Location);
                    } else std::cerr << "(AtPhys/API.h) LoadState(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ReloadState(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 0) {
                        ObjectManager->ReloadState();
                    } else std::cerr << "(AtPhys/API.h) ReloadState(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int SetDebug(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 2) {
                        int i = lua_tointeger(Lua, 1);
                        bool State = lua_toboolean(Lua, 2);

                        ObjectManager->SetDebug(i, State);
                    } else std::cerr << "(AtPhys/API.h) SetDebug(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int SetResolution(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 1) {
                        std::string Resolution = lua_tostring(Lua, 1);
                        int Width = AtTools::Strings::StringTo<int>(Resolution, 'x');
                        int Height = AtTools::Strings::StringTo<int>(Resolution, 'x', 1);

                        ObjectManager->SetResolution(Width, Height);
                    } else std::cerr << "(AtPhys/API.h) SetResolution(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int SetScale(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 2) {
                        float ScaleX = lua_tonumber(Lua, 1);
                        float ScaleY = lua_tonumber(Lua, 2);

                        ObjectManager->SetScale(ScaleX, ScaleY);
                    } else std::cerr << "(AtPhys/API.h) SetScale(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

        //Object
            //Access Functions
            static int ObjectDistanceFrom(lua_State *Lua) {
                int Results = 1;
                float Distance = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        std::string ID = luaL_checkstring(Lua, 2);
                        BaseType *Target = NULL;
                        ObjectManager->Get(Target, ID);
                        if (Target) {
                            Distance = Object->DistanceFrom(Target);
                        }
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectDistanceFrom(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Distance);

                return Results;
            }

            static int ObjectGetAnchor(lua_State *Lua) {
                int Results = 1;
                float Anchor = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        int Axis = lua_tointeger(Lua, 2);
                        Anchor = Object->GetAnchor(Axis);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectGetAnchor(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Anchor);

                return Results;
            }

            static int ObjectGetDirection(lua_State *Lua) {
                int Results = 1;
                float Direction = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Direction = Object->GetDirection();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectGetDirection(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Direction);

                return Results;
            }

            static int ObjectGetIndex(lua_State *Lua) {
                int Results = 1;
                std::string Index = "(None)";

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Index = Object->GetIndex();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectGetIndex(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, Index.c_str());

                return Results;
            }

            static int ObjectGetName(lua_State *Lua) {
                int Results = 1;
                std::string Name = "(None)";

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Name = Object->GetName();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectGetName(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, Name.c_str());

                return Results;
            }

            static int ObjectGetValue(lua_State *Lua) {
                int Results = 1;
                std::string Value;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Value = Object->GetValue();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectGetValue(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushstring(Lua, Value.c_str());

                return Results;
            }

            static int ObjectHeight(lua_State *Lua) {
                int Results = 1;
                int Height = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Height = Object->Height();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectHeight(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Height);

                return Results;
            }

            static int ObjectIsClicked(lua_State *Lua) {
                int Results = 1;
                bool Clicked = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    int Button = lua_tointeger(Lua, 2);
                    if (Object) {
                        Clicked = Object->IsClicked(Button);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectIsClicked(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, Clicked);

                return Results;
            }

            static int ObjectIsHovered(lua_State *Lua) {
                int Results = 1;
                bool Hovered = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Hovered = Object->IsHovered();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectIsHovered(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, Hovered);

                return Results;
            }

            static int ObjectIsInteractive(lua_State *Lua) {
                int Results = 1;
                bool Interactive = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Interactive = Object->GetInteractable()->IsInteractive();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectIsInteractive(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, Interactive);

                return Results;
            }

            static int ObjectIsMovable(lua_State *Lua) {
                int Results = 1;
                bool Movable = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Movable = Object->GetInteractable()->IsMovable();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectIsMovable(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, Movable);

                return Results;
            }

            static int ObjectIsResizable(lua_State *Lua) {
                int Results = 1;
                bool Resizable = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Resizable = Object->GetInteractable()->IsResizable();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectIsMovable(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, Resizable);

                return Results;
            }

            static int ObjectIsShown(lua_State *Lua) {
                int Results = 1;
                bool IsShown = false;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1 || Arguments == 2) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        bool State = true;

                        if (Arguments == 2) State = false;
                        IsShown = Object->IsShown(State);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectIsShown(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushboolean(Lua, IsShown);

                return Results;
            }

            static int ObjectWidth(lua_State *Lua) {
                int Results = 1;
                int Width = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Width = Object->Width();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectWidth(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Width);

                return Results;
            }

            static int ObjectX(lua_State *Lua) {
                int Results = 1;
                int X = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments >= 1 and Arguments <= 3) {
                    int Reference = 0;
                    float Position = 0.f;

                    if (Arguments == 2) {
                        Reference = lua_tointeger(Lua, 2);
                    } else if (Arguments == 3) {
                        Reference = lua_tointeger(Lua, 2);
                        Position = lua_tonumber(Lua, 3);
                    }

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        X = Object->X(Reference, Position);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectX(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, X);

                return Results;
            }

            static int ObjectY(lua_State *Lua) {
                int Results = 1;
                int Y = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments >= 1 or Arguments <= 3) {
                    int Reference = 0;
                    float Position = 0.f;

                    if (Arguments == 2) {
                        Reference = lua_tointeger(Lua, 2);
                    } else if (Arguments == 3) {
                        Reference = lua_tointeger(Lua, 2);
                        Position = lua_tonumber(Lua, 3);
                    }

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Y = Object->Y(Reference, Position);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectY(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                lua_pushnumber(Lua, Y);

                return Results;
            }

            //Process Functions
            static int ObjectAnchorAt(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    float HorizontalAnchor = lua_tonumber(Lua, 2);
                    float VerticalAnchor = lua_tonumber(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->AnchorAt(HorizontalAnchor, VerticalAnchor);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectAnchorAt(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectCallFunction(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    std::string FunctionName = lua_tostring(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object && FunctionName != "") {
                        std::string Script = Object->GetID() + ":" + FunctionName + "(); ";
                        AtTools::Lua::ExecuteScript(Lua, Script.c_str());
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectCallFunction(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectDropAt(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    float X = lua_tointeger(Lua, 2);
                    float Y = lua_tointeger(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->DropAt(X, Y);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectDropAt(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectEnable(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->Enable(State);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectEnable(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectEnableAutoSize(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->EnableAutoSize(State);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectSetAutoSize(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectEnableInteraction(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->GetInteractable()->EnableInteraction(State);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectEnableInteraction(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectEnableMoving(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->GetInteractable()->EnableMoving(State);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectEnableMoving(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectEnableResizing(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    bool State = lua_toboolean(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->GetInteractable()->EnableResizing(State);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectEnableResizing(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectHide(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->Hide();

                        //Unfocus dialog
                        /*Dialog *Dialog = NULL;
                        ObjectManager->Get(Dialog, Name);
                        if (Dialog) {
                            Dialog->SetFocused(false);
                        }*/
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectHide(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectLookAt(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    int Type = lua_type(Lua, 2);

                    switch(Type) {
                        case LUA_TNUMBER: {
                            float Direction = lua_tonumber(Lua, 2)*M_PI/180.f;

                            BaseType *Object = GetObject<BaseType>(Lua);
                            if (Object) {
                                Object->LookAt(Direction);
                            }
                        }
                        break;
                        case LUA_TSTRING: {
                            std::string ID = lua_tostring(Lua, 2);

                            BaseType *Object = GetObject<BaseType>(Lua);
                            if (Object) {
                                BaseType *Target = NULL;
                                ObjectManager->Get(Target, ID);
                                Object->LookAt(Target);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectLookAt(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectMoveBy(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    float X = lua_tointeger(Lua, 2);
                    float Y = lua_tointeger(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->MoveBy(X, Y);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectMoveBy(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectMoveTo(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    float X = lua_tointeger(Lua, 2);
                    float Y = lua_tointeger(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->MoveTo(X, Y);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectMoveTo(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectMoveToLocal(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    float X = lua_tonumber(Lua, 2);
                    float Y = lua_tonumber(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->MoveToLocal(X, Y);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectMoveToLocal(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectOffset(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    int Axis = lua_tointeger(Lua, 2);
                    float Position = lua_tonumber(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->Offset(Axis, Position);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectOffset(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectOnEvent(lua_State *Lua) {
                return 0;
            }

            static int ObjectResetInput(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->ResetInput();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectResetInput(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectResistDrag(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    float Resistance = lua_tonumber(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->ResistDrag(Resistance);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectResistDrag(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectResize(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        int Axis = lua_tointeger(Lua, 2);
                        float Size = lua_tonumber(Lua, 3);

                        BaseType *Object = GetObject<BaseType>(Lua);
                        if (Object) {
                            Object->Resize(Axis, Size);
                        }
                    } else std::cerr << "(AtPhys/API.h) ObjectResize(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ObjectResizeTo(lua_State *Lua) {
                int Results = 0;

                if (ObjectManager) {
                    int Arguments = lua_gettop(Lua);

                    if (Arguments == 3) {
                        float Width = lua_tonumber(Lua, 2);
                        float Height = lua_tonumber(Lua, 3);

                        BaseType *Object = GetObject<BaseType>(Lua);
                        if (Object) {
                            Object->ResizeTo(Width, Height);
                        }
                    } else std::cerr << "(AtPhys/API.h) ObjectResizeTo(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;
                }

                return Results;
            }

            static int ObjectRotateBy(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    float Angle = lua_tonumber(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->RotateBy(Angle);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectRotateBy(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectSetFunction(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    std::string FunctionName = lua_tostring(Lua, 2);
                    std::string FunctionContent = lua_tostring(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object && FunctionName != "") {
                        std::string Script = "function " + Object->GetID() + ":" + FunctionName + "() " + FunctionContent + " end";
                        AtTools::Lua::ExecuteScript(Lua, Script.c_str());
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectSetFunction(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectSetMember(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 3) {
                    std::string LuaIndex = lua_tostring(Lua, 2);
                    std::string Value = lua_tostring(Lua, 3);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        std::string Script = Object->GetID() + "." + LuaIndex + " = " + Value + ";";
                        AtTools::Lua::ExecuteScript(Lua, Script.c_str());
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectSetMember(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectSetValue(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 2) {
                    std::string Value = lua_tostring(Lua, 2);

                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->SetValue(Value);
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectSetValue(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectShow(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->Show();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectShow(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectStopMovement(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->StopMovement();
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectStopMovement(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }

            static int ObjectToggle(lua_State *Lua) {
                int Results = 0;

                int Arguments = lua_gettop(Lua);

                if (Arguments == 1) {
                    BaseType *Object = GetObject<BaseType>(Lua);
                    if (Object) {
                        Object->Toggle();

                        /*Dialog *Dialog = NULL;
                        ObjectManager->Get(Dialog, Name);
                        if (Dialog) {
                            if (Dialog->IsShown()) Dialog->SetFocused(true); else Dialog->SetFocused(false);
                        }*/
                    }
                } else std::cerr << "(AtPhys/API.h) ObjectToggle(): Function called with invalid number of arguments (" << Arguments << ")." << std::endl;

                return Results;
            }
    };

    template <class StateType, class BaseType>
    const luaL_Reg API<StateType, BaseType>::ObjectManagerMethods[] = {
        {"GetDebug", API::GetDebug},

        {"ClearState", API::ClearState},
        {"LoadState", API::LoadState},
        {"ReloadState", API::ReloadState},
        {"SetDebug", API::SetDebug},
        {"SetResolution", API::SetResolution},
        {"SetScale", API::SetScale},
        {NULL, NULL}
    };

    template <class StateType, class BaseType>
    const luaL_Reg API<StateType, BaseType>::ObjectMethods[] {
        {"DistanceFrom", API::ObjectDistanceFrom},
        {"GetAnchor", API::ObjectGetAnchor},
        {"GetDirection", API::ObjectGetDirection},
        {"GetIndex", API::ObjectGetIndex},
        {"GetName", API::ObjectGetName},
        {"GetValue", API::ObjectGetValue},
        {"Height", API::ObjectHeight},
        {"IsClicked", API::ObjectIsClicked},
        {"IsHovered", API::ObjectIsHovered},
        {"IsInteractive", API::ObjectIsInteractive},
        {"IsMovable", API::ObjectIsMovable},
        {"IsResizable", API::ObjectIsResizable},
        {"IsShown", API::ObjectIsShown},
        {"Width", API::ObjectWidth},
        {"X", API::ObjectX},
        {"Y", API::ObjectY},

        {"AnchorAt", API::ObjectAnchorAt},
        {"CallFunction", API::ObjectCallFunction},
        {"DropAt", API::ObjectDropAt},
        {"Enable", API::ObjectEnable},
        {"EnableAutoSize", API::ObjectEnableAutoSize},
        {"EnableInteraction", API::ObjectEnableInteraction},
        {"EnableMoving", API::ObjectEnableMoving},
        {"EnableResizing", API::ObjectEnableResizing},
        {"Hide", API::ObjectHide},
        {"LookAt", API::ObjectLookAt},
        {"MoveBy", API::ObjectMoveBy},
        {"MoveTo", API::ObjectMoveTo},
        {"MoveToLocal", API::ObjectMoveToLocal},
        {"Offset", API::ObjectOffset},

        {"OnClick", API::ObjectOnEvent},
        {"OnRightClick", API::ObjectOnEvent},
        {"OnHide", API::ObjectOnEvent},
        {"OnInteract", API::ObjectOnEvent},
        {"OnLoad", API::ObjectOnEvent},
        {"OnMouseIn", API::ObjectOnEvent},
        {"OnMouseOut", API::ObjectOnEvent},
        {"OnMove", API::ObjectOnEvent},
        {"OnProcess", API::ObjectOnEvent},
        {"OnReturn", API::ObjectOnEvent},
        {"OnShow", API::ObjectOnEvent},
        {"OnStop", API::ObjectOnEvent},
        {"OnValueChange", API::ObjectOnEvent},

        {"ResetInput", API::ObjectResetInput},
        {"ResistDrag", API::ObjectResistDrag},
        {"Resize", API::ObjectResize},
        {"ResizeTo", API::ObjectResizeTo},
        {"RotateBy", API::ObjectRotateBy},
        {"SetFunction", API::ObjectSetFunction},
        {"SetMember", API::ObjectSetMember},
        {"SetValue", API::ObjectSetValue},
        {"Show", API::ObjectShow},
        {"StopMovement", API::ObjectStopMovement},
        {"Toggle", API::ObjectToggle},
        {NULL, NULL}
    };

    template <class StateType, class BaseType>
    API<StateType, BaseType>::API() {}

    template <class StateType, class BaseType>
    API<StateType, BaseType>::~API() {}
}

#endif
