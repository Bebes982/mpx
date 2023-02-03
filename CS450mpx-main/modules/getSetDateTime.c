#include "mpx_supt.h"
#include <string.h>
#include <core/io.h>

// converts 2 digit int into char
unsigned char int_to_bcd(int num){
    unsigned char digit1 = num%10;
    num = num / 10;
    unsigned char digit2 = num%10;
    return (digit2<<4) | digit1;
}

// converts binary coded digit into an int
int bcd_to_int(char bcd){
  int digit1 = bcd & (char)15;
  int digit2 = (bcd & 15<<4)>>4;

  return digit1 + (digit2*10);
}

void getDate(){
    
    // get century
    outb(0x70, 0x32); // write an index to the RTC index register (0x70) with outb()
    unsigned char cent_bcd = inb(0x71); // read the result from the RTC data reguster (0x71) using inb()

    // get year
    outb(0x70, 0x09);
    unsigned char year_bcd = inb(0x71);

    // get month
    outb(0x70, 0x08);
    unsigned char month_bcd = inb(0x71);

    // get day
    outb(0x70, 0x07);
    unsigned char day_bcd = inb(0x71);
    
    // convert bcd to string here
    char message[] = {"yyyy-mm-dd"};
    char* year = &message[0];
    char* month = &message[5];
    char* day = &message[8];

    year[0] = ((cent_bcd & 0xF0)>>4) + '0';
    year[1] = (cent_bcd & 0xF) + '0';
    year[2] = ((year_bcd & 0xF0)>>4) + '0';
    year[3] = (year_bcd & 0xF) + '0';

    month[0] = ((month_bcd & 0xF0)>>4) + '0';
    month[1] = (month_bcd & 0xF) + '0';

    day[0] = ((day_bcd & 0xF0)>>4) + '0';
    day[1] = (day_bcd & 0xF) + '0';

    // print message
    int messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
}

void getTime(){
    
    // get hour
    outb(0x70, 0x04);
    unsigned char hr = inb(0x71);

    // get min
    outb(0x70, 0x02);
    unsigned char min = inb(0x71);

    // get sec
    outb(0x70, 0x00);
    unsigned char sec = inb(0x71);

    // convert bcd to string here
    int hrNum = bcd_to_int(hr);
    int minNum = bcd_to_int(min);
    int secNum = bcd_to_int(sec);

    char* hrStr = {0};
    hrStr = itoa(hrNum, hrStr);
    char* minStr = {0};
    minStr = itoa(minNum, minStr);
    char* secStr = {0};
    secStr = itoa(secNum, secStr);
    

    // construct, print/return string
    // convert bcd to string here
    char message[] = {"hh:mm:ss"};
    char* hour = &message[0];
    char* minute = &message[3];
    char* second = &message[6];

    hour[0] = ((hr & 0xF0)>>4) + '0';
    hour[1] = (hr & 0xF) + '0';

    minute[0] = ((min & 0xF0)>>4) + '0';
    minute[1] = (min & 0xF) + '0';

    second[0] = ((sec & 0xF0)>>4) + '0';
    second[1] = (sec & 0xF) + '0';

    int messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
}

void setDate(char* str){

    // check length of string
    if(strlen(str) != 10){
        char message[] = {"input wrong size"};
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }
    
    // covnert str in format yyyy-mm-dd to int
    int cent = (str[0]-'0')*10 + str[1]-'0';
    int year = (str[2]-'0')*10 + str[3]-'0';
    int month = (str[5]-'0')*10 + str[6]-'0';
    int day = (str[8]-'0')*10 + str[9]-'0';

    // check vals
    if(cent < 0 || year < 0 || month < 0 || day < 0 || month > 12 || day > 31){
        char message[] = {"invalid input"};
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }

    // convert ints to chars
    unsigned char bcdc = int_to_bcd(cent);
    unsigned char bcdy = int_to_bcd(year);
    unsigned char bcdm = int_to_bcd(month);
    unsigned char bcdd = int_to_bcd(day);

    // set bits
    
    cli();
    outb(0x70, 0x32);
    outb(0x71, bcdc);

    outb(0x70, 0x09);
    outb(0x71, bcdy); // bcd containing year data

    outb(0x70, 0x08);
    outb(0x71, bcdm);

    outb(0x70, 0x07);
    outb(0x71, bcdd);
    sti();
    
}

void setTime(char* str){
    
    if(strlen(str) != 8){
        char message[] = {"input wrong size"};
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }

    // hh:mm:ss
    int hour = (str[0]-'0')*10 + str[1]-'0';
    int minute = (str[3]-'0')*10 + str[4]-'0';
    int sec = (str[6]-'0')*10 + str[7]-'0';

    // check vals
    if(hour < 0 || minute < 0 || sec < 0 || hour > 24 || minute > 59 || sec > 59){
        char message[] = {"invalid input"};
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }

    unsigned char bcdh = int_to_bcd(hour);
    unsigned char bcdmin = int_to_bcd(minute);
    unsigned char bcds = int_to_bcd(sec);
    

    // set bits
    cli();
    outb(0x70, 0x04);
    outb(0x71, bcdh); // bcd containing year data

    outb(0x70, 0x02);
    outb(0x71, bcdmin);

    outb(0x70, 0x00);
    outb(0x71, bcds);

    sti();
}
