/*
 *
 * O servidor define um porto local para receber mensagens
 * A aplicação servidor não vai ter qualquer argumento
 * invocação para executar o servidor: ./servidor
 *
 * objetivo 1: ficar sempre "online"
 * objetivo 2: ler as mensagens do cliente que vai estar ligado ao porto e escrevê-las no stdout
 *       a comunicação com o cliente só está ligada entre aceitar e ler a mensagem
 * objetivo 3: definir um diálogo entre o cliente e o servidor
 *      o servidor pode ler e escrever no socket, isto significa que além de receber mensagens
 *      através do cliente também pode enviar mensagens
 *      1º organizar o código para ler do socket uma mensagem dentro de uma funçãp
 *      2º criar uma função para escrever e invocá-la no local pretendido
 *      diálogo: o servidor vai enviar para o cliente a mensagem enviada por este
 *          depois do servidor ler uma mensagem de um cliente,
 *            envia a mesma mensagem para este cliente
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

/*
 * função para enviar mensagem ao servidor - escrver mensagem no socket do servidor
 * precisamos saber a identificação do descritor do socket do servidor
 * precisamos do tamanho da mensagem a enviar
 * precisamos da mensagem a enviar
 */


void enviar_mensagem(int desc_cliente,size_t tamanho, char *mensagem){
  //1º - indicar qual o tamanho da mensagem
 write(desc_cliente,&tamanho,sizeof(tamanho));
 printf("[servidor] a enviar mensagem: %s\n", mensagem);
  //2º enviar a mensagem
 write(desc_cliente,mensagem,tamanho);


}

/*
 * função para ler uma mensagem do socket do cliente, receber mensagem de cliente
 * única informação que precisamos fora da função é a identificação do descritor do socket do cliente
 */

int receber_mensagem(int desc_cliente){

   //1º - ler o tamanho da mensagem
  size_t tamanho; //tamanho da mensagem
  if(read(desc_cliente,&tamanho,sizeof(tamanho)) == 0)
      return 0; //não tem mensagem no socket ligado ao cliente

  //2º ler a mensagem
  char *mensagem = (char *)malloc(tamanho);//reservar memória do tamanho da mensagem
  read(desc_cliente,mensagem, tamanho);

  printf("[servidor] mensagem recebida: %s\n", mensagem);
   //nesta ponto é que sabemos que mensagem enviar e a quem
  enviar_mensagem(desc_cliente,tamanho,mensagem);

  free(mensagem);//liberta o espaço de memória ocupado pela mensagem - varíável não pode ser mais utilizada

  return 0;
}


void *dialogo(void * arg){
  int socket_cliente = (intptr_t) arg;
   //no fim da conversa, tem de terminar o socket
  receber_mensagem(socket_cliente);//faz mais sentido ser aqui, porque a thread vai
  close(socket_cliente); //terminar o diálogo, logo também a comunicação
  //depois de terminar a comunicação, tem de ser terminada
pthread_exit(NULL);

}

int main(int argc, char * argv[]){


  int desc_socket;//descritor do socket, semalhante ao descritor de ficheros
  struct sockaddr_in nome;
  //struct sockaddr_in nome; //in rede de internet
  desc_socket = socket(PF_INET, SOCK_STREAM,0);
  //estabelecer o nome do socket
  nome.sin_family = AF_INET;
  nome.sin_addr.s_addr = INADDR_ANY;//definir qualquer endereço que possa fazer ligação a este porto
	nome.sin_port = htons(8888);

  char *mensagem = "";
  pthread_t id; //identificação de uma thread


do{
  struct sockaddr_in nome_cliente;
  int desc_cliente;
  socklen_t tamanho_nome_cliente;
  bind(desc_socket,(struct sockaddr*)&nome, sizeof(nome));

    listen(desc_socket,5); //fica à espera de pedidos (comunicações de clientes)

  while(strcmp(mensagem,"end") != 0){//quando for inserido "end" o ciclo termina

  desc_cliente = accept(desc_socket,(struct sockaddr*) &nome_cliente, &tamanho_nome_cliente);
  receber_mensagem(desc_cliente);




    printf("desc_cliente = %d\n", desc_cliente);

    pthread_create(&id, NULL, dialogo,(void *)(intptr_t)desc_cliente);
  }
}while (1);

    return 0;
  }
