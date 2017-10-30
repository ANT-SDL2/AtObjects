//Physics.h
#ifndef NEOOBJECTS_PHYSICS_H
#define NEOOBJECTS_PHYSICS_H

#include "Object.h"
namespace AtObjects {
    float CalculateDrag(float Velocity, float DragCoefficient, float Area);
    bool DiskInDisk(float X1, float Y1, float R1, float r1, float Angle1, float Direction1, float X2, float Y2, int R2, float r2, float Angle2, float Direction2, float &AdjustX, float &AdjustY);
    bool DiskInRectangle(float X1, float Y1, float R1, float r1, float Angle1, float Direction1, float X2, float Y2, int Width, int Height, float Direction, float &AdjustX, float &AdjustY);
    float Clamp(float Minimum, float Value, float Maximum);
    bool DetectCollisionBetween(Object *Object1, Object *Object2, int &Axis, float &AdjustX, float &AdjustY, float Interpolation);
    bool PointInDisk(float X1, float Y1, float X2, float Y2, float R2, float r2, float Angle, float Direction);
    bool PointInRectangle(float X1, float Y1, float X2, float Y2, int Width2, int Height2, float Direction2);
    bool PointInTriangle(float X1, float Y1, float X, float Y, int Width, int Height, float Direction);
    bool RectangleInRectangle(float X1, float Y1, int Width1, int Height1, float Direction1, float X2, float Y2, int Width2, int Height2, float Direction2, float &AdjustX, float &AdjustY);
    void RotatePointBy(float &X1, float &Y1, float CenterX, float CenterY, float Angle);
}

#endif
