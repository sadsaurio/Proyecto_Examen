#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

using namespace std;

void gotoxy(int x, int y)//funcion para mover el cursor y dibujarlo existe en algunas librerias pero mejor la declare
{   COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int menu() {
system("color 05");//hace que el texto de la consola sea mrado
    string opciones[] = {//opciones de el menu 
        "start exam",
        "modificar exam",
        "generar examen",
        "Salir"
    };
    int seleccion = 0;//variable de seleccion
    int tecla;//tecla pulsada
    while (true) {
        system("cls");//limpia la pantalla
        gotoxy(30, 5);
        cout << "=== Menu de agott ===";
        for (int i = 0; i < 4; i++) {   
            gotoxy(30, 8 + i);//imprimir con un renglon
            if (i == seleccion){
            cout << "> " << opciones[i];//pone un cursor en la primera opcion o bueno en el valor de seleccion este aso 0 y luego el texto
            }else
            cout << "  " << opciones[i];//deja espacio blanco en donde va la flecha y luego imprime el texto de el vectorde opciones
       }
        tecla = getch();//asigna el valor de la tecla que pulsaste a tecla
        if (tecla == 224) {//224 corresponde a teclas especiales en este caos solo las flechitasarriba y abajo 
            tecla = getch();//obtiene el valor de la tecla (en caso de ser teclas especialees manda 224 y luego el valor aqui seria 72 o 73)
            switch (tecla) {
                case 72: // es el segundo valor aparte de 224 72 es para la flecha de arriba
                    seleccion--;//sube un escalon de menu
                    if (seleccion < 0){//si es menor a 0 baja
                        seleccion = 3;
                        }
                    break;

                case 80: // Abajo es el valor de la flecha de abajo lo mismo que arroba
                    seleccion++;//baja una opcion
                    if (seleccion > 3){//si es mayor a 3 regresa a el primero
                        seleccion = 0;
                        }
                    break;
          }
      }
        else if (tecla == 13) { // valor de la tecla enter este no nececita 224 al ser tecla normal por eso esta fuera de el if 224
            return seleccion;//regresa el valor modificado o si no regresa el valor que ya estaba
    }
  }
} 
int main() {
system("color 05");//color morado en la consola de windows eso
    bool salir = false;//flag que se activara con la opcionde salir para romper ciclo
    while (!salir) {//ciclod e el menu
        int opcion = menu();//obtiene el valor de la opcion ose ael seleccion de arriba
        system("cls");//limpia la pantalla
        switch (opcion) {
            case 0://aqui va todo el proceso de apliacion de el examen
                cout << "seleccionaste start exam";
                break;
            case 1://aqui todo lo de modificacion
                cout << "seleccionaste modificar exam";
                break;
            case 2://lo mismo en generar
                cout << "seleccionaste generar examen";
                break;
            case 3://salida de el programa
                salir = true;
                cout << "saliendo........";
                break; }
        if (!salir) {//si no elegiste salir te regresa al inicio de el ciclo para volver al menu y repetir en bucle
            cout << "\n\npresiona algo para ir al menu...";
            getch();
        }
    }
    return 0;
}
