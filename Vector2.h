//Vector2.h
#ifndef NEOPHYSICS_VECTOR2_H
#define NEOPHYSICS_VECTOR2_H

namespace AtPhys {
    class Vector2 {
        //Members
        protected:
            float x, y;
        //Access Methods
        public:
            Vector2(float X = 0, float Y = 0);
            ~Vector2();
            void operator=(Vector2 Vector);
            void operator+=(Vector2 Vector);
            void operator-=(Vector2 Vector);
            bool operator==(Vector2 Vector);
            bool operator!();
            bool operator!=(Vector2 Vector);
            float Angle();
            float Length();
            float Length2();
            float X();
            float Y();
    };
}

#endif
