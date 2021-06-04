/* Xbox style controller featuring two joysticks, 17 buttons, and a toggle switch
   to allow the joystick to act as a mouse.

   This code was based off the Teensy USB Joystick example code as well as
   Gadget Reboot's USB Mouse code

   https://www.pjrc.com/teensy/td_joystick.html
   https://github.com/GadgetReboot/Arduino/tree/master/Teensy/USB_HID_Mouse
*/

const int numButtons = 20;

const int ledPin = LED_BUILTIN;
const int mouseToggleSwitch = 17;
const int leftClick = 2;
const int rightClick = 3;
const int xAxis = A0;
const int yAxis = A1; 

// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value

bool mouseEnabled = false;
int lastSwitchState = HIGH;

void setup() {
  pinMode(ledPin, OUTPUT);
  
  Joystick.useManualSend(true);
  
  for (int i=0; i<numButtons; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }

  Mouse.begin();
}

void loop() {
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis);

  digitalWrite(ledPin, HIGH);
  
  Joystick.X(analogRead(0));
  Joystick.Y(analogRead(1));
  Joystick.Z(analogRead(2));
  Joystick.Zrotate(analogRead(3));

  int switchState = digitalRead(mouseToggleSwitch);
  if(switchState != lastSwitchState)
  {
    if (switchState == HIGH)
    {
      mouseEnabled = !mouseEnabled;
    }
  }

  lastSwitchState = switchState;

  if (mouseEnabled)
  {
    Mouse.move(xReading, yReading, 0);

    if (digitalRead(leftClick) == LOW)
    {
      if (!Mouse.isPressed(MOUSE_LEFT))
      {
        Mouse.press(MOUSE_LEFT);
      }
    else
    {
      if (Mouse.isPressed(MOUSE_LEFT))
      {
        Mouse.release(MOUSE_LEFT);
      }
    }
      delay(5);
    }
  }

  if (digitalRead(rightClick) == LOW)
  {
    if (!Mouse.isPressed(MOUSE_RIGHT))
    {
      Mouse.press(MOUSE_RIGHT);
    }
    else
    {
      if (Mouse.isPressed(MOUSE_RIGHT))
      {
        Mouse.release(MOUSE_RIGHT);
      }
    }
    delay(5);
  }

  for (int i = 0; i < numButtons; i++)
  {
    if (digitalRead(i) == LOW && mouseEnabled == false)
    {
      Joystick.button(i + 1, 1);
    }
    else
    {
      Joystick.button(i + 1, 0);
    }
  }

  Joystick.button(7, 0);

  Joystick.send_now();
  delay(5);
}

int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}
