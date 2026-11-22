//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// OGO Structs - borrowed from OhmFlux
// common Structs like in every game object
// i port them over from OhmFlux and BaseElf
//
//-----------------------------------------------------------------------------
#pragma once

#include <string>
#include <format>
#include <cmath>
#include "Types.h"

namespace OGO {

    struct  Point2I{
        S32 x = 0, y = 0;
        // --- Scalar Operators ---
        Point2I operator-(S32 v) const { return {x - v, y - v}; }
        Point2I operator+(S32 v) const { return {x + v, y + v}; }
        Point2I operator*(S32 v) const { return {x * v, y * v}; }
        Point2I operator/(S32 v) const { return {x / v, y / v}; }

        // --- Point Operators ---
        Point2I operator-(const Point2I& v) const { return {x - v.x, y - v.y}; }
        Point2I operator+(const Point2I& v) const { return {x + v.x, y + v.y}; }
        Point2I operator*(const Point2I& v) const { return {x * v.x, y * v.y}; }
        Point2I operator/(const Point2I& v) const { return {x / v.x, y / v.y}; }

        bool operator==(const Point2I& v) const { return (x == v.x && y == v.y); }

        // --- Compound Assignment Operators ---
        Point2I& operator+=(S32 v) { x += v; y += v; return *this; }
        Point2I& operator-=(S32 v) { x -= v; y -= v; return *this; }
        Point2I& operator*=(S32 v) { x *= v; y *= v; return *this; }
        Point2I& operator/=(S32 v) { x /= v; y /= v; return *this; }

        Point2I& operator+=(const Point2I& v) { x += v.x; y += v.y; return *this; }
        Point2I& operator-=(const Point2I& v) { x -= v.x; y -= v.y; return *this; }
        Point2I& operator*=(const Point2I& v) { x *= v.x; y *= v.y; return *this; }
        Point2I& operator/=(const Point2I& v) { x /= v.x; y /= v.y; return *this; }

        bool isZero() const { return (x == 0 && y == 0); }
    };

    struct Point2F {
        F32 x = 0.f, y = 0.f;

        // --- Scalar Operators ---
        Point2F operator-(F32 v) const { return {x - v, y - v}; }
        Point2F operator+(F32 v) const { return {x + v, y + v}; }
        Point2F operator*(F32 v) const { return {x * v, y * v}; }
        Point2F operator/(F32 v) const {
            if (v==0.f) v =  1e-9f;
            return {x / v, y / v};
        }

        // --- Point Operators ---
        Point2F operator-(const Point2F& v) const { return {x - v.x, y - v.y}; }
        Point2F operator+(const Point2F& v) const { return {x + v.x, y + v.y}; }
        Point2F operator*(const Point2F& v) const { return {x * v.x, y * v.y}; }
        Point2F operator/(const Point2F& v) const {
            return {
                x / v.x != 0.f ? v.x : 1e-9f,
                y / v.y != 0.f ? v.y : 1e-9f,
            };

        }

        bool operator==(const Point2F& v) const { return (x == v.x && y == v.y); }

        // --- Compound Assignment Operators ---
        Point2F& operator+=(F32 v) { x += v; y += v; return *this; }
        Point2F& operator-=(F32 v) { x -= v; y -= v; return *this; }
        Point2F& operator*=(F32 v) { x *= v; y *= v; return *this; }
        Point2F& operator/=(F32 v) {
            if (v==0.f) v =  1e-9f;
            x /= v;
            y /= v;
            return *this;
        }

        Point2F& operator+=(const Point2F& v) { x += v.x; y += v.y; return *this; }
        Point2F& operator-=(const Point2F& v) { x -= v.x; y -= v.y; return *this; }
        Point2F& operator*=(const Point2F& v) { x *= v.x; y *= v.y; return *this; }
        Point2F& operator/=(const Point2F& v) {
            x /= v.x != 0.f ? v.x : 1e-9f;
            y /= v.y != 0.f ? v.y : 1e-9f;
            return *this;
        }

        // --- Conversion ---
        explicit operator Point2I() const {
            return { static_cast<S32>(x), static_cast<S32>(y) };
        }

        // --- Distance and Length ---
        F32 distSq(const Point2F& v) const {
            F32 dx = x - v.x;
            F32 dy = y - v.y;
            return (dx * dx) + (dy * dy);
        }

        F32 dist(const Point2F& v) const { return std::sqrt(distSq(v)); }
        F32 distance(const Point2F& v) const { return dist(v); }

        F32 lenSquared() const { return (x * x + y * y); }

        F32 len() const { return std::sqrt(lenSquared()); }
        bool isZero() const { return (x == 0.f && y == 0.f); }

        // --- Vector Operations ---
        void normalize() {
            F32 l = len();
            if (l > 0.0f) { x /= l; y /= l; }
        }

        Point2F normalized() const {
            F32 l = len();
            if (l > 0.0f) return { x / l, y / l };
            return { 0.0f, 0.0f };
        }

        F32 dot(const Point2F& v) const { return (x * v.x + y * v.y); }

        // 2D "Cross Product" (Scalar Z-component)
        F32 cross(const Point2F& v) const { return (x * v.y - y * v.x); }

        Point2I toPoint2I() const { return { static_cast<S32>(x), static_cast<S32>(y)  };}

        std::string to_string() { return std::format("{} {}", x,y);}

    }; //Point2F


    struct Point3F {
        F32 x = 0.f, y=0.f, z=0.f;

        F32 distSq(const Point3F& other) const {
            F32 dx = x - other.x;
            F32 dy = y - other.y;
            F32 dz = z - other.z;
            return dx*dx + dy*dy + dz*dz;
        }

        F32 dist(const Point3F& v) const { return std::sqrt(distSq(v)); }
        F32 distance(const Point3F& v) const { return dist(v); }


        F32 lenSquared() const {
            return (x * x + y * y + z * z);
        }
        F32 len() const {
            return sqrt(x*x + y*y + z*z);
        }

        Point3F operator+(const Point3F& v) const { return {x + v.x, y + v.y, z + v.z}; }
        Point3F operator-(const Point3F& v) const { return {x - v.x, y - v.y, z - v.z}; }
        Point3F operator*(const Point3F& v) const { return {x * v.x, y * v.y, z * v.z}; }
        Point3F operator/(const Point3F& v) const {
            return {
                x / v.x != 0.f ? v.x : 1e-9f,
                y / v.y != 0.f ? v.y : 1e-9f,
                z / v.z != 0.f ? v.z : 1e-9f
            };
        }

        bool operator==(const Point3F& v) const { return (x == v.x && y == v.y && z == v.z); }

        Point3F& operator+=(const Point2F& v) { x += v.x; y += v.y; return *this; }

        F32 dot(const Point3F& v) const { return (x * v.x + y * v.y + z * v.z); }

        Point2F toPoint2F() const { return { x,y}; }

        std::string to_string() { return std::format("{} {} {}", x,y,z);}

        bool isZero() const { return (x == 0.f && y == 0.f && z == 0.f); }

        void normalize() {
            F32 l = len();
            if (l > 0.0f) { x /= l; y /= l; z /= l;}
        }
    };
    //------------------------------------- Rects

    struct RectI{
        S32 x= 0, y=0, w=0, h=0;
        Point2I getPoint() const { return {x,y}; }
        Point2I getExtent() const { return { w, h };}

        void setPoint(const Point2I& value) {x = value.x; y = value.y; }
        void setExtent(const Point2I& value)  { w = value.x; h = value.y; }
        void setPoint(const Point2F& value) {x = (S32)value.x; y = (S32)value.y; }
        void setExtent(const Point2F& value)  { w = (S32)value.x; h = (S32)value.y; }


        bool pointInRect(const Point2I& pt) const {
            return (pt.x >= x && pt.x < x + w && pt.y >= y && pt.y < y + h);
        }
        S32 len_x() const { return w; }
        S32 len_y() const { return h; }
        bool isValidRect() const { return (w > 0 && h > 0); }

        bool contains(const RectI& other) const {
            return (other.x >= x &&
            other.y >= y &&
            other.x + other.w <= x + w &&
            other.y + other.h <= y + h);
        }

        bool intersects(const RectI& other) const {
            return (x < other.x + other.w &&
            x + w > other.x &&
            y < other.y + other.h &&
            y + h > other.y);
        }
        std::string to_string() { return std::format("{} {} {} {}", x,y,w,h);}
    };

    struct RectF
    {
        F32 x = 0.f, y=0.f, w=0.f, h=0.f;

        Point2F getPoint() const { return {x,y}; }
        Point2F getExtent() const { return { w, h };}

        Point2F getCenterPoint() const {
            return  { x + w / 2.f , y + h / 2.f };
        }


        void setPoint(const Point2I& value) {x = (F32)value.x; y = (F32)value.y; }
        void setExtent(const Point2I& value)  { w = (F32)value.x; h = (F32)value.y; }
        void setPoint(const Point2F& value) {x = value.x; y = value.y; }
        void setExtent(const Point2F& value)  { w = value.x; h = value.y; }



        bool pointInRect(const Point2F& pt) const {
            return (pt.x >= x && pt.x < x + w && pt.y >= y && pt.y < y + h);
        }
        F32 len_x() const { return w; }
        F32 len_y() const { return h; }
        bool isValidRect() const { return (w > 0 && h > 0); }
        RectI asRectI() const { return { static_cast<S32>(x), static_cast<S32>(y),static_cast<S32>(w), static_cast<S32>(h)};}

        void inflate(F32 amountX, F32 amountY) {
            x -= amountX;      // Move left
            y -= amountY;      // Move up
            w += amountX * 2;  // Expand width both ways
            h += amountY * 2;  // Expand height both ways
        }

        bool contains(const RectF& other) const {
            return (other.x >= x &&
            other.y >= y &&
            other.x + other.w <= x + w &&
            other.y + other.h <= y + h);
        }

        bool intersects(const RectF& other) const {
            return (x < other.x + other.w &&
            x + w > other.x &&
            y < other.y + other.h &&
            y + h > other.y);
        }

        std::string to_string() { return std::format("{} {} {} {}", x,y,w,h);}
    };



    // ------------------------------------------
    // Color4F
    // ------------------------------------------
    struct Color4F {
        F32 r = 1.f;
        F32 g = 1.f;
        F32 b = 1.f;
        F32 a = 1.f;


        // --- Arithmetic Operators ---
        // Add a scalar to all color channels
        Color4F operator+(F32 v) const { return { r + v, g + v, b + v, a + v }; }

        // Subtract a scalar from all color channels
        Color4F operator-(F32 v) const { return { r - v, g - v, b - v, a - v }; }

        // Multiply all color channels by a scalar (common for alpha fading)
        Color4F operator*(F32 v) const { return { r * v, g * v, b * v, a * v }; }

        // Divide all color channels by a scalar
        Color4F operator/(F32 v) const { return { r / v, g / v, b / v, a / v }; }

        // --- Compound Assignment Operators ---
        Color4F& operator+=(F32 v) { r += v; g += v; b += v; a += v; return *this; }
        Color4F& operator-=(F32 v) { r -= v; g -= v; b -= v; a -= v; return *this; }
        Color4F& operator*=(F32 v) { r *= v; g *= v; b *= v; a *= v; return *this; }
        Color4F& operator/=(F32 v) { r /= v; g /= v; b /= v; a /= v; return *this; }

        // --- Color-to-Color Operators (Optional but recommended for tinting) ---

        Color4F operator*(const Color4F& v) const { return { r * v.r, g * v.g, b * v.b, a * v.a }; }
        Color4F& operator*=(const Color4F& v) { r *= v.r; g *= v.g; b *= v.b; a *= v.a; return *this; }

        // Binary addition (Color + Color)
        Color4F operator+(const Color4F& v) const { return { r + v.r, g + v.g, b + v.b, a + v.a }; }


        bool operator==(const Color4F& other) const {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }

        void normalize() {
            if (r > 1.f) r /= 255.f;
            if (g > 1.f) g /= 255.f;
            if (b > 1.f) b /= 255.f;
            if (a > 1.f) a /= 255.f;
        }

        // Helper to create a color from a Hex integer (e.g., 0xFF5733)
        static Color4F FromHex(U32 hex) {
            Color4F c;
            // If the hex is > 0xFFFFFF, we assume it has Alpha (0xRRGGBBAA)
            if (hex > 0xFFFFFF) {
                c.r = ((hex >> 24) & 0xFF) / 255.0f;
                c.g = ((hex >> 16) & 0xFF) / 255.0f;
                c.b = ((hex >> 8)  & 0xFF) / 255.0f;
                c.a = (hex & 0xFF) / 255.0f;
            } else {
                // Otherwise assume standard RGB (0xRRGGBB)
                c.r = ((hex >> 16) & 0xFF) / 255.0f;
                c.g = ((hex >> 8)  & 0xFF) / 255.0f;
                c.b = (hex & 0xFF) / 255.0f;
                c.a = 1.0f;
            }
            return c;
        }

        // Converts the current float color back to a 32-bit Hex integer (0xRRGGBBAA)
        U32 toHex() const {
            U32 rr = static_cast<U32>(r * 255.0f + 0.5f);
            U32 gg = static_cast<U32>(g * 255.0f + 0.5f);
            U32 bb = static_cast<U32>(b * 255.0f + 0.5f);
            U32 aa = static_cast<U32>(a * 255.0f + 0.5f);

            // Bit-shift into the 0xRRGGBBAA pattern
            return (rr << 24) | (gg << 16) | (bb << 8) | aa;
        }
        std::string to_string() { return std::format("{} {} {} {}", r,g,b,a);}
    };

    //------------------------------------- Default Colors
    constexpr Color4F cl_NONE        = { -1.0f, -1.0f, -1.0f, -1.0f };
    constexpr Color4F cl_White        = { 1.0f, 1.0f, 1.0f, 1.0f };
    constexpr Color4F cl_Black        = { 0.0f, 0.0f, 0.0f, 1.0f };
    constexpr Color4F cl_Red          = { 1.0f, 0.0f, 0.0f, 1.0f };
    constexpr Color4F cl_Green        = { 0.0f, 1.0f, 0.0f, 1.0f };
    constexpr Color4F cl_Blue         = { 0.0f, 0.0f, 1.0f, 1.0f };
    constexpr Color4F cl_Yellow       = { 1.0f, 1.0f, 0.0f, 1.0f };
    constexpr Color4F cl_Cyan         = { 0.0f, 1.0f, 1.0f, 1.0f };
    constexpr Color4F cl_Magenta      = { 1.0f, 0.0f, 1.0f, 1.0f };
    constexpr Color4F cl_Gray         = { 0.5f, 0.5f, 0.5f, 1.0f };
    constexpr Color4F cl_LightGray    = { 0.75f, 0.75f, 0.75f, 1.0f };
    constexpr Color4F cl_DarkGray     = { 0.25f, 0.25f, 0.25f, 1.0f };
    constexpr Color4F cl_Orange       = { 1.0f, 0.5f, 0.0f, 1.0f };
    constexpr Color4F cl_Purple       = { 0.5f, 0.0f, 0.5f, 1.0f };
    constexpr Color4F cl_Brown        = { 0.6f, 0.3f, 0.0f, 1.0f };
    constexpr Color4F cl_Lime         = { 0.75f, 1.0f, 0.0f, 1.0f };
    constexpr Color4F cl_Pink         = { 1.0f, 0.4f, 0.7f, 1.0f };

    //------------------------------------- Some more Colors
    constexpr Color4F cl_Crimson      = { 0.86f, 0.08f, 0.24f, 1.0f }; // Modern Alert/Red
    constexpr Color4F cl_Emerald      = { 0.16f, 0.71f, 0.44f, 1.0f }; // Pleasant Success/Green
    constexpr Color4F cl_SkyBlue      = { 0.53f, 0.81f, 0.98f, 1.0f }; // UI Accent
    constexpr Color4F cl_Slate        = { 0.18f, 0.24f, 0.31f, 1.0f }; // Modern Dark Background
    constexpr Color4F cl_Gold         = { 1.00f, 0.84f, 0.00f, 1.0f }; // Pickups/Rare items
    constexpr Color4F cl_Transparent   = { 0.00f, 0.00f, 0.00f, 0.00f }; // Invisible/Default

    constexpr Color4F cl_Aquamarine   = { 0.50f, 1.00f, 0.83f, 1.0f }; // Tropical Water
    constexpr Color4F cl_Coral        = { 1.00f, 0.50f, 0.31f, 1.0f }; // Warm Reef color
    constexpr Color4F cl_DeepSea      = { 0.00f, 0.08f, 0.20f, 1.0f }; // Background Depths
    constexpr Color4F cl_Seafoam      = { 0.60f, 0.85f, 0.75f, 1.0f }; // Bubbles/Foam
    constexpr Color4F cl_Sand         = { 0.76f, 0.70f, 0.50f, 1.0f }; // Floor/Dirt
    constexpr Color4F cl_Kelp         = { 0.13f, 0.29f, 0.13f, 1.0f }; // Dark Underwater Plant

    constexpr Color4F cl_NeonPink     = { 1.00f, 0.00f, 0.50f, 1.0f }; // Physics Hitboxes
    constexpr Color4F cl_ElectricBlue = { 0.00f, 1.00f, 1.00f, 1.0f }; // Pathfinding nodes
    constexpr Color4F cl_AcidGreen    = { 0.50f, 1.00f, 0.00f, 1.0f }; // Collision triggers

    constexpr Color4F cl_Glass        = { 1.00f, 1.00f, 1.00f, 0.25f }; // Semi-transparent White
    constexpr Color4F cl_Shadow       = { 0.00f, 0.00f, 0.00f, 0.40f }; // Shadow Overlay
    constexpr Color4F cl_Ghost        = { 0.70f, 0.70f, 1.00f, 0.50f }; // Semi-transparent Blue

    // ------------------------------------------
    // Color
    // ------------------------------------------
    // from BaseElf using functions
    // so it's a inconsitent mix of functions
    // and methods.
    // i like functions more since i can reuse
    // them outside the struct
    // ------------------------------------------
    struct Color {
        U8 r = 0;
        U8 g = 0;
        U8 b = 0;
        U8 a = 0;
    };

    // -------------- Color --------------------
    inline bool operator==(const Color& a, const Color& b) {
        return (
            a.r == b.r &&
            a.g == b.g &&
            a.b == b.b &&
            a.a == b.a
        );
    }
    inline bool operator != (const Color& a, const Color& b) {
        return (
            a.r != b.r ||
            a.g != b.g ||
            a.b != b.b ||
            a.a != b.a
        );
    }
    inline void setValueScale(Color& color, F32 scale){
        scale = std::clamp(scale, 0.0f, 1.0f);

        F32 gray = 1.0f; //(color.r * 0.299f) + (color.g * 0.587f) + (color.b * 0.114f);

        color.r = static_cast<U8>(gray + (scale * (color.r - gray)));
        color.g = static_cast<U8>(gray + (scale * (color.g - gray)));
        color.b = static_cast<U8>(gray + (scale * (color.b - gray)));
    }

    inline void setScale(Color& color, F32 scale){
        scale = std::clamp(scale, 0.0f, 3.0f); //let overscale 3 times

        color.r = static_cast<U8>(std::clamp((int)(color.r * scale), 0, 255));
        color.g = static_cast<U8>(std::clamp((int)(color.g * scale), 0, 255));
        color.b = static_cast<U8>(std::clamp((int)(color.b * scale), 0, 255));

    }


    inline void setSaturationScale(Color& color, F32 scale){
        scale = std::clamp(scale, 0.0f, 1.0f);
        F32 maxChan = static_cast<F32>(std::max({color.r, color.g, color.b}));
        F32 minChan = static_cast<F32>(std::min({color.r, color.g, color.b}));
        if (maxChan == minChan || maxChan == 0.0f) {
            return;
        }
        F32 newMinChan = maxChan * (1.0f - scale);
        F32 rangeOld = maxChan - minChan;
        F32 rangeNew = maxChan - newMinChan;
        color.r = static_cast<U8>(maxChan - ((maxChan - color.r) * rangeNew / rangeOld) + 0.5f);
        color.g = static_cast<U8>(maxChan - ((maxChan - color.g) * rangeNew / rangeOld) + 0.5f);
        color.b = static_cast<U8>(maxChan - ((maxChan - color.b) * rangeNew / rangeOld) + 0.5f);
    }


    inline Color fromHSV(S32 hue, F32 saturation, F32 value, U8 alpha = 255) {
        hue        = std::clamp(hue, 0, 359);
        saturation = std::clamp(saturation, 0.f, 1.f);
        value      = std::clamp(value, 0.f, 1.f);

        Color color;
        color.a = alpha;

        // grayscale
        if (saturation == 0.f) {
            U8 gray = static_cast<U8>(value * 255.f + 0.5f);
            color.r = gray;
            color.g = gray;
            color.b = gray;
            return color;
        }

        S32 sector = hue / 60;
        F32 remainder = (hue % 60) / 60.f;

        F32 v_float = value * 255.f;
        F32 p_float = v_float * (1.f - saturation);
        F32 q_float = v_float * (1.f - (saturation * remainder));
        F32 t_float = v_float * (1.f - (saturation * (1.f - remainder)));

        U8 v = static_cast<U8>(v_float + 0.5f);
        U8 p = static_cast<U8>(p_float + 0.5f);
        U8 q = static_cast<U8>(q_float + 0.5f);
        U8 t = static_cast<U8>(t_float + 0.5f);

        switch (sector) {
            case 0:  color.r = v; color.g = t; color.b = p; break;
            case 1:  color.r = q; color.g = v; color.b = p; break;
            case 2:  color.r = p; color.g = v; color.b = t; break;
            case 3:  color.r = p; color.g = q; color.b = v; break;
            case 4:  color.r = t; color.g = p; color.b = v; break;
            default: color.r = v; color.g = p; color.b = q; break;
        }

        return color;
    }



    //------------------------------------- Vertex2D for batch rendering
    struct Vertex2D {
        Point3F pos;      // x, y, z
        Point2F uv;       // Final calculated UV (after offset/size/flip)
        Color4F color;    // uTint and uAlphaThreshold combined
    };


    //------------------------------------- BoundingBox partial from raylib
    struct BoundingBox {
       Point3F min;            // Minimum vertex box-corner
       Point3F max;            // Maximum vertex box-corner
    } ;


}
