//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
#include "SceneObject2D.h"
#include "SceneContainer2D.h"

#include <algorithm>
//-----------------------------------------------------------------------------
namespace OGO {


#define MIN_MASS 0.0001f
//-----------------------------------------------------------------------------
SceneObject2D::SceneObject2D() {
}

//-----------------------------------------------------------------------------
void SceneObject2D::setLayer(const F32 z) {
    if (z == this->mPosition.z) return;

    // layer should be in 0.f .. 1.f
    F32 finalZ = std::clamp(z, 0.f, 1.f);

    // MAX_2D_LAYERS
    finalZ = std::floor( finalZ * (F32)MAX_2D_LAYERS ) / MAX_2D_LAYERS;

    mPosition.z = finalZ;
}
//-----------------------------------------------------------------------------
void SceneObject2D::setPosition(const Point3F& pos) {
    if (pos == this->mPosition) return;
    this->mPosition.x = pos.x;
    this->mPosition.y = pos.y;
    setLayer(pos.z);
    this->onPositionChanged();
}
//-----------------------------------------------------------------------------
void SceneObject2D::setSize(const Point2F& size) {
    this->mSize = size;
    this->updateWorldBox();
}
//-----------------------------------------------------------------------------
void SceneObject2D::solveCollision(const CollisionInfo2D& info) {
    if ( mCollisionType == CollisionType::None || mCollisionType == CollisionType::Static) {
        return; //should be never come here it's checked before
    }
    if (mCollisionType == CollisionType::Trigger) {
        //FIXME onTriggerEnter ... / also leave must be handled than
        return;
    }

    // --- move out of other object
    if (info.mOther->mCollisionType != CollisionType::Trigger && info.mOther->mCollisionType != CollisionType::None) {
        // clamp overlap
        F32 slop = 0.01f;
        F32 penetration = std::max(0.0f, info.mOverlap - slop);
        F32 correctionScale = 0.8f;

        this->mPosition.x += info.mNormal.x * penetration * correctionScale;
        this->mPosition.y += info.mNormal.y * penetration * correctionScale;

        // this->mPosition.x += info.mNormal.x * info.mOverlap;
        // this->mPosition.y += info.mNormal.y * info.mOverlap;
        this->updateWorldBox();
    }

    // --- change speed
    if (mCollisionType == CollisionType::Kinematic) {
        // stop
        if (info.mNormal.x != 0.0f) this->mVelo.x = 0.0f;
        if (info.mNormal.y != 0.0f) this->mVelo.y = 0.0f;
    }
    //  with mass and oponent bounce
    else if (mCollisionType == CollisionType::Bounce) {
        F32 relVelX = this->mVelo.x - info.mOther->mVelo.x;
        F32 relVelY = this->mVelo.y - info.mOther->mVelo.y;

        F32 normalVel = relVelX * info.mNormal.x + relVelY * info.mNormal.y;

        if (normalVel < 0.0f) {

            F32 combinedRestitution = this->mRestitution * info.mOther->mRestitution;
            combinedRestitution *= info.mOther->mCollisionDamping;
            if (combinedRestitution < 0.0f) combinedRestitution = 0.0f;

            F32 massA = this->mMass;
            F32 massB = (info.mOther->mCollisionType == CollisionType::Static) ? 9999999.0f : info.mOther->mMass;

            F32 impulseScalar = -(1.0f + combinedRestitution) * normalVel;
            impulseScalar /= (1.0f / massA + 1.0f / massB);

            this->mVelo.x += info.mNormal.x * (impulseScalar / massA);
            this->mVelo.y += info.mNormal.y * (impulseScalar / massA);

            Point2F tangent = { -info.mNormal.y, info.mNormal.x };
            F32 tangentVel = relVelX * tangent.x + relVelY * tangent.y;
            F32 combinedFriction = this->mFriction * info.mOther->mFriction;

            F32 frictionScalar = -tangentVel * combinedFriction;
            frictionScalar /= (1.0f / massA + 1.0f / massB);

            this->mVelo.x += tangent.x * (frictionScalar / massA);
            this->mVelo.y += tangent.y * (frictionScalar / massA);

            // push the "other" ... should i - or not
            if (info.mOther->mCollisionType != CollisionType::Static) {
                info.mOther->mVelo.x -= info.mNormal.x * (impulseScalar / massB);
                info.mOther->mVelo.y -= info.mNormal.y * (impulseScalar / massB);

                info.mOther->mVelo.x -= tangent.x * (frictionScalar / massB);
                info.mOther->mVelo.y -= tangent.y * (frictionScalar / massB);
            }

        }
    }

    // else if (mCollisionType == CollisionType::Bounce) {
    //     // bounce
    //
    //     F32 normalVel = this->mVelo.x * info.mNormal.x + this->mVelo.y * info.mNormal.y;
    //
    //     if (normalVel < 0.0f) {
    //         F32 combinedRestitution = this->mRestitution * info.mOther->mRestitution;
    //         combinedRestitution *= info.mOther->mCollisionDamping;
    //
    //         // Impulse:
    //         F32 impulseStrength = -(1.0f + combinedRestitution) * normalVel;
    //
    //         this->mVelo.x += info.mNormal.x * impulseStrength;
    //         this->mVelo.y += info.mNormal.y * impulseStrength;
    //
    //         // Friction
    //         Point2F tangent = { -info.mNormal.y, info.mNormal.x };
    //         F32 tangentVel = this->mVelo.x * tangent.x + this->mVelo.y * tangent.y;
    //
    //         // combine
    //         F32 combinedFriction = this->mFriction * info.mOther->mFriction;
    //
    //         // reduce speed
    //         this->mVelo.x -= tangent.x * tangentVel * combinedFriction;
    //         this->mVelo.y -= tangent.y * tangentVel * combinedFriction;
    //     }
    // }
}


//-----------------------------------------------------------------------------
// NOTE: collision check here !
void SceneObject2D::onPositionChanged() {
    this->updateWorldBox();

    if ( mCollisionType != CollisionType::Static && mCollisionType != CollisionType::None
        && mShapeType != ShapeType::None
    ) {

        std::vector<CollisionInfo2D> collsionInfos;
        if (gClientSceneContainer2D.CheckCollide(this, collsionInfos)) {

            std::sort(collsionInfos.begin(), collsionInfos.end(),
                      [](const CollisionInfo2D& a, const CollisionInfo2D& b) {
                          return a.mOverlap > b.mOverlap;
                      }
            );


            // Con::warnf("--------- We did collide:");

            // FIXME ShapeType

            for (const auto& info : collsionInfos) {
                if (!this->mWorldBox.intersects(info.mOther->mWorldBox)) {
                    continue;
                }

                this->solveCollision(info);
            }
        }
    }
}
//-----------------------------------------------------------------------------
// TestScript for layer clamp/MAX_2D_LAYERS:
//  $foo = new SceneObject2D(){ Position = "10 10 0.123456";}; echo($foo.position);
//  $foo.position.z = 5; $foo.refresh(); echo($foo.position);
// NOTE it's designed for future use. moveing object in Container

// TODO move rotation here
// TODO shape type ..(circle)
void SceneObject2D::updateWorldBox() {

    mWorldBox.x = mPosition.x;
    mWorldBox.y = mPosition.y;
    mWorldBox.w = mSize.x;
    mWorldBox.h = mSize.y;


    mWorldBoxHalfSize.x = mSize.x / 2.f;
    mWorldBoxHalfSize.y = mSize.y / 2.f;

    // added for raw rectangle drawing ...
    mWorldPoints.topLeft = { mPosition.x, mPosition.y};
    mWorldPoints.topRight = { mPosition.x + mSize.x, mPosition.y};
    mWorldPoints.bottomLeft = { mPosition.x, mPosition.y + mSize.y};
    mWorldPoints.bottomRight = { mPosition.x + mSize.x, mPosition.y + mSize.y};

    // allow negativ mass
    if (mMass == 0.f) mMass = MIN_MASS;

}
//-----------------------------------------------------------------------------
void SceneObject2D::moveLinear(F32 dt, bool doRefresh) {
    this->mPosition.x += this->mVelo.x * dt;
    this->mPosition.y += this->mVelo.y * dt;
    // this->mPosition.z += this->mVelo.z * dt;

    if (doRefresh) onPositionChanged();
}
//-----------------------------------------------------------------------------
//NOTE gravity, softening, maxSpeed: depends on common object speed
void SceneObject2D::moveGravity(F32 dt, Point2F gravity, bool doRefresh) {
    this->mVelo.x += (gravity.x / this->mMass) * dt;
    this->mVelo.y += (gravity.y / this->mMass) * dt;
    // this->mVelo.z += (gravity.z / this->mMass) * dt;

    this->mPosition.x += this->mVelo.x * dt;
    this->mPosition.y += this->mVelo.y * dt;
    // this->mPosition.z += this->mVelo.z * dt;

    if (doRefresh) onPositionChanged();
}
//-----------------------------------------------------------------------------
void SceneObject2D::moveOrbital(F32 dt, Point2F centerPoint, F32 gravity
    , F32 softening, F32 maxSpeed, bool applyMass, bool doRefresh) {
    Point2F direction;
    direction.x = centerPoint.x - this->mPosition.x;
    direction.y = centerPoint.y - this->mPosition.y;

    F32 distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0001f) {
        F32 invDistance = 1.0f / distance;
        direction.x *= invDistance;
        direction.y *= invDistance;

        F32 gravityPull = (gravity * 1000.f) / (distance * distance + softening);

        if (applyMass /*&& this->mMass > 0.0001f*/) {
            gravityPull *= this->mMass;
        }

        this->mVelo.x += direction.x * gravityPull * dt;
        this->mVelo.y += direction.y * gravityPull * dt;
    }

    F32 currentSpeed = std::sqrt(this->mVelo.x * this->mVelo.x + this->mVelo.y * this->mVelo.y);
    if (currentSpeed > maxSpeed && currentSpeed > 0.0f) {
        F32 invSpeed = maxSpeed / currentSpeed;
        this->mVelo.x *= invSpeed;
        this->mVelo.y *= invSpeed;
    }

    this->mPosition.x += this->mVelo.x * dt;
    this->mPosition.y += this->mVelo.y * dt;

   if (doRefresh) onPositionChanged();
}

//-----------------------------------------------------------------------------
void SceneObject2D::applyRadialImpulse(const Point2F& center, F32 strength, F32 maxDistance) {
    F32 dirX = this->mPosition.x - center.x;
    F32 dirY = this->mPosition.y - center.y;

    F32 distance = std::sqrt(dirX * dirX + dirY * dirY);

    // allow negativ mass ..
    if (distance < 0.0001f || ( maxDistance != 0.f &&  distance > maxDistance) /*|| this->mMass <= 0.0001f*/) {
        return;
    }

    F32 normX = dirX / distance;
    F32 normY = dirY / distance;

    F32 deltaV = strength / this->mMass;

    this->mVelo.x += normX * deltaV;
    this->mVelo.y += normY * deltaV;
}
//-----------------------------------------------------------------------------
void SceneObject2D::applyLinearImpulse(Point2F direction, F32 strength) {
    // if (this->mMass <= 0.0001f) {
    //     return;
    // }

    F32 len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len < 0.0001f) {
        return;
    }

    F32 normX = direction.x / len;
    F32 normY = direction.y / len;

    F32 deltaV = strength / this->mMass;

    this->mVelo.x += normX * deltaV;
    this->mVelo.y += normY * deltaV;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool SceneObject2D::castRayLayers(Point2F pos, F32 minLayer, F32 maxLayer) {
    if (!mVisible || mPosition.z > maxLayer || mPosition.z < minLayer) return false;
    return mWorldBox.pointInRect(pos);
}
//-----------------------------------------------------------------------------
bool SceneObject2D::rectCollideLayers(RectF rect, F32 minLayer, F32 maxLayer) {
    if (!mVisible || mPosition.z > maxLayer || mPosition.z < minLayer) return false;
    return rect.intersects(mWorldBox);
}
//-----------------------------------------------------------------------------
void SceneObject2D::beginMove() {
      mSavPostion = mPosition;
}
void SceneObject2D::endMove() {
    if (mSavPostion.distance(mPosition) > 0.0001f )  {
        onPositionChanged();
    }
}


}//namespace
