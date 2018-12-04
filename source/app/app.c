#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <linux/kd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>



long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

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
unsigned int ledreddigit[] = {0,131072, 65536, 32768,
                            16384, 8192, 4096, 2048,
                            1024, 512, 256,
                            128, 64, 32, 16,
                            8, 4, 2, 1};//Led[i] para acender o led na posição i.
                            //O MORSE ESCREVE DE I=0 ATÉ I=8;
// Valores binarios na base 2
unsigned char binarios[] = {0x1, 0x2, 0x4, 0x8, 
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
char buffer[6];
int numeroDeChars =0;

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
void beepBuzzer(int dev);
void maquinaEstados(int dev);
void atualizarLedRed(int dev);

void controleDisplays(int dev);

int main() {
  
  int dev = open("/dev/de2i150_altera", O_RDWR);
  int tid;

  #pragma omp parallel sections num_threads(4) private(tid) shared(dev, botoes, switches, valorBotao, valorSwitch)
  {
    

    #pragma omp section
    {
      tid = omp_get_thread_num();
      // printf("%d", tid);
      beepBuzzer(dev);

    }

    #pragma omp section
    {
      tid = omp_get_thread_num();
      // printf("%d", tid);
      leitura(dev);
    }
      
    #pragma omp section
    {
      tid = omp_get_thread_num();
      // printf("%d", tid);
      maquinaEstados(dev);
    }

    #pragma omp section
    {
      tid = omp_get_thread_num();
      // printf("%d", tid);
      controleDisplays(dev);
    }
  }    


  close(dev);
  
  return 0;
}

char translateChar(char buf[]){
    int size = strlen(buf);
    int i;
    switch(size){
        case 1://Só tem um digito
            if(buf[0]== '.'){//.
                return 'e';
            }
            else{//-
                if(buf[0]=='/'){
                    return ' ';
                }
                else{
                    return 't';
                }
            }
            break;
        case 2:
            if(buf[0]== '.'){//.
                if(buf[1]== '.'){//..
                    return 'i';
                }
                else{//.-
                    return 'a';
                }
            }
            else{//-
                if(buf[1]== '.'){//-.
                    return 'n';
                }
                else{//--
                    return 'm';
                }
            }
            break;
        case  3:
            if(buf[0]== '.'){//.
                if(buf[1]== '.'){//..
                    if(buf[2]== '.'){//...
                        return 's';
                    }
                    else{//..-
                        return 'u';
                    }
                }
                else{//.-
                    if(buf[2]== '.'){//.-.
                        return 'r';
                    }
                    else{//.--
                        return 'w';
                    }
                }
            }
            else{//-
                if(buf[1]== '.'){//-.
                    if(buf[2]== '.'){//-..
                        return 'd';
                    }
                    else{//-.-
                        return 'k';
                    }
                }
                else{//--
                    if(buf[2]== '.'){//--.
                        return 'g';
                    }
                    else{//---
                        return 'o';
                    }
                }
            }
            break;
        case 4:
            if(buf[0]== '.'){//.
                if(buf[1]== '.'){//..
                    if(buf[2]== '.'){//...
                        if(buf[3]== '.'){//....
                            return 'h';
                        }
                        else{//...-
                            return 'v';
                        }
                    }
                    else{//..-
                        if(buf[3]== '.'){//..-.
                            return 'f';
                        }
                        else{//..--
                            return ' ';//NULL
                        }
                    }
                }
                else{//.-
                    if(buf[2]== '.'){//.-.
                        if(buf[3]== '.'){//.-..
                            return 'l';
                        }
                        else{//.-.-
                            return ' ';
                        }
                    }
                    else{//.--
                        if(buf[3]== '.'){//.--.
                            return 'p';
                        }
                        else{//.---
                            return 'j';
                        }
                    }
                }
            }
            else{//-
                if(buf[1]== '.'){//-.
                    if(buf[2]== '.'){//-..
                        if(buf[3]== '.'){//-...
                            return 'b';
                        }
                        else{//-..-
                            return 'x';
                        }
                    }
                    else{//-.-
                        if(buf[3]== '.'){//-.-.
                            return 'c';
                        }
                        else{//-.--
                            return 'y';
                        }
                    }
                }
                else{//--
                    if(buf[2]== '.'){//--.
                        if(buf[3]== '.'){//--..
                            return 'z';
                        }
                        else{//--.-
                            return 'q';
                        }
                    }
                    else{//---
                        if(buf[3]== '.'){//---.
                            return ' ';
                        }
                        else{//----
                            return ' ';
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
}


void translate(char vetor[]){
  char fraseTranslate[250];
  const char s[2] = " ";
  char *token;
   
   /* get the first token */
  token = strtok(vetor, s);
   
   /* walk through other tokens */
  int index = 0;
  while( token != NULL ) {
    fraseTranslate[index++] = translateChar(token);
    token = strtok(NULL, s);
  }
  fraseTranslate[index] = '\0';
  printf("Frase translate: %s \n", fraseTranslate);
}

void controleDisplays(int dev){
  int lixo;
  int k;
  while(1){
    k = hexdigit[numeroDeChars & 0xF]
            | (hexdigit[(numeroDeChars >>  4) & 0xF] << 8)
            | (hexdigit[(numeroDeChars >>  8) & 0xF] << 16)
            | (hexdigit[(numeroDeChars >> 12) & 0xF] << 24);
          k = ~k;
    read(dev, &lixo, INPORT);
    write(dev, &k, HEXPORT);
    }
}


void beepBuzzer(int dev){
    int buzz = open("/dev/tty10", O_RDONLY),i = 200,h; 
    if (buzz == -1 ){// não conseguiu acessar o arquivo
            printf("Não conseguimos acessar o buzzer . FD = %d",buzz);
    }
    h= 9000; // Frequência da Nota
    i= 10; // Tempo de duração
    int lixo = 0;
    int valorLedG;
    while(1){
        if(botoes[0]==1 && switches[0]==1){
          valorLedG = 255;
          write(dev, &valorLedG, LED_G);
          
          ioctl(buzz, KDMKTONE, (i<<16)+(1193180/h));//Toca o buzzer DESCOMENTAR DEPOIS
          // read(dev, &lixo, INPORT);
          // write(dev, &vInport, LED_R);
          usleep(1000*i);//sleep

        }
        else{
          valorLedG = 0;
          write(dev, &valorLedG, LED_G);
        } 
      atualizarLedRed(dev);
    }
}

int acenderRed = 0;

void atualizarLedRed(int dev){
  int i;
  int indice = 1;
  for(i=0;i<4;i++){
    if(buffer[i]=='.'){
      acenderRed = (acenderRed | ledreddigit[indice++]);
    }
    else if(buffer[i]=='-'){
      acenderRed = (acenderRed | ledreddigit[indice++]);
      acenderRed = (acenderRed | ledreddigit[indice++]);
    }
    indice++;
  }
  write(dev, &acenderRed, LED_R);
}

void maquinaEstados(int dev){

  int lixo = 0, k;
  int estado = 0;
  int ajusteWord = 0;
  long long tempoAntigo,tempoAtual, diferencaTempo = 0;
  char vazio[5] = {' ',' ',' ',' ',' '};
  char frase[250] = {0};
  int indiceFrase = 0;
  char atual = 'a';
  int i;
  int contador = 0;
  int oi;
  oi = hexdigit[256 & 0xF]
    | (hexdigit[(256 >>  4) & 0xF] << 8)
    | (hexdigit[(256 >>  8) & 0xF] << 16)
    | (hexdigit[(256 >> 12) & 0xF] << 24);
    oi = ~oi;
  write(dev, &oi, HEXDISPLAY_1);//Desliga o display 1 
  int controleAnt, controleAtu;
  int indiceBuffer = 0;
  while(1){
      controleAnt = controleAtu;
      controleAtu = botoes[1];
      switch(estado){
        case 0:
          if(switches[0] == 1){
            system("clear");
            printf("Inicie sua escrita \n");
            estado = 1;
          }
          if(controleAnt==0 && controleAtu==1){ //Só printa na subida do sinal do botão.
            system("clear");
            printf("Enviamos essa frase: |||%s|||\n",frase );
            translate(frase);
            numeroDeChars = 0;
            frase[0] ='\0';
            indiceFrase = 0;
          }
          break;
        case 1:
          if(botoes[0]==1){
            estado = 2;
            tempoAntigo = current_timestamp();
          }
          else if(switches[0]==0){
            estado = 0;
            printf("Estado de espera.\n");
          }
          break;
        case 2:
          if(switches[0]==0){
            printf("Estado de espera.");
            estado = 0;
          }
          // printf("%ld\n", acenderRed);
          if(botoes[0]==0){
            tempoAtual=current_timestamp();
            // printf("antig: %lld \tatual:%lld \n", tempoAntigo,tempoAtual );
            diferencaTempo = tempoAtual - tempoAntigo;
            // printf(" dif t %d\n", diferencaTempo );
            if(diferencaTempo <= 1200){
              estado = 4;//Estado 4 (Longo)
              tempoAntigo = current_timestamp(); //pega o tempo que soltou o botao 
            }
            else{
              estado = 3;//Estado 3 (curto)
              tempoAntigo = current_timestamp(); //pega o tempo que soltou o botao 
            }
          }

          break;
        case 3:
          if(switches[0]==0){
            printf("Estado de espera.");
            estado = 0;
          }
          frase[indiceFrase++] = '-';
          buffer[indiceBuffer++] ='-';
          contador++;
          system("clear");
          printf("Estado atual:\n");
          printf("%s\n",frase);
          while(1){
            tempoAtual = current_timestamp();
            diferencaTempo = tempoAtual - tempoAntigo;
            if(contador>=4){
              estado = 5;
              break;
            }
            if(diferencaTempo>=4000){ //É o fim de um char
              estado = 5;
              break;
            }
            else if(botoes[0]==1){ //Continuação de um char
              estado = 2;
              tempoAntigo = current_timestamp();
              break;
            }
          }
          break;
        case 4:
          if(switches[0]==0){
            printf("Estado de espera.");
            estado = 0;
          }
          frase[indiceFrase++] = '.';
          buffer[indiceBuffer++] ='.';
          contador++;
          system("clear");
          printf("Estado atual:\n");
          printf("%s\n",frase);
          while(1){
            if(switches[0] == 0){
              estado = 0;
              printf("Estado de espera.");
              break;
            }
            tempoAtual = current_timestamp();
            diferencaTempo = tempoAtual - tempoAntigo;
            if(contador>=4){
              estado = 5;
              ajusteWord = 4000;
              break;
            }
            if(diferencaTempo>=4000){ //É o fim de um char
              estado = 5;
              ajusteWord = 0;
              break;
            }
            else if(botoes[0]==1){ //Continuação de um char
              estado = 2;
              tempoAntigo = current_timestamp();
              break;
            }
          }
          break;
        case 5:
          numeroDeChars++;
          indiceBuffer =0;
          if(switches[0]==0){
            estado = 0;
          }
          frase[indiceFrase++] = ' ';
          contador = 0;
          system("clear");
          printf("Estado atual: (ENDCHAR)\n");
          printf("%s\n",frase);
          while(1){
            tempoAtual = current_timestamp();
            diferencaTempo = tempoAtual - tempoAntigo;
            if(diferencaTempo>=10000-ajusteWord){
              frase[indiceFrase++] = '/';
              frase[indiceFrase++] = ' ';
              estado = 1;
              system("clear");
              printf("Estado atual: (ENDWORD)\n");
              printf("%s\n",frase);
              int a=0;
              for(a=0;a<4;a++){
                buffer[a] ="0";
              }
              acenderRed = 0;
              break;
            }
            else if(botoes[0]==1){
              estado = 2;
              tempoAntigo = current_timestamp();
              int a=0;
              for(a=0;a<4;a++){
                buffer[a] ="0";
              }
              acenderRed = 0;
              break;
            }
          }
          break;
        default:
          break;
      }
    // while(estado == 0){
    //   if(botoes[0] == 1){
    //     estado = 1;
    //     tempoAntigo = current_timestamp();
    //   }
    // }

    // while(estado == 1){
    //   if(botoes[0] == 0){
    //     estado = 0;
    //     tempoAtual = current_timestamp();
    //   }
    // }

    // printf("Diferença: \t %lld\n", tempoAtual - tempoAntigo);
    // printf("switch %ld\n", valorSwitch);

    for(lixo = 0; lixo < 1000000; lixo++){

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
    // printf("Botao :");
    for(m = 0; m < 4; m++){
      botoes[m] = (~vButton & binarios[m] ) / expo(2, m);
    }
    // printf("\n");
    // for(m = 3; m >= 0; m--){
    //   printf("%d", botoes[m]);
    // }
    // printf("\n");

    for(lixo = 0; lixo < 100000000; lixo++){

    }


    read(dev, &lixo, INPORT);
    read(dev, &vInport, INPORT);

    valorSwitch = vInport;
    // printf("Valor switch minha thread %ld\n", valorSwitch);
    for(m = 0; m < 16; m++){
      if(m >= 8)
        switches[m] = ((vInport >> 8) & binarios[m - 8] ) / expo(2, m % 8);
      else
        switches[m] = (vInport & binarios[m] ) / expo(2, m);
    }
    // printf("Switches\n");
    // for(m = 15; m >= 0; m--){
    //   printf("%d", switches[m]);
    // }
    // printf("\n");


          
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
