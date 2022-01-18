// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float magnitude(sensors_vec_t& vec) {
  return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

sensors_vec_t vector(float x, float y, float z) {
  sensors_vec_t vector;
  vector.x = x;
  vector.y = y;
  vector.z = z;
  return vector;
}

sensors_vec_t unitVector(sensors_vec_t& vec) {
  float mag = magnitude(vec);
  return vector(vec.x/mag ,vec.y/mag, vec.z/mag);
}

float dotProduct(const sensors_vec_t vec1, const sensors_vec_t vec2) {
  return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

#define X_unitVector (vector(1,0,0))
#define Y_unitVector (vector(0,1,0))
#define Z_unitVector (vector(0,0,1))

#define X_minus_unitVector (vector(-1,0,0))
#define Y_minus_unitVector (vector(0,-1,0))
#define Z_minus_unitVector (vector(0,0,-1))

int max(double a, double b, double c, double d, double e, double f) {
  if (a>b && a>c && a>d && a>e && a>f)
  {
    return 0;
  }
  if (b>c && b>d && b>e && b>f)
  {
    return 1;
  }
  if (c>d && c>e && c>f)
  {
    return 2;
  }
  if (d>e && d>f)
  {
    return 3;
  }
  if (e>f)
  {
    return 4;
  }
  return 5;
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  int direction = max(
    dotProduct(a.acceleration, X_unitVector),
    dotProduct(a.acceleration, Y_unitVector),
    dotProduct(a.acceleration, Z_unitVector),
    dotProduct(a.acceleration, X_minus_unitVector),
    dotProduct(a.acceleration, Y_minus_unitVector),
    dotProduct(a.acceleration, Z_minus_unitVector)
  );

  float rotationRateThreshold = .2;
  bool isRotating = magnitude(g.gyro) > rotationRateThreshold;

  Serial.printf("Direction: %d\n",direction);
  Serial.printf("RotationRate: %f\n", magnitude(g.gyro));
  Serial.printf("Is rotating: %s\n", (isRotating?"TRUE":"FALSE"));

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}