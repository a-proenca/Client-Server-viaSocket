/*
 * ficheiro: cliente.c
 *
 * objetivo 1: enviar a mensagem através do socket para ser lida pelo servidor (que está ligado ao mesmo socket)
 * objetivo 2: a mensagem do utilizador fosse introduzida através do teclado
 *          o cliente deixa de ter argumentos
 * a invocação para a aplicação cliente ser executa: ./cliente
 * objetivo 3: o cliente estar sempre "online"
 * objetivo 4: o cliente poder receber mensagens do servidor
 *      alé de escrever, o cliente também vai poder ler do socket
 *      1º organização do código - código para escrever vamos organizá-lo numa função
 *      2ª cirar uma função para ler do socket - invocá-la no local pretendido
 *      diálogo: depois de enviar mensagem ao servidor,
                  vai receber uma mensagem do servidor
 */
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>//por causa das funções malloc, free
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <sys/un.h>
 #include <unistd.h>
 #include <arpa/inet.h> //inet_addr

 /*
  * função para enviar mensagem ao servidor - escrver mensagem no socket do servidor
  * precisamos saber a identificação do descritor do socket do servidor
  * precisamos do tamanho da mensagem a enviar
  * precisamos da mensagem a enviar
  */


void enviar_mensagem(int desc_socket,size_t tamanho, char *mensagem){
  //1º - indicar qual o tamanho da mensagem
  write(desc_socket,&tamanho,sizeof(tamanho));
  printf("[cliente] a enviar mensagem: %s\n", mensagem);
    //2º enviar a mensagem
  write(desc_socket,mensagem,tamanho);
}


/*
 * função para ler uma mensagem do socket do servidor, receber mensagem do servidor
 * única informação que precimaos fora da função é a identificação do descritor do socket do servidor
 */


int receber_mensagem(int desc_servidor){
  //1º - ler o tamanho da mensagem
  size_t tamanho; //tamanho da mensagem
  if(read(desc_servidor,&tamanho,sizeof(tamanho)) == 0)
      return 0; //não está mensagem no socket liago ao cliente

  //2º ler a mensagem
  char *mensagem = (char*)malloc(tamanho);//reservar memória do tamanho da mensagem
  read(desc_servidor,mensagem, tamanho);
  printf("[cliente] mensagem recebida.... %s\n", mensagem);

  free(mensagem);//liberta o espaço de memória ocupado pela mensagem - varíável não pode ser mais utilizada
  return 0;
}

 int main(int argc, char *argv[]){

   printf("Insira start para iniciar. \n");//digite "start" para iniciar a conexão com o servidor

 int desc_socket;
 char pergunta_cliente[100];
 scanf("%s", pergunta_cliente);

 size_t tamanho = 0; //tamanho da mensagem


       if(!strcmp(pergunta_cliente,"start")){


   //ler mensagem do teclado - variável mensagem para guardsar a string do teclado
   while(1){ //a aplicação cliente está sempre "online" (em execução)


    do{

         desc_socket = socket(AF_INET,SOCK_STREAM,0);
     char *mensagem = NULL; //mensagem vazia
     //como precisamos do tamanho, temos de o definir aqui
     size_t tamanho = 0;
     getline(&mensagem,&tamanho,stdin);

     //mensagem[strlen(mensagem)-1] = '\0';
       while(strcmp(mensagem,"end") != 0){

       int desc_socket;//descritor de ficheiro para o desc_socket
       desc_socket = socket(PF_INET,SOCK_STREAM,0);
       struct sockaddr_in nome;//estrutura para endereço ip


            //definir um ip local - atribuir um endereço ao porto definido para o socket
            nome.sin_addr.s_addr = inet_addr("127.0.0.1");
            nome.sin_family = AF_INET;
            nome.sin_port = htons(8888);

    //introduzir nOva mensagem
     printf("Indique a mensagem: ");

     getline(&mensagem,&tamanho,stdin);
     mensagem[strlen(mensagem)-1] = '\0';
     //estamos em condições de ligar ao servidor
      int k = connect(desc_socket,(struct sockaddr*)&nome,sizeof(nome));

     enviar_mensagem(desc_socket,tamanho,mensagem);
     receber_mensagem(desc_socket);

   }

}
while(1);
return 0;
}
}
}
