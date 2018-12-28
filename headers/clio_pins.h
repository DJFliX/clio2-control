#pragma once

#define RED_CLUSTER 0
#define BLACK_CLUSTER 1
#define BROWN_CLUSTER 2

#define GREEN 0
#define BLUE 1
#define YELLOW 2

#define GREEN_PIN A0    //Pulldown to GND
#define BLUE_PIN A1     //Pulldown to GND
#define YELLOW_PIN A2   //Pulldown to GND
#define BROWN_PIN 2     //Enable pin for Scrolling cluster
#define BLACK_PIN 5     //Enable pin for Red cluster (Vol-, Vol+, Toggle)
#define RED_PIN 6       //Enable pin for Black cluster (Mute, Src-, Src+)

#define HU_TRANSISTOR 9 //Transistor to enable or disable HU control output
