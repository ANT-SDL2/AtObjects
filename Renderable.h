//Renderable.h
#ifndef NEOOBJECTS_RENDERABLE_H
#define NEOOBJECTS_RENDERABLE_H

#include "Texture.h"
#include "Animation.h"

namespace AtObjects {
    namespace Color {
        static const int Idle = 0;
        static const int Hovered = 1;
        static const int Disabled = 2;
    }

    class Renderable {
        //Members
        protected:
            float BonusDuration;
            Animation *Animation;
            AtObjects::Texture *Texture;
            int AnimationDuration, AnimationProgress, AnimationTile, AnimationTileProgress, AnimationQueue, Color[4], HoverColor[4], IdleColor[4], DisabledColor[4], Tile;
            std::string Animationset, TextureID;
            std::vector<int> EventQueue;
        //Access Methods
        public:
            Renderable();
            ~Renderable();
            bool AnimationIsDone();
            AtObjects::Animation *GetAnimation();
            int GetAnimationDuration();
            void GetAnimationProgress(int &AnimationProgress, int &AnimationTile, int &AnimationTileProgress, float &BonusDuration);
            std::string GetAnimationset();
            int GetAnimationQueue();
            int *GetColor(int ColorMode = -1);
            AtObjects::Texture *GetTexture();
            int GetTile();
            std::string GetTextureID();
            int HandleEvents();
        //Process Methods
        public:
            void ProcessAnimation();
            void PushAnimation(int Animation);
            void RenderAsArea(std::vector< std::vector<int> > &TileMap, float TargetX, float TargetY, float Width = 0, float Height = 0, AtObjects::Texture *Texture = NULL);
            void RenderAsDialog(float TargetX, float TargetY, int Width = 0, int Height = 0, AtObjects::Texture *Texture = NULL);
            void RenderAsTexture(float TargetX, float TargetY, int Width = 0, int Height = 0, float ClipWidth = 1.0f, float ClipHeight = 1.0f, float ClipX = 0.f, float ClipY = 0.f, AtObjects::Texture *Texture = NULL, bool OverrideColour = false);
            void ResetAnimation();
            void SetAnimation(AtObjects::Animation *Animation);
            void SetAnimationDuration(int AnimationDuration);
            void SetAnimationProgress(int AnimationProgress, int AnimationTile, int AnimationTileProgress, float BonusDuration);
            void SetAnimationset(std::string Animationset);
            void SetColor(int ColorMode);
            void SetHoverColor(int Color[4]);
            void SetIdleColor(int Color[4]);
            void SetTile(int Tile);
            void SetTexture(AtObjects::Texture *Texture);
            void SetTextureID(std::string TextureID);
            void SetTransparency(float Transparency);
        protected:
            void Render(AtObjects::Texture *Texture, float TargetX, float TargetY, float Width = 0, float Height = 0, int Tile = 0, float ClipWidth = 1.0f, float ClipHeight = 1.0f, float ClipX = 0.f, float ClipY = 0.f);
    };
}

#endif
