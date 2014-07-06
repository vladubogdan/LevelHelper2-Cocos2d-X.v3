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
 By default we use the default SpriteKit physics engine. Its done this way in order for user to have a simpler initial setup.
 Box2D is the desired physics engine as the simulation will behave exactly like inside LevelHelper 2.
 To enable Box2D usage you need to set LH_USE_BOX2D to 1 or define a LH_USE_BOX2D as a preprocessor macro.
 You also need to include Box2d library in your Xcode project and header search file. Please read the guide on how to include Box2D further down.
 */
#ifndef LH_USE_BOX2D
#define LH_USE_BOX2D 0
#endif

//the demo project uses a preprocessor macro so LH_USE_BOX2D will be 0 in this file.

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

