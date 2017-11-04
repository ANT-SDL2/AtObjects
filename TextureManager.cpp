//TextureManager.cpp
#include "TextureManager.h"

using namespace AtUtility;

namespace AtObjects {
    bool TextureManager::IsAnimationRegistered(std::string Animationset, int AnimationID) {
        std::ostringstream Stream;
        Stream << Animationset << ":" << AnimationID;
        std::string ID = Stream.str();

        Animation *Animation = Animations[ID];
        if (Animation) return true; else return false;
    }

    Animation *TextureManager::LoadAnimation(std::string AnimationID) {
        if (Animations[AnimationID]) return Animations[AnimationID]; else return Animations.NewItem(AnimationID);
    }

    Texture *TextureManager::LoadTexture(std::string TextureID) {
        Texture *Texture = NULL;

        std::vector<std::string> Data = TextureIndex[TextureID];
        if (Data.size() < 6) {
            if (Data.size() > 0) {
                std::cerr << "(Graphics/TextureManager.cpp) LoadTexture(): Missing Texture data." << std::endl;
            } else {
                //std::cerr << "(Graphics/TextureManager.cpp) LoadTexture(): Requested unregistered TextureID: " << TextureID << std::endl;
                Data.resize(6);
                Data[0] = GraphicsDirectory+TextureID+".png";
                TextureIndex[TextureID] = Data;
            }
        }

        if (Data.size() == 6) {
            Texture = Textures[TextureID];

            if (!Texture) {
                //Tile size
                int Width = 0, Height = 0;
                if (Data[1] != "") {
                    Width = Strings::StringTo<int>(Data[1], 'x');
                    Height = Strings::StringTo<int>(Data[1], 'x', 1);
                }

                //LinearFiltering
                bool LinearFiltering = false;
                if (Data[5] != "") {
                    if (Data[5] == "true") LinearFiltering = true;
                }

                //Load Texture
                Texture = Textures.NewItem(TextureID);
                if (Texture) {
                    if (!Texture->LoadFromImage(Data[0], Width, Height, LinearFiltering)) {
                        Textures.RemoveItem(Texture);
                        Texture = NULL;
                    } //else std::cout << "   Loaded: " << Texture->GetID() << std::endl;
                }
            }
        }

        return Texture;
    }

    Texture *TextureManager::LoadTextureFromText(std::string FontPath, int Size, std::string Text, int Styles) {
        AtObjects::Texture *Texture = NULL;

        if (Text != "") {
            std::ostringstream Stream;
            Stream << "[" << FontPath << ":" << Size << ":" << Styles << "]" << Text;
            std::string ID = Stream.str();

            Texture = Textures[ID];
            if (!Texture) {
                Texture = Textures.NewItem(ID);
                if (Texture) {
                    if (!Texture->LoadFromText(FontPath, Size, Text, Styles)) {
                        Textures.RemoveItem(Texture);
                        Texture = NULL;
                    }
                }
            }
        }

        return Texture;
    }

    void TextureManager::ReleaseTextures() {
        Textures.Clear();
    }

    void TextureManager::SetGraphicsDirectory(std::string GraphicsDirectory) {
        if (this->GraphicsDirectory == "") this->GraphicsDirectory = GraphicsDirectory;
    }

    void TextureManager::SetTextureIndex(std::string TextureID, std::vector<std::string> &Data) {
        TextureIndex[TextureID] = Data;
    }

    TextureManager::TextureManager() {
        Animations.SetName("Animations");
        Textures.SetName("Textures");
    }

    void TextureManager::Who() {
        Textures.Who();
        Animations.Who();
    }

    TextureManager::~TextureManager() {
    }
}
