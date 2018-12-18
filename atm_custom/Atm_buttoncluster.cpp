#pragma once
#include "Atm_buttoncluster.hpp"

Atm_buttoncluster &Atm_buttoncluster::begin(char _red_pin, char _brown_pin, char _black_pin )
{
    // clang-format off
  const static state_t state_table[] PROGMEM = {
    /* Standard Mode: press/repeat */
    /*                        ON_ENTER    ON_LOOP       ON_EXIT     EVT_RED      EVT_BROWN      EVT_BLACK   ELSE */
    /* CLUSTER_RED    */       ENT_RED,        -1,    EXIT_RED,          -1, CLUSTER_BROWN, CLUSTER_BLACK,    -1,   
    /* CLUSTER_BROWN  */     ENT_BROWN,        -1,  EXIT_BROWN, CLUSTER_RED,            -1, CLUSTER_BLACK,    -1,    
    /* CLUSTER_BLACK  */     ENT_BLACK,        -1,  EXIT_BLACK, CLUSTER_RED, CLUSTER_BROWN,            -1,    -1
  };
    // clang-format on
    Machine::begin(state_table, ELSE);
    pinMode(_red_pin, OUTPUT);
    pinMode(_brown_pin, OUTPUT);
    pinMode(_black_pin, OUTPUT);
    red_pin = _red_pin;
    brown_pin = _brown_pin;
    black_pin = _black_pin;
    return *this;
}

int Atm_buttoncluster::event(int id)
{
    return 0;
}

void Atm_buttoncluster::action(int id)
{
    int press;
    switch (id)
    {
    case ENT_RED:
        digitalWrite(red_pin, HIGH);
        return;
    case ENT_BROWN:
        digitalWrite(brown_pin, HIGH);
        return;
    case ENT_BLACK:
        digitalWrite(black_pin, HIGH);
        return;
    case EXIT_RED:
        digitalWrite(red_pin, LOW);
        return;
    case EXIT_BROWN:
        digitalWrite(brown_pin, LOW);
        return;
    case EXIT_BLACK:
        digitalWrite(black_pin, LOW);
        return;
    }
}

Atm_buttoncluster &Atm_buttoncluster::trace(Stream &stream)
{
  setTrace(&stream, atm_serial_debug::trace,
    "BUTTCLUSTER\0EVT_RED\0EVT_BROWN\0EVT_BLACK\0ELSE\0CLUSTER_RED\0CLUSTER_BROWN\0CLUSTER_BLACK");
  return *this;
}