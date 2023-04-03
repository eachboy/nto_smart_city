#include <Wire.h>  //библиотека для I2C интерфейса


#include <Adafruit_PWMServoDriver.h>                          // библиотека для моторной платы
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x70); 
#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds9960;


#define sensor_addr 0x3F // Переключатели адреса в положении "OFF"

#define I2C_HUB_ADDR 0x70  // настройки I2C для платы MGB-I2C63EN
#define EN_MASK 0x08
#define DEF_CHANNEL 0x00
#define MAX_CHANNEL 0x08

//    Colors      //
#define RED 1
#define GRAY 2
#define PURPLE 3
#define BROWN 4
#define PINK 5

#define OTHER 255
//    Colors      //

int speed = 60;
uint16_t values[13];
uint16_t colors[13];
int sum(uint16_t*);
bool check = true;
bool pynkt = true; // до станции 
//bool pynkt = false; // до лаборатории
bool lab = true;
bool check_black = false;

void setup() {
  // Инициализация последовательного порта
  Serial.begin(115200);
  // Инициализация датчика
  Wire.begin();
  Wire.setClock(100000L);
  pwm.begin();
  // Частота (Гц)
  pwm.setPWMFreq(100);
  // Все порты выключены
  pwm.setPWM(8, 0, 4096);
  pwm.setPWM(9, 0, 4096);
  pwm.setPWM(10, 0, 4096);
  pwm.setPWM(11, 0, 4096);
  delay(100);
  setBusChannel(0x06);
  apds9960.begin();
  apds9960.enableColor(true);
  setBusChannel(0x07);
  init_sensor();
}

void loop() {
  setBusChannel(0x07);
  poll_sensor();
  if (pynkt == true){
    for (int i = 0; i < 13; i++){
     colors[i] = (values[i]>1400);
    }
    if ((sum(colors) == 13) && (check_black == false)){
      motorA_setpower(speed, false);
      motorB_setpower(speed, true);
      delay(300);
      check_black = true;
    } 
    setBusChannel(0x07);
    poll_sensor();
    for (int i = 0; i < 13; i++){
     colors[i] = (values[i]>1400);
    }     
    if ((sum(colors) == 13) && check_black){
      motorA_setpower(speed, false);
      motorB_setpower(speed, true);
      delay(300);
      motorA_setpower(speed, true);
      motorB_setpower(speed, true);
      delay(400);
      motorA_setpower(0, true);
      motorB_setpower(0, true);
      Serial.println(2);
    }
    setBusChannel(0x06);
    if (getColor() == 1){
        motorA_setpower(0, false);
        motorB_setpower(0, true); 
        pynkt == false; 
        Serial.println(3);
         
    } else{
      //Serial.println(getColor());
      setBusChannel(0x07);
      float u = PID(6.0, 0.1, 12.4);
      motorA_setpower(speed - u, false);
      motorB_setpower(speed + u, true);
      delay(10); 
      Serial.println(1);
    }
    //Serial.println(pynkt,lab);
  } else if (lab == true){
    for (int i = 0; i < 13; i++){
     colors[i] = (values[i]>1400);
    }
    //Serial.println(pynkt,lab);
    setBusChannel(0x06);
    if (getColor() == 1)
    {
      motorA_setpower(speed, false);
      motorB_setpower(speed, false);
      delay(900);
      motorA_setpower(0, true);
      motorB_setpower(0, true);
    }
    setBusChannel(0x07);
    float u = PID(6.0, 0.1, 12.4);
    motorA_setpower(speed - u, false);
    motorB_setpower(speed + u, true);
    setBusChannel(0x06);
    //Serial.println(getColor());
    if ((getColor() == 3) && (check == true))
    {
      motorA_setpower(speed, false);
      motorB_setpower(speed, true);
      delay(500);
      motorA_setpower(speed, false);
      motorB_setpower(speed, false);
      delay(450);
      motorA_setpower(speed, false);
      motorB_setpower(speed, true);
      delay(600);
      motorA_setpower(0, true);
      motorB_setpower(0, true);
      check = false;
    }
    
    if ((sum(colors) == 13 && lab))
    {
      motorA_setpower(speed, true);
      motorB_setpower(speed, true);
      delay(1000);
      motorA_setpower(0, false);
      motorB_setpower(0, true);
      delay(200);
      setBusChannel(0x07);
      poll_sensor();
       for (int i = 0; i < 13; i++){
        colors[i] = (values[i]>1400);
      }
      Serial.println(sum(colors));
      
      while (sum(colors) != 13)
      {
        setBusChannel(0x07);
        poll_sensor();
        for (int i = 0; i < 13; i++){
          colors[i] = (values[i]>1400);
         }
        motorA_setpower(speed, true);
        motorB_setpower(speed, false); 
      }
      motorA_setpower(0, false);
      motorB_setpower(0, true);
      lab = false;
      
    }
    //Serial.println(gg);
  }  
  
}


void motorA_setpower(float pwr, bool invert) {
  // Проверка, инвертирован ли мотор
  if (invert) {
    pwr = -pwr;
  }
  // Проверка диапазонов
  if (pwr < -100) {
    pwr = -100;
  }
  if (pwr > 100) {
    pwr = 100;
  }
  int pwmvalue = fabs(pwr) * 40.95;
  if (pwr < 0) {
    pwm.setPWM(10, 0, 4096);
    pwm.setPWM(11, 0, pwmvalue);
  } else {
    pwm.setPWM(11, 0, 4096);
    pwm.setPWM(10, 0, pwmvalue);
  }

}

// Мощность мотора "B" от -100% до +100% (от знака зависит направление вращения)
void motorB_setpower(float pwr, bool invert) {
  // Проверка, инвертирован ли мотор
  if (invert) {
    pwr = -pwr;
  }
  // Проверка диапазонов
  if (pwr < -100) {
    pwr = -100;
  }
  if (pwr > 100) {
    pwr = 100;
  }
  int pwmvalue = fabs(pwr) * 40.95;
  if (pwr < 0) {
    pwm.setPWM(8, 0, 4096);
    pwm.setPWM(9, 0, pwmvalue);
  } else {
    pwm.setPWM(9, 0, 4096);
    pwm.setPWM(8, 0, pwmvalue);
  }
}

int getErrPID(int val) {
  float e = 0;
  for (int i = 0; i < 13; i++) {
    e+= (values[i] > val) * (i - 6);
    
  }
  return e;
}

int16_t PID(float Kp, float Ki, float Kd) {
  static float I = 0;
  static int old_err = 0;
  float err = getErrPID(1400);
  uint32_t old_t = 0; 
  float dt = (millis() - old_t) / 1000;
  I += err;
  float D = (err - old_err) / dt;
  return ((float)Kp * err + Ki * I + Kd * D);
}

uint8_t getColor() {
  uint16_t red, green, blue, clear;
  while (!apds9960.colorDataReady()) {
    delay(5);
  }
  apds9960.getColorData(&red, &green, &blue, &clear);
  uint16_t full = red + green + blue;
  if (full != 0) {
    uint8_t r, g, b;
    r = (float)red / full * 100;
    g = (float)green / full * 100;
    b = (float)blue / full * 100;
    // Serial.print(r);
    // Serial.print(" ");
    // Serial.print(g);
    // Serial.print(" ");
    // Serial.print(b);
    // Serial.print(" ");
    // Serial.println();

    if (17 <= r && r <= 19) {
      if (30 <= g && g <= 32) {
        if (49 <= b && b <= 52) {
          return PURPLE;
        }
      }
    } else if (35 <= r && r <= 45) {
      if (21 <= g && g <= 28) {
        if (32 <= b && b <= 38) {
          return RED;
        }
      }
    }
    return OTHER;
   }
  return 0;
}
// Инициализация датчика
void init_sensor() {
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x10);       // Регистр настройки всей микросхемы
  Wire.write(0b00000000); // Нормальный режим работы
  Wire.write(0b01001111); // АЦП в непрерывном режиме, 200 ksps, встроенный ИОН для ЦАП
  Wire.endTransmission();
  delay(1000);
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x20);       // Регистр настройки порта 0 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x21);       // Регистр настройки порта 1 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x22);       // Регистр настройки порта 2 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x23);       // Регистр настройки порта 3 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x24);       // Регистр настройки порта 4 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x25);       // Регистр настройки порта 5 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x26);       // Регистр настройки порта 6 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x27);       // Регистр настройки порта 7 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x28);       // Регистр настройки порта 8 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x29);       // Регистр настройки порта 9 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2A);       // Регистр настройки порта 10 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2B);       // Регистр настройки порта 11 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2C);       // Регистр настройки порта 12 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2D);       // Регистр настройки порта 13 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2E);       // Регистр настройки порта 14 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2F);       // Регистр настройки порта 15 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x30);       // Регистр настройки порта 16 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x31);       // Регистр настройки порта 17 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x32);       // Регистр настройки порта 18 (подключен к оптическому сенсору)
  Wire.write(0b00000000); // Сброс конфигурации порта
  Wire.write(0b00000000);
  Wire.endTransmission();
  delay(1000);
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x20);       // Регистр настройки порта 0 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x21);       // Регистр настройки порта 1 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x22);       // Регистр настройки порта 2 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x23);       // Регистр настройки порта 3 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x24);       // Регистр настройки порта 4 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x25);       // Регистр настройки порта 5 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x26);       // Регистр настройки порта 6 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x27);       // Регистр настройки порта 7 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x28);       // Регистр настройки порта 8 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x29);       // Регистр настройки порта 9 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2A);       // Регистр настройки порта 10 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2B);       // Регистр настройки порта 11 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2C);       // Регистр настройки порта 12 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2D);       // Регистр настройки порта 13 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2E);       // Регистр настройки порта 14 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x2F);       // Регистр настройки порта 15 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x30);       // Регистр настройки порта 16 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x31);       // Регистр настройки порта 17 (подключен к оптическому сенсору)
  Wire.write(0b01110001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x32);       // Регистр настройки порта 18 (подключен к оптическому сенсору)
  Wire.write(0b01111001); // Диапазон входного напряжения 0 ... 10 В, встроенный ИОН, порт в режиме входа АЦП
  Wire.write(0b11100000); // Порт не ассоциирован с другим портом, количество выборок АЦП - 128
  Wire.endTransmission();
  delay(1000);
}

// Получение данных с датчика
void poll_sensor() {
  int adc_sensor_data[38] = {0};
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x40); // Регистр данных АЦП
  Wire.endTransmission();
  Wire.requestFrom(sensor_addr, 10);
  if (Wire.available() == 10)
  {
    adc_sensor_data[0] = Wire.read();
    adc_sensor_data[1] = Wire.read(); // ADC03
    adc_sensor_data[2] = Wire.read();
    adc_sensor_data[3] = Wire.read(); // ADC04
    adc_sensor_data[4] = Wire.read(); // ADC02
    adc_sensor_data[5] = Wire.read();
    adc_sensor_data[6] = Wire.read(); // ADC03
    adc_sensor_data[7] = Wire.read();
    adc_sensor_data[8] = Wire.read(); // ADC04
    adc_sensor_data[9] = Wire.read();
  }
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x45); // Регистр данных АЦП
  Wire.endTransmission();
  Wire.requestFrom(sensor_addr, 10);
  if (Wire.available() == 10)
  {
    adc_sensor_data[10] = Wire.read(); // ADC05
    adc_sensor_data[11] = Wire.read();
    adc_sensor_data[12] = Wire.read(); // ADC06
    adc_sensor_data[13] = Wire.read();
    adc_sensor_data[14] = Wire.read(); // ADC07
    adc_sensor_data[15] = Wire.read();
    adc_sensor_data[16] = Wire.read(); // ADC08
    adc_sensor_data[17] = Wire.read();
    adc_sensor_data[18] = Wire.read(); // ADC09
    adc_sensor_data[19] = Wire.read();
  }
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x4A); // Регистр данных АЦП
  Wire.endTransmission();
  Wire.requestFrom(sensor_addr, 10);
  if (Wire.available() == 10)
  {
    adc_sensor_data[20] = Wire.read(); // ADC10
    adc_sensor_data[21] = Wire.read();
    adc_sensor_data[22] = Wire.read(); // ADC11
    adc_sensor_data[23] = Wire.read();
    adc_sensor_data[24] = Wire.read(); // ADC12
    adc_sensor_data[25] = Wire.read();
    adc_sensor_data[26] = Wire.read(); // ADC13
    adc_sensor_data[27] = Wire.read();
    adc_sensor_data[28] = Wire.read(); // ADC14
    adc_sensor_data[29] = Wire.read();
  }
  Wire.beginTransmission(sensor_addr);
  Wire.write(0x4F); // Регистр данных АЦП
  Wire.endTransmission();
  Wire.requestFrom(sensor_addr, 8);
  if (Wire.available() == 8)
  {
    adc_sensor_data[30] = Wire.read(); // ADC15
    adc_sensor_data[31] = Wire.read();
    adc_sensor_data[32] = Wire.read(); // ADC16
    adc_sensor_data[33] = Wire.read();
    adc_sensor_data[34] = Wire.read(); // ADC17
    adc_sensor_data[35] = Wire.read();
    adc_sensor_data[36] = Wire.read(); // ADC18
    adc_sensor_data[37] = Wire.read();
  }
  values[0] = adc_sensor_data[30] * 256 + adc_sensor_data[31];
  values[1] = adc_sensor_data[28] * 256 + adc_sensor_data[29];
  values[2] = adc_sensor_data[26] * 256 + adc_sensor_data[27];
  values[3] = adc_sensor_data[24] * 256 + adc_sensor_data[25];
  values[4]  = adc_sensor_data[22] * 256 + adc_sensor_data[23];
  values[5]  = adc_sensor_data[20] * 256 + adc_sensor_data[21];
  values[6]  = adc_sensor_data[18] * 256 + adc_sensor_data[19];
  values[7]  = adc_sensor_data[16] * 256 + adc_sensor_data[17];
  values[8] = adc_sensor_data[14] * 256 + adc_sensor_data[15];
  values[9] = adc_sensor_data[12] * 256 + adc_sensor_data[13];
  values[10] = adc_sensor_data[10] * 256 + adc_sensor_data[11];
  values[11] = adc_sensor_data[8] * 256 + adc_sensor_data[9];
  values[12] = adc_sensor_data[6] * 256 + adc_sensor_data[7];
}

bool setBusChannel(uint8_t i2c_channel) // смена I2C порта
{
  if (i2c_channel >= MAX_CHANNEL)
  {
    return false;
  }
  else
  {
    Wire.beginTransmission(I2C_HUB_ADDR);
    Wire.write(i2c_channel | EN_MASK);
    Wire.endTransmission();
    return true;
  }
}
int sum(uint16_t *sp){
  int count;
  for (int i = 0;i < 13;i++){
    count += sp[i];
  }
  return count;
}