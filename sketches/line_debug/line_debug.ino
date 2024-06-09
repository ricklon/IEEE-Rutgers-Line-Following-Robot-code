#define motor1Dir 7
#define motor2Dir 8
#define motor1PWM 9
#define motor2PWM 10
#define motor1Enable 11
#define motor2Enable 12

#define numSensors 5
#define line_thresh 100

int sensors[numSensors];
int sensorsmax[numSensors] = { -1, -1, -1, -1, -1 };
int sensorsmin[numSensors] = { 1024, 1024, 1024, 1024, 1024 };

void initMotorDriver() {
    pinMode(motor1Dir, OUTPUT);
    pinMode(motor2Dir, OUTPUT);

    pinMode(motor1Enable, OUTPUT);
    pinMode(motor2Enable, OUTPUT);
    digitalWrite(motor1Enable, HIGH);
    digitalWrite(motor2Enable, HIGH);
    setLeftMotorSpeed(0); // make sure the motors are stopped
    setRightMotorSpeed(0);
}

void setMotorVel(int dirPin, int pwmPin, int velocity) {
    if (velocity >= 255) velocity = 255;
    if (velocity <= -255) velocity = -255;

    if (velocity == 0) {
        digitalWrite(dirPin, HIGH);
        digitalWrite(pwmPin, HIGH);
    } else if (velocity < 0) {  // Reverse
        digitalWrite(dirPin, HIGH);
        analogWrite(pwmPin, 255 + velocity);
    } else { // Forward
        digitalWrite(dirPin, LOW);
        analogWrite(pwmPin, velocity);
    }
}

void setLeftMotorSpeed(int velocity) {
    Serial.print("Set Left: ");
    Serial.println(velocity);
    setMotorVel(motor1Dir, motor1PWM, -velocity);
}

void setRightMotorSpeed(int velocity) {
    Serial.print("Set Right: ");
    Serial.println(velocity);
    setMotorVel(motor2Dir, motor2PWM, -velocity);
}

void goForward() {
    Serial.println("Go Forward!");
    setLeftMotorSpeed(255);
    setRightMotorSpeed(255);
}

void goRight() {
    Serial.println("Go Right!");
    setLeftMotorSpeed(255);
    setRightMotorSpeed(-255);
}

void goLeft() {
    Serial.println("Go Left!");
    setLeftMotorSpeed(-255);
    setRightMotorSpeed(255);
}

void stop() {
    Serial.println("Stop.");
    setLeftMotorSpeed(0);
    setRightMotorSpeed(0);
}

int testSensor(int sensor) {
    int val = analogRead(sensor);
    Serial.print("Reading Sensor: ");
    Serial.print(sensor);
    Serial.print(": ");
    Serial.println(val);
    return val;
}

void readSensors() {
    for (int ii = 0; ii < numSensors; ii++) {
        sensors[ii] = analogRead(ii);
        sensorsmin[ii] = min(sensorsmin[ii], sensors[ii]);
        sensorsmax[ii] = max(sensorsmax[ii], sensors[ii]);
    }
}

void printSensorValues() {
    Serial.print("Sensor Values: ");
    for (int ii = 0; ii < numSensors; ii++) {
        Serial.print("s");
        Serial.print(ii);
        Serial.print("= ");
        Serial.print(sensors[ii]);
        Serial.print(" Max: ");
        Serial.print(sensorsmax[ii]);
        Serial.print(" Min: ");
        Serial.print(sensorsmin[ii]);
        Serial.print(", ");
    }
    Serial.println();
}

void calibrateSensors() {
    Serial.println("Calibrating sensors...");
    for (int i = 0; i < 100; i++) { // Collect 100 samples for calibration
        readSensors();
        delay(10); // Small delay to ensure accurate readings
    }
    Serial.println("Calibration complete.");
    for (int ii = 0; ii < numSensors; ii++) {
        Serial.print("Sensor ");
        Serial.print(ii);
        Serial.print(" Min: ");
        Serial.print(sensorsmin[ii]);
        Serial.print(" Max: ");
        Serial.println(sensorsmax[ii]);
    }
}

void setup() {
    Serial.begin(115200); // Set the baud rate for the serial com.
    Serial.println("Line Sensor board Sensor test");
    Serial.println("Enter to show sensor value: 0=Sensor0, 1=Sensor1, 2=Sensor2, 3=Sensor3, 4=Sensor4");
    Serial.println("Enter p to print all sensor values");
    Serial.println("Enter to move: s = stop, w = forward, a = right, x = reverse, d = left");
    Serial.println("Enter c to calibrate sensors");
    initMotorDriver();
}

void loop() {
    char cc;
    int adcVal;

    if (Serial.available() > 0) {
        cc = Serial.read();

        switch (cc) {
            case '0': testSensor(0); break;
            case '1': testSensor(1); break;
            case '2': testSensor(2); break;
            case '3': testSensor(3); break;
            case '4': testSensor(4); break;
            case 'w': goForward(); break;
            case 'x': setLeftMotorSpeed(-254); setRightMotorSpeed(-254); break;
            case 's': stop(); break;
            case 'a': goLeft(); break;
            case 'd': goRight(); break;
            case 'p':
                for (int i = 0; i < numSensors; i++) {
                    testSensor(i);
                }
                break;
            case 'c':
                calibrateSensors();
                break;
        }
    } else {
        readSensors();
        printSensorValues();

        if ((sensors[3] < line_thresh) || (sensors[4] < line_thresh)) {
            Serial.println("goLeft.");
            goLeft();
            delay(30);
        } else if ((sensors[0] < line_thresh) || (sensors[1] < line_thresh)) {
            Serial.println("goRight.");
            goRight();
            delay(30);
        } else {
            Serial.println("goForward.");
            goForward();
            delay(100);
        }
    }
}
