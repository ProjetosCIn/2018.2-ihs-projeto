#include <stdlib.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <time.h>
#include <stdlib.h>

/*
Para rodar esta coisa na placa, é preciso rodar o comando:
		lsmod | grep -i spkr
	caso ele não retorne nada, isto indica que o buzzer não está ativo, então temos que ativalo, para isto, rode, caso contrário, goto 'deu bom':
		modprobe pcspkr
	Agora rode o 'lsmod | grep -i spkr' novamente
	Deu bom:
	Se tudo deu bom até agora, compile o código
		gcc beep.c -o beep
	e rode
		sudo ./beep
	este código só deve rodar com sudo, pois a escrita em um dispositivo só é permitida para o superusuário
	Ps.: Se algu, comando não rodar, coloque um sudo na frente, pois eu não tenho certeza se usei ou não, tenho certeza apenas no ultimo.
	Ps.: Se mesmo com sudo isto não roda, eu não sei o que fazer
*/

 
int main(int argc, char *argv[])
{
    int fd = open("/dev/tty10", O_RDONLY),i = 200,h; // i tempo h e fd é o dispositivo do buzzer
    // srand(time(NULL));   // inicializa o random
    if (fd == -1 ){// não conseguiu acessar o arquivo
            printf("Deu ruim DG, tenta meter o SUDO %d %d",fd,argc);
            return -1;
    }
    while(1){
    	//h = rand()% (5000 + 1 - 200) + 200;//gera um tempo rendomico
    	//i = rand() % (500 + 1 - 100) + 100;//gera uma nota rendomica
    	printf("%d\n", h);
    	h= 9000;
    	i= 100;

    	ioctl(fd, KDMKTONE, (i<<16)+(1193180/h));//coloca os valores pra tocar no buzzer
    	usleep(100*i);//da um sleep de i milisegundos
	}
}