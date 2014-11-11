In order to build for Android from inside Xcode you need to first set the environment variables.

1. Select "Android Build" target.
2. Go to "Build Phases" tab.
3. Set your ANT_ROOT, NDK_ROOT, ANDROID_SDK_ROOT variables
4. Set your Android target where it says "#setup target as desired here" - e.g change "android-19" to whaterver you have installed on your computer
5. Build pressing "Command+B"


The system was setup to be used with Genymotion simulator. If you want to build for a device you need to change the platform
in Application.mk to armeabi.


Install Genymotion for Mac OS from http://www.genymotion.com


To see the logs, navigate to "Show The Log Navigator" - the right most tab in Xcode.
and click on the first item. (while compiling, this item should display a spinning wheel)

