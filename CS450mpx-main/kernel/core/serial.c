/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>
#include "modules/mpx_supt.h"

#define NO_ERROR 0

// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device)
{
  outb(device + 1, 0x00); //disable interrupts
  outb(device + 3, 0x80); //set line control register
  outb(device + 0, 115200/9600); //set bsd least sig bit
  outb(device + 1, 0x00); //brd most significant bit
  outb(device + 3, 0x03); //lock divisor; 8bits, no parity, one stop
  outb(device + 2, 0xC7); //enable fifo, clear, 14byte threshold
  outb(device + 4, 0x0B); //enable interrupts, rts/dsr set
  (void)inb(device);      //read bit to reset port
  return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  outb(serial_port_out,'\r');
  outb(serial_port_out,'\n');  
  return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  if (*msg == '\r') outb(serial_port_out,'\n');
  return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device)
{
  serial_port_out = device;
  return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device)
{
  serial_port_in = device;
  return NO_ERROR;
}


////polling code

// The symbol used to indicate the cursor 
const char cursor_symbol = '|';

// returns the char corresponding to integer `a` which may be 0-9.
char digit_to_char(int a){
  if( a<0 || a > 9)
    return 0;

  return a + '0';
}

//prints integer to console
void print_int(int a){
  int numberplace = 10000;
  char any_printed = 0;

  char s[] = {0,0};
  
  if(a<0){
    s[0] = '-';
    serial_print(s);
  }

  do{
    int digit = (a / numberplace)%10;
    s[0] = digit_to_char(digit);

    switch(digit){
      case 0:
        if(!any_printed)
          break;
      default:
        serial_print(s);
        any_printed = 1;
    }

    numberplace /= 10;

  }while(numberplace > 0);
}

// print carriage return and print count+4 spaces to clear terminal
void clear_line(int count){
  count = count +4; //account for cursor and $.
  outb(serial_port_out,'\r');
  int i;
  for(i = 0; i<= count; i++){
    outb(serial_port_out,' ');
  }
  outb(serial_port_out,'\r');
}

//inserts character `c` at given index, shifting everything after index to the right.
void string_insert(char* buffer, int buffer_len, int* count, int index, char c){
  if(*count >= buffer_len)
    return;

  int i;
  for(i = *count; i > index; i--){
    buffer[i] = buffer[i-1];
  }
  buffer[index] = c;
  *count = *count +1;
}

// removes the character from the buffer string located at index
void string_remove(char* buffer, int* count, int index){
  if(*count < index)
    outb(serial_port_out,'!');
  int i;
  for(i = index; i<*count; i++){
    buffer[i] = buffer[i+1];
  }
  buffer[*count-1]='\0';
  if(*count > 0)
    (*count)--;
}

// prints content of buffer with the cursor inserted
void string_print_cursor(char* buffer, int count, int cursor_index){
  clear_line(count);
  serial_print("$ ");
  int i;
  for(i = 0; i<cursor_index && i<count; i++){
    outb(serial_port_out,buffer[i]);
  }

  if(cursor_index <= count)
    outb(serial_port_out, cursor_symbol);

  for(i = cursor_index; i<count; i++){
    outb(serial_port_out,buffer[i]);
  }
}

int *polling(char *buffer, int *count){
  memset(buffer, 0, *count);
  int buffer_len = *count-1;
  *count = 0;
  int cursor = 0;

  // get sec
  outb(0x70, 0x00);
  unsigned char sec = inb(0x71);
  
  char in[4] = {0,0,0,0};
  do{

    while(!inb(COM1 +5));

    
    *in = inb(COM1);

    outb(0x70, 0x00);
    unsigned char cursec = inb(0x71);
    if(sec!=cursec){
      clear_line(*count);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

      outb(0x70, 0x00);
      cursec = inb(0x71);
      sec = cursec;
      string_print_cursor(buffer, *count, cursor);
    }
    

    switch(*in){
      case '\r':
      case '\n':
      case '\0':
      break;
      case 27: // special control sequences
        while(!inb(COM1 +5));
        in[1] = inb(COM1);
        while(!inb(COM1 +5));
        in[2] = inb(COM1);

        switch(in[2]){
          case 'A': // up
          case 'B': // down
          break;
          case 'C': // right
            if(cursor >= *count)
              break;
            
            //serial_print(in);
            cursor++;
            break;
          case 'D': // left
            //serial_print(in);
            cursor = cursor>0? cursor-1 : 0;
            break;
          case 70: // end
            cursor = *count; break;
          case 72: // home
            cursor = 0; break;

          case '3': //delete
            while(!inb(COM1 +5));
            in[2] = inb(COM1);

            if(cursor >= *count){
              cursor = *count;
              break;
            }
            
            string_remove(buffer, count, cursor);
            string_print_cursor(buffer, *count, cursor);
            break;

          default:
            outb(serial_port_out,'\n');
            serial_print("unhandled control sequence: ");
            print_int(in[0]);
            outb(serial_port_out,' ');
            print_int(in[1]);
            outb(serial_port_out,' ');
            print_int(in[2]);
            outb(serial_port_out,' ');
            outb(serial_port_out,'\n');
        }

        string_print_cursor(buffer, *count, cursor);

        in[1] = '\0';
        break;
        //END special control sequences
      case 23:
      case 8: //Ascii code for Backspace
        if(cursor > 0) //decrease count by one
          cursor--;
        string_remove(buffer, count, cursor);
        string_print_cursor(buffer, *count, cursor);
        break;
      case 127: //Ascii code for Delete

            if(cursor >= *count){
              cursor = *count;
              break;
            }
            string_remove(buffer, count, cursor);
            string_print_cursor(buffer, *count, cursor);
            break;
      default:
        if(*in >= 32){
          string_insert(buffer,buffer_len, count,cursor, in[0]);
          cursor++;
          string_print_cursor(buffer, *count, cursor);
        }
        else{
          outb(serial_port_out, '\n');
          print_int(*in);
          outb(serial_port_out, '\n');
          string_print_cursor(buffer, *count, cursor);
        }
    }

  }while(*in != '\n' && *in != '\r');

  cursor = *count + 2;
  string_print_cursor(buffer, *count, cursor);
  outb(serial_port_out, '\n');

return count;
}
