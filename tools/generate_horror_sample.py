import os
import cv2
import numpy as np
import math
import random

folder = r"C:\Users\kadir\Desktop\DenemeAI"
out_dir = r"C:\Users\kadir\Desktop\MiniRetroTV_13"
out_file = os.path.join(out_dir, "korku_filmi_ornegi.mjpeg")

img1_path = os.path.join(folder, "camp_crystal_lake_collectible_display.jpg")
img2_path = os.path.join(folder, "camp_crystal_lake_slasher_kadir.jpg")
img3_path = os.path.join(folder, "camp_crystal_lake_birebir_yuz.jpg")

img1 = cv2.imread(img1_path)
img2 = cv2.imread(img2_path)
img3 = cv2.imread(img3_path)

def crop_center_square(img):
    h, w = img.shape[:2]
    min_dim = min(h, w)
    start_x = (w - min_dim) // 2
    start_y = (h - min_dim) // 2
    return img[start_y:start_y+min_dim, start_x:start_x+min_dim]

c1 = crop_center_square(img1)
c2 = crop_center_square(img2)
c3 = crop_center_square(img3)

target_size = 240
fps = 15
total_frames = 80 # ~5.3 saniye tam döngü

def apply_vhs_glitch(frame, intensity=0.15):
    h, w = frame.shape[:2]
    out = frame.copy()
    if random.random() < intensity:
        for _ in range(random.randint(1, 2)):
            y = random.randint(0, h - 8)
            shift = random.randint(-6, 6)
            out[y:y+5] = np.roll(out[y:y+5], shift, axis=1)
        b, g, r = cv2.split(out)
        r = np.roll(r, random.randint(2, 4), axis=1)
        out = cv2.merge([b, g, r])
    return out

frames = []

for i in range(total_frames):
    t = (i / float(total_frames)) * 6.0 # 0 to 6.0 zaman skalası
    
    # 0.0 - 2.2: Sahne 1 (Giriş / Göl ve Kamp)
    # 2.2 - 2.5: Yıldırım / Çakan Şimşek
    # 2.5 - 4.2: Sahne 2 (Slasher / Katil Yaklaşıyor)
    # 4.2 - 4.4: Kan Kırmızısı Şimşek
    # 4.4 - 6.0: Sahne 3 (Maskeli Yakın Çekim + Retro Başlık)
    
    if t < 2.2:
        zoom = 1.0 + (t / 2.2) * 0.2
        h, w = c1.shape[:2]
        crop_w, crop_h = int(w / zoom), int(h / zoom)
        x0 = (w - crop_w) // 2
        y0 = (h - crop_h) // 2
        sub = c1[y0:y0+crop_h, x0:x0+crop_w]
        frame = cv2.resize(sub, (target_size, target_size), interpolation=cv2.INTER_AREA)
        frame = (frame * 0.85).astype(np.uint8)
        frame = apply_vhs_glitch(frame, 0.2)
        if t > 0.6:
            cv2.putText(frame, "CAMP CRYSTAL LAKE", (18, 222), cv2.FONT_HERSHEY_DUPLEX, 0.52, (0, 0, 220), 1)

    elif t < 2.5:
        if i % 2 == 0:
            frame = np.full((target_size, target_size, 3), 255, dtype=np.uint8)
        else:
            frame = cv2.resize(c2, (target_size, target_size))
            frame = 255 - frame

    elif t < 4.2:
        p = (t - 2.5) / 1.7
        zoom = 1.0 + p * 0.15
        h, w = c2.shape[:2]
        crop_w, crop_h = int(w / zoom), int(h / zoom)
        x0 = (w - crop_w) // 2
        y0 = int((h - crop_h) * 0.3)
        sub = c2[y0:y0+crop_h, x0:x0+crop_w]
        frame = cv2.resize(sub, (target_size, target_size), interpolation=cv2.INTER_AREA)
        
        red_tint = int(math.sin(p * math.pi * 4) * 30)
        frame[:, :, 2] = np.clip(frame[:, :, 2].astype(int) + max(0, red_tint), 0, 255)
        frame = apply_vhs_glitch(frame, 0.25)
        cv2.putText(frame, "FRIDAY THE 13TH", (26, 222), cv2.FONT_HERSHEY_TRIPLEX, 0.55, (20, 20, 230), 1)

    elif t < 4.4:
        frame = np.zeros((target_size, target_size, 3), dtype=np.uint8)
        frame[:, :, 2] = 220
        cv2.putText(frame, "HE IS WATCHING", (25, 125), cv2.FONT_HERSHEY_DUPLEX, 0.65, (255, 255, 255), 2)

    else:
        p = (t - 4.4) / 1.6
        h, w = c3.shape[:2]
        zoom = 1.05 + p * 0.18
        crop_w, crop_h = int(w / zoom), int(h / zoom)
        x0 = (w - crop_w) // 2
        y0 = (h - crop_h) // 2
        sub = c3[y0:y0+crop_h, x0:x0+crop_w]
        frame = cv2.resize(sub, (target_size, target_size), interpolation=cv2.INTER_AREA)
        frame = apply_vhs_glitch(frame, 0.3)
        
        scan_y = int(210 + math.sin(t * 10) * 10)
        cv2.line(frame, (0, scan_y), (target_size, scan_y), (180, 180, 180), 1)
        cv2.putText(frame, "COLLECTIBLE DISPLAY", (15, 25), cv2.FONT_HERSHEY_SIMPLEX, 0.45, (0, 240, 255), 1)

    frames.append(frame)

# Save as MJPEG with Quality 65
encode_params = [int(cv2.IMWRITE_JPEG_QUALITY), 65]
with open(out_file, "wb") as f:
    for fr in frames:
        _, buf = cv2.imencode(".jpg", fr, encode_params)
        f.write(buf.tobytes())

print("Korku filmi videosu başarıyla üretildi:", out_file)
size = os.path.getsize(out_file)
print(f"Boyut: {size} bayt ({size/1024:.1f} KB)")
