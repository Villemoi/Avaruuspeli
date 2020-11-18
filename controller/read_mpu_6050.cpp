#include "mbed.h"
#include "MPU6050.h"

Ticker msTick;
int gX, ogX, gY, ogY, gZ, ogZ, ms, timestamp;
float degX, degY, degZ;

MPU6050 mpu(I2C_SDA,I2C_SCL);
void advanceTime(){
    ms++;
}

int main(){
    msTick.attach_us(advanceTime, 1000); // ajastin
    if(!mpu.testConnection()){
        printf("Unable to connect");
    }
    while (true) {
    //gX = mpu.getGyroRawX();
    gY = mpu.getGyroRawY(); // näytetään vain gY akseli
    //gZ = mpu.getGyroRawZ();
    // nollakorjaukset
    //gX = gX + 25;
    gY = gY + 310;
    //gZ = gZ + 150;
    // lasketaan kulmakertymä
    degY = degY + (gY * (ms - timestamp)) / 1000 / 131;
    timestamp = ms;
    printf("%d, %.3f, %d\n", 100, degY, -100);
    }
}
