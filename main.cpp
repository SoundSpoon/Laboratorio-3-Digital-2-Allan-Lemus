//▶︎ •၊၊||၊|။|||| |၊၊•    •၊၊||၊|။||||။‌‌‌‌‌၊|• 
// Universidad del Valle de Guatemala
// BE3029- Electronica digital 2
// Allan Lemus
// 8/08/25
// Laboratorio 3
// MCU: ESP32
//▶︎ •၊၊||၊|။|||| |၊၊•    •၊၊||၊|။||||။‌‌‌‌‌၊|•


//▶︎ •၊၊||၊|။|||| |၊၊•  Librerías  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

#include <Arduino.h>
#include <stdint.h>


//▶︎ •၊၊||၊|။|||| |၊၊•  Definiciones  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

#define led1 15
#define led2 2
#define led3 4
#define led4 16
#define led5 13
#define led6 12
#define led7 14
#define led8 27
#define b1 18
#define b2 19
#define buzz 26
#define cable 32


//▶︎ •၊၊||၊|။|||| |၊၊•  Prototipos de funciones  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

void binario(int);
void binariotimer(int);



//▶︎ •၊၊||၊|။|||| |၊၊•  Variables Globales  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

volatile int contador = 0; 
volatile int contador2 = 0; 
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED; 
hw_timer_t *timer = NULL;//instale el timer

//▶︎ •၊၊||၊|။|||| |၊၊•  ISRs rutinas de interrupción  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

//Variables para las ISRs
volatile unsigned long lastpressed1 = 0;
volatile unsigned long lastpressed2 = 0;
volatile bool reseteo = false;

void IRAM_ATTR avanzar() {
  unsigned long tiempo = millis();
  if (tiempo - lastpressed1 > 250) {
    portENTER_CRITICAL_ISR(&mux);
    contador = (contador + 1) % 16; //Desactivamos las interrupciones para cambiar el contador en el resto del código
    portEXIT_CRITICAL_ISR(&mux);
    lastpressed1 = tiempo;
  }
}

void IRAM_ATTR atras() {
  unsigned long tiempo = millis();
  if (tiempo - lastpressed2 > 250) {
    portENTER_CRITICAL_ISR(&mux);
    contador = (contador - 1 + 16) % 16; //Desactivamos las interrupciones para cambiar el contador en el resto del código
    portEXIT_CRITICAL_ISR(&mux);
    lastpressed2 = tiempo;
  }
}
void IRAM_ATTR reloj() {
  portENTER_CRITICAL_ISR(&mux);//desactivamos la ISR
  contador2 = (contador2 + 1) % 16; 
  portEXIT_CRITICAL_ISR(&mux);//activamos la ISR
}

//▶︎ •၊၊||၊|။|||| |၊၊•  Configuraciones  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

void setup() {
  //declaramos todas las variables del 
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
pinMode(led5, OUTPUT);
pinMode(led6, OUTPUT);
pinMode(led7, OUTPUT);
pinMode(led8, OUTPUT);
pinMode(b1, INPUT);
pinMode(b2, INPUT);
pinMode(buzz, OUTPUT);
attachInterrupt(b1, avanzar, HIGH);
attachInterrupt(b2, atras, HIGH);
timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &reloj, true);
  timerAlarmWrite(timer, 500000, true); 
  timerAlarmEnable(timer);
}

//▶︎ •၊၊||၊|။|||| |၊၊•  Loop infinito  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

void loop() {
  
  portENTER_CRITICAL(&mux);//desactivamos la ISR
  int valor2 = contador2;
  portEXIT_CRITICAL(&mux);//activamos la ISR
  binariotimer(valor2);

  portENTER_CRITICAL(&mux); 
  int valor1 = contador; //Nos permite utilizar la variable de las ISRs
  portEXIT_CRITICAL(&mux); 
  binario(valor1);
  delay(10);

  if (touchRead(cable)<40){//Lee si se toca el cable para poder resetear el contador del timer
    contador2 = 0;
    reseteo = false;
    digitalWrite(buzz, LOW);
  }
  delay(100);

  if (contador == contador2){//cambio de contador 2 para avanzar en el timer de las leds
    reseteo = !reseteo;
    if (reseteo){
      digitalWrite(buzz, HIGH);
    }
    else{
      digitalWrite(buzz, LOW);
    }
    contador2 = 0;
  }
}

//▶︎ •၊၊||၊|။|||| |၊၊•  Otras funciones  •၊၊||၊|။||||။‌‌‌‌‌၊|• 

void binario(int numero) {
  // Apagar todos los LEDs primero
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);

  // mostrar el numero
  switch (numero) {
    case 0: 
    break;
    case 1: 
      digitalWrite(led1, HIGH); 
    break;
    case 2: 
      digitalWrite(led2, HIGH); 
    break;
    case 3: 
      digitalWrite(led1, HIGH); 
      digitalWrite(led2, HIGH); 
    break;
    case 4: 
      digitalWrite(led3, HIGH); 
    break;
    case 5: 
      digitalWrite(led1, HIGH); 
      digitalWrite(led3, HIGH);
     break;
    case 6:
      digitalWrite(led2, HIGH); 
      digitalWrite(led3, HIGH);
    break;
    case 7: 
      digitalWrite(led1, HIGH); 
      digitalWrite(led2, HIGH); 
      digitalWrite(led3, HIGH); 
    break;
    case 8: 
      digitalWrite(led4, HIGH);
    break;
    case 9: 
      digitalWrite(led1, HIGH); 
      digitalWrite(led4, HIGH);
    break;
    case 10: 
      digitalWrite(led2, HIGH); 
      digitalWrite(led4, HIGH); 
    break;
    case 11: 
      digitalWrite(led1, HIGH); 
      digitalWrite(led2, HIGH); 
      digitalWrite(led4, HIGH); 
    break;
    case 12: 
      digitalWrite(led3, HIGH); 
      digitalWrite(led4, HIGH); 
    break;
    case 13: 
      digitalWrite(led1, HIGH); 
      digitalWrite(led3, HIGH); 
      digitalWrite(led4, HIGH); 
    break;
    case 14: 
      digitalWrite(led2, HIGH); 
      digitalWrite(led3, HIGH); 
      digitalWrite(led4, HIGH); 
    break;
    case 15: 
      digitalWrite(led1, HIGH); 
      digitalWrite(led2, HIGH); 
      digitalWrite(led3, HIGH); 
      digitalWrite(led4, HIGH);
    break;
    default: 
    break;
  }
}

void binariotimer(int numero2) {
  // Apagar todos los LEDs primero
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);

  // mostrar el numero
  switch (numero2) {
    case 0: 
    break;
    case 1: 
      digitalWrite(led5, HIGH); 
    break;
    case 2: 
      digitalWrite(led6, HIGH); 
    break;
    case 3: 
      digitalWrite(led5, HIGH); 
      digitalWrite(led6, HIGH); 
    break;
    case 4: 
      digitalWrite(led7, HIGH); 
    break;
    case 5: 
      digitalWrite(led5, HIGH); 
      digitalWrite(led7, HIGH);
     break;
    case 6:
      digitalWrite(led6, HIGH); 
      digitalWrite(led7, HIGH);
    break;
    case 7: 
      digitalWrite(led5, HIGH); 
      digitalWrite(led6, HIGH); 
      digitalWrite(led7, HIGH); 
    break;
    case 8: 
      digitalWrite(led8, HIGH);
    break;
    case 9: 
      digitalWrite(led5, HIGH); 
      digitalWrite(led8, HIGH);
    break;
    case 10: 
      digitalWrite(led6, HIGH); 
      digitalWrite(led8, HIGH); 
    break;
    case 11: 
      digitalWrite(led5, HIGH); 
      digitalWrite(led6, HIGH); 
      digitalWrite(led8, HIGH); 
    break;
    case 12: 
      digitalWrite(led7, HIGH); 
      digitalWrite(led8, HIGH); 
    break;
    case 13: 
      digitalWrite(led5, HIGH); 
      digitalWrite(led7, HIGH); 
      digitalWrite(led8, HIGH); 
    break;
    case 14: 
      digitalWrite(led6, HIGH); 
      digitalWrite(led7, HIGH); 
      digitalWrite(led8, HIGH); 
    break;
    case 15: 
      digitalWrite(led5, HIGH); 
      digitalWrite(led6, HIGH); 
      digitalWrite(led7, HIGH); 
      digitalWrite(led8, HIGH);
    break;
    default: 
    break;
  }
}