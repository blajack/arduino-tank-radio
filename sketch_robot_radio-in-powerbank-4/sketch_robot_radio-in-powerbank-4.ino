    ////////////////////////
    // Программа для      //
    // робота с радио     //
    // пультом            //
    ////////////////////////
    // Не работал при исп-//
    // ользовании 3 выхода//
    // сейчас работает    //
    // cократил дельту    //
    // разности при пово- //
    // роте               //
    ////////////////////////

// библиотека для приемника
#include <VirtualWire.h> 

// Постоянные для двигателя моторов
  int IN1 = 7; 
  int IN2 = 6;
  int IN3 = 8;
  int IN4 = 4;

  int EN1 = 5;
  int EN2 = 3;

// Общие значения
  int LED = 13;
  int MAXspeed = 100;

// Хз зачем это надо?
  int i;
  int t;

// Начальная скорость колес левого и правого
  int speedRight = 0;
  int speedLeft = 0;

// Переменные для получения и 
// Перевода данных джостика
// Намеренно делаю их глобальными
  int yrVal=500;
  int xrVal=500;
  int yVal=0;
  int xVal=0;



void setup()
  {
    Serial.begin(9600);

    // приемник
    vw_set_ptt_inverted(true); // Необходимо для DR3100
    vw_setup(2000); // Задаем скорость приема
    vw_rx_start(); // Начинаем мониторинг эфира
    
    
    //назначаем выводы двигателей
    pinMode (EN1, OUTPUT);
    pinMode (IN1, OUTPUT);
    pinMode (IN2, OUTPUT);
    pinMode (EN2, OUTPUT);
    pinMode (IN4, OUTPUT);
    pinMode (IN3, OUTPUT);
    
  }


void loop()
{
  
  // Переменные для приема сообщения с передатчика
  uint8_t buf[VW_MAX_MESSAGE_LEN]; // Буфер для сообщения
  uint8_t buflen = VW_MAX_MESSAGE_LEN; // Длина буфера


    //Начинаем мониторить эфир
    if (vw_get_message(buf, &buflen)) // Если принято сообщение
    {
      // Начинаем разбор
      int i; // переменная для передвижения по индексам массива buf[]

      // Если сообщение адресовано не нам, выходим
      if (buf[0] != 'z')
        {
          return; 
        }
      
      // ??? Кажется, что это больше не нужно т.к. на этом месте у нас пробел, но можно будет поставить
      // клик на джостик
      char command = buf[2]; // Команда находится на индексе 2, тк на индексе 1 у нас пробел
      
      // Числовой параметр начинается с индекса 3
      i = 3; 
      int number = 0; // переменная для сбора сообщения в строку
      
      // Посколько передача идет посимвольно, то нужно преобразовать набор символов в число
      // Считываем значения координаты Х пока не дойдем до символа "-"
      while (buf[i] != '-')
      {
        number *= 10;
        number += buf[i] - '0';
        i++;
      }
      
      // присваиваем координату X и обнуляем переменную
      xrVal = number;
      number = 0;
      i++; // передвигаемся по переменной buf на следующий индекс
      
      // Считываем значения координаты У пока не дойдем до символа " "
      while (buf[i] != ' ')
      {
        number *= 10;
        number += buf[i] - '0';
        i++;
      }
      
      // присваиваем координату У 
      yrVal = number;

      //Переводим в рабочие параметры    
        yVal = map(yrVal,0,1023, 110,-110);
        xVal = map(xrVal,0,1023,-40,20);

      // выводим в консоле полученные значения
        Serial.println("");Serial.println("");Serial.println("");
        Serial.print("xrVal=");   Serial.print(xrVal);
        Serial.print(" yrVal=");  Serial.print(yrVal);
        Serial.print("xVal=");    Serial.print(xVal);
        Serial.print(" yVal=");   Serial.println(yVal);
        


      // уже едем!

        if (yVal>=5) robo_forward (); // Если скорость положительная то едем вперед
        if (yVal>-5 && yVal<5) robo_stop ();
        if (yVal<=-5) robo_back (); 
        //else 
          //robo_back ();

    }
 
}

//functions

// езда вперед
int robo_forward ()
{
  // настройки для езды вперед
            
  digitalWrite (IN1, HIGH); 
  digitalWrite (IN2, LOW);
            
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW); 
            
  // если переменная Х больше нуля, то едем вправо 
  if (xVal>=0)
  {
   speedRight = abs(yVal - abs(xVal)); //убрал разность
   speedLeft =  abs(yVal + xVal);
  }
  else
  {
    speedRight = abs(yVal + abs(xVal));
    speedLeft =  abs(yVal - abs(xVal)*1.1); // убрал разность
  }
  
  Serial.println("****************");
  Serial.println("*I'm FORWARDing*");
  Serial.println("****************");
         
  analogWrite(EN1, speedRight);
  analogWrite(EN2, speedLeft);

  data_print ();

}




// езда назад
int robo_back ()
{
  // настройки для езды назад
        
  digitalWrite (IN2, HIGH); 
  digitalWrite (IN1, LOW);

  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW); 

  // если Х больше нуля, то едем влево
  if (xVal>=0)
    {
      speedRight = abs(yVal) + xVal;
      speedLeft =  abs(yVal) ;//- xVal/2;
    }
  else
    {
      speedRight = abs(yVal) ;//- abs(xVal/2);
      speedLeft =  abs(yVal) + abs(xVal);
    }

  if (speedRight<10) speedRight=0;
  if (speedLeft<10) speedLeft=0;

  Serial.println("***************");
  Serial.println("**I'm BACKing**");
  Serial.println("***************");

  // !!! Зависает на этих строчках
  analogWrite(EN1, speedRight);
  analogWrite(EN2, speedLeft);

  // выводим данные скорости в консоль
  data_print ();
          
}


// остановка
int robo_stop ()
{
  // настройки для езды назад
        
  digitalWrite (IN2, HIGH); 
  digitalWrite (IN1, LOW);

  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW); 

  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  // выводим данные скорости в консоль
  Serial.println("***************");
  Serial.println("**I'm STOPing**");
  Serial.println("***************");
  data_print ();
          
}

// для отладки вывод значений

int data_print ()
{
  Serial.println("***************");
  Serial.print("speedRight = ");  Serial.print (speedRight);  Serial.print(" | ");
  Serial.print("speedLeft = ");   Serial.print(speedLeft);    Serial.print(" | ");
  Serial.print("yVal = ");        Serial.print(yVal);         Serial.print(" | ");
  Serial.print("xVal = ");        Serial.println(xVal);
  Serial.println("***************");
}






