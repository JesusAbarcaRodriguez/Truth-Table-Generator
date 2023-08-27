#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;
string abrirTxt();
void leer();
int main(int argc, char const *argv[])
{
    int respuesta = 0;
    string archivo;
    string archi;
    char variables[5];
    for (int i = 0; i < 5; ++i) {
        variables[i] = '\0';  
    }
    do
    {
        respuesta = -1;
        cout << "-----------------------------------------------------" << endl;
        cout << "Menu'" << endl;
        cout << " 1 Digitar documento" << endl;
        cout << " 2 Ver mapas de karnaugh " << endl;
        cout << " 3 Ver simplificacion " << endl;
        cout << " 0 finalizar" << endl;
        cin >> respuesta;
        cout << "-----------------------------------------------------" << endl;
        cout << "Su respuesta fue " << respuesta << endl;
        cout << "-----------------------------------------------------" << endl;
        switch (respuesta)
        {
        case 1:
            archivo = abrirTxt();
            cout << archivo << endl;
            break;
        case 2:
            cout << "Primero debe ingresar un documento" << endl;
            break;
        case 3:
            break;
        case 0:
            break; // este caso es para que no de el mensaje de default porque 0 si es una obcion
        default:
            cout << "Su respuestas no esta entre las opciones " << endl;
        }
    } while (respuesta != 0);
    return 0;
}

string abrirTxt()
{

    string archi;

    cout << "Digite el nombre del archivo" << endl;
    fflush(stdin);
    getline(cin, archi);
    fflush(stdin);

    string txt;
    ifstream arc;

    arc.open(archi.c_str(), ios::in);

    if (arc.fail())
    {

        cout << "error" << endl;
        exit(1);
    }

    while (!arc.eof())
    {
        getline(arc, txt);
		
    }
    cout << txt << endl;
    arc.close();

    return txt;
}