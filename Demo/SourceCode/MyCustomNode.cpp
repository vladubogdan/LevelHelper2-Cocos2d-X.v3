//
//  MyCustomNode.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/08/14.
//
//

#include "MyCustomNode.h"

MyCustomNode* MyCustomNode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    MyCustomNode *ret = new MyCustomNode();
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

bool MyCustomNode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status){
        //retrieve elements from the LHSprite object here

        this->addChildRepresentation();
        
        printf("Did create node of type MyCustomNode with name %s.\n", this->getName().c_str());
        
    }
    
    return status;
}

void MyCustomNode::addChildRepresentation()
{
    DrawNode* gw = DrawNode::create();

    Size selfSize = getContentSize();
    
    gw->drawSegment(Point(selfSize.width, 0),
                    Point(selfSize.width, selfSize.height), 1, Color4F::BLUE);

    gw->drawSegment(Point(0, 0),
                    Point(0, selfSize.height), 1, Color4F::BLUE);

    gw->drawSegment(Point(0, selfSize.height),
                    Point(selfSize.width, selfSize.height), 1, Color4F::BLUE);

    gw->drawSegment(Point(0, 0),
                    Point(selfSize.width, 0), 1, Color4F::BLUE);

    
    this->addChild(gw);
}

void MyCustomNode::printCustomNodeMessage()
{
    printf("I'm a MyCustomNode object.\n");
}

MyCustomNode::MyCustomNode(){
    
}

MyCustomNode::~MyCustomNode(){
    
}
