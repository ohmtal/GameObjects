//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// NOTE: Example only how you can override draw and use SceneObject2D
// full example see project : ElfFlux
//-----------------------------------------------------------------------------
#include "SceneObject2D.h"

#include "common/Types.h"


namespace OGO {


class SpriteObject : public SceneObject2D {
public:

    SpriteObject() {
         mCollisionType = CollisionType::Kinematic; // default Kinematic
    }

    S32 mTextureID = 0;
    RectF mTextureRect = {0.f,0.f,0.f,0.f};
    F32 mRotation = 0.f;
    Point2F mRorationOrgin = {0.f, 0.f};
    // F32 mSpeed = 0.f;
    // Point2F mForwardVector = {0.f, 0.f};
    bool mFlipX = false;
    bool mFlipY = false;

    // Velocty


    //TODO :EXAMPLE: virtual void draw(const F32& dt) override;
    static void initPersistFields();

    // void doBounce(F32 x , F32 y, F32 width, F32 height);
    // static bool _setForwardVector( void *object, const char *index, const char *data );

    void setForwardVectorByRotation(F32 rotation, F32 speed = 1.f);

    void setForwardVector(Point2F vector, F32 speed = 1.f);
    void setForwardVectorByAngle(F32 rot, F32 speed = 1.f);
    void setRotationAndForwardVector(F32 rot);

};
//-----------------------------------------------------------------------------
// // //-----------------------------------------------------------------------------
// // void SpriteObject::draw(const F32& dt) {
// //
// //     // if (mSpeed != 0.f) {
// //     //     F32 speed = mSpeed *  dt;
// //     //     mPosition.x += mForwardVector.x * speed;
// //     //     mPosition.y += mForwardVector.y * speed;
// //     //     refreshWorldBox();
// //     // }
// //
// //     if (!mVisible) return;
// //
// //
// //     // RLAPI void DrawTexturePro(Texture2D texture, RectF source, RectF dest, Point2F origin, float rotation, Color tint);
// //
// //     Texture* tex = ElfResource::TextureMap.get(mTextureID);
// //     if (!tex) return;
// //
// //     RectF srcRect;
// //     if (mTextureRect.w <= 0.f || mTextureRect.h <= 0.f) {
// //         srcRect = {0.f, 0.f, (F32)tex->width, (F32)tex->height};
// //     } else {
// //         srcRect = mTextureRect;
// //     }
// //
// //
// //     if (mFlipX) srcRect.width = -srcRect.width;
// //     if (mFlipY) srcRect.height = -srcRect.height;
// //
// //     ::DrawTexturePro(*tex, srcRect, mWorldBox, mRorationOrgin, mRotation, mColor);
// //
// //     Parent::draw(dt);
// // }
//-----------------------------------------------------------------------------
void SpriteObject::setForwardVectorByRotation(F32 rotation, F32 speed) {
    F32 radians = rotation * M_DEG2RAD;
    // mForwardVector.x = std::cos(radians);
    // mForwardVector.y = std::sin(radians);

    mVelo.x = std::cos(radians) * speed;
    mVelo.y = std::sin(radians) * speed;
}

// -----------------------------------------------------------------------------

void SpriteObject::setForwardVector(Point2F vector, F32 speed)
{
    vector = vector.normalized() * speed;
    mVelo = vector;
}

// -----------------------------------------------------------------------------
void SpriteObject::setForwardVectorByAngle(F32 rot, F32 speed)
{
    setForwardVectorByRotation(rot, speed);
}
// -----------------------------------------------------------------------------
void SpriteObject::setRotationAndForwardVector(F32 rot)
{
    mRotation = rot;
    setForwardVectorByRotation(rot);
}
// // -----------------------------------------------------------------------------
// DefineEngineMethod(SpriteObject, doBounce, void, (F32 x , F32 y, F32 width, F32 height)
//     , ,"usefull for bouncing on borders")
// {
//     object->doBounce(x,y,width,height);
// }
// // -----------------------------------------------------------------------------

} //namespace
