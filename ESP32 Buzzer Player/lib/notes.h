#define NOTE_H   0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_REST 0
#define REST 0

// flat aliases
#define NOTE_DB1 NOTE_CS1
#define NOTE_EB1 NOTE_DS1
#define NOTE_GB1 NOTE_FS1
#define NOTE_AB1 NOTE_GS1
#define NOTE_BB1 NOTE_AS1

#define NOTE_DB2 NOTE_CS2
#define NOTE_EB2 NOTE_DS2
#define NOTE_GB2 NOTE_FS2
#define NOTE_AB2 NOTE_GS2
#define NOTE_BB2 NOTE_AS2

#define NOTE_DB3 NOTE_CS3
#define NOTE_EB3 NOTE_DS3
#define NOTE_GB3 NOTE_FS3
#define NOTE_AB3 NOTE_GS3
#define NOTE_BB3 NOTE_AS3

#define NOTE_DB4 NOTE_CS4
#define NOTE_EB4 NOTE_DS4
#define NOTE_GB4 NOTE_FS4
#define NOTE_AB4 NOTE_GS4
#define NOTE_BB4 NOTE_AS4

#define NOTE_DB5 NOTE_CS5
#define NOTE_EB5 NOTE_DS5
#define NOTE_GB5 NOTE_FS5
#define NOTE_AB5 NOTE_GS5
#define NOTE_BB5 NOTE_AS5

#define NOTE_DB6 NOTE_CS6
#define NOTE_EB6 NOTE_DS6
#define NOTE_GB6 NOTE_FS6
#define NOTE_AB6 NOTE_GS6
#define NOTE_BB6 NOTE_AS6

#define NOTE_DB7 NOTE_CS7
#define NOTE_EB7 NOTE_DS7
#define NOTE_GB7 NOTE_FS7
#define NOTE_AB7 NOTE_GS7
#define NOTE_BB7 NOTE_AS7

#define NOTE_DB8 NOTE_CS8
#define NOTE_EB8 NOTE_DS8

struct Note {
  uint16_t freq;       // Hz, 0 = rest
  uint16_t duration;   // milliseconds
};

struct Track {
  const Note* notes;
  uint16_t count;
  uint16_t idx;
  uint32_t left;     // samples left in current note
  uint32_t total;    // total samples in current note
  uint32_t phase;
  uint32_t step;
  bool done;
};