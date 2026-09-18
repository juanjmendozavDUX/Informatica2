sweet crush - desafio 1

archivos:
- menu.cpp
- funciones.cpp
- logica.h
- SweetCrush.pro
- CMakeLists.txt

configuracion con qt creator usando cmake:
1. abre la carpeta del proyecto o CMakeLists.txt.
2. selecciona un kit de Qt instalado.
3. deja que Qt Creator configure el proyecto otra vez.
4. si aparece cmake_check_build_system, cierra el proyecto y elimina SOLO la carpeta de build generada por Qt Creator. no borres los archivos fuente.
5. vuelve a abrir CMakeLists.txt y deja que Qt Creator cree una carpeta de build nueva.

configuracion usando qmake:
- tambien se incluye SweetCrush.pro.
- abre SweetCrush.pro en Qt Creator y selecciona un kit de Qt.

importante:
- QCoreApplication no es necesaria para este programa.
- el programa usa C++ de consola y enlaza Qt Core en CMake para mantener el proyecto dentro del framework Qt.
- las fichas ocupan exactamente 3 bits.
- se usa memoria dinamica y unsigned char* para el tablero.
