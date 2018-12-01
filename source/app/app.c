#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
// #include "../biblioMapping.h"

#define BUTTON 1
#define LED_G 2
#define LED_R 3
#define INPORT 4
#define HEXPORT 5
#define HEXDISPLAY_1 6

unsigned char hexdigit[] = {0x3F, 0x06, 0x5B, 0x4F,
                            0x66, 0x6D, 0x7D, 0x07, 
                            0x7F, 0x6F, 0x77, 0x7C,
			                      0x39, 0x5E, 0x79, 0x71};


long int readButton(int dev){
  long int vButton;
  read(dev, &vButton, BUTTON);
  read(dev, &vButton, BUTTON);
  return vButton;
}

long int readInport(int dev){
  long int vInport;
  read(dev, &vInport, INPORT);
  read(dev, &vInport, INPORT);
  return vInport;
}


int main() {
  long int i, vInport,vButton, k, l;
  
  int dev = open("/dev/de2i150_altera", O_RDWR);
  

  while(1){
    vInport = readInport(dev);
    
    
    printf("leitura inport %ld | \n",vInport );
    
    

    // read(dev, &vButton, BUTTON);
    // read(dev, &vButton, BUTTON);
    // printf("leitura butao %ld\n",vButton );

    k = hexdigit[vInport & 0xF]
      | (hexdigit[(vInport >>  4) & 0xF] << 8)
      | (hexdigit[(vInport >>  8) & 0xF] << 16)
      | (hexdigit[(vInport >> 12) & 0xF] << 24);
    k = ~k;
    write(dev, &vInport, LED_R);


  }     
    


  close(dev);
  
  return 0;
}

