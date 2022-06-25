//Texture.h
#ifndef NEOOBJECTS_TEXTURE_H
#define NEOOBJECTS_TEXTURE_H

#include <SDL2/SDL.h>
#include <gl/gl.h>
#include <string>
namespace sdlObjects {
    class Texture {
        //Members
        public:
            Texture *Next, *Previous;
        protected:
            GLubyte *Data;
            GLuint GLTexture;
            int Height, TileHeight, TileWidth, Uses, Width;
            std::string ID;
        //Access Methods
        public:
            Texture();
            ~Texture();
            int GetColumns();
            void GetData(GLubyte *Data);
            GLuint GetGLTexture();
            int GetHeight(bool State = false);
            std::string GetID();
            int GetRows();
            int GetTilesNumber();
            int GetUses();
            int GetWidth(bool State = false);
        //Process Methods
        public:
            int LoadFromImage(std::string Filename, int TileWidth = 0, int TileHeight = 0, bool LinearFiltering = false);
            void LoadFromScreen(std::string ID, int Width, int Height);
            void LoadFromSurface(SDL_Surface *Image, bool LinearFiltering = false);
            int LoadFromText(std::string FontPath, int Size, std::string Text, int Styles);
            void SetUses(int Uses);
            void Who();
    };
}

#endif
