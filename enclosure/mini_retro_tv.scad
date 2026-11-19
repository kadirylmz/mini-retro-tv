// =============================================================
//  MINI RETRO TV - Parametrik kasa (Sesli + MicroSD Sürümü)
//  Donanım:
//    - Voltaj 1.69" 240x280 ST7789 SPI IPS Ekran (8-pin)
//    - TLS Robotik ESP32-C3 SuperMini (Lehimsiz)
//    - MAX98357A I2S 3W Amfi + 20mm / 14x20mm 8 Ohm 1W Mini Hoparlör
//    - SPI MicroSD Kart Modülü
//    - TP4056 Type-C Korumalı Şarj + Powermaster 603035 600mAh LiPo
//
//  Parçalar (part değişkeni):
//    "preview" : montajlı görünüm (renkli)
//    "body"    : ana gövde  (ön yüz tabla üzerinde basılır, destek gerekmez)
//    "back"    : arka kapak (sürtünmeli geçme, vida yok)
//    "knobs"   : 2 adet dekor düğme
//    "legs"    : 4 adet konik retro ayak
//    "antenna" : anten tabanı + 2 boncuk (çubuk için 1 mm tel/ataş kullanın)
//    "plate"   : tüm parçalar tek baskı tablasında
// =============================================================

part = "preview"; // [preview, body, back, knobs, legs, antenna, plate]

/* [Elektronik - 1.3" ST7789 Ekran + ESP32-S3 + MicroSD] */
pcb_w   = 59.0;  // 10 mm (1 cm) genişletildi: 1.3" ekran ve sol kenardaki kablo/pin geçişi için
pcb_h   = 32.0;  // Ekran PCB yüksekliği
pcb_t   = 16.0;  // ekran + ESP32-S3 + MicroSD kart modülü derinliği
win_w   = 27.5;  // 1.3" IPS 240x240 aktif ekran penceresi
win_h   = 26.5;
win_r   = 3.5;   // retro tüplü TV köşe yuvarlaklığı
win_dx  = -8.0;  // sol tarafa 1 cm montaj payı bırakıldı
win_dz  = 0;     // (+yukarı)
bat_t   = 6.5;   // LiPo pil payı

/* [TP4056 USB-C, MicroSD Yuvası ve Buton Delikleri] */
usb_side = "left";  // [left, right, bottom, top]
usb_off  = 0;       // kenar boyunca kaydırma
usb_y    = 7.5;     // ön iç yüzeyden TP4056 Type-C soket merkezine derinlik
usb_len  = 10.5;    // delik uzunluğu
usb_dia  = 4.6;     // delik yüksekliği

sd_slot  = true;    // sağ yanda harici MicroSD kart yuvası
sd_side  = "right"; // [left, right, bottom, top]
sd_off   = -4;      // kenar boyunca kaydırma
sd_y     = 13.0;    // ön iç yüzeyden MicroSD soket derinliği
sd_len   = 14.5;    // MicroSD kart genişliği (~11 mm + parmak payı)
sd_dia   = 3.2;     // slot yüksekliği

btn_hole = true;    // kanal/Wi-Fi/uyku butonu deliği
btn_side = "top";   // [left, right, bottom, top]
btn_off  = 8;
btn_y    = 8.0;
btn_dia  = 2.6;

/* [Gövde] */
wall     = 1.8;
front    = 2.2;  // ön duvar (kör düğme pim yuvası PCB'ye değmez)
clr      = 0.4;  // kart çevresi boşluk
edge_r   = 3.0;  // dış kenar yuvarlaklığı
panel    = true; // sağda retro düğme & açık hoparlör ızgarası
panel_w  = 2.5;  // 8-pin lehim/kablo dönüş payı
back_t   = 1.6;
fit      = 0.15; // arka kapak geçme toleransı (sıkıysa artırın)
wire_gap = 2.5;  // pil arkası kablo boşluğu

/* [Aksesuar] */
knob_d   = 7.5;
knob_h   = 3.5;
peg_d    = 2.6;  // düğme/ayak pimi çapı
knob_peg = 1.4;  // kör pim derinliği (ön duvar 2.2 mm -> arkadaki PCB'ye değmez)
leg_h    = 6.5;
leg_d1   = 2.2;  // ayak ucu
leg_d2   = 4.2;  // ayak üstü
ant_d    = 10;
wire_d   = 1.15; // anten teli deliği (ataş ~1 mm)

$fn = 48;

// ---------- türetilmiş ölçüler ----------
in_w  = pcb_w + 2*clr + (panel ? panel_w : 0);
in_h  = pcb_h + 2*clr;
in_d  = pcb_t + bat_t + wire_gap;
out_w = in_w + 2*wall;
out_h = in_h + 2*wall;
out_d = front + in_d;
bx    = wall + clr + pcb_w/2;             // PCB merkezi X
bz    = out_h/2;                          // PCB merkezi Z
wx    = bx + win_dx;                      // ekran penceresi merkezi X
lip   = max(1.5, wire_gap - 0.3);         // arka kapak geçme dudağı
px    = ((wx + win_w/2) + (out_w - wall)) / 2; // sağ retro panel merkezi X

echo(str("DIS OLCU (gövde): ", out_w, " x ", out_h, " x ", out_d + back_t, " mm"));

// ---------- yardımcılar ----------
module rr2(w, h, r) { offset(r = r) square([w - 2*r, h - 2*r], center = true); }

// yerel (u,v,w) -> global (x=u, y=w, z=v) : ön yüzde çizim için
module onFront() { rotate([-90, 0, 0]) mirror([0, 1, 0]) children(); }

module rbox(s, r) {
    hull() for (x = [r, s.x - r], y = [r, s.y - r], z = [r, s.z - r])
        translate([x, y, z]) sphere(r);
}

// kenar deliği: side, kenar boyunca kaydırma, derinlik(y), uzunluk, çap
module sideHole(side, off, y, len, dia) {
    l = max(0, len - dia);
    if (side == "left" || side == "right") {
        x0 = side == "left" ? -1 : out_w - wall - 1;
        hull() for (s = [-1, 1])
            translate([x0, front + y, bz + off + s*l/2])
                rotate([0, 90, 0]) cylinder(d = dia, h = wall + 2);
    } else {
        z0 = side == "bottom" ? -1 : out_h - wall - 1;
        hull() for (s = [-1, 1])
            translate([bx + off + s*l/2, front + y, z0])
                cylinder(d = dia, h = wall + 2);
    }
}

leg_pos = [[7, 6], [out_w - 7, 6], [7, out_d - 6], [out_w - 7, out_d - 6]];

// ---------- GÖVDE ----------
module body() {
    difference() {
        intersection() {
            rbox([out_w, out_d + edge_r + 1, out_h], edge_r);
            cube([out_w, out_d, out_h]);            // arka yüz düz
        }
        // iç boşluk (arkası açık)
        translate([wall, front, wall]) cube([in_w, in_d + 1, in_h]);

        // ekran penceresi + pah (bezel)
        onFront() translate([wx, bz + win_dz, 0]) {
            hull() {
                translate([0, 0, -0.01]) linear_extrude(0.01) rr2(win_w + 2.4, win_h + 2.4, win_r + 1.2);
                linear_extrude(1.2) rr2(win_w, win_h, win_r);
            }
            translate([0, 0, -0.1]) linear_extrude(front + 0.2) rr2(win_w, win_h, win_r);
        }

        // ekran çevresi dekor oluk
        onFront() translate([wx, bz + win_dz, -0.01])
            linear_extrude(0.5) difference() {
                rr2(win_w + 5.4, win_h + 5.4, win_r + 2.7);
                rr2(win_w + 4.4, win_h + 4.4, win_r + 2.2);
            }

        if (panel) {
            // düğme kör pim yuvaları (PCB'ye değmez)
            for (zz = [out_h*0.72, out_h*0.44])
                translate([px, -0.5, zz]) rotate([-90, 0, 0]) cylinder(d = peg_d + 0.3, h = knob_peg + 0.6);
            // GERÇEK AÇIK HOPARLÖR IZGARASI (ses çıkışı için ön duvarı tamamen deler)
            for (i = [0:3])
                onFront() translate([px, 4.5 + i*2, -0.1])
                    linear_extrude(front + 0.2) rr2(8.5, 1.1, 0.5);
        }

        // ayak pim delikleri
        for (p = leg_pos) translate([p[0], p[1], -1]) cylinder(d = peg_d + 0.3, h = wall + 0.6);

        // TP4056 USB-C, MicroSD yuvası ve buton
        sideHole(usb_side, usb_off, usb_y, usb_len, usb_dia);
        if (sd_slot)  sideHole(sd_side, sd_off, sd_y, sd_len, sd_dia);
        if (btn_hole) sideHole(btn_side, btn_off, btn_y, btn_dia, btn_dia);
    }
}

// ---------- ARKA KAPAK (merkezli, dudak +Z) ----------
module back() {
    lw = in_w - 2*fit;  lh = in_h - 2*fit;
    press = in_d - pcb_t - bat_t - 0.2;   // pili öne bastıran kaburga
    difference() {
        linear_extrude(back_t) rr2(out_w, out_h, edge_r);
        // havalandırma + ek ses çıkış yarıkları (retro)
        for (i = [0:4])
            translate([0, out_h*0.18 - i*2.6, -1]) linear_extrude(back_t + 2) rr2(out_w*0.55, 1.2, 0.55);
        // arka yazı (dışarıdan düz okunur)
        translate([0, -out_h*0.28, -0.01]) mirror([1, 0, 0]) linear_extrude(0.5)
            text("MINI TV", size = 4, font = "Liberation Sans:style=Bold", halign = "center", valign = "center");
    }
    // geçme dudağı
    translate([0, 0, back_t - 0.01]) linear_extrude(lip) difference() {
        rr2(lw, lh, 0.6);
        rr2(lw - 2.4, lh - 2.4, 0.2);
    }
    // bastırma kaburgaları
    if (press > 0.4)
        for (sx = [-1, 1]) translate([sx*lw/4 - 0.6, -lh/2 + 1, back_t - 0.01]) cube([1.2, lh - 2, press]);
}

// ---------- DÜĞME (yüz z=0, pim +Z) ----------
module knob() {
    difference() {
        union() {
            cylinder(d1 = knob_d - 1, d2 = knob_d, h = 0.6);
            translate([0, 0, 0.6]) cylinder(d = knob_d, h = knob_h - 0.6);
        }
        for (a = [0:30:359]) rotate(a) translate([knob_d/2 + 0.25, 0, -1]) cylinder(d = 1.1, h = knob_h + 2, $fn = 12);
        translate([-0.4, 0, -0.01]) cube([0.8, knob_d/2 - 0.8, 0.5]); // gösterge çizgisi
    }
    translate([0, 0, knob_h - 0.01]) cylinder(d = peg_d - 0.1, h = knob_peg - 0.1);
}

// ---------- AYAK (uç z=0, pim +Z) ----------
module leg() {
    cylinder(d1 = leg_d1, d2 = leg_d2, h = leg_h);
    translate([0, 0, leg_h - 0.01]) cylinder(d = peg_d - 0.1, h = wall - 0.3);
}

// ---------- ANTEN TABANI + BONCUK ----------
module antenna_base() {
    difference() {
        union() {
            scale([1, 1, 0.55]) difference() { sphere(d = ant_d); translate([0, 0, -ant_d]) cube(2*ant_d, center = true); }
            cylinder(d = ant_d + 3, h = 0.8);
        }
        for (s = [-1, 1]) translate([0, 0, 1.2]) rotate([0, s*28, 0]) cylinder(d = wire_d, h = ant_d, $fn = 16);
    }
}
module bead() { difference() { sphere(d = 3.2, $fn = 24); translate([0, 0, -2]) cylinder(d = wire_d, h = 3.2, $fn = 12); } }

// ---------- ÇIKTI ----------
module preview() {
    color("#c8452f") body();
    color("#f1e6cf") translate([out_w/2, out_d + back_t, out_h/2]) rotate([90, 0, 0]) back();
    color("#151515") onFront() translate([wx, bz + win_dz, 0.8]) linear_extrude(0.4) rr2(win_w, win_h, win_r);
    if (panel) for (zz = [out_h*0.72, out_h*0.44])
        color("#f1e6cf") translate([px, -knob_h, zz]) rotate([-90, 0, 0]) knob();
    for (p = leg_pos) color("#6b4a2b") translate([p[0], p[1], -leg_h]) leg();
    color("#f1e6cf") translate([wx, out_d/2, out_h - 0.3]) antenna_base();
    for (s = [-1, 1]) color("silver") translate([wx, out_d/2, out_h + 1.5])
        rotate([0, s*28, 0]) { cylinder(d = 1, h = 24); translate([0, 0, 24]) color("#f1e6cf") sphere(d = 3.2); }
}

if (part == "preview")      preview();
else if (part == "body")    rotate([90, 0, 0]) body();      // ön yüz z=0'da (tabla)
else if (part == "back")    back();
else if (part == "knobs")   for (i = [0:1]) translate([i*11, 0, 0]) knob();
else if (part == "legs")    for (i = [0:3]) translate([i*8, 0, 0]) leg();
else if (part == "antenna") { antenna_base(); for (i = [0:1]) translate([12 + i*6, 0, 1.6]) bead(); }
else if (part == "plate")   {
    translate([0, out_h, 0]) rotate([90, 0, 0]) body();
    translate([out_w + 8 + out_w/2, out_h/2, 0]) back();
    for (i = [0:1]) translate([5 + i*11, out_h + 10, 0]) knob();
    for (i = [0:3]) translate([30 + i*8, out_h + 10, 0]) leg();
    translate([out_w + 20, out_h + 14, 0]) antenna_base();
    for (i = [0:1]) translate([out_w + 34 + i*6, out_h + 14, 1.6]) bead();
}
