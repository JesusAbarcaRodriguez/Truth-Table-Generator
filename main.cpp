#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <cmath>
using namespace std;

class Variable
{
private:
    char nombre;
    bool valores[16];
    bool noValores[16];
    int contFilas;

public:
    Variable()
    {
        this->nombre = ' ';
        for (int i = 0; i < 16; i++)
        {
            this->valores[i] = false;
            noValores[i] = false;
        }
    }
    Variable(char nombre)
    {
        this->nombre = nombre;
        for (int i = 0; i < 16; i++)
        {
            this->valores[i] = false;
        }
    }
    void setValor(bool valor)
    {
        this->valores[contFilas] = valor;
        this->noValores[contFilas] = !valor;
        contFilas++;
    }
    bool getValor(int fila)
    {
        return this->valores[fila];
    }
    bool getNoValor(int fila)
    {
        return this->noValores[fila];
    }
    char getNombre()
    {
        return this->nombre;
    }
    void setNombre(char nombre)
    {
        this->nombre = nombre;
    }
};

string contarVariables(string);
void imprimirTabla();
string abrirTxt();
void leer();
void tablaVerdad();
void generarTablaVerdad();
void separarExpresiones(string);
int maxFila = 0;
int maxColumna = 0;
stack<string> pila;
string archivo;
string variablesStr = " ";
Variable arrVariables[5];

int main(int argc, char const *argv[])
{

    int respuesta = 0;
    do
    {
        respuesta = -1;
        cout << "-----------------------------------------------------" << endl;
        cout << "Menu'" << endl;
        cout << " 1 Digitar documento" << endl;
        cout << " 2 Ver tablas" << endl;
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
            variablesStr = contarVariables(archivo);
            cout << archivo << endl;
            break;
        case 2:
            tablaVerdad();
            imprimirTabla();
            // separarExpresiones(archivo);
            // generarTablaVerdad();
            cout << "Primero debe ingresar un documento" << endl;
            break;
        case 3:
            break;
        case 0:
            break;
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
    arc.close();

    return txt;
}
string contarVariables(const string txt)
{
    string stringVariables = "";
    for (char c : txt)
    {
        if (isalpha(c))
        {
            if (stringVariables.find(c) == string::npos)
            {
                stringVariables += c;
            }
        }
    }

    sort(stringVariables.begin(), stringVariables.end()); // abc
    for (int i = 0; i < stringVariables.length(); i++)
    {
        arrVariables[i].setNombre(stringVariables[i]);
    }
    return stringVariables;
}

void tablaVerdad()
{
    maxFila = pow(2, variablesStr.length());
    maxColumna = variablesStr.length();
  

    for (int i = 0; i < maxFila; i++)
    {
        for (int j = maxColumna - 1; j >= 0; j--)
        {
            if (((i / (int)pow(2, j)) % 2) == 1)
            {
                arrVariables[maxColumna -(j+1)].setValor(true);
            }
            else
            {
                arrVariables[maxColumna - (j+1)].setValor(false);
            }
        }
        cout << endl;
    }
}
void imprimirTabla()
{
    for (int i = 0; i < maxColumna; i++)
    {
        cout << arrVariables[i].getNombre() << "| ";
    }
    cout << endl;
    for (int i = 0; i < maxFila; i++)
    {
        for (int j = 0; j < maxColumna; j++)
        {
            if (arrVariables[j].getValor(i))
            {
                cout << "1"<< "| ";
            }
            else
            {
                cout << "0"<< "| ";
            }
        }
        cout << endl;
    }
}
void separarExpresiones(string archivo)
{
    string expresion = "";
    for (char c : archivo)
    {
        if (c == ',')
        {
            pila.push(expresion);
            expresion = "";
        }
        else
        {
            expresion += c;
        }
    }
    pila.push(expresion);
    cout << pila.top() << endl;
}
void generarTablaVerdad()
{ /*
     stack<string> pilaAux = pila;
     for (int i = 0; i < maxColumna; i++)
     {
         cout << "|" << variables[i] << "|";
     }
     for (int p = 0; p < pila.size(); p++)
     {
         cout << pilaAux.top() << endl;
         pilaAux.pop();
     }
     cout << endl;
     for (int i = 0; i < maxFila; i++)
     {
         for (int j = 0; j < maxColumna; j++)
         {
             cout << table[i][j] << " ";
         }
         cout << endl;

         // operacion
     }
     */
}
