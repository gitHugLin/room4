#！/bin/bash
ndk-build
adb root
adb remount
adb shell input keyevent 3
adb push libs/armeabi/libMFDenoise.so /system/lib
adb shell stop media
adb shell start media
sleep 1
adb shell am start -n com.android.camera2/com.android.camera.CameraLauncher
adb logcat -s mfdenoise
adb logcat -c