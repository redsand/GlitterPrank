// Celebration Confetti Dropper - OpenSCAD
// Parametric, split for 200x200mm print beds

part = "left_body"; // left_body, right_body, lid_left, lid_right, gate, alignment_pin, servo_horn_adapter

// Global parameters
body_length = 260; // mm
body_width = 55;
body_height = 55;
wall = 2.4;

half_length = body_length / 2;

chute_length = 70;
chute_width = 28;

magnet_d = 10.2;
magnet_depth = 3.2;
magnet_rows = 2;
magnet_cols = 2;

m3_clear = 3.4;
m3_pilot = 2.8;

alignment_pin_d = 5;
alignment_pin_l = 12;

hinge_pin_d = 3.2; // use 3mm filament or rod
hinge_knuckle_len = 8;

servo_bay_l = 26;
servo_bay_w = 13;
servo_bay_h = 23;
servo_bay_offset = 20; // from left end

lip_height = 2.0;
lid_thickness = 2.0;

// Helper modules
module rounded_cube(size, r=2) {
  minkowski() {
    cube([size[0]-2*r, size[1]-2*r, size[2]-2*r]);
    sphere(r);
  }
}

module magnet_pockets(half_len) {
  pocket_x = [half_len * 0.25, half_len * 0.75];
  pocket_z = [body_height * 0.25, body_height * 0.75];
  for (x = pocket_x) {
    for (z = pocket_z) {
      translate([x, body_width - wall, z])
        rotate([90, 0, 0])
          cylinder(d = magnet_d, h = magnet_depth, $fn = 48);
    }
  }
}

module screw_bosses(half_len, is_left=true) {
  boss_positions = [
    [half_len - 12, 12, body_height * 0.3],
    [half_len - 12, body_width - 12, body_height * 0.3],
    [half_len - 12, 12, body_height * 0.7],
    [half_len - 12, body_width - 12, body_height * 0.7]
  ];
  for (p = boss_positions) {
    translate(p)
      rotate([0, 90, 0])
        cylinder(d = is_left ? m3_clear : m3_pilot, h = wall * 2, $fn = 32);
  }
}

module alignment_pin_holes(half_len, is_left=true) {
  pin_positions = [
    [half_len - 20, body_width * 0.35, body_height * 0.5],
    [half_len - 20, body_width * 0.65, body_height * 0.5]
  ];
  for (p = pin_positions) {
    translate(p)
      rotate([0, 90, 0])
        cylinder(d = is_left ? alignment_pin_d + 0.3 : alignment_pin_d - 0.2,
                 h = wall * 2, $fn = 32);
  }
}

module hinge_mounts(x_offset) {
  knuckle_positions = [
    x_offset + (chute_length / 2) - hinge_knuckle_len,
    x_offset + (chute_length / 2),
    x_offset + (chute_length / 2) + hinge_knuckle_len
  ];
  for (kx = knuckle_positions) {
    difference() {
      translate([kx, (body_width - chute_width) / 2, 0])
        cube([hinge_knuckle_len, hinge_knuckle_len, hinge_pin_d + 4]);
      translate([kx + hinge_knuckle_len / 2, (body_width - chute_width) / 2 + hinge_knuckle_len / 2, 0])
        rotate([90, 0, 0])
          cylinder(d = hinge_pin_d + 0.4, h = hinge_knuckle_len + 1, $fn = 32);
    }
  }
}

module body_half(is_left=true) {
  half_len = body_length / 2;
  x_offset = is_left ? 0 : 0;

  difference() {
    rounded_cube([half_len, body_width, body_height], r=2);

    // Hollow interior
    translate([wall, wall, wall])
      cube([half_len - 2*wall, body_width - 2*wall, body_height - 2*wall]);

    // Chute aperture
    translate([
      (half_len - chute_length) / 2,
      (body_width - chute_width) / 2,
      -0.1
    ])
      cube([chute_length, chute_width, wall + 0.2]);

    // Servo bay (left half only)
    if (is_left) {
      translate([servo_bay_offset, wall, body_height - servo_bay_h - wall])
        cube([servo_bay_l, servo_bay_w, servo_bay_h]);
    }

    // Magnet pockets (rear face)
    magnet_pockets(half_len);

    // Screw holes and alignment pins on split face
    screw_bosses(half_len, is_left);
    alignment_pin_holes(half_len, is_left);
  }

  // Hinge mounts on bottom edge (one side of chute)
  hinge_mounts((half_len - chute_length) / 2);

  // Gate stop ridge
  translate([
    (half_len - chute_length) / 2,
    (body_width - chute_width) / 2 + chute_width - 3,
    wall
  ])
    cube([chute_length, 3, 2]);
}

module lid_half(is_left=true) {
  half_len = body_length / 2;
  difference() {
    cube([half_len, body_width, lid_thickness]);
    // screw clearance to align with bosses
    screw_bosses(half_len, true);
  }
  // lip to reduce spill
  translate([wall, wall, lid_thickness])
    cube([half_len - 2*wall, body_width - 2*wall, lip_height]);
}

module gate_flap() {
  flap_len = chute_length + 6;
  flap_w = chute_width + 6;
  flap_t = 2.4;

  // main plate
  cube([flap_len, flap_w, flap_t]);

  // hinge barrel along one long edge
  translate([0, 0, flap_t])
    difference() {
      rotate([0, 90, 0])
        cylinder(d = hinge_pin_d + 4, h = flap_len, $fn = 32);
      rotate([0, 90, 0])
        cylinder(d = hinge_pin_d + 0.4, h = flap_len + 0.2, $fn = 32);
    }

  // pusher tab near servo side
  translate([flap_len - 10, flap_w / 2 - 3, flap_t])
    cube([8, 6, 6]);
}

module alignment_pin() {
  cylinder(d = alignment_pin_d, h = alignment_pin_l, $fn = 32);
}

module servo_horn_adapter() {
  // simple adapter for SG90 horn
  difference() {
    cylinder(d = 12, h = 4, $fn = 48);
    cylinder(d = 2.2, h = 5, $fn = 24);
  }
}

if (part == "left_body") {
  body_half(true);
} else if (part == "right_body") {
  body_half(false);
} else if (part == "lid_left") {
  lid_half(true);
} else if (part == "lid_right") {
  lid_half(false);
} else if (part == "gate") {
  gate_flap();
} else if (part == "alignment_pin") {
  alignment_pin();
} else if (part == "servo_horn_adapter") {
  servo_horn_adapter();
}
