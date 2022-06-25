//Renderable.cpp
#include "Renderable.h"
#include "Object.h"
#include <iostream>
#include <math.h>

#include <sdlUtility/Renderer.h>

using namespace sdlUtility;

namespace sdlObjects {
    bool Renderable::AnimationIsDone() {
        if (Animation) return (AnimationTile >= Animation->GetTiles()-1 && AnimationProgress >= AnimationDuration); else return true;
    }

    sdlObjects::Animation *Renderable::GetAnimation() {
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

    sdlObjects::Texture *Renderable::GetTexture() {
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

    void Renderable::Render(sdlObjects::Texture *Texture, float TargetX, float TargetY, float TargetWidth, float TargetHeight, int TargetTile, float ClipWidth, float ClipHeight, float ClipX, float ClipY) {
        if (Texture) {
            int TextureColumns = Texture->GetColumns();
            int TextureRows = Texture->GetRows();
            int TextureTiles = Texture->GetTilesNumber();
            int TextureWidth = Texture->GetWidth(true);
            int TextureHeight = Texture->GetHeight(true);
            int TextureTileWidth = Texture->GetWidth();
            int TextureTileHeight = Texture->GetHeight();

            //Target dimensions
            if (!TargetWidth) TargetWidth = TextureTileWidth;
            if (!TargetHeight) TargetHeight = TextureTileHeight;

            //Calculate target column and row
            int TargetColumn = 1, TargetRow = 1;
            if (TargetTile > 0 && TargetTile <= TextureTiles) {
                TargetRow = TargetTile/TextureColumns;
                if (TargetTile%TextureColumns != 0) TargetRow+=1;
                TargetColumn = TargetTile-(TargetRow-1)*TextureColumns;
            }

            //Limit clipping
            if (ClipWidth > 1) ClipWidth = 1; else if (ClipWidth < 0) ClipWidth = 0;
            if (ClipHeight > 1) ClipHeight = 1; else if (ClipHeight < 0) ClipHeight = 0;
            if (ClipX <= 0) ClipX = 0; else if (ClipX > 1) ClipX = 1;
            if (ClipY <= 0) ClipY = 0; else if (ClipY > 1) ClipY = 1;

            //Quad position
            float X1 = 0, X2 = 0, X3 = 0, X4 = 0;
            float Y1 = 0, Y2 = 0, Y3 = 0, Y4 = 0;

            X1 = TargetX+ClipX*TargetWidth; Y1 = TargetY+ClipY*TargetHeight;
            X2 = TargetX+ClipWidth*TargetWidth; Y2 = TargetY+ClipY*TargetHeight;
            X3 = TargetX+ClipWidth*TargetWidth; Y3 = TargetY+ClipHeight*TargetHeight;
            X4 = TargetX+ClipX*TargetWidth; Y4 = TargetY+ClipHeight*TargetHeight;

            //Texture mapping
            float MappingWidth = TextureTileWidth*(ClipWidth), MappingHeight = TextureTileHeight*(ClipHeight);

            float TextureX1 = (float)(TargetColumn-1+ClipX)/TextureColumns, TextureY1 = (float)(TargetRow-1+ClipY)/TextureRows;
            float TextureX2 = (float)TargetColumn*MappingWidth/TextureWidth, TextureY2 = (float)(TargetRow-1+ClipY)/TextureRows;
            float TextureX3 = (float)TargetColumn*MappingWidth/TextureWidth, TextureY3 = (float)TargetRow*MappingHeight/TextureHeight;
            float TextureX4 = (float)(TargetColumn-1+ClipX)/TextureColumns, TextureY4 = (float)TargetRow*MappingHeight/TextureHeight;

            //Texture mapping fix
            //float Modifier = 0.25f;
            //TextureX1 += (float)Modifier/TextureWidth; TextureY1 += (float)Modifier/TextureHeight;
            //TextureX2 -= (float)Modifier/TextureWidth; TextureY2 += (float)Modifier/TextureHeight;
            //TextureX3 -= (float)Modifier/TextureWidth; TextureY3 -= (float)Modifier/TextureHeight;
            //TextureX4 += (float)Modifier/TextureWidth; TextureY4 -= (float)Modifier/TextureHeight;

            //if (FlipX) {Scale(GL_TEXTURE, -1, 1);}

            //Render quad
            //glLoadIdentity();
            glBegin(GL_QUADS);
                glTexCoord2f(TextureX1, TextureY1); glVertex2f(X1, Y1);
                glTexCoord2f(TextureX2, TextureY2); glVertex2f(X2, Y2);
                glTexCoord2f(TextureX3, TextureY3); glVertex2f(X3, Y3);
                glTexCoord2f(TextureX4, TextureY4); glVertex2f(X4, Y4);
            glEnd();
        }
    }

    Renderable::Renderable() {
        BonusDuration = 0.f;
        AnimationDuration = AnimationProgress = AnimationTile = AnimationTileProgress = AnimationQueue = Tile = 0;
        Color[0] = Color[1] = Color[2] = Color[3] = IdleColor[0] = IdleColor[1] = IdleColor[2] = IdleColor[3] = 255; DisabledColor[0] = DisabledColor[1] = DisabledColor[2] = DisabledColor[3] = 150;
        HoverColor[0] = HoverColor[1] = HoverColor[2] = HoverColor[3] = -1;
        Animation = NULL;
        Texture = NULL;
    }

    void Renderable::RenderAsArea(std::vector< std::vector<int> > &TileMap, float TargetX, float TargetY, float TargetWidth, float TargetHeight, sdlObjects::Texture *Texture) {
        if (!Texture) Texture = this->Texture;

        if (Texture) {
            float TileWidth = Texture->GetWidth();
            float TileHeight = Texture->GetHeight();
            int Columns = 0, Rows = 0;

            if (TileMap.size() > 0) {
                Columns = TileMap.size();
                Rows = TileMap[0].size();
            }

            if (!TargetWidth) TargetWidth = TileWidth;
            if (!TargetHeight) TargetHeight = TileHeight;

            GLuint GLTexture = Texture->GetGLTexture();
            glBindTexture(GL_TEXTURE_2D, GLTexture);

            for (int i = 0; i<Columns; i++) {
                for (int j = 0; j<Rows; j++) {
                    if (TileMap[i][j]) Render(Texture, TargetX + j*TargetWidth, TargetY + i*TargetHeight, TargetWidth, TargetHeight, TileMap[i][j]);
                }
            }

            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void Renderable::RenderAsDialog(float TargetX, float TargetY, int TargetWidth, int TargetHeight, sdlObjects::Texture *Texture) {
        if (!Texture) Texture = this->Texture;
        Renderer::SetColor(Color[0], Color[1], Color[2], Color[3]);

        if (Texture) {
            float TileWidth = Texture->GetWidth();
            float TileHeight = Texture->GetHeight();

            int Columns = TargetWidth/TileWidth-2;
            int Rows = TargetHeight/TileHeight-2;

            float WidthModulo = fmod(TargetWidth, TileWidth);
            float HeightModulo = fmod(TargetHeight, TileHeight);

            GLuint GLTexture = Texture->GetGLTexture();
            glBindTexture(GL_TEXTURE_2D, GLTexture);

            //Border corner (Top left)
            Render(Texture, TargetX, TargetY, TileWidth, TileHeight, 1);
            //Border corner (Bottom left)
            Render(Texture, TargetX, TargetY+TargetHeight-TileHeight, TileWidth, TileHeight, 7);

            //Border fill (Left)
            if (HeightModulo && Rows > -1) Render(Texture, TargetX, TargetY+(Rows+1) *TileHeight, TileWidth, HeightModulo, 4);

            //Border corner (Top right)
            Render(Texture, TargetX+TargetWidth-TileWidth, TargetY, TileWidth, TileHeight, 3);
            //Border corner (Bottom right)
            Render(Texture, TargetX+TargetWidth-TileWidth, TargetY+TargetHeight-TileHeight, TileWidth, TileHeight, 9);

            //Border fill (Right)
            if (HeightModulo && Rows > -1) Render(Texture, TargetX+TargetWidth-TileWidth, TargetY+TileHeight *(Rows+1), TileWidth, HeightModulo, 6);

            //Border fill (Top)
            if (WidthModulo && Columns > -1) Render(Texture, TargetX+TileWidth*(Columns+1), TargetY, WidthModulo, TileHeight, 2);
            //Border fill (Bottom)
            if (WidthModulo && Columns > -1) Render(Texture, TargetX+TileWidth*(Columns+1), TargetY+TargetHeight-TileHeight, WidthModulo, TileHeight, 8);

            for (int j = 0; j<Columns; j++) {
                //Border (Top)
                Render(Texture, TargetX+TileWidth *(j+1), TargetY, TileWidth, TileHeight, 2);
                //Border (Bottom)
                Render(Texture, TargetX+TileWidth *(j+1), TargetY+TargetHeight-TileHeight, TileWidth, TileHeight, 8);

                //Content fill (Bottom)
                if (HeightModulo && Rows > -1) Render(Texture, TargetX+TileWidth*(j+1), TargetY+(Rows+1)*TileHeight, TileWidth, HeightModulo, 5);
            }

            for (int i = 0; i<Rows; ++i) {
                //Border (Left)
                Render(Texture, TargetX, TargetY+TileHeight *(i+1), TileWidth, TileHeight, 4);
                //Border (Right)
                Render(Texture, TargetX+TargetWidth-TileWidth, TargetY+TileHeight*(i+1), TileWidth, TileHeight, 6);

                //Content
                for (int j = 0; j<Columns; j++) Render(Texture, TargetX+TileWidth*(j+1), TargetY+TileHeight*(i+1), TileWidth, TileHeight, 5);

                //Content fill (Right)
                if (WidthModulo && Columns > -1) Render(Texture, TargetX+TileWidth*(Columns+1), TargetY+TileHeight*(i+1), WidthModulo, TileHeight, 5);
            }

            //Content fill (Bottom right)
            if (WidthModulo && HeightModulo && Columns > -1 && Rows > -1) Render(Texture, TargetX+TileWidth*(Columns+1), TargetY+TileHeight*(Rows+1), WidthModulo, HeightModulo, 5);

            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void Renderable::RenderAsTexture(float TargetX, float TargetY, int TargetWidth, int TargetHeight, float ClipWidth, float ClipHeight, float ClipX, float ClipY, sdlObjects::Texture *Texture, bool OverrideColour) {
        if (!Texture) Texture = this->Texture;
        if (!OverrideColour) Renderer::SetColor(Color[0], Color[1], Color[2], Color[3]);

        if (Texture) {
            GLuint GLTexture = Texture->GetGLTexture();
            glBindTexture(GL_TEXTURE_2D, GLTexture);
            Render(Texture, TargetX, TargetY, TargetWidth, TargetHeight, Tile, ClipWidth, ClipHeight, ClipX, ClipY);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void Renderable::ResetAnimation() {
        AnimationProgress = 0;
        AnimationTile = 0;
        AnimationTileProgress = 0;
        if (Animation) Tile = Animation->GetTile(0); else Tile = 0;
    }

    void Renderable::SetAnimation(sdlObjects::Animation *Animation) {
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

    void Renderable::SetTexture(sdlObjects::Texture *Texture) {
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
