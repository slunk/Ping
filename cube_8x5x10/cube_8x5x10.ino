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


const int ground[] = {g0,g1,g2,g3,g4,g5,g6,g7,g8,g9};
const int positive[] = {p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,
                        p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,
                        p30,p31,p32,p33,p34,p35,p36,p37,p38,p39};


const int ground_length = sizeof(ground)/sizeof(ground[0]);
const int positive_length = sizeof(positive)/sizeof(positive[0]);

int matrix[8][5][10];

void setup() {
  // initialize serial communication to 9600 bits per second:
  Serial.begin(9600);
  
  // set all matrix elements to 0
  memset(matrix,0, sizeof(matrix));

  // set all ground and led pins to OUTPUT
  for (int i=0; i<ground_length; i++) {
    pinMode(ground[i], OUTPUT);
  }
  for (int i=0; i<positive_length; i++) {
    pinMode(positive[i], OUTPUT);
  }
  
  
  // turn everything off initially
  for (int i=0; i<ground_length; i++) {
    digitalWrite(ground[i], HIGH);  // will act as ground when LOW;
  }
  for (int i=0; i<positive_length; i++) {
    digitalWrite(positive[i], LOW);
  }
  
  //digitalWrite(ground[0], LOW);  // enable ground for the layer with paddle 1
  //digitalWrite(ground[ground_length-1], LOW);  // enable ground for the layer with paddle 2
}

void loop() {
  delay(1000);
  /*
  if (Serial.available()) {
    //String input = Serial.readString();
    String input = simulate();
    Serial.println(input);
  }
  */
  String input = simulate();
  input.trim();
  if (input.length() > 0) {
    bool continue_parsing = 1;
    int first_pos = input.indexOf(':');
    int last_pos = input.lastIndexOf(':');
    
    continue_parsing = first_pos != last_pos ? 1:0;
    if (!continue_parsing) { Serial.println("Error1"); return; }
    
    String paddle1 = input.substring(0,first_pos); paddle1.trim();
    String paddle2 = input.substring(first_pos+1,last_pos); paddle2.trim();
    String puck = input.substring(last_pos+1); puck.trim();
    
    // Parse out the coordinates for paddle 1
    first_pos = paddle1.indexOf(',');
    last_pos = paddle1.lastIndexOf(',');
    
    continue_parsing = first_pos != last_pos ? 1:0;
    if (!continue_parsing) { Serial.println("Error2"); return; }
    
    int paddle1_x = (paddle1.substring(0,first_pos)).toInt();
    int paddle1_y = (paddle1.substring(first_pos+1,last_pos)).toInt();
    int paddle1_z = (paddle1.substring(last_pos+1)).toInt();
    
    // Parse out the coordinates for paddle 2
    first_pos = paddle2.indexOf(',');
    last_pos = paddle2.lastIndexOf(',');
    continue_parsing = first_pos != last_pos ? 1:0;
    if (!continue_parsing) { Serial.println("Error3"); return; }
    int paddle2_x = (paddle2.substring(0,first_pos)).toInt();
    int paddle2_y = (paddle2.substring(first_pos+1,last_pos)).toInt();
    int paddle2_z = (paddle2.substring(last_pos+1)).toInt();
    
    // Parse out the coordinates for the puck
    first_pos = puck.indexOf(',');
    last_pos = puck.lastIndexOf(',');
    if (!continue_parsing) { Serial.println("Error4"); return; }
    int puck_x = (puck.substring(0,first_pos)).toInt();
    int puck_y = (puck.substring(first_pos+1,last_pos)).toInt();
    int puck_z = (puck.substring(last_pos+1)).toInt();
    
    // Now take the coordinates and map them to the array
    
    for (int i=0; i<8; i++) {
      for (int j=0; j<5; j++) {
        for (int k=0; j<10; k++) {
          // set matrix values to 1 or 0
          }
        }
      }
    }
    
    // Turn on Paddle 1 LEDs
    int index = (7-paddle1_x) + 8*paddle1_y;
    for (int i=0; i<8; i++) {
      if ((i+(8*paddle1_y)) >= index-2 && (i+(8*paddle1_y)) <= index) {
        digitalWrite(positive[i + (8 * paddle1_y) ], HIGH);  // enable LED
      }
      else {
        digitalWrite(positive[i + (8 * paddle1_y)], LOW);
      }
    }
    
    // Turn on Paddle 2 LEDs
    index = (7-paddle2_x) + 8*paddle2_y;
    for (int i=0; i<8; i++) {
      if ((i+(8*paddle2_y)) >= index-2 && (i+(8*paddle2_y)) <= index) {
        digitalWrite(positive[i + (8*paddle2_y) ], HIGH);  // enable LED
      }
      else {
        digitalWrite(positive[i + (8*paddle2_y)], LOW);
      }
    }
    
  }
  else {
    Serial.println("Error5");
  }
  
  /*
   * Goes through and blinks all lights real fast to simulate them all being on at the same time
   */
  /*
  for (int i=0; i<ground_length; i++) {
    digitalWrite(ground[i], LOW);
  }
  delay(15);
  for (int i=0; i<ground_length; i++) {
    digitalWrite(ground[i], HIGH);
  }
  */
}

int counter = 0;
int counter2 = 0;

String simulate() {
  String result = String(counter%6) + ",1,0:" + String(counter%6) + ",3,1:2,2,2";
  counter++;
  counter2++;
  return result;
}

void display_matrix(int* matrix) {
  
}

