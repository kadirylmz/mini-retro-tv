#!/usr/bin/env python3
"""
Mini Retro TV - Video Dönüştürücü (240x240 MJPEG)
Herhangi bir video dosyasını (.mp4, .mkv, .avi, .mov vb.) 
Mini Retro TV'nin 1.3" ST7789 ekranına uygun 240x240 15 FPS .mjpeg dosyasına çevirir.
"""

import os
import sys
import cv2

def convert_video(input_path, output_path=None, target_fps=15, target_size=(240, 240), quality=75, max_seconds=30):
    if not os.path.exists(input_path):
        print(f"HATA: Dosya bulunamadı: {input_path}")
        return False

    if output_path is None:
        base, _ = os.path.splitext(input_path)
        output_path = f"{base}_240x240.mjpeg"

    cap = cv2.VideoCapture(input_path)
    if not cap.isOpened():
        print(f"HATA: Video açılamadı: {input_path}")
        return False

    orig_fps = cap.get(cv2.CAP_PROP_FPS)
    orig_total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    orig_w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    orig_h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    if orig_fps <= 0:
        orig_fps = 25.0

    print("=" * 50)
    print("  📺 MINI RETRO TV - VIDEO DÖNÜŞTÜRÜCÜ")
    print("=" * 50)
    print(f"Girdi Dosyası    : {os.path.basename(input_path)}")
    print(f"Orijinal Çözünürlük: {orig_w}x{orig_h} @ {orig_fps:.1f} FPS")
    print(f"Hedef Çözünürlük: {target_size[0]}x{target_size[1]} @ {target_fps} FPS (MJPEG)")
    print(f"Maksimum Süre   : {max_seconds} saniye")
    print("Dönüştürme başlıyor...")

    # Hedef boyuta orantılı kırpma/ölçekleme (Center crop)
    tw, th = target_size
    frame_interval = orig_fps / target_fps
    current_orig_frame = 0.0
    saved_frames = 0
    max_frames = max_seconds * target_fps

    encode_params = [int(cv2.IMWRITE_JPEG_QUALITY), quality]

    with open(output_path, "wb") as out_f:
        while True:
            cap.set(cv2.CAP_PROP_POS_FRAMES, int(current_orig_frame))
            ret, frame = cap.read()
            if not ret:
                break

            # Orantılı ölçekle ve ortadan kırp (Cover / Fill)
            scale = max(tw / orig_w, th / orig_h)
            new_w = int(orig_w * scale)
            new_h = int(orig_h * scale)
            resized = cv2.resize(frame, (new_w, new_h), interpolation=cv2.INTER_AREA)

            x_start = (new_w - tw) // 2
            y_start = (new_h - th) // 2
            cropped = resized[y_start:y_start+th, x_start:x_start+tw]

            # JPEG olarak kodla ve dosyaya yaz
            success, enc_jpg = cv2.imencode(".jpg", cropped, encode_params)
            if success:
                out_f.write(enc_jpg.tobytes())
                saved_frames += 1

            current_orig_frame += frame_interval
            if saved_frames >= max_frames or int(current_orig_frame) >= orig_total_frames:
                break

            if saved_frames % 15 == 0:
                print(f" -> İşlendi: {saved_frames} kare ({(saved_frames/target_fps):.1f} sn)", end="\r")

    cap.release()
    file_size_kb = os.path.getsize(output_path) / 1024

    print(f"\nTamamlandı! Toplam {saved_frames} kare kaydedildi.")
    print(f"Oluşturulan Dosya: {output_path}")
    print(f"Dosya Boyutu     : {file_size_kb:.1f} KB ({(file_size_kb/1024):.2f} MB)")
    print("=" * 50)
    print("Şimdi bu .mjpeg dosyasını Wi-Fi üzerinden TV'ye (192.168.4.1) yükleyebilirsiniz!")
    return True

if __name__ == "__main__":
    if len(sys.argv) > 1:
        input_file = sys.argv[1]
        convert_video(input_file)
    else:
        print("Kullanım: python video_donusturucu.py <video_dosyasi>")
        print("Veya video dosyasını bu betiğin (veya .bat dosyasının) üzerine sürükleyip bırakın.")
        input("\nÇıkmak için Enter'a basın...")
