//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// SceneObject2D / Container System
//
// Together with SceneObject2D it handle layers and can be used for drawing
//
//-----------------------------------------------------------------------------
// FIXME:
//      - culling
//-----------------------------------------------------------------------------
//TEST SCRIPT:
/*
$foo = new SceneObject2D() { position = "10 10 0";};
$bar = new SceneObject2D() { position = "10 10 0.1";};
ClientContainer2DListObjects();
echo(ClientContainer2DRayCast("11 11"));
$foo.setPosition("10 10 0.2");   //shift foo's layer down
echo(ClientContainer2DRayCast("11 11"));
echo(ClientContainer2DBoxEmpty("1 1 0", "20 20 1")); // should be 0
echo(ClientContainer2DBoxEmpty("100 100 0", "200 200 1"));// should be 1
$boxObjects = ClientContainer2DGetBoxObjects( "1 1 0", "20 20 1");
$boxObjects.listObjects(); // should list $foo and $bar (id's)
$boxObjects.delete();
*/
//-----------------------------------------------------------------------------
#pragma once

#include "common/Types.h"
#include "common/Structs.h"
#include "common/Math.h"
#include <vector>

#ifndef RESERVED_SCENE_OBJECTS
#define RESERVED_SCENE_OBJECTS 250
#endif

namespace OGO {

class SceneObject2D;

struct CollisionInfo2D {
    SceneObject2D* mOther = nullptr; // we did collide with
    Point2F mNormal = {0.0f, 0.0f};  // the direction where the collision come from
    F32 mOverlap = 0.0f;             // how much we overlap
};

class SceneContainer2D {
private:
    std::vector<SceneObject2D*> mObjects;
    bool mLocked = false;

public:
    SceneContainer2D() {
        mObjects.reserve(RESERVED_SCENE_OBJECTS);
    }
    void registerObject(SceneObject2D* obj);
    void unregisterObject(SceneObject2D* obj);

    SceneObject2D* castRay(Point2F pos,   F32 minLayer = 0.f, F32 maxLayer = 1.f);


    void findObjectsInBox(BoundingBox searchBox, std::vector<SceneObject2D*>& outResults, bool returnOnlyFirst = false);
    bool CheckCollide(SceneObject2D* object, std::vector<CollisionInfo2D>& outResults);

    bool boxEmpty(BoundingBox searchBox);
    bool fetchBoxObjects(BoundingBox searchBox, std::vector<SceneObject2D*>& result);


    const std::vector<SceneObject2D*> getObjects() { return  mObjects; }

    virtual void drawObjects();

    bool mDoSortObects = true;
    void sortObjects(); // resort objects for layer drawing

    void reserveMore(S32 count) {
        mObjects.reserve(count);
    }


};

// global Container
extern SceneContainer2D gClientSceneContainer2D;

} // namespace
