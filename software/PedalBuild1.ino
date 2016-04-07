#include <OSCBundle.h>
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial);
 
int LOAD = 12;
int DATA = 11;
int CLK = 10;

byte temp1[5] = {0,0,0,0,0};
byte temp2[5] = {0,0,0,0,0}; 
byte temp3[5] = {0,0,0,0,0};
byte temp4[5] = {0,0,0,0,0};

byte serialinput[100];   //temparray for serial buffer input
byte displayarray[100];  //filled buffer with correct details


int gotserial = 0;
int currentarraylength = 0;

int language = 0;     //default to latin alphabet
int marqueeCheck = 0; //default to scroll 


int address = 0;     //track array read location
int serialsize = 0;
int interval = 100;
unsigned long prevMillis = 0;

static byte alphabet[][5] =
{
  {0x00, 0x20, 0x40, 0x60, 0x80}, //BLANK
  {0x04, 0x24, 0x44, 0x60, 0x84}, //!
  {0x0A, 0x2A, 0x40, 0x60, 0x80}, //"
  {0x0A, 0x3F, 0x4A, 0x7F, 0x8A}, //#
  {0x0F, 0x34, 0x4E, 0x65, 0x9E}, //$
  {0x19, 0x3A, 0x44, 0x6B, 0x93}, //%
  {0x08, 0x34, 0x45, 0x72, 0x8D}, //&
  {0x0C, 0x2C, 0x44, 0x68, 0x80}, // APOSTROPHE CLOSED
  {0x02, 0x24, 0x44, 0x64, 0x82}, // (
  {0x08, 0x24, 0x44, 0x64, 0x88}, // )
  {0x0C, 0x2C, 0x48, 0x64, 0x80}, // ' APOST OPEN
  {0x04, 0x24, 0x5F, 0x64, 0x84}, //+
  {0x00, 0x2C, 0x4C, 0x64, 0x88}, //COMMA
  {0x00, 0x20, 0x5F, 0x60, 0x80}, //HYPHEN
  {0x00, 0x20, 0x40, 0x6C, 0x8C}, //PERIOD
  {0x01, 0x22, 0x44, 0x68, 0x90}, // / 

  {0x0E, 0x33, 0x55, 0x79, 0x8E}, //0
  {0x04, 0x2C, 0x44, 0x64, 0x8E}, //1
  {0x1E, 0x21, 0x46, 0x68, 0x9F}, //2
  {0x1E, 0x21, 0x4E, 0x61, 0x9E}, //3
  {0x06, 0x2A, 0x5F, 0x62, 0x82}, //4
  {0x1F, 0x30, 0x5E, 0x61, 0x9E}, //5
  {0x06, 0x28, 0x5E, 0x71, 0x8E}, //6
  {0x1F, 0x22, 0x44, 0x68, 0x88}, //7
  {0x0E, 0x31, 0x4E, 0x71, 0x8E}, //8
  {0x0E, 0x31, 0x4F, 0x62, 0x8C}, //9

  {0x0C, 0x2C, 0x40, 0x6C, 0x8C}, //:
  {0x0C, 0x20, 0x4C, 0x64, 0x88}, //; SEMICOLON
  {0x02, 0x24, 0x48, 0x64, 0x82}, //<
  {0x00, 0x3F, 0x40, 0x7F, 0x80}, //=
  {0x08, 0x24, 0x42, 0x64, 0x88}, //>
  {0x0E, 0x31, 0x42, 0x64, 0x88}, //?
  {0x0E, 0x35, 0x57, 0x70, 0x8E}, //@

  {0x04, 0x2A, 0x5F, 0x71, 0x91}, //A
  {0x1E, 0x29, 0x4E, 0x69, 0x9E}, //B
  {0x0F, 0x30, 0x50, 0x70, 0x8F}, //C
  {0x1E, 0x29, 0x49, 0x69, 0x9E}, //D
  {0x1F, 0x30, 0x5E, 0x70, 0x9F}, //E
  {0x1F, 0x30, 0x5E, 0x70, 0x90}, //F
  {0x0F, 0x30, 0x53, 0x71, 0x8F}, //G
  {0x11, 0x31, 0x5F, 0x71, 0x91}, //H
  {0x0E, 0x24, 0x44, 0x64, 0x8E}, //I
  {0x01, 0x21, 0x41, 0x71, 0x8E}, //J
  {0x13, 0x34, 0x58, 0x74, 0x93}, //K
  {0x10, 0x30, 0x50, 0x70, 0x9F}, //L
  {0x11, 0x3B, 0x55, 0x71, 0x91}, //M
  {0x11, 0x39, 0x55, 0x73, 0x91}, //N
  {0x0E, 0x31, 0x51, 0x71, 0x8E}, //O
  {0x1E, 0x31, 0x5E, 0x70, 0x90}, //P
  {0x0C, 0x32, 0x56, 0x72, 0x8D}, //Q
  {0x1E, 0x31, 0x5E, 0x74, 0x92}, //R
  {0x0F, 0x30, 0x4E, 0x61, 0x9E}, //S
  {0x1F, 0x24, 0x44, 0x64, 0x84}, //T
  {0x11, 0x31, 0x51, 0x71, 0x8E}, //U
  {0x11, 0x31, 0x51, 0x6A, 0x84}, //V
  {0x11, 0x31, 0x55, 0x7B, 0x91}, //W
  {0x11, 0x2A, 0x44, 0x6A, 0x91}, //X
  {0x11, 0x2A, 0x44, 0x64, 0x84}, //Y
  {0x1F, 0x22, 0x44, 0x68, 0x9F}, //Z
    
  {0x07, 0x24, 0x44, 0x64, 0x87}, //[
  {0x10, 0x28, 0x44, 0x62, 0x81}, //\
  {0x1C, 0x24, 0x44, 0x64, 0x9C}, //]
  {0x04, 0x2A, 0x51, 0x60, 0x80}, //^
  {0x00, 0x20, 0x40, 0x60, 0x9F}, //_
  {0x02, 0x22, 0x40, 0x60, 0x80}, //' 

  {0x00, 0x2E, 0x52, 0x72, 0x8D}, //a
  {0x10, 0x30, 0x5E, 0x71, 0x9E}, //b
  {0x00, 0x2F, 0x50, 0x70, 0x8F}, //c
  {0x01, 0x21, 0x4F, 0x71, 0x8F}, //d
  {0x00, 0x2E, 0x5F, 0x70, 0x8E}, //e
  {0x04, 0x2A, 0x48, 0x7C, 0x88}, //f
  {0x00, 0x2F, 0x50, 0x73, 0x8F}, //g
  {0x10, 0x30, 0x56, 0x79, 0x91}, //h
  {0x04, 0x20, 0x4C, 0x64, 0x8E}, //i
  {0x00, 0x26, 0x42, 0x72, 0x8C}, //j
  {0x10, 0x30, 0x56, 0x78, 0x96}, //k
  {0x0C, 0x24, 0x44, 0x64, 0x8E}, //l
  {0x00, 0x2A, 0x55, 0x71, 0x91}, //m
  {0x00, 0x36, 0x59, 0x71, 0x91}, //n
  {0x00, 0x2E, 0x51, 0x71, 0x8E}, //o
  {0x00, 0x3E, 0x51, 0x7E, 0x90}, //p
  {0x00, 0x2F, 0x51, 0x6F, 0x81}, //q
  {0x00, 0x33, 0x54, 0x78, 0x90}, //r
  {0x00, 0x23, 0x44, 0x62, 0x8C}, //s
  {0x08, 0x3C, 0x48, 0x6A, 0x84}, //t
  {0x00, 0x32, 0x52, 0x72, 0x8D}, //u
  {0x00, 0x31, 0x51, 0x6A, 0x84}, //v
  {0x00, 0x31, 0x55, 0x7B, 0x91}, //w
  {0x00, 0x32, 0x4C, 0x6C, 0x92}, //x
  {0x00, 0x31, 0x4A, 0x64, 0x98}, //y
  {0x00, 0x3E, 0x44, 0x68, 0x9E},  //z
  {0x04, 0x2E, 0x55, 0x64, 0x84}, //ARROW UP
  {0x04, 0x24, 0x55, 0x6E, 0x84}, //ARROWN DOWN
  {0x04, 0x22, 0x5F, 0x62, 0x84}, //ARROW LEFT
  {0x04, 0x28, 0x5F, 0x68, 0x84} //ARROW RIGHT
};

static byte kanji[35][5] =
{
  {0x00, 0x20, 0x40, 0x60, 0x80}, //BLANK
  {0x1F, 0x21, 0x5F, 0x62, 0x84},
  {0x1F, 0x21, 0x46, 0x64, 0x88},
  {0x01, 0x22, 0x46, 0x6A, 0x82},
  {0x04, 0x3F, 0x51, 0x61, 0x86},
  {0x00, 0x3F, 0x44, 0x64, 0x9F},
  {0x02, 0x3F, 0x46, 0x6A, 0x92},
  {0x08, 0x3F, 0x49, 0x6A, 0x88},
  {0x1F, 0x21, 0x45, 0x67, 0x8C},
  {0x02, 0x3F, 0x51, 0x62, 0x8C},
  {0x08, 0x3F, 0x49, 0x69, 0x92},
  {0x04, 0x3F, 0x44, 0x7F, 0x84},
  {0x0F, 0x29, 0x51, 0x62, 0x8C},
  {0x08, 0x2F, 0x52, 0x62, 0x82},
  {0x0F, 0x21, 0x41, 0x61, 0x9F},
  {0x0A, 0x3F, 0x4A, 0x62, 0x8C},
  {0x19, 0x21, 0x59, 0x62, 0x9C},
  {0x0F, 0x29, 0x55, 0x63, 0x8C},
  {0x01, 0x3E, 0x42, 0x7F, 0x86},
  {0x15, 0x35, 0x55, 0x62, 0x8C},
  {0x0E, 0x20, 0x5F, 0x64, 0x98},
  {0x08, 0x28, 0x4C, 0x6A, 0x90},
  {0x04, 0x3F, 0x44, 0x64, 0x98},
  {0x0E, 0x20, 0x40, 0x60, 0x9F},
  {0x1F, 0x21, 0x4A, 0x64, 0x9A},
  {0x04, 0x3E, 0x44, 0x6E, 0x95},
  {0x04, 0x24, 0x44, 0x68, 0x90},
  {0x04, 0x22, 0x51, 0x71, 0x91},
  {0x10, 0x3F, 0x50, 0x70, 0x8F},
  {0x1F, 0x21, 0x41, 0x62, 0x8C},
  {0x0E, 0x20, 0x4E, 0x60, 0x8F},
  {0x04, 0x28, 0x51, 0x7F, 0x81},
  {0x01, 0x21, 0x4A, 0x64, 0x8A},
  {0x1F, 0x28, 0x5F, 0x68, 0x87},
  {0x1E, 0x22, 0x42, 0x62, 0x9F}

};

static byte hellomessage[74] = {'h','e','l','l','o',' ','a','n','d',' ','t','h','a','n','k',' ','u',' ','f','o','r',' ','u','r',' ','t','i','m','e',' ','i','m',' ','j','u','s','t',' ','a',' ','b','a','b','u',' ','o','n','e',' ','d','a','y',' ','i','l','l',' ','b','e',' ','a',' ','m','a','m','u',' ','t','h','a','n','k',' ','u'};
int IOprev[8] = {0};
int IOcur[8] = {0};

void setup()
{
  SLIPSerial.begin(57600);
  Serial.begin(57600);
  Serial.setTimeout(50);       //needs to be lower than screen's delay rate 
  pinMode(12, OUTPUT);         //load
  pinMode(11, OUTPUT);         //SDATA
  pinMode(10, OUTPUT);         //SDCLK
  digitalWrite(12, HIGH);      //inhibit chip
  pinMode(9, OUTPUT);
  for(int i = 0; i < 5; i++)   //clear screen
  {
    byte tmpar[5];
    for(int g = 0; g < 5; g++)
    {
      tmpar[g] = alphabet[26][g];
    }
    sendany(i, tmpar);
  }
  pinMode(2, INPUT);   //stomp1
  pinMode(3, INPUT);   //stomp2
  pinMode(4, INPUT);   //toggle1
  pinMode(5, INPUT);   //toggle2
  pinMode(14, INPUT);  //knob1
  pinMode(15, INPUT);  //knob2
  pinMode(16, INPUT);  //knob3
  pinMode(17, INPUT);  //knob4

  pinMode(6, OUTPUT);  //led0
  pinMode(7, OUTPUT);  //led1

  for(int t = 0; t < 74; t++)              //set the fucking hello message
  {
    displayarray[t] = hellomessage[t];
  }
  currentarraylength = 74;
  sendDisplay(0xF1);         //power levels as detailed page 10 (F0 -F6, F8 = shutdown)
  
}


byte led0 = 0;  //must be a better way probs
byte led1 = 0;

void loop()
{
  OSCMessage interface("/interface");
  
  getserial();
  parseserial();
  marqueeSet(interval);
  backupIO();
  readIO();

  //quick and dirty OSC send, timecrunch
  for(int i = 0; i < 4; i++)
  {
    if(IOcur[i] != IOprev[i])
    {
      
      
      
        interface.add((int32_t)i);
        interface.add((int32_t)IOcur[i]);
        SLIPSerial.beginPacket();
        interface.send(SLIPSerial);
        SLIPSerial.endPacket();
      
    }
  }
  for(int i =4; i <8; i++)
  {

        //if(IOcur[i] > (IOprev[i] + 0x01) || IOcur[i] < (IOprev[i] - 0x01))  //TOTAL FUCKUPS
        //if((IOcur[i] / 4) != (IOprev[i] / 4))
        if(IOcur[i] / 4 != IOprev[i] /4)
        {
          interface.add((int32_t)i);
          interface.add((int32_t)IOcur[i]);
          SLIPSerial.beginPacket();
          interface.send(SLIPSerial);
          SLIPSerial.endPacket();
        }
  }

  digitalWrite(6, led0);  //set them quick and dirty
  digitalWrite(7, led1);

  //get japanese to scroll the other way
  
}

void sendOSC()  //currently untested variant
{
  OSCMessage interface("/interface");
  for(int i = 0; i < 8; i++)
  {
    if(IOcur[i] != IOprev[i])
      interface.add((int32_t)IOcur[i]);
      SLIPSerial.beginPacket();
      interface.send(SLIPSerial);
      SLIPSerial.endPacket();
  }
 

  
}

void readIO()
{
  for(int i = 0; i < 8; i++)
  {
    if(i < 4)
    {
      IOcur[i] = digitalRead(i+2);  //start at D2, go up to D6
    }
    else
    {
      IOcur[i] = analogRead(i-4);
    }
  }
}

void backupIO()
{
  for(int i = 0; i<8;i++)
  {
    IOprev[i] = IOcur[i];
  }
}
//-------------SERIAL PARSING AND INPUT------------------
void getserial()
{
  serialsize = 0;        //temp var for sniffing the serial events
  gotserial = 0;
  while(Serial.available() > 0)
  {
    serialsize = Serial.readBytesUntil('<', serialinput, 100);  //MUST HAVE LINE TERMINATOR jesus christ how i've Learnt
    if(serialsize > 0)gotserial=1;                              //set flag if we caught one pappi
  }
}
  
void parseserial()
{
  if(gotserial)
  {
    if(serialinput[0] == 'A' && serialinput[1] == 'T')  //analog of AT commands i'm pretty sure I used once
    {

      switch (serialinput[2])
      {

        case 'B': 
          interval = serialinput[3] + serialinput[4] + serialinput[5] + serialinput[6];
          gotserial = 0;
          break;  
        case 'C':
          language = serialinput[3];
          gotserial = 0;
          break;
        case 'D':
          marqueeCheck = serialinput[3];
          gotserial = 0;
          break;
        case 'L':
          led0 = serialinput[3];
          gotserial = 0;
          break;
        case 'M':
          led1 = serialinput[3];
          gotserial = 0;
          break;
        case 'S':
          //sendDisplay(0xF1);         //power levels as detailed page 10 (F0 -F6, F8 = shutdown)
          sendDisplay(0xF0 | serialinput[3]);
          gotserial = 0;
          break;
          
      }
      
      //ledshit = serialinput[2];                         //no further parsing, just accept the 3rd position as interval data
      //gotserial = 0;
    }  //COULD ALSO USE CASE FUNCTION FOR [ATB3] or [ATO6] ETC
  
    
    else
    {
      for(int i = 0; i < serialsize; i++)
      {
        displayarray[i] = serialinput[i];
      }
      currentarraylength = serialsize + 1;  //make sure we get the size of the array into a fresh variable or it'll reset AND ADD ONE
      gotserial = 0;                    //reset flag
      //Serial.print(currentarraylength);  //debugging purposes
    } 
  }
}


//----------ALL DISPLAY BELOW------------------
void sendDisplay(int screen)
{
  digitalWrite(LOAD, LOW);        //chip listen now
  digitalWrite(CLK, LOW);         //clock starts low
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(DATA, screen >> i & 1);  //bitshift left i and 1 to peek the bit
    digitalWrite(CLK, HIGH);              //latches on rising edge
    digitalWrite(CLK, LOW);               //set it back down to toggle data
  }
  digitalWrite(LOAD, HIGH);               //inhibit any more data input
}


void sendany(int disp, byte letter[5])
{
 int tempdisp = 0xA0;     //upper nibble for display change command 
 tempdisp |= disp;        //disp between 0 and 3
 sendDisplay(tempdisp);
 for(int i = 0; i < 5; i++)  //send 5 bytes for pixels
 {
   sendDisplay(letter[i]);
 }
}

byte arraycopy(byte destinationarray[], byte sourcearray[]) 
{
  for(int copyar =0; copyar < 5; copyar++) 
    {
      destinationarray[copyar] = sourcearray[copyar];         
    }
}


void quickmarquee(byte letterarray[], int arraysize)
{
 if(address >= arraysize-1)  //we are prepending a space as seen below
  {
    address = 0; 
  }

  
  if(language == 0)  //latin set
  {
    for(int i = 0; i < 5; i++)
    {
      if(address==0)temp4[i] = alphabet[0][i]; //add a space at first slot PREPEND THAT OR SUFFER
      else if(letterarray[address-1]==32) temp4[i] = alphabet[letterarray[address-1]-32][i];  //ASCII 32 is space, alphabet[26], subtract 6
      else temp4[i] = alphabet[letterarray[address-1]-32][i];                         //convert from ASCII to alphabet array format
    }
  }
  if(language == 1)   //kanji set
  {
    for(int i = 0; i < 5; i++)
    {
      if(address==0)temp4[i] = kanji[0][i]; //add a space at first slot PREPEND THAT OR SUFFER
      else if(letterarray[address-1]==32) temp4[i] = kanji[0][i]; //if 32 add space
      else temp4[i] = kanji[letterarray[address-1]-64][i];  //otherwise take the number and subtract 64
    }
  
  }  
  sendany(0, temp1);  //push each to screen
  sendany(1, temp2);
  sendany(2, temp3);
  sendany(3, temp4);
  
  arraycopy(temp1, temp2); //shift to the right section
  arraycopy(temp2, temp3);
  arraycopy(temp3, temp4);
 
  address++;  //increment address for next time
}

void marqueeSet(int delaytime)
{
  unsigned long currentMillis = millis();  //get current time babu

  if(currentMillis - prevMillis >= delaytime) //peep them differences and do it or not
  {
    prevMillis = currentMillis;
    if(marqueeCheck == 0)                 //you now have a choice!
    {
    quickmarquee(displayarray,currentarraylength);
    }
    else
    flashdisplay(displayarray,currentarraylength);
  }
}

void flashdisplay(byte letterarray[], int arraysize)
{
  if(address >= arraysize-2)  // the minus two is a mystery but it works?
  {
    address = 0;
  }
 if(letterarray[address]==32)
  {
     address++; //if you land on a space, advance to next char
  }
  
  for(int i=0;i<5;i++)
  {
    if(language==0)
    {
      //if(letterarray[address]==32)temp1[i] = alphabet[0][i];
       temp1[i] = alphabet[letterarray[address]-32][i];
    }
    if(language==1)
    {
      //if(letterarray[address]=32)temp1[i] = kanji[0][i];
      temp1[i] = kanji[letterarray[address]-64][i];
    }
    
  }
  address++; 

  
  if(address >= arraysize-2 || letterarray[address]==32)
  {
    for(int i=0;i<5;i++)
    {
      temp2[i] =alphabet[0][i];
    }
  }
  else
  {
    for(int i=0;i<5;i++)
    {
      if(language==0)
      {
        //if(letterarray[address]==32)temp2[i] = alphabet[0][i];
         temp2[i] = alphabet[letterarray[address]-32][i];
      }
      if(language==1)
      {
        //if(letterarray[address]=32)temp2[i] = kanji[0][i];
        temp2[i] = kanji[letterarray[address]-64][i];
      }
     }
      
    }
    address++;


  if(address >= arraysize-2 || letterarray[address]==32)
  {
    for(int i=0;i<5;i++)
    {
      temp3[i] =alphabet[0][i];
    }
  }
  else
  {
    for(int i=0;i<5;i++)
    {
      if(language==0)
      {
       temp3[i] = alphabet[letterarray[address]-32][i];
      }
      if(language==1)
      {
        temp3[i] = kanji[letterarray[address]-64][i];
      }
    }
    address++;
  }


  if(address >= arraysize-2 || letterarray[address]==32)
  {
    for(int i=0;i<5;i++)
    {
      temp4[i] =alphabet[0][i];
    }
    address++; //finally increment to get past space
  
  }
  else
  {
    for(int i=0;i<5;i++)
    {
      if(language==0)
      {
        temp4[i] = alphabet[letterarray[address]-32][i];
      }
      if(language==1)
      {
        temp4[i] = kanji[letterarray[address]-64][i];
      }
    }
    address++;
  }
  

  sendany(0, temp1);  //push each to screen
  sendany(1, temp2);
  sendany(2, temp3);
  sendany(3, temp4);
  
}
