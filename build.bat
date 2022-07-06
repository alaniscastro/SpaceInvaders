Echo off
cls
Echo Instrucoes: 
Echo  Para compilar, use o programa da sua preferencia (g++, gcc...)
Echo ....
Echo on
g++.exe -Wall -g -I"freeglut\include" -c main.cpp -o main.o
pause
g++.exe -L"freeglut\lib" -o SpaceInvaders_Alanis_Matheus_Rodrigo.exe main.o -lSOIL  -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 "SOIL\libSOIL.a"
pause
