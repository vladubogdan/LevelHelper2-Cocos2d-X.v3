#! /bin/bash
#

BASEDIR=$(dirname $0)
echo $BASEDIR
cd $BASEDIR


$SIMULATOR_NAME = nexus_5_armeabi;

#setup this paths as located on your computer
# export ANT_ROOT=/Volumes/PersonalFiles/Tools/Android/apache-ant-1.9.4/bin
# export NDK_ROOT=/Volumes/PersonalFiles/Tools/Android/android-ndk-r9d
# export ANDROID_SDK_ROOT=/Volumes/PersonalFiles/Tools/Android/adt-bundle-mac-x86_64-20140321/sdk/


# if [ ! -d "${ANT_ROOT}" ]; then
# echo "error: \"ANT_ROOT\" not found or not defined. Please read Android_Build_Help for more info."

# /usr/bin/osascript <<-EOF

#     tell application "System Events"
#         activate
#         display dialog "\"ANT_ROOT\" not found or not defined. Please read Android_Build_Help for more info."
#     end tell
# EOF


# exit 1
# fi

# if [ ! -d "${NDK_ROOT}" ]; then
# echo "error: \"NDK_ROOT\" not found or not defined. Please read Android_Build_Help for more info."

# /usr/bin/osascript <<-EOF

#     tell application "System Events"
#         activate
#         display dialog "\"NDK_ROOT\" not found or not defined. Please read Android_Build_Help for more info."
#     end tell
# EOF

# exit 1
# fi

# if [ ! -d "${ANDROID_SDK_ROOT}" ]; then
# echo "error: \"ANDROID_SDK_ROOT\" not found or not defined. Please read Android_Build_Help for more info."

# /usr/bin/osascript <<-EOF

#     tell application "System Events"
#         activate
#         display dialog "\"ANDROID_SDK_ROOT\" not found or not defined. Please read Android_Build_Help for more info."
#     end tell
# EOF

# exit 1
# fi


if [ "$1" == "compile" ]; then

cocos2d/tools/cocos2d-console/bin/cocos compile -p android

open ./proj.android/bin

elif [ "$1" == "launch_simulator" ]; then

#specify your emulator name here - dont forget to add @ in from of the emulator name
${ANDROID_SDK_ROOT}/tools/emulator @"$SIMULATOR_NAME" &

elif [ "$1" == "copy_on_simulator" ]; then

${ANDROID_SDK_ROOT}/platform-tools/adb -e install proj.android/bin/LevelHelper2-Cocos2d-X.v3-debug.apk
${ANDROID_SDK_ROOT}/platform-tools/adb logcat

elif [ "$1" == "copy_on_device" ]; then

${ANDROID_SDK_ROOT}/platform-tools/adb -d install proj.android/bin/LevelHelper2-Cocos2d-X.v3-debug.apk
${ANDROID_SDK_ROOT}/platform-tools/adb logcat

else

echo "HELP"
echo "---------------------------------"
echo "AVAILABLE ARGUMENTS:";
echo "\"compile\" - compiles the project for Android";
echo "\"launch_simulator\" - launches the android simulator with a specific name";
echo "\"copy_on_simulator\" - copies apk file on simulator";
echo "\"copy_on_device\" - copies apk file on device";
echo "---------------------------------"
fi

