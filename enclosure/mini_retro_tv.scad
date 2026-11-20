// =============================================================
//  MINI RETRO TV - Parametrik Kasa Tasarımı
//  (Sesli + MicroSD + LiPo Şarjlı + Wi-Fi Video Sürümü)
//
//  Donanım Desteği:
//    - Ekran: 1.3" IPS 240x240 (Varsayılan) veya 1.69" IPS 240x280
//    - Beyin: ESP32-S3 SuperMini / ESP32-C3 SuperMini
//    - Ses:   MAX98357A I2S 3W Dijital Amfi + 20mm Mini Hoparlör
//    - Depolama: SPI 16GB MicroSD Kart Modülü
//    - Güç:   TP4056 Type-C Korumalı Şarj Devresi + LiPo Pil
//
//  Baskı Parçaları (part değişkeni):
//    "preview"       : Montajlı TV görünümü (Yekpare V-Anten ve Buton Pimi ile)
//    "body"          : Ana TV gövdesi (ön yüz tablaya düz basar, destek gerekmez)
//    "back"          : Arka kapak (tırnaklı geçme, vidasız)
//    "knobs"         : 2 adet dekoratif ön çevirme düğmesi
//    "legs"          : 4 adet retro konik TV ayağı
//    "button_cap"    : Üst kanal/ses butonu basma pimi (T-şekilli, delikten düşmez)
//    "antenna_solid" : Yekpare 3D basılabilir V-Anten (Desteksiz)
//    "antenna"       : Geleneksel tel/ataş yuvası + 2 boncuk
//    "plate"         : TÜM PARÇALAR TEK TABLADA (Gövde+Kapak+Düğmeler+Ayaklar+Anten+Buton Pimi)
// =============================================================

part = "preview"; // [preview, body, back, knobs, legs, button_cap, antenna_solid, antenna, plate]

/* [Ekran Modeli Seçimi] */
display_type = "1.3"; // [1.3, 1.69]

/* [Elektronik ve Ekran Ölçüleri] */
pcb_w   = 59.0;  // Sol taraftan 10 mm (1 cm) genişletilmiş: Ekran pinleri ve DuPont kablo dönüşü için
pcb_h   = (display_type == "1.3") ? 32.0 : 32.2;
pcb_t   = 16.0;  // Ekran + ESP32 + MicroSD modülü derinlik payı
win_w   = (display_type == "1.3") ? 27.5 : 32.8; // Aktif ekran penceresi genişliği
win_h   = (display_type == "1.3") ? 26.5 : 28.2; // Aktif ekran penceresi yüksekliği
win_r   = (display_type == "1.3") ? 3.5  : 4.0;  // Retro tüplü TV köşe yuvarlaklığı
win_dx  = (display_type == "1.3") ? -8.0 : -6.0; // Sol kablo montaj payı için merkez kaydırma
win_dz  = 0;     // Dikey merkezleme
bat_t   = 7.0;   // Li-Po batarya payı (603035 / 703048)

/* [Giriş ve Çıkış Delikleri] */
usb_side = "left";  // [left, right, bottom, top]
usb_off  = 0;       // Kenar boyunca kaydırma
usb_y    = 7.5;     // Ön iç yüzeyden TP4056 Type-C soket merkezine derinlik
usb_len  = 10.5;    // Type-C delik genişliği
usb_dia  = 4.6;     // Type-C delik yüksekliği

sd_slot  = true;    // Sağ yanda harici MicroSD kart yuvası
sd_side  = "right"; // [left, right, bottom, top]
sd_off   = -4;      // Kenar boyunca kaydırma
sd_y     = 13.0;    // Ön iç yüzeyden MicroSD soket derinliği
sd_len   = 14.5;    // MicroSD kart genişliği (~11 mm + parmak payı)
sd_dia   = 3.2;     // Slot yüksekliği

btn_hole = true;    // Kanal / Wi-Fi / Sessize Alma butonu deliği
btn_side = "top";   // [left, right, bottom, top]
btn_off  = 8;
btn_y    = 8.0;
btn_dia  = 3.6;     // Standart 6x6mm mikro buton ve 3D basılabilir buton pimi için 3.6 mm optimize delik

/* [Gövde Yapısı] */
wall     = 1.8;  // Duvar kalınlığı
front    = 2.2;  // Ön duvar kalınlığı
clr      = 0.4;  // Kart çevresi montaj boşluğu
edge_r   = 3.0;  // Dış kasa köşe yuvarlaklığı
panel    = true; // Sağ tarafta retro düğme ve açık hoparlör ızgarası
panel_w  = 2.5;  // Sağ panel payı
back_t   = 1.6;  // Arka kapak et kalınlığı
fit      = 0.15; // Arka kapak tırnak geçme toleransı
wire_gap = 2.5;  // Kasa içi kablo ve hava boşluğu

/* [Aksesuar Ölçüleri] */
knob_d   = 7.5;  // Düğme çapı
knob_h   = 3.5;  // Düğme yüksekliği
peg_d    = 2.6;  // Düğme ve ayak montaj pimi çapı
knob_peg = 1.4;  // Kör pim derinliği (ön duvarı delmez, PCB'ye değmez)
leg_h    = 6.5;  // Ayak yüksekliği
leg_d1   = 2.2;  // Ayak zemin ucu çapı
leg_d2   = 4.2;  // Ayak gövde üstü çapı
ant_d    = 10.0; // Anten taban kubbesi çapı
wire_d   = 1.15; // Ataş / tel deliği çapı (~1 mm ataş teli için)

$fn = 48;

// ---------- Türetilmiş Ölçüler ----------
in_w  = pcb_w + 2*clr + (panel ? panel_w : 0);
in_h  = pcb_h + 2*clr;
in_d  = pcb_t + bat_t + wire_gap;
out_w = in_w + 2*wall;
out_h = in_h + 2*wall;
out_d = front + in_d;
bx    = wall + clr + pcb_w/2;
bz    = out_h/2;
wx    = bx + win_dx;
lip   = max(1.5, wire_gap - 0.3);
px    = ((wx + win_w/2) + (out_w - wall)) / 2;

echo(str("MINI RETRO TV DIS OLCULER: ", out_w, " x ", out_h, " x ", out_d + back_t, " mm"));

// ---------- Yardımcı Geometriler ----------
module rr2(w, h, r) { offset(r = r) square([w - 2*r, h - 2*r], center = true); }

module onFront() { rotate([-90, 0, 0]) mirror([0, 1, 0]) children(); }

module rbox(s, r) {
    hull() for (x = [r, s.x - r], y = [r, s.y - r], z = [r, s.z - r])
        translate([x, y, z]) sphere(r);
}

module sideHole(side, off, y, len, dia) {
    l = max(0, len - dia);
    if (side == "left" || side == "right") {
        x0 = (side == "left") ? -1 : out_w - wall - 1;
        hull() for (s = [-1, 1])
            translate([x0, front + y, bz + off + s*l/2])
                rotate([0, 90, 0]) cylinder(d = dia, h = wall + 2);
    } else {
        z0 = (side == "bottom") ? -1 : out_h - wall - 1;
        hull() for (s = [-1, 1])
            translate([bx + off + s*l/2, front + y, z0])
                cylinder(d = dia, h = wall + 2);
    }
}

leg_pos = [[7, 6], [out_w - 7, 6], [7, out_d - 6], [out_w - 7, out_d - 6]];

// ---------- 1. ANA GÖVDE (Ön yüz tabla üzerinde basılır) ----------
module body() {
    difference() {
        intersection() {
            rbox([out_w, out_d + edge_r + 1, out_h], edge_r);
            cube([out_w, out_d, out_h]); // Arka yüz düz açılır
        }
        // Kasa içi elektronik boşluğu
        translate([wall, front, wall]) cube([in_w, in_d + 1, in_h]);

        // Ekran çerçevesi (Pahlı Bezel)
        onFront() translate([wx, bz + win_dz, 0]) {
            hull() {
                translate([0, 0, -0.01]) linear_extrude(0.01) rr2(win_w + 2.4, win_h + 2.4, win_r + 1.2);
                linear_extrude(1.2) rr2(win_w, win_h, win_r);
            }
            translate([0, 0, -0.1]) linear_extrude(front + 0.2) rr2(win_w, win_h, win_r);
        }

        // Retro tüplü TV çerçeve dekor oluğu
        onFront() translate([wx, bz + win_dz, -0.01])
            linear_extrude(0.5) difference() {
                rr2(win_w + 5.4, win_h + 5.4, win_r + 2.7);
                rr2(win_w + 4.4, win_h + 4.4, win_r + 2.2);
            }

        if (panel) {
            // Düğme montaj kör delikleri
            for (zz = [out_h*0.72, out_h*0.44])
                translate([px, -0.5, zz]) rotate([-90, 0, 0]) cylinder(d = peg_d + 0.3, h = knob_peg + 0.6);
            // Açık hoparlör ızgarası (Doğrudan ses çıkışı sağlar)
            for (i = [0:3])
                onFront() translate([px, 4.5 + i*2, -0.1])
                    linear_extrude(front + 0.2) rr2(8.5, 1.1, 0.5);
        }

        // Ayak montaj delikleri
        for (p = leg_pos) translate([p[0], p[1], -1]) cylinder(d = peg_d + 0.3, h = wall + 0.6);

        // Soket delikleri
        sideHole(usb_side, usb_off, usb_y, usb_len, usb_dia);
        if (sd_slot)  sideHole(sd_side, sd_off, sd_y, sd_len, sd_dia);
        if (btn_hole) sideHole(btn_side, btn_off, btn_y, btn_dia, btn_dia);
    }
}

// ---------- 2. ARKA KAPAK (Vidasız geçme tırnaklı) ----------
module back() {
    lw = in_w - 2*fit;  lh = in_h - 2*fit;
    press = in_d - pcb_t - bat_t - 0.2; // Bataryayı sabitleyen iç kaburgalar
    difference() {
        linear_extrude(back_t) rr2(out_w, out_h, edge_r);
        // Havalandırma ve akustik ses çıkış ızgaraları
        for (i = [0:4])
            translate([0, out_h*0.18 - i*2.6, -1]) linear_extrude(back_t + 2) rr2(out_w*0.55, 1.2, 0.55);
        // Arka kabartma yazı
        translate([0, -out_h*0.28, -0.01]) mirror([1, 0, 0]) linear_extrude(0.5)
            text("MINI TV", size = 4, font = "Liberation Sans:style=Bold", halign = "center", valign = "center");
    }
    // Geçme dudağı
    translate([0, 0, back_t - 0.01]) linear_extrude(lip) difference() {
        rr2(lw, lh, 0.6);
        rr2(lw - 2.4, lh - 2.4, 0.2);
    }
    // Pil sabitleme kaburgaları
    if (press > 0.4)
        for (sx = [-1, 1]) translate([sx*lw/4 - 0.6, -lh/2 + 1, back_t - 0.01]) cube([1.2, lh - 2, press]);
}

// ---------- 3. ÇEVİRME DÜĞMESİ (Tırtıklı Retro Kanal/Ses Düğmesi) ----------
module knob() {
    difference() {
        union() {
            cylinder(d1 = knob_d - 1, d2 = knob_d, h = 0.6);
            translate([0, 0, 0.6]) cylinder(d = knob_d, h = knob_h - 0.6);
        }
        for (a = [0:30:359]) rotate(a) translate([knob_d/2 + 0.25, 0, -1]) cylinder(d = 1.1, h = knob_h + 2, $fn = 12);
        translate([-0.4, 0, -0.01]) cube([0.8, knob_d/2 - 0.8, 0.5]); // Çentik göstergesi
    }
    translate([0, 0, knob_h - 0.01]) cylinder(d = peg_d - 0.1, h = knob_peg - 0.1);
}

// ---------- 4. RETRO KONİK AYAK ----------
module leg() {
    cylinder(d1 = leg_d1, d2 = leg_d2, h = leg_h);
    translate([0, 0, leg_h - 0.01]) cylinder(d = peg_d - 0.1, h = wall - 0.3);
}

// ---------- 4b. ÜST KANAL/SES BUTON PİMİ (T-ŞEKİLLİ, %100 DESTEKSİZ) ----------
module button_cap() {
    union() {
        // Alt tutucu flanş (delikten dışarı fırlamasını engeller)
        cylinder(d = btn_dia + 2.4, h = 1.0);
        // Delikten dışarı çıkan basma şaftı
        translate([0, 0, 0.99]) cylinder(d = btn_dia - 0.5, h = wall + 2.2);
        // Üst yuvarlatılmış parmak dokunma kubbesi
        translate([0, 0, 1.0 + wall + 2.2]) scale([1, 1, 0.7]) sphere(d = btn_dia - 0.5);
    }
}

// ---------- 5. YEKPARE V-ANTEN (3D BASKIYA UYGUN, %100 DESTEKSİZ) ----------
module antenna_solid() {
    difference() {
        union() {
            // Taban kubbesi ve alt flanş
            scale([1, 1, 0.55]) difference() {
                sphere(d = ant_d);
                translate([0, 0, -ant_d]) cube(2*ant_d, center = true);
            }
            cylinder(d = ant_d + 3, h = 0.8);
            
            // 2 adet açılı V anten çubuğu (28 derece açılı, 62° eğimle desteksiz basılır)
            for (s = [-1, 1]) translate([0, 0, 1.0])
                rotate([0, s*28, 0]) {
                    cylinder(d1 = 2.2, d2 = 1.8, h = 24);
                    translate([0, 0, 24]) sphere(d = 3.6);
                }
        }
        // Tabanı kusursuz düzleştir (Tabla üzerine tam yapışması için)
        translate([0, 0, -2]) cylinder(d = ant_d + 10, h = 2);
    }
}

// ---------- 6. GELENEKSEL TEL/ATAŞ ANTEN TABANI + BONCUKLAR ----------
module antenna_base() {
    difference() {
        union() {
            scale([1, 1, 0.55]) difference() { sphere(d = ant_d); translate([0, 0, -ant_d]) cube(2*ant_d, center = true); }
            cylinder(d = ant_d + 3, h = 0.8);
        }
        for (s = [-1, 1]) translate([0, 0, 1.2]) rotate([0, s*28, 0]) cylinder(d = wire_d, h = ant_d, $fn = 16);
    }
}

module bead() {
    difference() {
        sphere(d = 3.2, $fn = 24);
        translate([0, 0, -2]) cylinder(d = wire_d, h = 3.2, $fn = 12);
    }
}

// ---------- 7. MONTAJLI ÖNİZLEME ----------
module preview() {
    color("#c8452f") body();
    color("#f1e6cf") translate([out_w/2, out_d + back_t, out_h/2]) rotate([90, 0, 0]) back();
    color("#151515") onFront() translate([wx, bz + win_dz, 0.8]) linear_extrude(0.4) rr2(win_w, win_h, win_r);
    if (panel) for (zz = [out_h*0.72, out_h*0.44])
        color("#f1e6cf") translate([px, -knob_h, zz]) rotate([-90, 0, 0]) knob();
    for (p = leg_pos) color("#6b4a2b") translate([p[0], p[1], -leg_h]) leg();
    color("#f1e6cf") translate([wx, out_d/2, out_h - 0.3]) antenna_solid();
    // Üst Kanal/Ses Buton Pimi
    color("#f1e6cf") translate([bx + btn_off, front + btn_y, out_h - wall - 1.0]) button_cap();
}

// ---------- ÇIKTI MODLARI ----------
if (part == "preview")              preview();
else if (part == "body")            rotate([90, 0, 0]) body();      // Ön yüz z=0'da (Tabla)
else if (part == "back")            back();
else if (part == "knobs")           for (i = [0:1]) translate([i*11, 0, 0]) knob();
else if (part == "legs")            for (i = [0:3]) translate([i*8, 0, 0]) leg();
else if (part == "button_cap")      button_cap();
else if (part == "antenna_solid")   antenna_solid();
else if (part == "antenna")         { antenna_base(); for (i = [0:1]) translate([12 + i*6, 0, 1.6]) bead(); }
else if (part == "plate") {
    // 1. Ana Gövde (Ön yüz tablaya düz basar)
    translate([0, out_h, 0]) rotate([90, 0, 0]) body();
    // 2. Arka Kapak (Düz arka yüzey tablaya basar)
    translate([out_w + 8 + out_w/2, out_h/2, 0]) back();
    // 3. Çevirme Düğmeleri (2 Adet)
    for (i = [0:1]) translate([5 + i*11, out_h + 10, 0]) knob();
    // 4. Konik Retro Ayaklar (4 Adet)
    for (i = [0:3]) translate([30 + i*8, out_h + 10, 0]) leg();
    // 5. Yekpare V-Anten (Desteksiz, 100% 3D basılabilir)
    translate([out_w + 24, out_h + 16, 0]) antenna_solid();
    // 6. Üst Kanal Buton Pimi (T-Şekilli)
    translate([out_w + 40, out_h + 10, 0]) button_cap();
}
