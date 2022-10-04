
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>	// mmap/munmap
#include <unistd.h>
#include <fcntl.h>


#include "myleds.h"

/* This function sends a message to the kernel */
void ioctl_set_msg(int file_desc, char *message)
{

}

/* This function reads a message from the kernel */
/* and just printk the message in kernel space */
void ioctl_get_msg(int file_desc)
{

}

int pow_2(int exponent)
{
  int result=1;
  for (int i=0; i<exponent; i++)
  {
    result = result*2;
  }
  return result;
}

int main()
{
  int mydevice_file; 
  
//   msg_received = malloc(msg_length); 

  mydevice_file = open(MYDEVICE_PATH, O_RDWR);
  if (mydevice_file == -1) 
  { 
    printf("ERROR OPENING FILE %s\n", MYDEVICE_PATH); 
    exit(EXIT_FAILURE); 
  }

//   // BASIC WRITE/READ TEST
  int i;
  for (i = 0; i < 3; i++)
  {
    // char c = i + '0';Z
    char *msg_passed = "0xff";
    char *msg_received; 
    int msg_length; 

    msg_length = strlen(msg_passed);
    write(mydevice_file, msg_passed, msg_length);
    read(mydevice_file, msg_received, msg_length);
    sleep(1);
    msg_passed = "0x00";
    write(mydevice_file, msg_passed, msg_length);
    read(mydevice_file, msg_received, msg_length);
    sleep(1);
  }
  int msg = 1;

  char buff[6];
  for (i = 0; i < 8; i++)
  {
    char *msg_received; 
    int num = pow_2(i);
    sprintf(buff, "0x%02x", num);
    int msg_length; 
    msg_length = strlen(buff);

    write(mydevice_file, buff, msg_length);
    read(mydevice_file, msg_received, msg_length);
    sleep(1);
  }
  for (i = 7; i >= 0; i--)
  {
    int num = pow_2(i);
    sprintf(buff, "0x%02x", num);
    int msg_length; 
    msg_length = strlen(buff);

    write(mydevice_file, buff, msg_length);
    
    sleep(1);
  }

  char *msg_received;
  int msg_length; 

  msg_length = strlen(msg_received);

  read(mydevice_file, msg_received, msg_length); 

  close(mydevice_file);
  return 0;
}

