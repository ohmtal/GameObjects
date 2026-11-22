//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// 2D container with layer sorted draw calls, rayCast, boxEmpty and GetBoxObjects
//-----------------------------------------------------------------------------
// NOTE: I use a flat list of Objects (mObjects) at the moment. For many many
//       objects it should be implemented as a Spatial Grid. But the you have
//       the moving to other cells overhead.
//-----------------------------------------------------------------------------
#include "SceneContainer2D.h"
#include "common/Types.h"
#include "common/Structs.h"
#include <vector>
#include <cmath>
#include "SceneObject2D.h"

namespace OGO {
// -----------------------------------------------------------------------------
    SceneContainer2D gClientSceneContainer2D;
// -----------------------------------------------------------------------------
void SceneContainer2D::sortObjects()
{
    if (mLocked) return;
    if (!mDoSortObects) return;
    std::stable_sort(mObjects.begin(), mObjects.end(),
                     [](const SceneObject2D* a, const SceneObject2D* b) {
                         return a->mPosition.z > b->mPosition.z;
                     }
    );

}
// -----------------------------------------------------------------------------
void SceneContainer2D::registerObject(SceneObject2D* obj) {
    if (mLocked) return;
    mObjects.push_back(obj);
    sortObjects();
}

// -----------------------------------------------------------------------------
void SceneContainer2D::unregisterObject(SceneObject2D* obj) {
    if (mLocked) return;
    auto it = std::find(mObjects.begin(), mObjects.end(), obj);
    if (it != mObjects.end()) {
        mObjects.erase(it);
    }
}
// -----------------------------------------------------------------------------
SceneObject2D* SceneContainer2D::castRay(Point2F pos,  F32 minLayer, F32 maxLayer) {
    // objects are sorted from highest layer to lowest so we
    // start at the end of the list
    // hit should be the "closest "
    // for (U32 i = 0; i < mObjects.size(); i++) {
    for (U32 i = mObjects.size() - 1;i >= 0;  i--) {
        SceneObject2D* obj = mObjects[i];
        if (obj->castRayLayers(pos, minLayer,maxLayer)) {
            return obj;

        }
    }
    return nullptr;
}

// -----------------------------------------------------------------------------
// check ... to use ...
// bool shouldCollide(SceneObject2D* a, SceneObject2D* b) {
//     return (a->mCollisionMask & b->mCollisionLayer) &&
//     (b->mCollisionMask & a->mCollisionLayer);
// }
// -----------------------------------------------------------------------------
bool SceneContainer2D::CheckCollide(SceneObject2D* a,  std::vector<CollisionInfo2D>& outResults) {
    bool foundOther = false;
    for (S32 i = 0; i < mObjects.size(); i++) {
        SceneObject2D* b = mObjects[i];
        //filter  same object check
        if (a == b) continue;

        //filter  collision mask / layer  check
        bool aWantsB = (a->mCollisionMask & b->mTypeMask) != 0;
        bool bWantsA = (b->mCollisionMask & a->mTypeMask) != 0;
        if (!aWantsB || !bWantsA) continue;


        // finally check we collide

        // FIXME ShapeType - need a custom function which check what collsion check have to be used.
        // i guess i have to implement my own collision check !

        if ( a->mWorldBox.intersects(b->mWorldBox)) {
            CollisionInfo2D infoA;
            infoA.mOther = b;

            // overlap calculation
            // F32 aHalfW = a->mWorldBox.width / 2.0f;
            // F32 bHalfW = b->mWorldBox.width / 2.0f;
            // F32 diffX = (a->mPosition.x + aHalfW) - (b->mPosition.x + bHalfW);
            // F32 overlapX = (aHalfW + bHalfW) - std::abs(diffX);
            //
            // F32 aHalfH = a->mWorldBox.height / 2.0f;
            // F32 bHalfH = b->mWorldBox.height / 2.0f;
            // F32 diffY = (a->mPosition.y + aHalfH) - (b->mPosition.y + bHalfH);
            // F32 overlapY = (aHalfH + bHalfH) - std::abs(diffY);


            F32 diffX = (a->mWorldBox.x +  a->mWorldBoxHalfSize.x) - (b->mWorldBox.x + b->mWorldBoxHalfSize.x);
            F32 overlapX = (a->mWorldBoxHalfSize.x + b->mWorldBoxHalfSize.x) - std::abs(diffX);


            F32 diffY = (a->mWorldBox.y + a->mWorldBoxHalfSize.y) - (b->mWorldBox.y + b->mWorldBoxHalfSize.y);
            F32 overlapY = (a->mWorldBoxHalfSize.y + b->mWorldBoxHalfSize.y) - std::abs(diffY);


            // smallest check
            if (overlapX < overlapY) {
                infoA.mOverlap = overlapX;
                infoA.mNormal.x = (diffX > 0.0f) ? 1.0f : -1.0f; // 1 = von rechts, -1 = von links
                infoA.mNormal.y = 0.0f;
            } else {
                infoA.mOverlap = overlapY;
                infoA.mNormal.x = 0.0f;
                infoA.mNormal.y = (diffY > 0.0f) ? 1.0f : -1.0f; // 1 = von unten, -1 = von oben
            }

            outResults.push_back(infoA);
            // a->onCollision(infoA); a get the info from this result!

            CollisionInfo2D infoB;
            infoB.mOther = a;
            infoB.mOverlap = infoA.mOverlap;
            infoB.mNormal = infoA.mNormal * -1;
            b->onCollision(infoB); // onCollision is a passive one

            // Events

            foundOther = true;
        }
    }
    return foundOther;
}
// -----------------------------------------------------------------------------
void SceneContainer2D::findObjectsInBox(BoundingBox searchBox, std::vector<SceneObject2D*>& outResults, bool returnOnlyFirst) {

    RectF rect;
    rect.x = searchBox.min.x;
    rect.y = searchBox.min.y;
    rect.w = searchBox.max.x - searchBox.min.x;
    rect.h = searchBox.max.y - searchBox.min.y;

    if (rect.w <= 0.f || rect.h <= 0.f ) {
        // Con::errorf("findObjectsInBox: invalid searchBox min is heigher then max!");
        return;
    }

    F32 minLayer = std::clamp(searchBox.min.z, 0.f, 1.f);
    F32 maxLayer = std::clamp(searchBox.max.z, 0.f, 1.f);
    if (minLayer > maxLayer) {
        F32 dummy = minLayer;
        minLayer = maxLayer;
        maxLayer = dummy;
    }



    for (S32 i = 0; i < mObjects.size(); i++) {
        SceneObject2D* obj = mObjects[i];

        if ( obj->rectCollideLayers(rect, minLayer, maxLayer)) {
            outResults.push_back(obj);
            if (returnOnlyFirst) return;
        }
    }
}
// -----------------------------------------------------------------------------
bool SceneContainer2D::boxEmpty(BoundingBox searchBox) {
    std::vector<SceneObject2D*> queryResult;
    findObjectsInBox(searchBox, queryResult, true);
    return queryResult.size() == 0;
}
// -----------------------------------------------------------------------------
// return a SimSet or nullptr if nothing found
bool SceneContainer2D::fetchBoxObjects(BoundingBox searchBox, std::vector<SceneObject2D*>& result) {

    findObjectsInBox(searchBox, result);

    if (result.size() == 0) return false;


    return true;
}
// -----------------------------------------------------------------------------
void SceneContainer2D::drawObjects() {
    if (mLocked) return;
    for (U32 i = 0; i < mObjects.size(); i++) {
        if (mObjects[i]->mVisible) mObjects[i]->draw();
    }
}
// -----------------------------------------------------------------------------
} //namespace
