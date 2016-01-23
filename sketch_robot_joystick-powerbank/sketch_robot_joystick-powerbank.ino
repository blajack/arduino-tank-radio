int IN1 = 7; 
int IN2 = 6;
int IN3 = 5;
int IN4 = 4;

int EN1 = 9;
int EN2 = 3;

int LED = 13;
int MAXspeed = 100;
int i;
int t;

// добавляю джостик
const int POS_Y_PIN = 5;
const int POS_X_PIN = 4;
const int BUTTON_PIN = 2;
const int MAX_X = 1023;   // VRx и VRy выдают значения от 0 до 1023
const int MAX_Y = 1023;  
int speedRight = 0;
int speedLeft = 0;



void setup()
{
  Serial.begin(9600);
  
  //выводы двигателей
  pinMode (EN1, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (EN2, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (IN3, OUTPUT);

 
  
}


void loop()
{

  // переменные для считывания положения джостика
   int yVal = map(analogRead(POS_Y_PIN),0,1023, 100,-100);
   int xVal = map(analogRead(POS_X_PIN),0,1023,-40,40);
  //int yVal = analogRead(POS_Y_PIN);
  //int xVal = analogRead(POS_X_PIN); 




 if (yVal>=0) // Если скорость положительная то едем вперед
 {

// настройки для езды вперед

  digitalWrite (IN1, HIGH); 
  digitalWrite (IN2, LOW);

  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW); 

  // если вправо, то 
  if (xVal>=0)
  {
    speedRight = yVal + xVal;
    speedLeft =  yVal - xVal;
  }
  else
  {
    
    speedRight = yVal - abs(xVal);
    speedLeft =  yVal + abs(xVal);
  }
  
   if (speedRight<10) speedRight=0;
  if (speedLeft<10) speedLeft=0;
  
  analogWrite(EN1, 100);
  analogWrite(EN2, speedLeft);
 }
 else if (yVal<=0)
 {
     // настройки для езды назад

  digitalWrite (IN2, HIGH); 
  digitalWrite (IN1, LOW);

  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW); 

   // если вправо, то 
  if (xVal>=0)
  {
    speedRight = abs(yVal) + xVal;
    speedLeft =  abs(yVal) - xVal;
  }
  else
  {
    
    speedRight = abs(yVal) - abs(xVal);
    speedLeft =  abs(yVal) + abs(xVal);
  }

  if (speedRight<10) speedRight=0;
  if (speedLeft<10) speedLeft=0;
  
  analogWrite(EN1, speedRight);
  analogWrite(EN2, speedLeft);
  
 }

  Serial.print("speedRight = ");
  Serial.print (speedRight);
  Serial.print(" | ");
  Serial.print("speedLeft = ");
  Serial.print(speedLeft);  
  Serial.print(" | ");
  Serial.print("yVal = ");
  Serial.print(yVal);
  Serial.print(" | ");
  Serial.print("xVal = ");
  Serial.println(xVal);
  delay(100);
  
}

//functions

//STOP
int robo_stop () {
  // обнуление сигнала
  analogWrite (EN1, 0);
  analogWrite (EN2, 0);

}


//FORTH
int robo_forth () {
    // настройки для езды вперед

  digitalWrite (IN1, HIGH); 
  digitalWrite (IN2, LOW);

  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW); 
  
 // for (i = 50; i <= 150; ++i) // увеличение скорости
 // {
   i = MAXspeed;
      analogWrite(EN1, i);
      analogWrite(EN2, i);
      delay(100);
 // }
}

//TURN
int robo_turn () {
 // настройки для кругом
  digitalWrite (IN2, HIGH); 
  digitalWrite (IN1, LOW);
 
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW); 
  
  //for (i = 50; i <= 150; ++i) // увеличение скорости
  //{
    i = 150;//MAXspeed;
      analogWrite(EN1, i);
      analogWrite(EN2, i);
      delay(1);
  //}
  delay(100); // здесь регулировать угол поворота.
}

//BACK
int robo_back () {
    // настройки для езды назад

    digitalWrite (IN2, HIGH); 
  digitalWrite (IN1, LOW);

  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW); 
  
  //for (i = 50; i <= 200; ++i) // увеличение скорости
  //{
    i = MAXspeed;
      analogWrite(EN1, i);
      analogWrite(EN2, i);
      delay(150);
  //}
}



