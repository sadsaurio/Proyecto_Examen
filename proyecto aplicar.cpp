#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <cctype>
using namespace std;

 struct pregunta{
    string  cuestion;//pregunta pero ps para no repetir con el nombre de la estructura
    string opcion1;//opcoones
    string opcion2;
    string opcion3;
    string opcion4;
    float puntos;//valor de puntos de cada opcion
    int correcta;//la respuseta correcta 
    int respuesta;
    int puntaje;//se explica solita
    pregunta *sig;//pregutna sig
    pregunta *ant;//pregunta anterior 
 };


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
bool creararchivo(pregunta *&inicio, pregunta *&actual ){
    ifstream arch("examen.txt");
    if(!arch.is_open()) return false;
    string linea;
    pregunta *nuevo=NULL;
    inicio=actual=NULL;

    while(getline(arch,linea)){
        if(!linea.empty()&& linea.back()=='\r'){
            linea.pop_back(); // quitar \r de windows
        }
        if(linea.find(":p;")==0){
            nuevo=new pregunta;
            nuevo->cuestion=linea.substr(3); // substr es para extraer una parte de una cadena
            nuevo->respuesta=0; //inicializar sin contestar
            nuevo->sig=nuevo->ant=NULL;
        }
        else if(!nuevo){
            continue;
        }
        else if(linea.find(":op1;")==0){
            nuevo->opcion1=linea.substr(5); // osea quitar :op1;
        }
        else if(linea.find(":op2;")==0){
            nuevo->opcion2=linea.substr(5);
        }
        else if(linea.find(":op3;")==0){
            nuevo->opcion3=linea.substr(5);
        }
        else if(linea.find(":op4;")==0){
            nuevo->opcion4=linea.substr(5);
        }
        else if(linea.find(":r;")==0){
            string r=linea.substr(3);// stoi convertir en char en int
            nuevo->correcta=(!r.empty()&&isdigit(r.back()))? r.back()-'0':stoi(r);
        }
        else if(isdigit (linea[0])){  // verificar si todos los carecteres de char son digitos numericos
            string pts=linea;
            if(!pts.empty()&& pts.back()=='.'){
                pts.pop_back();
            }
            nuevo->puntos=stof(pts); // en float
            nuevo->ant=actual;
            if(inicio==NULL){
                inicio=nuevo;
            }
            else{
                actual->sig=nuevo;
            }
            actual=nuevo;
        }
    }
    arch.close();
    return (inicio!=NULL);
}
void generarExam(pregunta *&inicio ,pregunta *&actual){
    int r;
    bool salir = false;
    int tecla;
    if(!actual){
        cout<<"No hay pregunta, agregas una ? (1=Si / 0=No): ";
        cin>>r;
        if(r!=1) return;
        agregarPreg(inicio,actual);
    }
    while(!salir){
        mostrarPreg(actual,true);
        cout<<"\n[<-  ->] Navegar  [E] Editar  [A] Aplicar   [S] Salir\n";
        tecla=getch();
        if(tecla==224){
            tecla=getch();
            if(tecla==75 && actual->ant){
                actual=actual->ant; //izquierda
            }
            if(tecla==77 && actual->sig){
                actual->sig; // derecha
            }
        }
        else if(tecla == 'e' || tecla=='E'){
            string tmp;
            cin.ignore();
            system("cla");
            cout<<"=== EDITAR (enter vacio = no cambiar)===\n";
            cout<<"Pregunta ["<<actual->cuestion<<"]: ";
            getline(cin,tmp);
            if(!tmp.empty()) actual->cuestion=tmp;
            cout<<"Op1 ["<<actual->opcion1<<"]: ";
            getline(cin,tmp);
            if(!tmp.empty()) actual->opcion1=tmp;
            cout<<"Op2 ["<<actual->opcion2<<"]: ";
            getline(cin,tmp);
            if(!tmp.empty()) actual->opcion2=tmp;
            cout<<"Op3 ["<<actual->opcion3<<"]: ";
            getline(cin,tmp);
            if(!tmp.empty()) actual->opcion3=tmp;
            cout<<"Op4 ["<<actual->opcion4<<"]: ";
            getline(cin,tmp);
            if(!tmp.empty()) actual->opcion4=tmp;
            
            cout<<"Correcta (1-4) ["<<actual->correcta <<"]: "; 
            getline(cin,tmp);
            if(!tmp.empty())  actual->correcta=stoi(tmp);
            cout<<"Puntos ["<<actual->correcta <<"]: "; 
            getline(cin,tmp);
            if(!tmp.empty())  actual->puntos=stof(tmp);
        }
        else if(tecla == 'a'|| tecla=='A'){
            agregarPreg(inicio,actual);
        }
        else if(tecla == 's'||tecla=='S'){
            salir=true;
        }
    }
}
void guardar(pregunta *inicio){
    ofstream arch("examen.txt");
    pregunta *aux=inicio;
    while(aux){
        arch<<":p;"<<aux->cuestion<<endl;
        arch<<":op1;"<<aux->opcion1<<endl;
        arch<<":op2;"<<aux->opcion2<<endl;
        arch<<":op3;"<<aux->opcion3<<endl;
        arch<<":op4;"<<aux->opcion4<<endl;
        arch<<":r;op"<<aux->correcta<<endl;
        arch<<aux->puntos<<endl;
        aux=aux->sig;
    }
    arch.close();
}
void aplicarExam(pregunta *inicio){
    pregunta*actual=inicio;
    bool terminar=false;
    int tecla;
    while(!terminar){
        mostrarPreg(actual,false);
        if(actual->respuesta==0){
            cout<<"Respuesta: (sin contestar)";
        }
        else{
            cout<<"Respuesta: op"<<actual->respuesta<<endl;
        }
        cout<<"\n[<- ->] Navegar [1-4] Contestar  [F]Finalizar\n";
        tecla=getch();
        if(tecla==224){
            tecla=getch();
            if(tecla==75&&actual->ant){
                actual=actual->ant; //izquierda
            }
            if(tecla==77&&actual->sig){
                actual=actual->sig; //derecha
            }
        }
        else if(tecla >='1'&&tecla <= '4'){
            actual->respuesta=tecla-'0'; //convierte char a int
        }
        else if(tecla=='f'||tecla=='F'){
            terminar=true;
        }
    }
    //recorrer la lista y sumar puntos
    float obtenidos=0, total=0;
    pregunta *aux=inicio;
    while(aux){
        total += aux->puntos;
        if(aux->respuesta==aux->correcta){
            obtenidos+=aux->puntos;
        }
        aux=aux->sig;
    }
    system("cls");
    cout<<"Examen finalizado\n";
    cout<<"Puntos obtenidos: "<<obtenidos<<"/"<<total<<"\n";
    cout<<"Calificacion: "<<(obtenidos/total*100.0f)<<"%\n";
}
void mostrarPreg(pregunta *p, bool modoEdic){
    system("cla");
    cout<<p->cuestion<<"\n";
    cout<<"1) "<<p->opcion1<<"\n";
    cout<<"2) "<<p->opcion1<<"\n";
    cout<<"3) "<<p->opcion1<<"\n";
    cout<<"4) "<<p->opcion1<<"\n";
    cout<<"Puntos: "<<p->puntos;
    if(modoEdic){
        cout<<"Correcta: op"<<p->correcta;
        cout<<endl;
    }
}
void agregarPreg(pregunta *&inicio, pregunta *&actual){
    pregunta *nuevo =new pregunta;
    nuevo->sig=NULL;
    nuevo->ant=NULL;
    nuevo->puntos=1;
    nuevo->correcta=1;
    nuevo->respuesta=0;
    cin.ignore();
    system("cls");
    cout<<"==== NUEVA PREGUNTA ====\n";
    cout<<"Pregunta : ";
    getline(cin, nuevo->cuestion);
    cout<<"Opc 1: ";
    getline(cin,nuevo->opcion1);
    cout<<"Opc 2: ";
    getline(cin,nuevo->opcion2);
    cout<<"Opc 3: ";
    getline(cin,nuevo->opcion3);
    cout<<"Opc 4: ";
    getline(cin,nuevo->opcion4);
    cout<<"Correcta (1-4)";
    cin>>nuevo->correcta;
    cout<<"Puntos: ";
    cin>>nuevo->puntos;
    if(!inicio){
        inicio=actual=nuevo; //primer modo de la lista
    }
    else{
        pregunta *final=inicio; //buscar el ultimo y insertar al final
         while(final->sig){
            final=final->sig;
        }
        final->sig=nuevo;
        nuevo->ant=final;
        actual=nuevo;

    }

}
int main() {
system("color 05");//color morado en la consola de windows eso
    bool salir = false;//flag que se activara con la opcionde salir para romper ciclo
    pregunta *inicio=NULL;
    pregunta *actual =NULL; 
    while (!salir) {//ciclod e el menu
        int opcion = menu();//obtiene el valor de la opcion ose ael seleccion de arriba
        system("cls");//limpia la pantalla
        switch (opcion) {
            case 0://aqui va todo el proceso de apliacion de el examen
                creararchivo(inicio,actual);
                if(inicio){
                    aplicarExam(inicio);
                }
                else{
                    cout<<"No hay examen";
                }
                break;
            case 1://aqui todo lo de modificacion
                cout << "seleccionaste modificar exam";
                break;
            case 2://lo mismo en generar
                creararchivo(inicio,actual);
                actual=inicio;
                generarExam(inicio,actual);
                guardar(inicio);
                cout<<"Guardar...";
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
