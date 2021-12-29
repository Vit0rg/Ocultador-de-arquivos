#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Criar();
void Extrair_oculto();
void Separar();

int main()
{
  unsigned short int opcao = 0;
  printf("Ocultador de Arquivos\nInsira uma opção:\n1-Criar novo\n2-Extrair oculto\n3-Separar arquivos\n");
  scanf("%hu", &opcao);
  switch(opcao)
  {
    case 1: Criar();
    break;
    case 2: Extrair_oculto();
    break;
    case 3: Separar();
    break;
    default: 
    printf("\nOpção inválida\n");
    exit(1);
  }

  return 0;
}

//CRIAR:
void Criar()
{
  FILE *original_1, *original_2, *modificado;
  char arquivo_original_1[100],
       arquivo_original_2[100],
       arquivo_modificado[100];

  //Limpar stdin:
  getc(stdin);

  /*
    Receber nome do arquivo que aparecerá,
    Setar último caractere como nulo ao invés de '\n':
  */
  printf("Insira o caminho do arquivo que será mostrado:\n");
  fgets(arquivo_original_1, 100, stdin);
  arquivo_original_1[strlen(arquivo_original_1)-1] = 0x00;

  //Abrir arquivo no modo de leitura binária:
  original_1 = fopen(arquivo_original_1, "rb");

  //Se arquivo não existir, sai do programa com status de erro:
  if (original_1 == NULL)
  {
    perror("Erro ao abrir o arquivo 1");
    exit(1);
  }

  /*
    Receber nome do arquivo que ficará oculto,
    Setar último caractere como nulo ao invés de '\n':
  */
  printf("Insira o caminho do arquivo que ficará oculto:\n");
  fgets(arquivo_original_2, 100, stdin);
  arquivo_original_2[strlen(arquivo_original_2)-1] = 0x00;

  //Abrir arquivo no modo de leitura binária:
  original_2 = fopen(arquivo_original_2, "rb");

  //Se arquivo não existir, sai do programa com status de erro:
  if (original_2 == NULL)
  {
    perror("Erro ao abrir o arquivo 2");
    exit(1);
  }

  /*
    Definir nome do arquivo resultante,
    Setar último caractere como nulo ao invés de '\n':
  */
  printf("Salvar arquivo criado como:\n");
  fgets(arquivo_modificado, 100, stdin);
  arquivo_modificado[strlen(arquivo_modificado)-1] = 0x00;

  //Abrir arquivo no modo de escrita binária:
  modificado = fopen(arquivo_modificado, "wb");

  /*Parte crucial do programa:
   Se o buffer não for correspondente a 2 bytes, 
   uma série de erros acontecerá, 
   verifique a arquitetura da máquina com 
   sizeof antes de alterar o tipo do buffer
  */
  short int buffer;

  /*
    Copia todo o conteúdo do arquivo 1 no arquivo modificado,
    2 bytes por vez, valores de 00 a FF são possíveis.
  */
  while ( (buffer = fgetc(original_1) ) != EOF)
  {
    //printf("|%02X|", buffer_1);
    fputc(buffer, modificado);
  }
  /*
    Adiciona flag SEPARADOR que é usada para extrair o oculto ou
    separar os 2 arquivos originais:
  */
  fputs("SEPARADOR", modificado);

  //Fechar arquivo 1;
  fclose (original_1);

  /*
    Copia todo o conteúdo do arquivo 2 no arquivo modificado,
    2 bytes por vez, valores de 00 a FF são possíveis.
  */
  while( (buffer = fgetc(original_2) ) != EOF )
  {
    fputc(buffer, modificado);
  }

  //Fechar arquivo 2 e arquivo modificado:
  fclose (original_2);
  fclose (modificado);

  printf("\nArquivo salvo como: %s\n", arquivo_modificado);
}

//EXTRAIR:
void Extrair_oculto()
{
  FILE *original_2, *modificado;
  char arquivo_original_2[100],
       arquivo_modificado[100];
  
  //Limpar stdin:
  getc(stdin);

  /*
    Receber nome do arquivo modificado,
    Setar último caractere como nulo ao invés de '\n':
  */
  printf("Insira o caminho do arquivo a ser lido:\n");
  fgets(arquivo_modificado, 100, stdin);
  arquivo_modificado[strlen(arquivo_modificado)-1] = 0x00;

  //Abrir arquivo no modo de leitura binária:
  modificado = fopen(arquivo_modificado, "rb");

  //Se arquivo não existir, sai do programa com status de erro:
  if (modificado == NULL)
  {
    perror("Erro ao abrir o arquivo");
    exit(1);
  }

  /*Parte crucial do programa:
   Se o buffer não for correspondente a 2 bytes, 
   uma série de erros acontecerá, 
   verifique a arquitetura da máquina com 
   sizeof antes de alterar o tipo do buffer
  */
  short int buffer = 0;
  //unsigned short int i = 0;

  //variável usada para comparação assíncrona em busca do SEPARADOR: 
  char flag[9];

  /*
    Retornar ao início do arquivo, 
    útil caso o programa não tenha sido encerrado corretamente: 
  */
  rewind(modificado);

  //Iniciar leitura do arquivo, 2 bytes por vez:
  while( (buffer = fgetc(modificado)) != EOF)
  {
    //Procurar pelo primeiro caractere da flag SEPARADOR:
    if (buffer == 'S')
    {
      //i armazena o offset enquanto percorre o arquivo:
      short int i=0;
      /*
        Enquanto o fim do arquivo não é encontrado,
        salva os próximos 8 caracteres na variável flag:
       */
      while((flag[i] = fgetc(modificado)) != EOF && i < 8)
      {
        //Incrementa o offset
        i++;
      }
      //Seta o último caractere da flag como nulo:
      flag[8] = 0x00;

      //Comparar flag lida com a string EPARADOR:  
      if( (strcmp(flag, "EPARADOR")) == 0) 
      {   
        /*
          Definir nome do arquivo a ser salvo,
          Setar último caractere como nulo ao invés de '\n':
        */
        printf("Salvar o arquivo oculto como:\n");
        fgets(arquivo_original_2, 100, stdin);
        arquivo_original_2[strlen(arquivo_original_2)-1] = 0x00;

        //Abrir arquivo no modo de escrita binária:
        original_2 = fopen(arquivo_original_2, "wb");
        
        //Recuar ponteiro em 1 caractere:
        fseek(modificado, -1, SEEK_CUR);

        //Copia conteúdo do original lido exatamente após a flag:
        while( (buffer = fgetc(modificado) ) != EOF )
        {
          fputc(buffer, original_2);
        }
        
        printf("\nArquivo oculto salvo como: %s", arquivo_original_2);
        //Fecha arquivo modificado e arquivo 2 e sai do programa:
        fclose(original_2);
        fclose (modificado);
        exit (0);
      }
      /*Caso a flag obtida não seja igual a EPARADOR,
       recua o ponteiro usando a quantidade de caracteres lidos:*/
      else
      {
        fseek(modificado, -i-1, SEEK_CUR);
      }
    }
  }
  /*Indica que o arquivo não contém a flag SEPARADOR,
   fecha o arquivo e sai do programa com status de erro:*/
  puts("\nSeparador não encontrado, arquivo inválido\n");
  fclose(modificado);
  exit(1);
}

//SEPARAR:
void Separar()
{

  FILE *original_1, *original_2, *modificado;
  char arquivo_original_1[100],
       arquivo_original_2[100],
       arquivo_modificado[100];
  
  //Limpar stdin:
  getc(stdin);

  /*
    Receber nome do arquivo que será lido,
    Setar último caractere como nulo ao invés de '\n':
  */
  printf("Insira o caminho do arquivo a ser lido:\n");
  fgets(arquivo_modificado, 100, stdin);
  arquivo_modificado[strlen(arquivo_modificado)-1] = 0x00;

  //Abrir arquivo no modo de leitura binária:
  modificado = fopen(arquivo_modificado, "rb");

  //Se arquivo não existir, sai do programa com status de erro:
  if (modificado == NULL)
  {
    perror("Erro ao abrir o arquivo");
    exit(1);
  }

  /*
    Definir como arquivo aparente deve ser salvo,
    Setar último caractere como nulo ao invés de '\n':
  */
  printf("Insira o caminho para salvar o arquivo aparente:\n");
  fgets(arquivo_original_1, 100, stdin);
  arquivo_original_1[strlen(arquivo_original_1)-1] = 0x00;

  //Abrir arquivo no modo de escrita binária:
  original_1 = fopen(arquivo_original_1, "wb");

  //Se arquivo não existir, sai do programa com status de erro:
  if (original_1 == NULL)
  {
    perror("Erro ao abrir o arquivo");
    exit(1);
  }

  /*Parte crucial do programa:
   Se o buffer não for correspondente a 2 bytes, 
   uma série de erros acontecerá, 
   verifique a arquitetura da máquina com 
   sizeof antes de alterar o tipo do buffer
  */
  short int buffer = 0;
  //unsigned short int i = 0;

  //variável usada para comparação assíncrona em busca do SEPARADOR: 
  char flag[9];

  /*Retornar ao início do arquivo, 
    útil caso o programa não tenha sido encerrado corretamente: 
  */
  rewind(modificado);
  
  //Iniciar leitura do arquivo, 2 bytes por vez:
  while( (buffer = fgetc(modificado)) != EOF)
  {
    //Procurar pelo primeiro caractere da flag SEPARADOR:
    if (buffer == 'S')
    {
      //i armazena o offset enquanto percorre o arquivo:
      short int i=0;
      /*
        Enquanto o fim do arquivo não é encontrado,
        salva os próximos 8 caracteres na variável flag:
       */
      while((flag[i] = fgetc(modificado)) != EOF && i < 8)
      {
        i++;
      } 
      //Seta o último caractere da flag como nulo:
      flag[8] = 0x00;
      //Comparar flag lida com a string EPARADOR:       
      if( (strcmp(flag, "EPARADOR")) == 0)
      {
         /*
          Definir nome do arquivo a ser salvo,
          Setar último caractere como nulo ao invés de '\n':
        */        
        printf("Insira o caminho para salvar o arquivo oculto:\n");
        fgets(arquivo_original_2, 100, stdin);
        arquivo_original_2[strlen(arquivo_original_2)-1] = 0x00;
        /*Fechar arquivo aparente,
         abrir arquivo oculto no modo de escrita binária:*/
        fclose(original_1);
        original_2 = fopen(arquivo_original_2, "wb");
        //Recuar ponteiro em 1 caractere:
        fseek(modificado, -1, SEEK_CUR);
        /*Copia conteúdo pós-flag do 
          arquivo modificado no arquivo oculto:*/
        while( (buffer = fgetc(modificado) ) != EOF )
        {
          fputc(buffer, original_2);
        }
        printf("\nArquivo oculto salvo como: %s\n", arquivo_original_2);
        //Fecha arquivo modificado e arquivo 2 e sai do programa:
        fclose(original_2);
        fclose(modificado);
        exit(0);
      }
      /*Caso a flag obtida não seja igual a EPARADOR,
       recua o ponteiro usando a quantidade de caracteres lidos
       e adiciona o caractere S no arquivo original:
       */
      else
      {
        fseek(modificado, -i-1, SEEK_CUR);
        //printf("\n[%ld][%c]\n", ftell(modificado), buffer);
        fputc(buffer, original_1);
      }
    }
    else
    /*Caso os bytes lidos sejam diferentes do caractere S,
     copia no arquivo aparente.*/
    {
      fputc(buffer, original_1);
    }
  }
  //Fecha arquivo aparente e modificado:
  fclose(original_1);
  fclose(modificado);

  printf("\nArquivo oculto salvo como: %s\n", arquivo_original_1);
}