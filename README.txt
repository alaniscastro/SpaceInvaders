Integrantes do grupo: 
	- Alanis Viana Castro
	- Matheus Freire Henrique Fonseca 
	- Rodrigo Luiz Panta Júnior

Itens adicionais implementados:
	- Música de fundo
	- Fases "curadas"
	- Vidas
	- Controle via mouse
	- Imagem de fundo
	- Telas

Bibliotecas usadas:
	"GL/glew.h"
	"GL/freeglut.h"
	<time.h>
	"SOIL/SOIL.h"
	"stdlib.h"
	"stdio.h"
	"constantes.h"

Instruções para compilação:
	1. Você pode apenas clicar no arquivo build.bat que ele compilará o arquivo para você, restando apenas clicar no arquivo executável gerado

	2. Para compilar usando o g++:
		A. abra a linha de comando do seu computador e passe o caminho para a pasta onde se encontra o arquivo "main.cpp"
Ex: 	cd\
	cd Users\alani\OneDrive\Documentos\SpaceInvaders

		B. Digite o seguinte comando no prompt: g++.exe -Wall -g -I"freeglut\include" -c main.cpp -o main.o

		C. E finalmente, esse comando: g++.exe -L"freeglut\lib" -o SpaceInvaders_Alanis_Matheus_Rodrigo.exe main.o -lSOIL  -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 "SOIL\libSOIL.a"
