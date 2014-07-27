//
//  NodeTransform.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 23/07/14.
//
//

#ifndef __LevelHelper2API_NODE_TRANSFORM__
#define __LevelHelper2API_NODE_TRANSFORM__

#include "cocos2d.h"
using namespace cocos2d;

class LHNodeTransform
{
public:

    static Point convertToWorldScale(Node* node, Point nodeScale);
    static Point convertToNodeScale(Node* node, Point worldScale);
    
    static float globalAngleFromLocalAngle(Node* node, float la);
    static float localAngleFromGlobalAngle(Node* node, float ga);
};

#endif //__LevelHelper2API_NODE_TRANSFORM__
