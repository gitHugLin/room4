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
adb pull /data/local/result.jpg ../mfdPics/
adb pull /data/local/1.jpg ../mfdPics/
adb pull /data/local/2.jpg ../mfdPics/
adb pull /data/local/3.jpg ../mfdPics/
adb pull /data/local/4.jpg ../mfdPics/
adb pull /data/local/5.jpg ../mfdPics/
