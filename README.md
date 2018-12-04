# 2018.2-ihs-projeto

## Projeto Tradutor Morse

O objetivo do projeto é traduzir mensagens Morse -> Português e Português -> Morse, em um modelo de chat, 
utilizando da placa DE2i-250.

### Versões

- #### Com server
  Servidor para receber e enviar as mensagens.
  - ##### Para executar
  Entrar na pasta crow-template e seguir os passos do README.md.
- #### Sem server
  Sistema com a leitura de botões e tradução simultânea do morse.
  - ##### Para executar
  Entrar na pasta source. Modificar o caminho do *Makefile* para a pasta que contém ~source/driver~.
  Pela primeira vez que executar o código:
  ```
  source deploySemDrive
  ```
  Depois executar usando:
  ```
  source deploy
  ```
Autores:
- João Rafael
- Jonathan Kilner
- José Carlos
- Luiz Antônio
- Marco Aurélio
- Thiago Augusto

Professor:
- Adriano Sarmento
