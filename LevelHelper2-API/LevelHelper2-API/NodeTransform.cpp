//
//  NodeTransform.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 23/07/14.
//
//

#include "NodeTransform.h"
#include "LHScene.h"
#include "LHUtils.h"

Point LHNodeTransform::convertToWorldScale(Node* node, Point nodeScale)
{
    for (Node *p = node->getParent();
         p != nullptr && !LHScene::isLHScene(p);
         p = p->getParent())
    {
        Point scalePt = Point(p->getScaleX(), p->getScaleY());
        nodeScale.x *= scalePt.x;
        nodeScale.y *= scalePt.y;
    }
    return nodeScale;
}

Point LHNodeTransform::convertToNodeScale(Node* node, Point worldScale)
{
    for (Node *p = node->getParent(); p != nullptr && !LHScene::isLHScene(p);
         p = p->getParent())
    {
        Point scalePt = Point(p->getScaleX(), p->getScaleY());
        worldScale.x /= scalePt.x;
        worldScale.y /= scalePt.y;
    }
    return worldScale;
}

float LHNodeTransform::globalAngleFromLocalAngle(Node* node, float la)
{
    Node* prnt = node->getParent();
    while(prnt && !LHScene::isLHScene(prnt)){
        la += prnt->getRotation();
        prnt = prnt->getParent();
    }
    return la;
}

float LHNodeTransform::localAngleFromGlobalAngle(Node* node, float ga){

    Node* prnt = node->getParent();
    while(prnt && !LHScene::isLHScene(prnt)){
        ga -= prnt->getRotation();
        prnt = prnt->getParent();
    }
    return ga;
}

float LHNodeTransform::convertToWorldAngle(Node* node, float rotation)
{
    Point rot = Vec2::forAngle(-CC_DEGREES_TO_RADIANS(rotation));
    Point worldPt = node->convertToWorldSpace(rot);
    Point worldOriginPt = node->convertToWorldSpace(Point(0,0));
    Point worldVec = worldPt - worldOriginPt;
    float ang = -CC_RADIANS_TO_DEGREES(worldVec.getAngle());
    return LHUtils::LHNormalAbsoluteAngleDegrees(ang);
}

float LHNodeTransform::convertToNodeAngle(Node* node, float rotation)
{
    Point rot = Vec2::forAngle(-CC_DEGREES_TO_RADIANS(rotation));
    Point nodePt = node->convertToNodeSpace(rot);
    Point nodeOriginPt = node->convertToNodeSpace(Point(0,0));
    Point nodeVec = nodePt- nodeOriginPt;
    float ang = -CC_RADIANS_TO_DEGREES(nodeVec.getAngle());
    return LHUtils::LHNormalAbsoluteAngleDegrees(ang);
}


Point LHNodeTransform::unitForGlobalPosition(Node* node, Point globalpt)
{
    Point local = node->convertToNodeSpace(globalpt);
    
    Size sizer = node->getContentSize();
    
    float centerPointX = sizer.width*0.5;
    float centerPointY = sizer.height*0.5;
    
    local.x += centerPointX;
    local.y += centerPointY;
    
    return  Point(local.x/sizer.width, local.y/sizer.height);
}
