//Texture.cpp
#include "Texture.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <gl/glext.h>
#include <iostream>
#include <sstream>

namespace sdlObjects {
    int Texture::GetColumns() {
        if (TileWidth) return Width/TileWidth; else return 1;
    }

    void Texture::GetData(GLubyte *Data) {
        glBindTexture(GL_TEXTURE_2D, GLTexture);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint Texture::GetGLTexture() {
        return GLTexture;
    }

    int Texture::GetHeight(bool State) {
        if (State == false && TileHeight) return TileHeight; else return Height;
    }

    std::string Texture::GetID() {
        return ID;
    }

    int Texture::GetRows() {
        if (TileHeight) return Height/TileHeight; else return 1;
    }

    int Texture::GetTilesNumber() {
        return GetRows() *GetColumns();
    }

    int Texture::GetUses() {
        return Uses;
    }

    int Texture::GetWidth(bool State) {
        if (State == false && TileWidth) return TileWidth; else return Width;
    }

    int Texture::LoadFromImage(std::string Filename, int TileWidth, int TileHeight, bool LinearFiltering) {
        int LoadFromImage = 0;

        if (TileWidth) this->TileWidth = TileWidth;
        if (TileHeight) this->TileHeight = TileHeight;

        //Load image with SDL
        SDL_Surface *Image = IMG_Load(Filename.c_str());
        if (Image) {
            LoadFromSurface(Image, LinearFiltering);
            SDL_FreeSurface(Image);
            ID = Filename;
            LoadFromImage = 1;
        } else std::cerr << "(Graphics/Texture.cpp) LoadFromImage(): Unable to load image file '" << Filename << "'." << std::endl;

        return LoadFromImage;
    }

    void Texture::LoadFromScreen(std::string ID, int Width, int Height) {
        glBindTexture(GL_TEXTURE_2D, GLTexture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0,0, Width, Height, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        this->ID = ID;
        this->Width = Width;
        this->Height = Height;
        TileWidth = TileHeight = 0;
    }

    void Texture::LoadFromSurface(SDL_Surface *Image, bool LinearFiltering) {
        if (Image) {
            SDL_Surface *OptimizedImage = NULL;
            GLenum ImageFormat = 0;
            GLint BitsPerPixel = 0;

            BitsPerPixel = Image->format->BytesPerPixel;
            if (BitsPerPixel == 4) {
                if (Image->format->Rmask == 0x000000ff) ImageFormat = GL_RGBA; else ImageFormat = GL_BGRA;
            } else if (BitsPerPixel== 3) {
                if (Image->format->Rmask == 0x000000ff) ImageFormat = GL_RGB; else ImageFormat = GL_BGR;
            } else {
                //OptimizedImage = SDL_DisplayFormat(Image);
                BitsPerPixel = OptimizedImage->format->BytesPerPixel;
                if (BitsPerPixel == 4) {
                    if (OptimizedImage->format->Rmask == 0x000000ff) ImageFormat = GL_RGBA; else ImageFormat = GL_BGRA;
                } else if (BitsPerPixel== 3) {
                    if (OptimizedImage->format->Rmask == 0x000000ff) ImageFormat = GL_RGB; else ImageFormat = GL_BGR;
                }
            }

            if (!OptimizedImage) OptimizedImage = Image;

            glGenTextures(1, &GLTexture);
            glBindTexture(GL_TEXTURE_2D, GLTexture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, LinearFiltering?GL_LINEAR:GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, LinearFiltering?GL_LINEAR:GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, BitsPerPixel, OptimizedImage->w, OptimizedImage->h, 0, ImageFormat, GL_UNSIGNED_BYTE, OptimizedImage->pixels);
            glBindTexture(GL_TEXTURE_2D, 0);

            this->Width = OptimizedImage->w;
            this->Height = OptimizedImage->h;
        }
    }

    int Texture::LoadFromText(std::string FontPath, int Size, std::string Text, int Styles) {
        int LoadFromText = 0;

        if (TTF_Init() != -1) {
            if (Size > 0 && Size < 10000) {
                TTF_Font *Font = TTF_OpenFont(FontPath.c_str(), Size);
                if (Font) {
                    TTF_SetFontStyle(Font, Styles);
                    SDL_Color Colour = {255, 255, 255};
                    SDL_Surface *Message = TTF_RenderText_Blended(Font, Text.c_str(), Colour);
                    LoadFromSurface(Message);

                    std::ostringstream Stream;
                    Stream << "[" << FontPath << ":" << Size << ":" << Styles << "]" << Text;
                    ID = Stream.str();
                    LoadFromText = 1;
                    if (Message) SDL_FreeSurface(Message);
                } else std::cerr << "(Graphics/Texture.cpp) LoadFromText(): Unable to load font '" << FontPath << "' to render: '" << Text << "', " << Size << ", " << Styles << "." << std::endl;

                TTF_CloseFont(Font);
                TTF_Quit();
            } else std::cerr << "(Graphics/Texture.cpp) LoadFromText(): Invalid font size " << Size << " rendering '" << Text << "'."<< std::endl;
        } else std::cerr << "(Graphics/Texture.cpp) LoadFromText(): Unable to initialize SDL_ttf." << std::endl;

        return LoadFromText;
    }

    Texture::Texture() {
        Next = Previous = NULL;
        Data = NULL;
        GLTexture = 0;
        Height = TileHeight = TileWidth = Uses = Width = 0;
    }

    void Texture::SetUses(int Uses) {
        this->Uses += Uses;
    }

    void Texture::Who() {
        std::cout << GLTexture << "\t" << Width << "x" << Height << "\t" << TileWidth << "x" << TileHeight << "\t" << Uses << "\t" << ID << std::endl;
    }

    Texture::~Texture() {
        Next = Previous = NULL;
        Data = NULL;
        if (GLTexture != 0) { glDeleteTextures(1, &GLTexture); GLTexture = 0; }
        Height = TileWidth = TileHeight = Uses = Width = 0;
    }
}
