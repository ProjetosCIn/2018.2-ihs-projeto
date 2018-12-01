#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

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

// Valores binarios na base 2
unsigned char binarios[] = {       0x1, 0x2, 0x4, 0x8, 
                             0x10, 0x20, 0x40, 0x80};


long int readButton(int dev, int *vButton){
  read(dev, vButton, BUTTON);
  read(dev, vButton, BUTTON);
}

long int readInport(int dev, int *vInport){
  read(dev, vInport, INPORT);
  read(dev, vInport, INPORT);
}

int botoes[4], valorBotao;
int switches[18]; 
long int valorSwitch;

int expo(int x, int n){
  int i = 0, result = 1;
  if( n == 0)
    return 1;
  for(; i < n; i++){
    result *= x;
  }
  return result;
}

void leitura(int dev);
void maquinaEstados(int dev);

int main() {
  
  int dev = open("/dev/de2i150_altera", O_RDWR);
  int tid;

  #pragma omp parallel sections num_threads(2) private(tid) shared(dev, botoes, switches, valorBotao, valorSwitch)
  {
    #pragma omp section
    {
      tid = omp_get_thread_num();
      printf("%d", tid);
      leitura(dev);
    }
      
    #pragma omp section
    {
      tid = omp_get_thread_num();
      printf("%d", tid);
      maquinaEstados(dev);
    }
  }    


  close(dev);
  
  return 0;
}

void maquinaEstados(int dev){
  int lixo = 0, k;
  int estado = 0;
  while(1){
    
      switch(estado){
        case 0:
          if(switches[0] == 1)
            estado = 1;

          break;
        case 1:
            
          break;
        default:
          break;
      }
    while(estado == 0){
      if(switches[0] == 1)
        estado = 1;
    }

    while(estado == 1){

    }

    printf("switch %d\n", valorSwitch);
    read(dev, &lixo, INPORT);
    write(dev, &valorSwitch, HEXPORT);
    for(lixo = 0; lixo < 100000000; lixo++){

    }
  }
}

void leitura (int dev) {
  while(1){
    long int vInport, vButton, lixo, i, k, l, m; 
    // Leitura dos botoes
    read(dev, &lixo, BUTTON);
    read(dev, &vButton, BUTTON);

    valorBotao = vButton;
    // Atualizacao Valores dos botoes para boolean
    printf("Botao :");
    for(m = 0; m < 4; m++){
      botoes[m] = (~vButton & binarios[m] ) / expo(2, m);
    }
    printf("\n");
    for(m = 3; m >= 0; m--){
      printf("%d", botoes[m]);
    }
    printf("\n");

    for(lixo = 0; lixo < 100000000; lixo++){

    }


    read(dev, &lixo, INPORT);
    read(dev, &vInport, INPORT);

    valorSwitch = vInport;
    printf("Valor switch minha thread %d\n", valorSwitch);
    for(m = 0; m < 16; m++){
      if(m >= 8)
        switches[m] = ((vInport >> 8) & binarios[m - 8] ) / expo(2, m % 8);
      else
        switches[m] = (vInport & binarios[m] ) / expo(2, m);
    }
    printf("Switches\n");
    for(m = 15; m >= 0; m--){
      printf("%d", switches[m]);
    }
    printf("\n");


          
        //   // read(dev, &vInport, INPORT);
        //   // printf("%ld\n", vInport);

        //   // readInport(dev, &vInport);
        //   // readButton(dev, &vButton);
          
        //   printf("asada\n");
        //   for(lixo = 0; lixo < 1000000; lixo++){

        //   }
        //   // printf("leitura inport %ld  ",vInport );   

        //   // read(dev, &vButton, BUTTON);
        //   // read(dev, &vButton, BUTTON);
          
        //   printf("leitura vButton %ld \n", vButton);

          // k = hexdigit[vButton & 0xF]
          //   | (hexdigit[(vButton >>  4) & 0xF] << 8)
          //   | (hexdigit[(vButton >>  8) & 0xF] << 16)
          //   | (hexdigit[(vButton >> 12) & 0xF] << 24);
          // k = ~k;
          
          // read(dev, &lixo, INPORT);
          // write(dev, &k, HEXPORT);

          //  k = hexdigit[vButton & 0xF]
          //   | (hexdigit[(vButton >>  4) & 0xF] << 8)
          //   | (hexdigit[(vButton >>  8) & 0xF] << 16)
          //   | (hexdigit[(vButton >> 12) & 0xF] << 24);
          // k = ~k;

        //   read(dev, &lixo, INPORT);
        //   write(dev, &k, HEXDISPLAY_1);

        //   read(dev, &lixo, INPORT);
        //   write(dev, &vInport, LED_R);
        }
}
