@echo off
cd %~dp0

adb shell "echo 1018 >/sys/class/gpio/export"
adb shell "echo 14 >/sys/class/gpio/export"
adb shell "echo 38 >/sys/class/gpio/export"


adb shell "echo out >/sys/class/gpio/gpio1018/direction"
adb shell "echo out >/sys/class/gpio/gpio14/direction"
adb shell "echo out >/sys/class/gpio/gpio38/direction"



adb shell "echo 0 > /sys/class/gpio/gpio1018/value"
adb shell "echo 1 > /sys/class/gpio/gpio14/value"
adb shell "echo 1 > /sys/class/gpio/gpio38/value"
timeout /t 1 /nobreak >nul
adb shell "echo 1 > /sys/class/gpio/gpio1018/value"

echo ESP32_ENTER_DOWNLOAD
timeout /t 1 /nobreak >nul


