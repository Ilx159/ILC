#ifndef ILCTERMINALINPUT_H
#define ILCTERMINALINPUT_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void die(const char *s);
void rawModeOff();
void rawModeOn();
char sInput();
char input();

#ifdef ILCTERMINALINPUT_IMPLEMENTATION
struct termios origin_termios;

char ver = 0;

void die(const char *s){
  perror(s);
  exit(1);
}

void rawModeOff(){
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios) == -1)
    die("tcsetattr");
  ver = 0;
}

void rawModeOn(){

  if(tcgetattr(STDIN_FILENO, &origin_termios) == -1) die("tcgetattr");
  atexit(rawModeOff);

  struct termios raw = origin_termios;

  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  //raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_cc[VMIN]=0;
  raw.c_cc[VTIME]=1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
  ver = 1;
} 

char sInput(){
  char key;

  if(!ver){
    printf("raw_mode is disable");
    return -1;
  }
  if(read(STDIN_FILENO, &key, 1) == -1 && errno !=EAGAIN) die("read");
  
  return key;
}

char input(){
  char key;

  read(STDIN_FILENO, &key, 1);
  
  return key;
}

#endif
#endif
