#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <cctype>
#include <new>//para el placemtentt que se usa junto con malloc

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
    coord.X = x;//coordenada en x de donde va a quedar el cursor
    coord.Y = y;//coordenada en y de donde va a quedar el cursor
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);//mueve el cursor a esa posicion
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

//declaraciones adelantadas (se necesitanf porque generarExam y aplicarExam usan estas funciones antes de su definicion)
void mostrarPreg(pregunta *p, bool modoEdic);
void agregarPreg(pregunta *&inicio, pregunta *&actual);
pregunta* crearNodo();
void liberarNodo(pregunta *nodo);
void liberarLista(pregunta *inicio);
bool esNumero(string s);

pregunta* crearNodo(){//reserva la memoria con malloc 
    pregunta *nodo=(pregunta*) malloc(sizeof(pregunta));//reserva memoria cruda de el tamano de la estructura
    new (nodo) pregunta();// con placement new se construye el objeto en esa memoria, si no se hace esto los string quedan basura y truena
    return nodo;
}
void liberarNodo(pregunta *nodo){//libera un nodo creado con malloc
    nodo->~pregunta();//hay que llamar al destructor a mano para que los string se liberen bien
    free(nodo);//ya con eso si se puede liberar la memoria
}
void liberarLista(pregunta *inicio){//recorre toda la lista y va liberando nodo por nodo
    pregunta *aux=inicio;
    while(aux){
        pregunta *siguiente=aux->sig;//se guarda antes de borrar pa no perder la referencia
        liberarNodo(aux);
        aux=siguiente;
    }}
bool esNumero(string s){//revisa que el string sea un numero valido (admite un punto para los decimales)
    if(s.empty()) return false;
    int puntos=0;
    for(int i=0;i<(int)s.size();i++){
        if(s[i]=='.'){
            puntos++;
            if(puntos>1) return false;//no puede tener mas de un punto
        }
        else if(!isdigit(s[i])){//funcion que retorna si algo es un digito
            return false;//si hay una letra o un simbolo raro ya no es numero
        }
    }
    return true;
}

bool creararchivo(string nombreArchivo, pregunta *&inicio, pregunta *&actual ){//lee el archivo .txt y va armando la lista de preguntas
    liberarLista(inicio);//si ya habia una lista cargada de antes se libera pa no dejar memoria tirada
    inicio=actual=NULL;//se resetea desde aqui para que quede limpio aunque el archivo no se pueda abrir
    ifstream arch(nombreArchivo);//abre el archivo para tener su contenido
    if(!arch.is_open()) return false;//si no se abrio bien regresa falso
    string linea;//
    pregunta *nuevo=NULL;//puntero pa ir armando la pregunta que se esta leyendo

    while(getline(arch,linea)){
        if(!linea.empty()&& linea.back()=='\r'){
            linea.pop_back(); // quitar \r de windows
        }
        if(linea.find(":p;")==0){
            nuevo=crearNodo();
            nuevo->cuestion=linea.substr(3); // substr es para extraer una parte de una cadena
            nuevo->respuesta=0; //inicializar sin contestar
            nuevo->sig=nuevo->ant=NULL;
        }
        else if(!nuevo){//si todavia no hay pregunta creada se brinca la linea
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
            nuevo->ant=actual;//conecta esta pregunta con la de antes
            if(inicio==NULL){
                inicio=nuevo;//si es la primera pregunta la pone como inicio
            }
            else{
                actual->sig=nuevo;//si no la engancha al final de la lista
            }
            actual=nuevo;//actual pasa a ser esta pregunta nueva
        }
    }
    arch.close();
    return (inicio!=NULL);//regresa true si si se pudo armar aunque sea una pregunta
}
void generarExam(pregunta *&inicio ,pregunta *&actual){//aqui se editan o se van agregando preguntas de el examen
    char r;//guarda si quiere agregar pregunta o no
    bool salir = false;//bandera para salir de el ciclo de edicion
    int tecla;
    if(!actual){//si la lista esta vacia pregunta si quiere meter una pregunta nueva
        cout<<"No hay pregunta, agregas una ?(S/N): ";
        cin>>r;
        if(r!='s' && r!='S') return;
        agregarPreg(inicio,actual);
    }
    while(!salir){//ciclo principal para navegar y editar las preguntas
        mostrarPreg(actual,true);
        cout<<"\n[<-  ->] Navegar  [E] Editar  [A] Añadir   [S] Salir\n";
        tecla=getch();
        if(tecla==224){
            tecla=getch();
            if(tecla==75 && actual->ant){
                actual=actual->ant; //izquierda
            }
            if(tecla==77 && actual->sig){
                actual=actual->sig; // derecha
            }
        }
        else if(tecla == 'e' || tecla=='E'){//modo editar la pregunta en la que estamos parados
            string tmp;
            cin.ignore();//limpia el buffer de entrada
            system("cls");
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
            if(!tmp.empty()){
                if(esNumero(tmp)) actual->correcta=stoi(tmp);//solo se cambia si si es numero, si no truena el stoi
                else cout<<"valor invalido, no se cambio\n";
            }
            cout<<"Puntos ["<<actual->puntos <<"]: "; 
            getline(cin,tmp);
            if(!tmp.empty()){
                if(esNumero(tmp)) actual->puntos=stof(tmp);//mismo cuento aqui pero con stof
                else cout<<"valor invalido, no se cambio\n";
            }
        }
        else if(tecla == 'a'|| tecla=='A'){//agrega una pregunta nueva a la lista
            agregarPreg(inicio,actual);
        }
        else if(tecla == 's'||tecla=='S'){
            salir=true;//con esto se rompe el ciclo de edicion
        }
    }
}
void guardar(string nombreArchivo, pregunta *inicio){//escribe toda la lista de preguntas en el .txt
    ofstream arch(nombreArchivo);//abre o crea el archivo para escribir
    pregunta *aux=inicio;//puntero pa recorrer la lista desde el inicio
    while(aux){//va recorriendo cada pregunta y la guarda en el formato de el archivo
        arch<<":p;"<<aux->cuestion<<endl;
        arch<<":op1;"<<aux->opcion1<<endl;
        arch<<":op2;"<<aux->opcion2<<endl;
        arch<<":op3;"<<aux->opcion3<<endl;
        arch<<":op4;"<<aux->opcion4<<endl;
        arch<<":r;op"<<aux->correcta<<endl;
        arch<<aux->puntos<<"."<<endl;
        aux=aux->sig;//avanza a la siguiente pregunta de la lista
    }
    arch.close();//cierra el archivo ya que se guardo
}
void aplicarExam(pregunta *inicio){//aqui se va contestando el examen completo
    pregunta*actual=inicio;//arranca desde la primera pregunta de la lista
    bool terminar=false;//bandera pa saber cuando ya se acabo el examen
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
            terminar=true;//ya se acabo el examen entonces sale del ciclo
        }
    }
    //recorrer la lista y sumar puntos
    float obtenidos=0, total=0;
    pregunta *aux=inicio;
    while(aux){
        total += aux->puntos;//va sumando los puntos de todas las preguntas
        if(aux->respuesta==aux->correcta){
            obtenidos+=aux->puntos;//si la respuesta quedo bien suma esos puntos
        }
        aux=aux->sig;
    }
    system("cls");
    cout<<"Examen finalizado\n";
    cout<<"Puntos obtenidos: "<<obtenidos<<"/"<<total<<"\n";
    cout<<"Calificacion: "<<(obtenidos/total*100.0f)<<"%\n";
}
void mostrarPreg(pregunta *p, bool modoEdic){//imprime la pregunta en la que estamos parados con sus opciones
    system("cls");//limpia pantalla antes de mostrar la pregunta
    cout<<p->cuestion<<"\n";
    cout<<"1) "<<p->opcion1<<"\n";
    cout<<"2) "<<p->opcion2<<"\n";
    cout<<"3) "<<p->opcion3<<"\n";
    cout<<"4) "<<p->opcion4<<"\n";
    cout<<"Puntos: "<<p->puntos;
    cout<<endl;//pa que no se quede pegado con lo que sigue
    if(modoEdic){//solo en modo edicion se muestra cual es la correcta
        cout<<"Correcta: op"<<p->correcta;
        cout<<endl;
    }
}
void agregarPreg(pregunta *&inicio, pregunta *&actual){//crea una pregunta nueva y la mete al final de la lista
    pregunta *nuevo =crearNodo();//nodo nuevo para la pregunta
    nuevo->sig=NULL;
    nuevo->ant=NULL;
    nuevo->puntos=1;//valor de puntos por defecto
    nuevo->correcta=1;//valor por defecto
    nuevo->respuesta=0;
    cin.ignore();//limpia el buffer de entrada
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
    if(cin.fail()){//si meten una letra en vez de numero se limpia el stream y se pone un valor por defecto
        cin.clear();
        cin.ignore(1000,'\n');
        nuevo->correcta=1;
    }
    cout<<"Puntos: ";
    cin>>nuevo->puntos;
    if(cin.fail()){
        cin.clear();
        cin.ignore(1000,'\n');
        nuevo->puntos=1;
    }
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
        actual=nuevo;//la pregunta nueva pasa a ser la actual

    }

}
int main() {
system("color 05");//color morado en la consola de windows eso
    bool salir = false;//flag que se activara con la opcionde salir para romper ciclo
    pregunta *inicio=NULL;//inicio de la lista de preguntas
    pregunta *actual =NULL; //pregunta en la que estamos parados
    while (!salir) {//ciclod e el menu
        int opcion = menu();//obtiene el valor de la opcion ose ael seleccion de arriba
        system("cls");//limpia la pantalla
        switch (opcion) {
            case 0:{//aqui va todo el proceso de apliacion de el examen
                system("dir *.txt /b");//muestra los examenes .txt disponibles en la carpeta
                string archivoAp;
                cout<<"\nNombre del examen a aplicar: ";
                cin>>archivoAp;
                if(creararchivo(archivoAp,inicio,actual)){
                    aplicarExam(inicio);
                }
                else{
                    cout<<"No hay examen";
                }
                break;
            }
            case 1:{//aqui todo lo de modificacion
                 
                system("dir *.txt /b");//muestra los examenes .txt disponibles en la carpeta
                string archivoMod;
                cout<<"\nnombre del examen a modificar: ";
                cin>>archivoMod;
                if(creararchivo(archivoMod,inicio,actual)){
                    actual=inicio;
                    generarExam(inicio,actual);
                    guardar(archivoMod,inicio);
                    cout<<"guardando...";
                }
                else{
                    cout<<"No se encontro ese archivo";
                }
                break;
            }
            case 2:{//lo mismo en generar
                string archivoGen;
                cout<<"Nombre del archivo a generar (nuevo o existente): ";
                cin>>archivoGen;

                if(archivoGen.find(".txt")==string::npos){
                archivoGen += ".txt";
                }
                creararchivo(archivoGen,inicio,actual);
                actual=inicio;
                generarExam(inicio,actual);
                guardar(archivoGen,inicio);
                cout<<"Guardar...";
                break;
            }
            case 3://salida de el programa
                salir = true;
                cout << "saliendo........";
                break; }
        if (!salir) {//si no elegiste salir te regresa al inicio de el ciclo para volver al menu y repetir en bucle
            cout << "\n\npresiona algo para ir al menu...";
            getch();
        }
    }
    liberarLista(inicio);//ya que se va a cerrar el programa se libera toda la memoria que se reservo con malloc
    return 0;
}
