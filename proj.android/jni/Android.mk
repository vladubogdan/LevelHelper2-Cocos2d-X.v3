LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../LevelHelper2-API/LHScene.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHAnimationProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationFrames/LHFrame.cpp \
                   ../../LevelHelper2-API/Animations/AnimationFrames/LHOpacityFrame.cpp \
                   ../../LevelHelper2-API/Animations/AnimationFrames/LHPositionFrame.cpp \
                   ../../LevelHelper2-API/Animations/AnimationFrames/LHRotationFrame.cpp \
                   ../../LevelHelper2-API/Animations/AnimationFrames/LHScaleFrame.cpp \
                   ../../LevelHelper2-API/Animations/AnimationFrames/LHSpriteFrame.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHCameraActivateProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHChildrenOpacitiesProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHChildrenPositionsProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHChildrenRotationsProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHChildrenScalesProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHOpacityProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHPositionProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHRotationProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHScaleProperty.cpp \
                   ../../LevelHelper2-API/Animations/AnimationProperties/LHSpriteFrameProperty.cpp \
                   ../../LevelHelper2-API/Animations/LHAnimation.cpp \
                   ../../LevelHelper2-API/Protocols/LHJointsProtocol.cpp \
                   ../../LevelHelper2-API/Protocols/LHNodeAnimationProtocol.cpp \
                   ../../LevelHelper2-API/Protocols/LHNodeProtocol.cpp \
                   ../../LevelHelper2-API/Protocols/LHPhysicsProtocol.cpp \
                   ../../LevelHelper2-API/Protocols/LHUserProperties.cpp \
                   ../../LevelHelper2-API/Utilities/LHBox2dCollisionHandling.cpp \
                   ../../LevelHelper2-API/Utilities/LHBox2dDebugDrawNode.cpp \
                   ../../LevelHelper2-API/Utilities/LHDevice.cpp \
                   ../../LevelHelper2-API/Utilities/LHDictionary.cpp \
                   ../../LevelHelper2-API/Utilities/LHUtils.cpp \
                   ../../LevelHelper2-API/Utilities/NodeTransform.cpp \
                   ../../LevelHelper2-API/Nodes/LHAsset.cpp \
                   ../../LevelHelper2-API/Nodes/LHBackUINode.cpp \
                   ../../LevelHelper2-API/Nodes/LHBezier.cpp \
                   ../../LevelHelper2-API/Nodes/LHCamera.cpp \
                   ../../LevelHelper2-API/Nodes/LHDistanceJointNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHDrawNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHGameWorldNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHGearJointNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHGravityArea.cpp \
                   ../../LevelHelper2-API/Nodes/LHNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHParallax.cpp \
                   ../../LevelHelper2-API/Nodes/LHParallaxLayer.cpp \
                   ../../LevelHelper2-API/Nodes/LHPrismaticJointNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHPulleyJointNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHRevoluteJointNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHRopeJointNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHShape.cpp \
                   ../../LevelHelper2-API/Nodes/LHSprite.cpp \
                   ../../LevelHelper2-API/Nodes/LHUINode.cpp \
                   ../../LevelHelper2-API/Nodes/LHWater.cpp \
                   ../../LevelHelper2-API/Nodes/LHWeldJointNode.cpp \
                   ../../LevelHelper2-API/Nodes/LHWheelJointNode.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/DemoExamples/LHSceneDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneIntroductionDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneCameraDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneCameraFollowDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneParallaxDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneCharacterAnimationDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneAssetDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneAssetWithJointsDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneRopeJointDemo.cpp \
                   ../../Classes/DemoExamples/LHScenePulleyJointDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneGearJointDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneWeldJointsDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneCollisionHandlingDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneShapesDemo.cpp \
                   ../../Classes/DemoExamples/LHScenePhysicsTransformationsDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneBeziersDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneGravityAreasDemo.cpp \
                   ../../Classes/DemoExamples/LHSceneUserPropertiesDemo.cpp \
                   ../../Classes/LHSceneTags.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../cocos2d/cocos/ui \
					$(LOCAL_PATH)/../../cocos2d/external \
					$(LOCAL_PATH)/../../cocos2d/external/Box2D/ \
					$(LOCAL_PATH)/../../Classes/DemoExamples \
					$(LOCAL_PATH)/../../LevelHelper2-API \
					$(LOCAL_PATH)/../../LevelHelper2-API/Animations/AnimationProperties \
					$(LOCAL_PATH)/../../LevelHelper2-API/Animations/AnimationFrames/ \
					$(LOCAL_PATH)/../../LevelHelper2-API/Animations \
					$(LOCAL_PATH)/../../LevelHelper2-API/Nodes \
					$(LOCAL_PATH)/../../LevelHelper2-API/Protocols \
					$(LOCAL_PATH)/../../LevelHelper2-API/Utilities \
					

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
# $(call import-module,network)
$(call import-module,extensions)
