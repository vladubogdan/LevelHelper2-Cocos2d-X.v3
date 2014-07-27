//
//  NodeTransform.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 23/07/14.
//
//

#include "NodeTransform.h"
#include "LHScene.h"

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
