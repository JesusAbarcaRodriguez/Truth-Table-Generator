#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <cmath>
#define NOT 39

using namespace std;

class Variable
{
private:
    char nombre;
    bool valores[32];
    bool noValores[32];
    int contFilas;

public:
    Variable()
    {
        contFilas = 0;
        this->nombre = ' ';
        for (int i = 0; i < 32; i++)
        {
            this->valores[i] = false;
            noValores[i] = true;
        }
    }
    Variable(char nombre)
    {
        this->nombre = nombre;
        for (int i = 0; i < 32; i++)
            this->valores[i] = false;
    }
    void setValor(bool valor)
    {
        this->valores[contFilas] = valor;
        this->noValores[contFilas] = !valor;
        contFilas++;
    }
    bool getValor(int fila)
    { return this->valores[fila]; }
    bool getNoValor(int fila)
    { return this->noValores[fila]; }
    char getNombre() const
    {
        return nombre;
    }
    void setNombre(char nombre)
    {  this->nombre = nombre;   }
    void setAllValores(bool valor[])
    {
        for (int i = 0; i < 32; i++)
    this->valores[i] = valor[i];
    }

    bool *getAllValores()
    { return this->valores; }

    void invertirValores()
    {
        for (int i = 0; i < 32; i++)
        {
            this->valores[i] = !this->valores[i];
            this->noValores[i] = !this->noValores[i];
        }
    }
};

Variable solucion(string);
string contarVariables(string);
void imprimirTabla();
string abrirTxt();
void leer();
void tablaVerdad(string variablesStr);
void generarTablaVerdad();
void separarExpresiones(string);
Variable getVariables(char);
Variable getVariableByList(char, Variable[]);
Variable operationXor(Variable, Variable);
Variable operationAnd(Variable, Variable);
Variable operationOr(Variable, Variable);

void aplicarOperador(stack<Variable> &pilaVariables, char operador);

void imprimirSolucion(Variable, string);
void imprimirSolucionList(stack<Variable>, stack<string>);
int prioridad(char);
int maxFila = 0;
int maxColumna = 0;
stack<string> pilaExpresiones;
stack<string> pilaExpresiones2;
Variable arrVariables[5];
stack<Variable> soluciones;
int main(int argc, char const *argv[])
{
    string archivo;
    string variablesStr = " ";
    int respuesta = 0;
    Variable solu;
    do
    {
        system("cls");
        respuesta = -1;
        cout << "-----------------------------------------------------" << endl;
        cout << "Menu'" << endl;
        cout << " 1 Digitar documento" << endl;
        cout << " 0 finalizar" << endl;
        cin >> respuesta;
        cout << "-----------------------------------------------------" << endl;
        cout << "Su respuesta fue " << respuesta << endl;
        cout << "-----------------------------------------------------" << endl;
        switch (respuesta)
        {
        case 1:
            archivo = abrirTxt();
            if (archivo == "error"){
                system("pause");
                break;
            }
            variablesStr = contarVariables(archivo);
            tablaVerdad(variablesStr);
            separarExpresiones(archivo);
            pilaExpresiones2 = pilaExpresiones;
            imprimirTabla();
            while (!pilaExpresiones.empty())
            {
                cout << pilaExpresiones.top() << endl;
                solu = solucion(pilaExpresiones.top());
                imprimirSolucion(solu, pilaExpresiones.top());
                pilaExpresiones.pop();
                soluciones.push(solu);
                system("pause");
                system("cls");
            }
            if (soluciones.size() > 1)
                imprimirSolucionList(soluciones, pilaExpresiones2);
            break;
        case 0:
            break;
        default:
            cout << "Su respuesta no esta entre las opciones." << endl;
            break;
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
    if (arc.fail()){
        cout << "error el archivo no se encuentra" << endl;
        return "error";
    }
    else
    while (!arc.eof())
        getline(arc, txt);

    arc.close();

    return txt;
}
string contarVariables(const string txt)
{
    string stringVariables = "";
    for (char c : txt)
        if (isalpha(c))
            if (stringVariables.find(c) == string::npos)
                stringVariables += c;
    sort(stringVariables.begin(), stringVariables.end());
    for (int i = 0; i < stringVariables.length(); i++)
        arrVariables[i].setNombre(stringVariables[i]);
    return stringVariables;
}

void tablaVerdad(string variablesStr)
{
    maxFila = pow(2, variablesStr.length());
    maxColumna = variablesStr.length();

    for (int i = 0; i < maxFila; i++)
        for (int j = maxColumna - 1; j >= 0; j--)
            ((i / (int)pow(2, j)) % 2) == 1 ? arrVariables[maxColumna - (j + 1)].setValor(true) 
            : arrVariables[maxColumna - (j + 1)].setValor(false);
}

void imprimirTabla()
{
    for (int i = 0; i < maxColumna; i++)
        cout << arrVariables[i].getNombre() << "| ";
    cout << endl;
    for (int i = 0; i < maxFila; i++)
    {
        for (int j = 0; j < maxColumna; j++)
            if (arrVariables[j].getValor(i))
                cout << "1"<< "| "; 
            else
                cout << "0"<< "| ";
            
        cout << endl;
    }
}
void separarExpresiones(string archivo)
{
    string expresion = "";
    for (char c : archivo)
        if (c == ',')
        {
            pilaExpresiones.push(expresion);
            expresion = "";
        }
        else
            expresion += c; 

    pilaExpresiones.push(expresion);
}

Variable solucion(string expresion)
{
    stack<Variable> pilaVariables;
    stack<char> pilaOperadores;
    for (char c : expresion)
    {
        if (c == NOT && !pilaVariables.empty())
        {
            Variable change = pilaVariables.top();
            change.invertirValores();
            pilaVariables.pop();
            pilaVariables.push(change);
        }
        else if (c == '(')
        { pilaOperadores.push(c); }
        else if (c == ')')
        {
            while (!pilaOperadores.empty() && pilaOperadores.top() != '(')
            {
                char operador = pilaOperadores.top();
                pilaOperadores.pop();
                aplicarOperador(pilaVariables, operador);
            }
            if (!pilaOperadores.empty())
            { pilaOperadores.pop(); }
        }
        else if (c == '*' || c == '+' || c == '#')
        {
            while (!pilaOperadores.empty() && pilaOperadores.top() != '(' &&
                   prioridad(pilaOperadores.top()) >= prioridad(c))
            {
                char operador = pilaOperadores.top();
                pilaOperadores.pop();
                aplicarOperador(pilaVariables, operador);
            }
            pilaOperadores.push(c);
        }
        else if (isalpha(c))
        {
            Variable variable = getVariables(c);
            pilaVariables.push(variable);
        }
    }

    while (!pilaOperadores.empty())
    {
        char operador = pilaOperadores.top();
        pilaOperadores.pop();
        if (pilaVariables.size() > 1)
            aplicarOperador(pilaVariables, operador);
    }

    return pilaVariables.top();
}
void aplicarOperador(stack<Variable> &pilaVariables, char operador)
{
    Variable variable2 = pilaVariables.top();
    pilaVariables.pop();
    Variable variable1 = pilaVariables.top();
    pilaVariables.pop();

    if (operador == '*')
        pilaVariables.push(operationAnd(variable1, variable2));
    else if (operador == '+')
        pilaVariables.push(operationOr(variable1, variable2));
    else if (operador == '#')
        pilaVariables.push(operationXor(variable1, variable2));
}

int prioridad(char operador)
{
    if (operador == '*')
        return 2;
    else if (operador == '+')
        return 1;
    else if (operador == '#')
        return 3;
    return 0;
}

Variable operationAnd(Variable variable1, Variable variable2)
{
    Variable outVariable;

    for (int i = 0; i < maxFila; i++)
        variable1.getValor(i) && variable2.getValor(i)? outVariable.setValor(true) 
            : outVariable.setValor(false);

    return outVariable;
}
Variable operationOr(Variable variable1, Variable variable2)
{
    Variable outVariable;

    for (int i = 0; i < maxFila; i++)
        variable1.getValor(i) || variable2.getValor(i)? outVariable.setValor(true)
        :outVariable.setValor(false);
    return outVariable;
}
Variable operationXor(Variable variable1, Variable variable2)
{
    Variable outVariable;

    for (int i = 0; i < maxFila; i++)
        variable1.getValor(i) != variable2.getValor(i)? outVariable.setValor(true)
        :outVariable.setValor(false);
    return outVariable;
}

Variable getVariables(char variableChar)
{
    for (int i = 0; i < maxColumna; i++)
    {
        char valueA = arrVariables[i].getNombre();
        if (valueA == variableChar)
            return arrVariables[i];
    }
    return false;
}

void imprimirSolucion(Variable variable, string variablesStr)
{
    cout << "Solucion:  " << variablesStr << endl;
    for (int i = 0; i < maxColumna; i++)
    {
        cout.width(3);
        cout << arrVariables[i].getNombre() << "| ";
    }
    cout << variablesStr << "|";
    cout << endl;
    cout << endl;
    for (int i = 0; i < maxFila; i++)
    {
        for (int j = 0; j < maxColumna; j++)
        {
            if (arrVariables[j].getValor(i))
            {
                cout.width(3);
                cout << "1"
                    << "| ";
            }
            else
            {
                cout.width(3);
                cout << "0"
                    << "| ";
            }
        }
        if (variable.getValor(i))
        {

            cout.width((variablesStr.length() / 2) + 1);
            cout << "1";
        }
        else
        {
            cout.width((variablesStr.length() / 2) + 1);
            cout << "0";
        }
        cout.width(variablesStr.length() / 2 + 1);
        cout << "|";

        cout << endl;
    }
}
void imprimirSolucionList(stack<Variable> arrSoluciones, stack<string> expresionesSolu)
{
    int cantExpresiones = expresionesSolu.size();
    stack<string> expresiones2 = expresionesSolu;
    stack<Variable> arrSoluciones2 = arrSoluciones;
    for (int i = 0; i < maxColumna; i++)
    {
        cout.width(3);
        cout << arrVariables[i].getNombre() << "| ";
    }
    while (!expresiones2.empty())
    {
        cout.width(expresiones2.top().length() + 1);
        cout << expresiones2.top() << "|";
        expresiones2.pop();
    }
    cout << endl;

    for (int i = 0; i < maxFila; i++)
    {
        expresiones2 = expresionesSolu;
        arrSoluciones2 = arrSoluciones;
        for (int j = 0; j < maxColumna; j++)
        {
            if (arrVariables[j].getValor(i))
            {
                cout.width(3);
                cout << "1"
                     << "| ";
            }
            else
            {
                cout.width(3);
                cout << "0"
                     << "| ";
            }
        }
        for (int j = 0; j < cantExpresiones; j++)
        {
            if (arrSoluciones2.top().getValor(i))
            {
                cout.width((expresiones2.top().length() / 2) + 1);
                cout << "1";
            }
            else
            {
                cout.width((expresiones2.top().length() / 2) + 1);
                cout << "0";
            }
            cout.width(expresiones2.top().length() / 2 + 2);
            cout << "|";
            expresiones2.pop();
            arrSoluciones2.pop();
        }
        cout << endl;
    }
    system("pause");
    system("cls");
}
