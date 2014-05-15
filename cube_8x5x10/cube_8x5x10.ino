// Mapping for the grounds (analog in ports 0-9)
const int g0 = A0;
const int g1 = A1;
const int g2 = A2;
const int g3 = A3;
const int g4 = A4;
const int g5 = A5;
const int g6 = A6;
const int g7 = A7;
const int g8 = A8;
const int g9 = A9;

// Mapping for the LEDs (22-53, skip 42,43, 6-13)
const int p0 = 22;
const int p1 = 23;
const int p2 = 24;
const int p3 = 25;
const int p4 = 26;
const int p5 = 27;
const int p6 = 28;
const int p7 = 29;
const int p8 = 30;
const int p9 = 31;
const int p10 = 32;
const int p11 = 33;
const int p12 = 34;
const int p13 = 35;
const int p14 = 36;
const int p15 = 37;
const int p16 = 38;
const int p17 = 39;
const int p18 = 40;
const int p19 = 41;
const int p20 = 42;
const int p21 = 43;
// skip 44 and 45
const int p22 = 46;
const int p23 = 47;
const int p24 = 48;
const int p25 = 49;
const int p26 = 50;
const int p27 = 51;
const int p28 = 52;
const int p29 = 53;

const int p30 = 4;
const int p31 = 5;
const int p32 = 6;
const int p33 = 7;
const int p34 = 8;
const int p35 = 9;
const int p36 = 10;
const int p37 = 11;
const int p38 = 12;
const int p39 = 13;


const int ground[] = {g0, g1, g2, g3, g4, g5, g6, g7, g8, g9};

int positive2[][5] = {{p7, p15, p23, p31, p39},
                      {p6, p14, p22, p30, p38},
                      {p5, p13, p21, p29, p37},
                      {p4, p12, p20, p28, p36},
                      {p3, p11, p19, p27, p35},
                      {p2, p10, p18, p26, p34},
                      {p1, p9, p17, p25, p33},
                      {p0, p8, p16, p24, p32}};

const int ground_length = sizeof(ground) / sizeof(ground[0]);
int matrix[8][5][10];

// Initial settings for paddle 1, paddle 2, and the puck
int paddle1_x = 2;
int paddle1_y = 1;
int paddle1_z = 0;
int paddle2_x = 2;
int paddle2_y = 1;
int paddle2_z = 9;
int puck_x = 3;
int puck_y = 2;
int puck_z = 4;


String input;

void setup() {
  
  // initialize serial communication to 9600 bits per second:
  //Serial.begin(1843200);
  Serial.begin(115200);
  
  input = "";

  // set all matrix elements to 0
  memset(matrix, 0, sizeof(matrix));

  // set all ground and led pins to OUTPUT
  for (int i = 0; i < ground_length; i++) {
    pinMode(ground[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 5; j++) {
      pinMode(positive2[i][j], OUTPUT);
    }
  }
  
  // turn everything off initially
  for (int i = 0; i < ground_length; i++) {
    digitalWrite(ground[i], HIGH);  // will act as ground when LOW;
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 5; j++) {
      digitalWrite(positive2[i][j], LOW);
    }
  }
  /*pinMode(2,OUTPUT);    // port B pin 25  
  analogWrite(2,255);   // sets up some other registers I haven't worked out yet
  REG_PIOB_PDR = 1<<25; // disable PIO, enable peripheral
  REG_PIOB_ABSR= 1<<25; // select peripheral B
  REG_TC0_WPMR=0x54494D00; // enable write to registers
  REG_TC0_CMR0=0b00000000000010011100010000000000; // set channel mode register (see datasheet)
  REG_TC0_RC0=100; // counter period
  REG_TC0_RA0=30000000;  // PWM value
  REG_TC0_CCR0=0b101;    // start counter
  REG_TC0_IER0=0b00010000; // enable interrupt on counter=rc
  REG_TC0_IDR0=0b11101111; // disable other interrupts

  NVIC_EnableIRQ(TC0_IRQn); // enable TC0 interrupts*/
}

/*void TC0_Handler()
{
    long dummy=REG_TC0_SR0; // vital - reading this clears some flag
                            // otherwise you get infinite interrupts
    //display_matrix(matrix);
    //Serial.println("blah");
}*/
char serInput[20];

void loop() {
  input = "";
  char inChar;
  int idx = 0;
  memset(serInput, 0, sizeof(serInput));
  while (Serial.available()) {
    inChar = Serial.read();
    //if (inChar != ' ')
    serInput[idx++] = inChar;
    display_matrix(matrix);
    //input = Serial.readString();
  }
  input = String(serInput);
 
  //input = simulate();
  input.trim();
  if (input.length() > 0) {
    bool continue_parsing = 1;
    int first_pos = input.indexOf(':');
    int last_pos = input.lastIndexOf(':');
    
    continue_parsing = first_pos != last_pos ? 1 : 0;
    if (!continue_parsing) { Serial.println("Error1"); return; }
    
    String paddle1 = input.substring(0, first_pos); paddle1.trim();
    String paddle2 = input.substring(first_pos + 1, last_pos); paddle2.trim();
    String puck = input.substring(last_pos + 1); puck.trim();

    // Parse out the coordinates for paddle 1
    first_pos = paddle1.indexOf(',');
    last_pos = paddle1.lastIndexOf(',');
    
    continue_parsing = first_pos != last_pos ? 1 : 0;
    if (!continue_parsing) { Serial.println("Error2"); return; }

    int p1_x = (paddle1.substring(0, first_pos)).toInt();
    int p1_y = (paddle1.substring(first_pos + 1, last_pos)).toInt();
    int p1_z = (paddle1.substring(last_pos + 1)).toInt();

    // Parse out the coordinates for paddle 2
    first_pos = paddle2.indexOf(',');
    last_pos = paddle2.lastIndexOf(',');
    
    continue_parsing = first_pos != last_pos ? 1 : 0;
    if (!continue_parsing) { Serial.println("Error3"); return; }
    
    int p2_x = (paddle2.substring(0, first_pos)).toInt();
    int p2_y = (paddle2.substring(first_pos + 1, last_pos)).toInt();
    int p2_z = (paddle2.substring(last_pos + 1)).toInt();

    // Parse out the coordinates for the puck
    first_pos = puck.indexOf(',');
    last_pos = puck.lastIndexOf(',');
    
    continue_parsing = first_pos != last_pos ? 1 : 0;
    if (!continue_parsing) { Serial.println("Error4"); return; }
    
    int pu_x = (puck.substring(0, first_pos)).toInt();
    int pu_y = (puck.substring(first_pos + 1, last_pos)).toInt();
    int pu_z = (puck.substring(last_pos + 1)).toInt();
    
    // Bounds Checking
    if ((p1_x >= 0 && p1_x <= 5) && (p2_x >= 0 && p2_x <= 5) && (pu_x >= 0 && pu_x <= 7) &&
        (p1_y >= 0 && p1_y <= 4) && (p2_y >= 0 && p2_y <= 4) && (pu_y >= 0 && pu_y <= 5) &&
        (p1_z >= 0 && p1_z <= 9) && (p2_z >= 0 && p2_z <= 9) && (pu_z >= 0 && pu_z <= 9)) {
          
          
          paddle1_x = p1_x;
          paddle1_y = p1_y;
          paddle1_z = p1_z;
          paddle2_x = p2_x;
          paddle2_y = p2_y;
          paddle2_z = p2_z;
          puck_x = pu_x;
          puck_y = pu_y;
          puck_z = pu_z;          
        }
  }
  
  // enable paddle 1 and paddle 2 LEDs
  for (int i = 0; i < 3; i++) {
    for (int j=0; j<2; j++) {
      matrix[paddle1_x + i][paddle1_y+j ][paddle1_z] = 1;
      matrix[paddle2_x + i][paddle2_y+j ][paddle2_z] = 1;
    }
  }
  // enable the puck LED
  matrix[puck_x][puck_y][puck_z] = 1;


  // flick all the lights on/off
  display_matrix(matrix);
}


/*
 *  Turn on lights corresponding to the matrix parameter
 */
void display_matrix(int matrix[][5][10]) {

  for (int i = 0; i < 10; i++) {
    digitalWrite(ground[i], LOW);  // enable ground
    
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 5; k++) {
        if (matrix[j][k][i]) {
          //Serial.println("matrix[" + String(j) + "][" + String(k) + "][" + String(i) + "] = 1");
          
          // revert matrix cell back to 0 (off)
          matrix[j][k][i] = 0;
          digitalWrite(positive2[j][k], HIGH);
        }
      }
    }
    
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 5; k++) {
        digitalWrite(positive2[j][k], LOW);
      }
    }
    
    digitalWrite(ground[i], HIGH);  // disable ground
  }
}


/*
 * simulate() is just a temporary function used to simulate data.
 * Should be removed after testing is done.
 *
 */
int counter = 1;
int counter2 = 0;
int counter3 = 2;
String simulate() {
  if (counter2 % 500 == 0) {
    //Serial.println("\n\nCHANGED\n\n");
    counter++;
    counter3++;
  }
  String result = String(counter3%6) + ",0,0:" + String(counter%6) + ",1,9:5,1,4";
  counter2++;
  return result;
}
