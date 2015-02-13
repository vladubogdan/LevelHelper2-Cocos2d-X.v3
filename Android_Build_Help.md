Please follow this steps in order to build for Android

1. Open Terminal and type the following command in order to compile the project for android

    [your project path]/cocos2d/tools/cocos2d-console/bin/cocos compile -p android
    
    Once the compilation is done your apk file will be located inside
    
    [your project path]/proj.android/bin


        If this is your first time doing this it will ask for the path to ndk, android sdk and apache ant.
        
        Download ndk from here: https://developer.android.com/tools/sdk/ndk/index.html
        Download android sdk from here: https://developer.android.com/sdk/index.html#Other
        Download the apache ant from here: http://ant.apache.org/bindownload.cgi

        Follow the on screen instruction to set the paths.

    Next you will need to copy the apk file on the simulator or on the device.
    
2. Launch Android Simulator by opening Terminal and typing
    [your android sdk path]/tools/android avd

        Install a simualtor if none is present or run one of the available simulator
        
        After you have installed a simulator you can also type this command to directly lauch that simulator. Dont forget to add @ in from of the name.
        
        [your android sdk path]/tools/emulator @yourSimulatorName
        
3. Once the simulator is running (takes a while to start), copy the apk file by typing in this command in Terminal.

    [your android sdk path]/platform-tools/adb -e install [your project path]/proj.android/bin/[your apk file name].apk
        
        
4. See logs by typing this command in a Terminal window

    [your android sdk path]/platform-tools/adb logcat
    
    
Other Info

- Modify Android.mk file located inside proj.android/jni to include new files or libraries 