//
//  LHBezier.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHBezier.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"


//! a Point with a vertex point, a tex coord point and a color 4B
typedef struct _LH_V2F_C4B
{
	//! vertices (2F)
	Vertex2F		vertices;
	//! colors (4B)
	Color4B		colors;
} lhV2F_C4B;


static float MAX_BEZIER_STEPS = 24.0f;


LHBezier::LHBezier()
{
}

LHBezier::~LHBezier()
{
    
}

LHBezier* LHBezier::bezierNodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHBezier *ret = new LHBezier();
    if (ret && ret->initWithDictionary(dict, prnt))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LHBezier::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        loadGenericInfoFromDictionary(dict);

        //physics body needs to be created before adding this node to the parent
        loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        
        prnt->addChild(this);
        
        this->setContentSize(dict->sizeForKey("size"));
        
        Point unitPos   = dict->pointForKey("generalPosition");
        Point pos       = LHScene::positionForNode(this, unitPos);
        
        LHDictionary* devPositions = dict->dictForKey("devicePositions");
        if(devPositions)
        {
            std::string unitPosStr = LHDevice::devicePosition(devPositions, LH_SCREEN_RESOLUTION);
            
            if(unitPosStr.length()>0){
                Point unitPos = PointFromString(unitPosStr);
                pos = LHScene::positionForNode(this, unitPos);
            }
        }
        
        this->setOpacity(dict->floatForKey("alpha"));
        this->setRotation(dict->floatForKey("rotation"));
        this->setZOrder(dict->floatForKey("zOrder"));
        
        
        Point scl = dict->pointForKey("scale");
        this->setScaleX(scl.x);
        this->setScaleY(scl.y);
        
        
        Point anchor = dict->pointForKey("anchor");
        anchor.y = 1.0f - anchor.y;
        this->setAnchorPoint(anchor);
        
        this->setPosition(pos);
        
        LHArray* childrenInfo = dict->arrayForKey("children");
        if(childrenInfo)
        {
            for(int i = 0; i < childrenInfo->count(); ++i)
            {
                LHDictionary* childInfo = childrenInfo->dictAtIndex(i);
                
                Node* node = LHScene::createLHNodeWithDictionary(childInfo, this);
#pragma unused (node)
            }
        }
        
        createAnimationsFromDictionary(dict);
        
        return true;
    }
    
    return false;
}

/*
        CCColor* colorOverlay = [dict colorForKey:@"colorOverlay"];

        NSArray* points = [dict objectForKey:@"points"];
        BOOL closed = [dict boolForKey:@"closed"];
        
        linePoints = [[NSMutableArray alloc] init];
        
        NSValue* prevValue = nil;
        
        NSDictionary* previousPointDict = nil;
        for(NSDictionary* pointDict in points)
        {
            if(previousPointDict != nil)
            {
                CGPoint control1 = [previousPointDict pointForKey:@"ctrl2"];
                if(![previousPointDict boolForKey:@"hasCtrl2"]){
                    control1 = [previousPointDict pointForKey:@"mainPt"];
                }
                
                CGPoint control2 = [pointDict pointForKey:@"ctrl1"];
                if(![pointDict boolForKey:@"hasCtrl1"]){
                    control2 = [pointDict pointForKey:@"mainPt"];
                }
                
                CGPoint vPoint = {0.0f, 0.0f};
                for(float t = 0.0; t <= (1 + (1.0f / MAX_BEZIER_STEPS)); t += 1.0f / MAX_BEZIER_STEPS)
                {
                    vPoint = LHPointOnCurve([previousPointDict pointForKey:@"mainPt"],
                                            control1,
                                            control2,
                                            [pointDict pointForKey:@"mainPt"],
                                            t);
                    
                    CGPoint pt = CGPointMake(vPoint.x, -vPoint.y);
                    

                    if(prevValue){
                        CGPoint prevPt = CGPointFromValue(prevValue);
                        [self drawSegmentFrom:prevPt to:pt radius:1 color:colorOverlay];
                    }
                    prevValue = LHValueWithCGPoint(pt);
                    
                    [linePoints addObject:prevValue];
                }
            }
            previousPointDict = pointDict;
        }
        if(closed){
            if([points count] > 1)
            {
                NSDictionary* ptDict = [points objectAtIndex:0];
                
                CGPoint control1 = [previousPointDict pointForKey:@"ctrl2"];
                if(![previousPointDict boolForKey:@"hasCtrl2"]){
                    control1 =  [previousPointDict pointForKey:@"mainPt"];
                }
                
                CGPoint control2 = [ptDict pointForKey:@"ctrl1"];
                if(![ptDict boolForKey:@"hasCtrl1"]){
                    control2 = [ptDict pointForKey:@"mainPt"];
                }
                
                CGPoint vPoint = {0.0f, 0.0f};
                for(float t = 0; t <= (1 + (1.0f / MAX_BEZIER_STEPS)); t += 1.0f / MAX_BEZIER_STEPS)
                {
                    vPoint = LHPointOnCurve([previousPointDict pointForKey:@"mainPt"],
                                            control1,
                                            control2,
                                            [ptDict pointForKey:@"mainPt"],
                                            t);
                 
                    CGPoint pt = CGPointMake(vPoint.x, -vPoint.y);
                    if(prevValue){
                        CGPoint prevPt = CGPointFromValue(prevValue);
                        [self drawSegmentFrom:prevPt to:pt radius:1 color:colorOverlay];
                    }
                    prevValue = LHValueWithCGPoint(pt);
                    
                    [linePoints addObject:prevValue];
                }
            }
        }
 */


//-(NSMutableArray*)linePoints{
//    return linePoints;
//}


void LHBezier::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    visitActiveAnimation();
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}
