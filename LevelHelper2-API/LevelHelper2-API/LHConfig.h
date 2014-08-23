//
//  LHConfig.h
//  LevelHelper2API
//
//  Created by Bogdan Vladu on 15/05/14.
//  Copyright (c) 2014 VLADU BOGDAN DANIEL PFA. All rights reserved.
//
#ifndef LevelHelper2_API_LHConfig_h
#define LevelHelper2_API_LHConfig_h

/*
 To enable Box2d usage define LH_USE_BOX2D=1 as a preprocessor macro.
 */

// To enable debug mode set LH_DEBUG to 1 or define a LH_DEBUG as a preprocessor macro.
#ifndef LH_DEBUG
#define LH_DEBUG 1
#endif



#endif


/*Integrating Box2D library GUIDE
 
 1. Download the latest Box2D library from http://box2d.org
 2. Copy the Box2D folder in "Library" folder of your Cocos2d project.
 3. In Xcode choose your tager and go to "Build Settings".
 4. Search and find "Header Search Paths".
 5. Double click and add the following line "$(SRCROOT)/$(PROJECT_NAME)/Libraries/"
 If Box2D folder is not located in the "Library" folder of your project you need to change the path to match the path on your computer
 */

