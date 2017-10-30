//Renderer.h
#ifndef NEOOBJECTS_RENDERER_H
#define NEOOBJECTS_RENDERER_H

#include "Texture.h"
#include <vector>
namespace AtObjects {
    namespace Renderer {
        void Clear();
        SDL_Cursor *CreateCursorFromData(Uint8 *Pixels, int Width, int Height);
        void GetStringSize(std::string FontPath, int Size, std::string Text, int &Width, int &Height);
        int GetViewportHeight();
        int GetViewportWidth();
        SDL_Surface *LoadImage(std::string Location);
        void Render(Texture *Texture, float TargetX, float TargetY, float Width = 0, float Height = 0, int Tile = 0, float ClipWidth = 1.0f, float ClipHeight = 1.0f, float ClipX = 0.f, float ClipY = 0.f);
        void RenderArea(Texture *Texture, std::vector< std::vector<int> > &TileMap, float TargetX, float TargetY, float Width = 0, float Height = 0);
        void RenderDialog(Texture *Texture, float TargetX, float TargetY, float Width = 0, float Height = 0);
        void RenderDisk(float TargetX, float TargetY, float OuterRadius, float InnerRadius, float Angle = 0, bool Filled = true);
        void RenderRectangle(float TargetX, float TargetY, float Width, float Height, bool Filled = true);
        void RenderTexture(Texture *Texture, float TargetX, float TargetY, float Width = 0, float Height = 0, int Tile = 0, float ClipWidth = 1.0f, float ClipHeight = 1.0f, float ClipX = 0.f, float ClipY = 0.f);
        void RenderTriangle(float TargetX, float TargetY, float Width, float Height);
        void Reset(GLenum Mode);
        void Rotate(GLenum Mode, float Angle, float CenterX, float CenterY);
        void Scale(GLenum Mode, float ScaleX, float ScaleY, float CenterX, float CenterY);
        bool SaveScreenshot(std::string Location);
        void SetClearColor(float R, float G, float B, float A = 0);
        void SetClearColor(int R, int G, int B, int A = 0);
        void SetColor(float R, float G, float B, float A = 1);
        void SetColor(int R, int G, int B, int A = 255);
        void Setup2D(int Width, int Height);
        void Setup3D(int Width, int Height);
        void SetViewport(int Width, int Height);
        void Who();
    }
}

#endif
