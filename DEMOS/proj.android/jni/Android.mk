LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/base)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BlueRobotSprite.cpp \
                   ../../Classes/CameraPhysicsTest.cpp \
                   ../../Classes/DynamicObjectsTransformAnimationTest.cpp \
                   ../../Classes/LHSceneAssetDemo.cpp \
                   ../../Classes/LHSceneAssetWithJointsDemo.cpp \
                   ../../Classes/LHSceneBeziersDemo.cpp \
                   ../../Classes/LHSceneCameraDemo.cpp \
                   ../../Classes/LHSceneCameraFollowDemo.cpp \
                   ../../Classes/LHSceneCharacterAnimationDemo.cpp \
                   ../../Classes/LHSceneCollisionHandlingDemo.cpp \
                   ../../Classes/LHSceneComplexPhysicsDemo.cpp \
                   ../../Classes/LHSceneDemo.cpp \
                   ../../Classes/LHSceneGearJointDemo.cpp \
                   ../../Classes/LHSceneGravityAreasDemo.cpp \
                   ../../Classes/LHSceneIntroductionDemo.cpp \
                   ../../Classes/LHSceneJointWithCameraBUGFix.cpp \
                   ../../Classes/LHSceneNodesSubclassingDemo.cpp \
                   ../../Classes/LHSceneOnTheFlySpritesWithPhysicsDemo.cpp \
                   ../../Classes/LHSceneParallaxDemo.cpp \
                   ../../Classes/LHScenePhysicsTransformationsDemo.cpp \
                   ../../Classes/LHScenePulleyJointDemo.cpp \
                   ../../Classes/LHSceneRemoveOnCollisionDemo.cpp \
                   ../../Classes/LHSceneRopeJointDemo.cpp \
                   ../../Classes/LHSceneShapesDemo.cpp \
                   ../../Classes/LHSceneSkeletalCharacterDemo.cpp \
                   ../../Classes/LHSceneUserPropertiesDemo.cpp \
                   ../../Classes/LHSceneWaterAreaDemo.cpp \
                   ../../Classes/LHSceneWeldJointsDemo.cpp \
                   ../../Classes/MyCustomNode.cpp \
                   ../../LevelHelper2-API/LevelHelper2-API/LHAnimation.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHAnimationProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHAsset.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHb2BuoyancyController.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBackUINode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBezier.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBodyShape.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBone.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBoneFrame.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBoneNodes.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBox2dCollisionHandling.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHBox2dDebugDrawNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHCamera.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHCameraActivateProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHChildrenOpacitiesProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHChildrenPositionsProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHChildrenRotationsProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHChildrenScalesProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHChildrenSpriteFramesProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHContactInfo.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHDevice.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHDictionary.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHDistanceJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHDrawNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHFrame.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHGameWorldNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHGearJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHGravityArea.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHJointsProtocol.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHNodeAnimationProtocol.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHNodeProtocol.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHOpacityFrame.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHOpacityProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHParallax.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHParallaxLayer.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHPhysicsProtocol.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHPositionFrame.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHPositionProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHPrismaticJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHPulleyJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHRevoluteJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHRootBoneProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHRopeJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHRotationFrame.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHRotationProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHScaleFrame.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHScaleProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHScene.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHShape.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHSprite.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHSpriteFrame.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHSpriteFrameProperty.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHUINode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHUserProperties.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHUtils.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHWater.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHWeldJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/LHWheelJointNode.cpp \
				   ../../../LevelHelper2-API/LevelHelper2-API/NodeTransform.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../LevelHelper2-API/LevelHelper2-API

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += box2d_static
LOCAL_STATIC_LIBRARIES += cocos_extension_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,Box2D)
$(call import-module,extensions)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
