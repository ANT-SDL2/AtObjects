//TextureManager.h
#ifndef NEOOBJECTS_TEXTUREMANAGER_H
#define NEOOBJECTS_TEXTUREMANAGER_H

#include <sstream>
#include <AtUtility/Strings.h>
#include "Animation.h"
#include "Texture.h"
#include "Renderable.h"
#include "List.h"

using namespace AtUtility;

namespace AtObjects {
    class TextureManager {
        //Members
        protected:
            std::map< std::string, std::vector<std::string> > TextureIndex;
            std::string GraphicsDirectory;
            List<std::string, Animation> Animations;
            List<std::string, Texture> Textures;
        //Access Methods
        public:
            TextureManager();
            ~TextureManager();
            bool IsAnimationRegistered(std::string Animationset, int AnimationID);
            void ReleaseTextures();
            void Who();
        //Process Methods
        public:
            Animation *LoadAnimation(std::string AnimationID);
            Texture *LoadTexture(std::string TextureID);
            Texture *LoadTextureFromText(std::string FontPath, int Size, std::string Text, int Styles);
            void SetGraphicsDirectory(std::string GraphicsDirectory);
            void SetTextureIndex(std::string TextureID, std::vector<std::string> &Data);
            template <class IDType, class Type, class InheritedType> void ProcessAnimationQueue(List<IDType, Type, InheritedType> &List);
            template <class IDType, class Type, class InheritedType> void ReloadListTextures(List<IDType, Type, InheritedType> &List);
            template <class BaseType> void SetObjectTexture(BaseType *Object, std::string TextureID);
    };

    template <class IDType, class Type, class InheritedType>
    void TextureManager::ProcessAnimationQueue(List<IDType, Type, InheritedType> &List) {
        Type *Object = List.GetFirst();
        while (Object) {
            AtObjects::Renderable *Renderable = Object->GetRenderable();
            if (Renderable->GetAnimationQueue()) {
                std::ostringstream Stream;
                Stream << Renderable->GetAnimationset() << ":" << Renderable->GetAnimationQueue();
                std::string ID = Stream.str();

                Animation *Animation = Animations[ID];
                if (Animation) {
                    Renderable->SetAnimation(Animation);
                    //Object->OnAnimationChange();
                }
            }

            AtObjects::Renderable *LowerRenderable = Object->GetLowerRenderable();
            if (LowerRenderable->GetAnimationQueue()) {
                std::ostringstream Stream;
                Stream << LowerRenderable->GetAnimationset() << ":" << LowerRenderable->GetAnimationQueue();
                std::string ID = Stream.str();

                Animation *Animation = Animations[ID];
                if (Animation) {
                    LowerRenderable->SetAnimation(Animation);
                }

                //if (Object->GetName() == "Neotms") std::cout << ID << std::endl;
            }

            Object = Object->Next;
        }
    }

    template <class IDType, class Type, class InheritedType>
    void TextureManager::ReloadListTextures(List<IDType, Type, InheritedType> &List) {
        Type *Object = List.GetFirst();
        while (Object) {
            AtObjects::Renderable *Renderable = Object->GetRenderable();
            std::string ID = Renderable->GetTextureID();

            if (ID != "") {
                AtObjects::Texture *Texture = Renderable->GetTexture();
                AtObjects::Texture *NewTexture = LoadTexture(ID);
                if (Texture != NewTexture) {
                    Renderable->SetTexture(NewTexture);
                }
            } else Renderable->SetTexture(NULL);

            Object = Object->Next;
        }
    }

    template <class BaseType>
    void TextureManager::SetObjectTexture(BaseType *Object, std::string TextureID) {
        if (Object) {
            if (TextureID == "") {
                AtObjects::Renderable *Renderable = Object->GetRenderable();
                Renderable->SetTexture(NULL);
                Renderable->SetTextureID("");
            } else {
                std::vector<std::string> Data = TextureIndex[TextureID];
                if (Data.size() < 6) {
                    if (Data.size() > 0) std::cerr << "(Graphics/TextureManager.cpp) SetElementTexture(): Missing Texture data." << std::endl; else {
                        //std::cerr << "(Graphics/TextureManager.h) SetObjectTexture(): Requested unregistered TextureID: " << TextureID << std::endl;
                        Data.resize(6);
                        Data[0] = GraphicsDirectory+TextureID+".png";
                        TextureIndex[TextureID] = Data;
                    }
                }

                int Color[4]; Color[0] = Color[1] = Color[2] = Color[3] = 255;

                //Color
                if (Data[2] != "") {
                    for (int i = 0; i<3; i++) Color[i] = Strings::StringTo<int>(Data[2], ' ', i);
                }

                //Transparency
                if (Data[3] != "") {
                    Color[3] = Strings::StringTo<float>(Data[3])*255.0f/100.0f;
                }

                AtObjects::Renderable *Renderable = Object->GetRenderable();
                Renderable->SetIdleColor(Color);

                //Hover Color
                if (Data[4] != "") {
                    int HoverColor[4];
                    for (int i = 0; i<3; i++) HoverColor[i] = Strings::StringTo<int>(Data[4], ' ', i);

                    //Transparency
                    HoverColor[3] = 255;

                    AtObjects::Renderable *Renderable = Object->GetRenderable();
                    Renderable->SetHoverColor(HoverColor);
                }

                //Load Texture
                AtObjects::Texture *Texture = LoadTexture(TextureID);
                if (Texture) {
                    Renderable->SetTexture(Texture);
                    Renderable->SetTextureID(TextureID);
                }
            }
        }
    }
}

#endif
