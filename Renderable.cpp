//Renderable.cpp
#include "Renderable.h"
#include "Renderer.h"
#include <iostream>
#include <math.h>

namespace AtObjects {
    bool Renderable::AnimationIsDone() {
        if (Animation) return (AnimationTile >= Animation->GetTiles()-1 && AnimationProgress >= AnimationDuration); else return true;
    }

    AtObjects::Animation *Renderable::GetAnimation() {
        return Animation;
    }

    int Renderable::GetAnimationDuration() {
        return AnimationDuration;
    }

    void Renderable::GetAnimationProgress(int &AnimationProgress, int &AnimationTile, int &AnimationTileProgress, float &BonusDuration) {
        AnimationProgress = this->AnimationProgress;
        AnimationTile = this->AnimationTile;
        AnimationTileProgress = this->AnimationTileProgress;
        BonusDuration = this->BonusDuration;
    }

    std::string Renderable::GetAnimationset() {
        return Animationset;
    }

    int Renderable::GetAnimationQueue() {
        return AnimationQueue;
    }

    int *Renderable::GetColor(int ColorMode) {
        int *Return = NULL;

        if (ColorMode < 0) {
            Return = Color;
        } else if (ColorMode == Color::Idle) {
            Return = IdleColor;
        } else if (ColorMode == Color::Hovered) {
            Return = HoverColor;
        } else if (ColorMode == Color::Disabled) {
            Return = DisabledColor;
        }

        return Return;
    }

    AtObjects::Texture *Renderable::GetTexture() {
        return Texture;
    }

    int Renderable::GetTile() {
        return Tile;
    }

    std::string Renderable::GetTextureID() {
        return TextureID;
    }

    int Renderable::HandleEvents() {
        int Event = Events::Idle;

        if (EventQueue.size() > 0) {
            Event = EventQueue[0];
            EventQueue.erase(EventQueue.begin());
        }

        return Event;
    }

    void Renderable::ProcessAnimation() {
        if (Animation) {
            float TileDuration = Animation->GetWeight(AnimationTile)*AnimationDuration;
            float RoundedDuration = roundf(TileDuration);

            //if (Animationset == "MaleModel" && Animation->GetState() == 4) std::cout << AnimationTile << " " << Animation->GetTile(AnimationTile) << " " << Animation->GetWeight(AnimationTile) << std::endl;

            //Add extra frame
            if (AnimationTileProgress == 0 && TileDuration != RoundedDuration) BonusDuration += (RoundedDuration-TileDuration);

            if (fabs(BonusDuration) > 0.99f) {
                if (BonusDuration < 0) {
                    if (AnimationTileProgress >= RoundedDuration-1) {
                        RoundedDuration += 1;
                        BonusDuration += 1;
                    }
                } else {
                    if (TileDuration > 1 && AnimationTileProgress == RoundedDuration-2) {
                        RoundedDuration -= 1;
                        BonusDuration -= 1;
                    }
                }
            }

            AnimationProgress += 1;
            AnimationTileProgress += 1;

            //if (Animationset == "MaleModel" && Animation->GetState() == 4) std::cout << Animation->GetState() << ": " << AnimationProgress << "/" << AnimationDuration << " | " << AnimationTile+1 << "/" << Animation->GetTiles() << " " << Animation->GetWeight(AnimationTile) << " " << TileDuration << " " << AnimationTileProgress << "/" << RoundedDuration << " " << BonusDuration << std::endl;

            //Reset Tile
            if (AnimationTileProgress >= RoundedDuration) {
                if (AnimationTile+1 < Animation->GetTiles()) { //Conditional to prevent moving out of bounds on extra animation duration
                    AnimationTile += 1;
                    AnimationTileProgress = 0;
                }
            }

            Tile = Animation->GetTile(AnimationTile);
        }
    }

    void Renderable::PushAnimation(int Animation) {
        AnimationQueue = Animation;
    }

    Renderable::Renderable() {
        BonusDuration = 0.f;
        AnimationDuration = AnimationProgress = AnimationTile = AnimationTileProgress = AnimationQueue = Tile = 0;
        Color[0] = Color[1] = Color[2] = Color[3] = IdleColor[0] = IdleColor[1] = IdleColor[2] = IdleColor[3] = 255; DisabledColor[0] = DisabledColor[1] = DisabledColor[2] = DisabledColor[3] = 150;
        HoverColor[0] = HoverColor[1] = HoverColor[2] = HoverColor[3] = -1;
        Animation = NULL;
        Texture = NULL;
    }

    void Renderable::RenderAsDialog(float TargetX, float TargetY, int Width, int Height) {
        AtObjects::Renderer::SetColor(Color[0], Color[1], Color[2], Color[3]);
        AtObjects::Renderer::RenderDialog(Texture, TargetX, TargetY, Width, Height);
    }

    void Renderable::RenderAsTexture(float TargetX, float TargetY, int Width, int Height, float ClipWidth, float ClipHeight, float ClipX, float ClipY) {
        AtObjects::Renderer::SetColor(Color[0], Color[1], Color[2], Color[3]);
        AtObjects::Renderer::RenderTexture(Texture, TargetX, TargetY, Width, Height, Tile, ClipWidth, ClipHeight, ClipX, ClipY);
    }

    void Renderable::ResetAnimation() {
        AnimationProgress = 0;
        AnimationTile = 0;
        AnimationTileProgress = 0;
        if (Animation) Tile = Animation->GetTile(0); else Tile = 0;
    }

    void Renderable::SetAnimation(AtObjects::Animation *Animation) {
        if (Animation && Animation->GetState() == AnimationQueue) {
            //if (Animationset == "" && Animation->GetState() != 1) std::cout << Animationset << " set to " << Animation->GetState() << std::endl;

            this->Animation = Animation;
            AnimationDuration = Animation->GetDuration(); //Backup Animation Duration

            //Reset Queue
            AnimationQueue = 0;

            //Reset Animation
            AnimationProgress = 0;
            AnimationTile = 0;
            AnimationTileProgress = 0;

            BonusDuration = 0.f;

            EventQueue.push_back(Events::AnimationChange);
        } else {
            this->Animation = NULL;

            //Reset Queue
            AnimationQueue = 0;

            //Reset Animation
            AnimationProgress = 0;
            AnimationTile = 0;
            AnimationTileProgress = 0;

            BonusDuration = 0.f;
        }
    }

    void Renderable::SetAnimationDuration(int AnimationDuration) {
        this->AnimationDuration = AnimationDuration;
    }

    void Renderable::SetAnimationProgress(int AnimationProgress, int AnimationTile, int AnimationTileProgress, float BonusDuration) {
        this->AnimationProgress = AnimationProgress;
        this->AnimationTile = AnimationTile;
        this->AnimationTileProgress = AnimationTileProgress;
        this->BonusDuration = BonusDuration;
    }

    void Renderable::SetAnimationset(std::string Animationset) {
        this->Animationset = Animationset;
    }

    void Renderable::SetColor(int ColorMode) {
        if (ColorMode == Color::Idle) {
            for (int i = 0; i<4; i++) Color[i] = IdleColor[i];
        } else if (ColorMode == Color::Hovered) {
            for (int i = 0; i<4; i++) if (HoverColor[i] > -1) Color[i] = HoverColor[i];
        } else if (ColorMode == Color::Disabled) {
            for (int i = 0; i<4; i++) if (DisabledColor[i] > -1) Color[i] = DisabledColor[i];
        }
    }

    void Renderable::SetHoverColor(int Color[4]) {
        for (int i = 0; i<4; i++) HoverColor[i] = Color[i];
    }

    void Renderable::SetIdleColor(int Color[4]) {
        for (int i = 0; i<4; i++) if (Color[i] > -1) IdleColor[i] = this->Color[i] = Color[i];
    }

    void Renderable::SetTile(int Tile) {
        this->Tile = Tile;
    }

    void Renderable::SetTexture(AtObjects::Texture *Texture) {
        if (this->Texture) this->Texture->SetUses(-1);

        this->Texture = Texture;

        if (Texture) Texture->SetUses(1);
        if (Texture) EventQueue.push_back(Events::TextureChange);
    }

    void Renderable::SetTextureID(std::string TextureID) {
        this->TextureID = TextureID;
    }

    void Renderable::SetTransparency(float Transparency) {
        Color[3] = Transparency*255.0f;
        HoverColor[3] = Transparency*255.0f;
    }

    Renderable::~Renderable() {
        BonusDuration = 0.f;
        AnimationDuration = AnimationProgress = AnimationTile = AnimationTileProgress = AnimationQueue = Tile = 0;
        Color[0] = Color[1] = Color[2] = Color[3] = IdleColor[0] = IdleColor[1] = IdleColor[2] = IdleColor[3] = 255;
        HoverColor[0] = HoverColor[1] = HoverColor[2] = HoverColor[3] = -1;
        Animation = NULL;
        Texture = NULL;
    }
}
