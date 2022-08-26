#define DEBUGGING true
#define BAUDRATE  115200


#define OUT_A   2 // CLK
#define OUT_B   3 // DT
#define SWITCH  4

bool SwitchReleased = true;

float CurrentAttanuation = 0;
float LastAttanuation = 0;
float SetAttanuation = 0;
const float ATTAN_MIN = 0;
const float ATTAN_MAX = 31.75;
const float ATTAN_STEP = 0.25;


#define SCREEN_ADDR     0x3D
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
bool isDisplay = false;


#define ATT1_SS_PIN 5
#define ATT2_SS_PIN 6
