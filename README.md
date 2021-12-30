# Ocultador de arquivos

## Programa simples para ocultação de arquivos (imagens, textos, áudios) em outros arquivos, de preferência imagens.

> Atualmente suportado apenas em distribuições Linux devido a limitações de arquitetura.
> Todas as funções possuem feedback.
> Alguns formatos de arquivo não são suportados.

## Funções:

  > Criação:
  Abre arquivo 1, copia no arquivo alvo,  fecha arquivo 1, adiciona <SEPARADOR> .
  Abre arquivo 2, copia no arquivo alvo, fecha arquivo 1, fecha arquivo alvo.
  
  > Extração de arquivo oculto:
  Abre arquivo alvo, procura <SEPARADOR>
  Se achar, salva restante do conteúdo do arquivo alvo como arquivo 2, fecha arquivo 2, fecha arquivo alvo.
     
  > Separação:
  Separa o arquivo alvo em 2 arquivos similares aos originais.
  Abre arquivo alvo, abre arquivo 1, procura <SEPARADOR>.
  Enquanto <SEPARADOR> não é encontrado, todo conteúdo fica salva como arquivo 1.
  Se <SEPARADOR> é encontrado, fecha arquivo 1, abre arquivo 2, salva restante do conteúdo, fecha arquivo 2, fecha arquivo alvo.


## Futuras atualizações:
>Otimização geral, reduzindo a quantidade de repetições de linhas no código.
  
>Introdução do uso de alocação dinâmica de memória.
