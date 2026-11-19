@echo off
chcp 65001 > nul
title Mini Retro TV - Video Donusturucu (240x240 MJPEG)

if "%~1"=="" (
    echo ========================================================
    echo      MINI RETRO TV - VIDEO DONUSTURUCU (240x240)
    echo ========================================================
    echo  Herhangi bir videoyu (MP4, MKV, AVI, MOV) bu .bat dosyasinin
    echo  uzerine surukleyip birakin!
    echo ========================================================
    pause
    exit /b
)

python "%~dp0video_donusturucu.py" "%~1"
echo.
pause
