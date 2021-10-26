void checkForRestart() {
  if (restartNow) {
    ESP.restart();
  }
}
//aligns string center of array
void centerString(long *inputmsg, long *returnvar, int count) {
  if (count < UNITSAMOUNT) {
    int emptySpaceAmount = (UNITSAMOUNT - count) / 2;
    
    for (int i = 0; i < UNITSAMOUNT; i++) {
      if (i >=emptySpaceAmount && i< (count+emptySpaceAmount)) {
        returnvar[i] = inputmsg[i-emptySpaceAmount];
      } else {
        returnvar[i] = (long)' ';
      }
    }
  } else {
    for (int i = 0; i < UNITSAMOUNT; i++) {
      returnvar[i] = inputmsg[i];
    }
  }
}

//aligns string on right side of array and fills empty chars with spaces
void rightString(long *inputmsg, long *returnvar, int count) {
  if (count < UNITSAMOUNT) {
    for (int i = 0; i < UNITSAMOUNT; i++) {
      if(i<(UNITSAMOUNT-count)) {
        returnvar[i] = (long)' ';
      } else {
        returnvar[i] = inputmsg[(i-(UNITSAMOUNT-count))];
      }
    }
  } else {
    for (int i = 0; i < UNITSAMOUNT; i++) {
      returnvar[i] = inputmsg[i];
    }
  }
}

//aligns string on left side of array and fills empty chars with spaces
void leftString(long *inputmsg, long *returnvar, int count) {
  if (count < UNITSAMOUNT) {
    for (int i = 0; i < UNITSAMOUNT; i++) {
      if(i<count) {
        returnvar[i] = inputmsg[i];
      } else {
        returnvar[i] = (long)' ';
      }
    }
  } else {
    for (int i = 0; i < UNITSAMOUNT; i++) {
      returnvar[i] = inputmsg[i];
    }
  }
}

//converts input string to uppercase
String cleanString(String message) {
  message.toUpperCase();
  return message;
}

int convertSpeed(String speedSlider) {
  int speedSliderInt;
  speedSliderInt = speedSlider.toInt();
  speedSliderInt = map(speedSliderInt, 1, 100, MINSPEED, MAXSPEED);
  return speedSliderInt;
}
