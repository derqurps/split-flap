void setupi2c() {
  Wire.begin(4, 5);
}

void updateFlapData() {
  //Mode Selection
    if (devicemode == "text") {
      showNewData(flaptext);
    } else if (devicemode == "date") {
      showDate();
    } else if (devicemode == "clock") {
      showClock();
    }
}

//checks for new message to show
void showNewData(String message) {
  if (writtenLast != message || (devicemode == "text" && lastAlignment != alignment) ) {
    showMessage(message, convertSpeed(flapSpeed));
  }
}

//pushes message to units
void showMessage(String message, int flapSpeed) {

  // wait while display is still moving
  if(isDisplayMoving()) {
    #ifdef serial
      Serial.println("wait for display to stop");
    #endif
    delay(500);
  } else {
    
    message = cleanString(message);
    
    #ifdef serial
      Serial.println(message);
    #endif
    
    long msgArray[UNITSAMOUNT];
    int count = convertToUnicode(message, msgArray);
    
    long formattedArray[UNITSAMOUNT];
    
    //Format string per alignment choice
    if (alignment == "left") {
      leftString(msgArray, formattedArray, count);
    } else if (alignment == "right") {
      rightString(msgArray, formattedArray, count);
    } else if (alignment == "center") {
      centerString(msgArray, formattedArray, count);
    } else {
      centerString(msgArray, formattedArray, count);
    }

    for (int i = 0; i < UNITSAMOUNT; i++) {
  
      int currentLetterPosition = translateLettertoInt((char)formattedArray[i]);

      #ifdef serial
        Serial.print("Unit Nr.: ");
        Serial.print(count);
        Serial.print(" Letter: ");
        Serial.print((char)formattedArray[i]);
        Serial.print(" Letter position: ");
        Serial.println(currentLetterPosition);
      #endif
      writeToUnit(i, currentLetterPosition, flapSpeed);
    }
    writtenLast = message;
    lastAlignment = alignment;
  }
}

int convertToUnicode(String message, long *returnvar) {
  
  int maxamount = message.length();
  int count = 0;
  for (int i = 0; i < maxamount; i++) {

    long unicodeVal = 0;
    int countBytes = 1;
    if (message[i]>=194 && message[i] <=223) {
      countBytes = 2;
    } else if (message[i]>=224 && message[i] <=239) {
      countBytes = 3;
    } else if (message[i]>=240 && message[i] <=255) {
      countBytes = 4;
    }
    int usize = (countBytes*2)+2;
    char unicode[usize];
    unicode[0] = '0';
    unicode[1] = 'x';
    for (int j=0;j<countBytes;j++) {
      String cl = String(message[(i+j)], HEX);
      unicode[((j*2)+2)] = cl[0];
      unicode[((j*2)+3)] = cl[1];
    }
    unicodeVal = strtol(unicode, 0, 16); //convert the string
    returnvar[count]=unicodeVal;
    count++;
    i = i + (countBytes - 1);
  }
  return count;
}


//translates char to letter position
int translateLettertoInt(char letterchar) {
  for (int i = 0; i < FLAPAMOUNT; i++) {
    if (letterchar == letters[i]) {
      return i;
    }
  }
}

//write letter position and speed in rpm to single unit
void writeToUnit(int address, int letter, int flapSpeed) {
  int sendArray[2] = {letter, flapSpeed}; //Array with values to send to unit

  Wire.beginTransmission(address);

  //Write values to send to slave in buffer
  for (int i = 0; i < sizeof sendArray / sizeof sendArray[0]; i++) {
#ifdef serial
    Serial.print("sendArray: ");
    Serial.println(sendArray[i]);
#endif
    Wire.write(sendArray[i]);
  }
  Wire.endTransmission(); //send values to unit
}


//checks if unit in display is currently moving
bool isDisplayMoving() {
  //Request all units moving state and write to array
  for (int i = 0; i < UNITSAMOUNT; i++) {
    displayState[i] = checkIfMoving(i);
    if (displayState[i] == 1) {
#ifdef serial
      Serial.println("A unit in the display is busy");
#endif
      return true;

      //if unit is not available through i2c
    } else if (displayState[i] == -1) {
#ifdef serial
      Serial.println("A unit in the display is sleeping");
#endif
      return true;
    }
  }
#ifdef serial
  Serial.println("Display is standing still");
#endif
  return false;
}

//checks if single unit is moving
int checkIfMoving(int address) {
  int active;
  Wire.requestFrom(address, ANSWERSIZE, true);
  active = Wire.read();
#ifdef serial
  Serial.print(address);
  Serial.print(":");
  Serial.print(active);
  Serial.println();
#endif
  if (active == -1) {
#ifdef serial
    Serial.println("Try to wake up unit");
#endif
    Wire.beginTransmission(address);
    Wire.endTransmission();
    //delay(5);
  }
  return active;
}
