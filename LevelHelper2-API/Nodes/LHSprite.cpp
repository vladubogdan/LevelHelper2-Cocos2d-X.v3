//
//  LHSprite.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHSprite.h"
//#import "LHUtils.h"
#include "LHScene.h"
#include "LHDictionary.h"
#include "LHArray.h"
#include "LHDevice.h"

LHSprite::LHSprite()
{
}

LHSprite::~LHSprite()
{

}

//bool LHSprite::initWithDictionary(LHDictionary* dictionary){
//    
//    std::string imgPath = LHSettings::sharedInstance()->imagePath(dictionary->stringForKey("SheetImage"));
//    
//    if(imgPath == "")
//        return false;
//    
//    
//    LHDictionary* texDict = dictionary->dictForKey("TextureProperties");
//    CCRect rect = texDict->rectForKey("Frame");
//    
//    rect = LHSettings::sharedInstance()->transformedTextureRect(rect,imgPath);
//    
//    if(initWithFile(imgPath.c_str(), rect))
//    {
//        setImageFile(imgPath);
//        loadInformationFromDictionary(dictionary);
//        
//        return true;
//    }
//    
//    return false;
//}
//

bool LHSprite::initWithDictionary(LHDictionary* dict, Node* prnt)
{
        LHScene* scene = (LHScene*)prnt->getScene();
        

//        NSString* imagePath = [LHUtils imagePathWithFilename:[dict objectForKey:@"imageFileName"]
//                                                      folder:[dict objectForKey:@"relativeImagePath"]
//                                                      suffix:[scene currentDeviceSuffix:NO]];
//
//        NSString* plistPath = [LHUtils imagePathWithFilename:[dict objectForKey:@"imageFileName"]
//                                                      folder:[dict objectForKey:@"relativeImagePath"]
//                                                      suffix:[scene currentDeviceSuffix:YES]];
//        
//        CCTexture* texture = [scene textureWithImagePath:imagePath];
//        
//        CCSpriteFrame* spriteFrame = nil;
//        
//        NSString* spriteName = [dict objectForKey:@"spriteName"];
//        if(spriteName){
//            NSString* atlasName = [[plistPath lastPathComponent] stringByDeletingPathExtension];
//            atlasName = [[scene relativePath] stringByAppendingPathComponent:atlasName];
//            atlasName = [atlasName stringByAppendingPathExtension:@"plist"];
//            
//            [[CCSpriteFrameCache sharedSpriteFrameCache] addSpriteFramesWithFile:atlasName texture:texture];
//            spriteFrame = [[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:spriteName];
//        }
//        else{
//            spriteFrame = [texture createSpriteFrame];
//        }
//
//        

    std::string imageFile = dict->stringForKey("imageFileName");
    std::string relativeImgPath = dict->stringForKey("relativeImagePath");
    
//    printf("image file %s\n", imageFile.c_str());
//    printf("relative image path %s\n", relativeImgPath.c_str());
    
    std::string imagePath = LHScene::imagePathWithFilename(imageFile,
                                                           relativeImgPath,
                                                           scene->getCurrentDeviceSuffix());

//    printf("final path %s\n", imagePath.c_str());

    bool initResult = false;
    
    if(dict->objectForKey("spriteName"))
    {
        std::string spriteName = dict->stringForKey("spriteName");
        
        std::string plist_path = imagePath;
        size_t extPos = plist_path.rfind('.');
        if (extPos != std::string::npos)
        {
            // Erase the current extension.
            plist_path.erase(extPos);
            // Add the new extension.
            plist_path.append(".plist");
        }
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist_path, imagePath);
        
        initResult = initWithSpriteFrameName(spriteName);
    }
    else{
        initResult = initWithFile(imagePath);
    }
    
    
//    bool Sprite::initWithSpriteFrameName(const std::string& spriteFrameName)
//    {
//        CCASSERT(spriteFrameName.size() > 0, "Invalid spriteFrameName");
//        
//        SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
//        return initWithSpriteFrame(frame);
//    }
//    
//    bool Sprite::initWithSpriteFrame(SpriteFrame *spriteFrame)
//    {
//        CCASSERT(spriteFrame != nullptr, "");
//        
//        bool bRet = initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
//        setSpriteFrame(spriteFrame);
//        
//        return bRet;
//    }
//    
    
    if(initResult)
    {
        prnt->addChild(this);
        
        _physicsBody = NULL;
        
        setName(dict->stringForKey("name"));
        setUuid(dict->stringForKey("uuid"));
        setTags(dict->arrayForKey("tags"));
        loadUserPropertyWithDictionary(dict, this);
        
//        if(this->getUserProperty()){
//            printf("user prop name %s\n", this->getUserProperty()->getClassName().c_str());
//        }
        
        
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

        
        this->setColor(dict->colorForKey("colorOverlay"));
        this->setOpacity(dict->floatForKey("alpha"));
        this->setRotation(dict->floatForKey("rotation"));
        this->setZOrder(dict->floatForKey("zOrder"));

//        [LHUtils loadPhysicsFromDict:[dict objectForKey:@"nodePhysics"]
//                             forNode:self];
        
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
        
        createAnimationsFromDictionary(dict, this);
        
        
        return true;
    }
        

    return false;
}


LHSprite* LHSprite::spriteNodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHSprite *ret = new LHSprite();
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

//void LHSprite::setOpacity(GLubyte opacity)
//{
//    Sprite::setOpacity(opacity);
//}
//
//void LHSprite::setRotation(float rotation)
//{
//    Sprite::setRotation(rotation);
//    
//    //*own stuff here*//
//}
//void LHSprite::setScaleX(float scaleX)
//{
//    Sprite::setScaleX(scaleX);
//}
//
//void LHSprite::setScaleY(float scaleY)
//{
//    Sprite::setScaleY(scaleY);
//}
//
//void LHSprite::setPosition(const Point& pos)
//{
//    Sprite::setPosition(pos);
//}
//
//Scene* LHSprite::getScene()
//{
//    return Sprite::getScene();
//}

void LHSprite::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    visitActiveAnimation();
    
    Sprite::visit(renderer, parentTransform, parentTransformUpdated);
}

/*
@implementation LHSprite
{
    NSTimeInterval lastTime;
    
    NSString* _uuid;
    NSArray* _tags;
    id<LHUserPropertyProtocol> _userProperty;

    NSMutableArray* _animations;
    __weak LHAnimation* activeAnimation;
}

-(void)dealloc{

    LH_SAFE_RELEASE(_uuid);
    LH_SAFE_RELEASE(_tags);
    LH_SAFE_RELEASE(_userProperty);
    LH_SAFE_RELEASE(_animations);
    activeAnimation = nil;
    LH_SUPER_DEALLOC();
}

+ (instancetype)spriteNodeWithDictionary:(NSDictionary*)dict
                                  parent:(CCNode*)prnt{
    return LH_AUTORELEASED([[self alloc] initSpriteNodeWithDictionary:dict
                                                               parent:prnt]);
}


- (instancetype)initSpriteNodeWithDictionary:(NSDictionary*)dict
                                      parent:(CCNode*)prnt{

    LHScene* scene = (LHScene*)[prnt scene];
    
    NSString* imagePath = [LHUtils imagePathWithFilename:[dict objectForKey:@"imageFileName"]
                                                  folder:[dict objectForKey:@"relativeImagePath"]
                                                  suffix:[scene currentDeviceSuffix:NO]];

    NSString* plistPath = [LHUtils imagePathWithFilename:[dict objectForKey:@"imageFileName"]
                                                  folder:[dict objectForKey:@"relativeImagePath"]
                                                  suffix:[scene currentDeviceSuffix:YES]];
    
    CCTexture* texture = [scene textureWithImagePath:imagePath];
    
    CCSpriteFrame* spriteFrame = nil;
    
    NSString* spriteName = [dict objectForKey:@"spriteName"];
    if(spriteName){
        NSString* atlasName = [[plistPath lastPathComponent] stringByDeletingPathExtension];
        atlasName = [[scene relativePath] stringByAppendingPathComponent:atlasName];
        atlasName = [atlasName stringByAppendingPathExtension:@"plist"];
        
        [[CCSpriteFrameCache sharedSpriteFrameCache] addSpriteFramesWithFile:atlasName texture:texture];
        spriteFrame = [[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:spriteName];
    }
    else{
        spriteFrame = [texture createSpriteFrame];
    }

    
    
    
    
    if(self = [super initWithSpriteFrame:spriteFrame]){
        
        [prnt addChild:self];
        
        [self setName:[dict objectForKey:@"name"]];
        
        _uuid = [[NSString alloc] initWithString:[dict objectForKey:@"uuid"]];
        _userProperty = [LHUtils userPropertyForNode:self fromDictionary:dict];
        [LHUtils tagsFromDictionary:dict
                       savedToArray:&_tags];
        
        
        CGPoint unitPos = [dict pointForKey:@"generalPosition"];
        CGPoint pos = [LHUtils positionForNode:self
                                      fromUnit:unitPos];
        
        NSDictionary* devPositions = [dict objectForKey:@"devicePositions"];
        if(devPositions)
        {

            #if TARGET_OS_IPHONE
            NSString* unitPosStr = [LHUtils devicePosition:devPositions
                                                   forSize:LH_SCREEN_RESOLUTION];
            #else
            NSString* unitPosStr = [LHUtils devicePosition:devPositions
                                                   forSize:scene.size];
            #endif
            
            if(unitPosStr){
                CGPoint unitPos = LHPointFromString(unitPosStr);
                pos = [LHUtils positionForNode:self
                                      fromUnit:unitPos];
            }
        }

        [self setColor:[dict colorForKey:@"colorOverlay"]];
        
        float alpha = [dict floatForKey:@"alpha"];
        [self setOpacity:alpha/255.0f];
        
        
        float rot = [dict floatForKey:@"rotation"];
        [self setRotation:rot];
        
        float z = [dict floatForKey:@"zOrder"];
        [self setZOrder:z];
        
        [LHUtils loadPhysicsFromDict:[dict objectForKey:@"nodePhysics"]
                             forNode:self];
        
        CGPoint scl = [dict pointForKey:@"scale"];
        [self setScaleX:scl.x];
        [self setScaleY:scl.y];
        
        
        CGPoint anchor = [dict pointForKey:@"anchor"];
        anchor.y = 1.0f - anchor.y;
        [self setAnchorPoint:anchor];
        
        [self setPosition:pos];

        NSArray* childrenInfo = [dict objectForKey:@"children"];
        if(childrenInfo)
        {
            for(NSDictionary* childInfo in childrenInfo)
            {
                CCNode* node = [LHScene createLHNodeWithDictionary:childInfo
                                                            parent:self];
                #pragma unused (node)
            }
        }
        
        [LHUtils createAnimationsForNode:self
                         animationsArray:&_animations
                         activeAnimation:&activeAnimation
                          fromDictionary:dict];
        
        
    }
    return self;
}

-(CCNode <LHNodeProtocol>*)childNodeWithName:(NSString*)name{
    return [LHScene childNodeWithName:name
                              forNode:self];
}

-(CCNode <LHNodeProtocol>*)childNodeWithUUID:(NSString*)uuid{
    return [LHScene childNodeWithUUID:uuid
                              forNode:self];
}

-(NSMutableArray*)childrenOfType:(Class)type{
    return [LHScene childrenOfType:type
                           forNode:self];
}

-(NSMutableArray*)childrenWithTags:(NSArray*)tagValues containsAny:(BOOL)any{
    return [LHScene childrenWithTags:tagValues containsAny:any forNode:self];
}

-(NSString*)uuid{
    return _uuid;
}

-(NSArray*)tags{
    return _tags;
}

-(id<LHUserPropertyProtocol>)userProperty{
    return _userProperty;
}

*/