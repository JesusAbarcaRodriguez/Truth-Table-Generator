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
        contFilas = 0;
        this->nombre = ' ';
        for (int i = 0; i < 16; i++)
        {
            this->valores[i] = false;
            noValores[i] = true;
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
    char getNombre() const
    {
        return nombre;
    }
    void setNombre(char nombre)
    {
        this->nombre = nombre;
    }
    void setAllValores(bool valor[])
    {
        for (int i = 0; i < 16; i++)
        {
            this->valores[i] = valor[i];
        }
    }

    bool *getAllValores()
    {
        return this->valores;
    }

    void invertirValores()
    {
        for (int i = 0; i < 16; i++)
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
void imprimirSolucion(Variable, string);

int precedence(char);
int maxFila = 0;
int maxColumna = 0;
stack<string> pilaExpresiones;

Variable arrVariables[5];

int main(int argc, char const *argv[])
{
    string archivo;
    string variablesStr = " ";
    int respuesta = 0;
    Variable solu;
    do
    {
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
            variablesStr = contarVariables(archivo);
            tablaVerdad(variablesStr);
            separarExpresiones(archivo);
            imprimirTabla();
            solu = solucion(pilaExpresiones.top());
            imprimirSolucion(solu, pilaExpresiones.top());
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
    sort(stringVariables.begin(), stringVariables.end());
    for (int i = 0; i < stringVariables.length(); i++)
    {
        arrVariables[i].setNombre(stringVariables[i]);
    }
    return stringVariables;
}

void tablaVerdad(string variablesStr)
{
    maxFila = pow(2, variablesStr.length());
    maxColumna = variablesStr.length();

    for (int i = 0; i < maxFila; i++)
    {
        for (int j = maxColumna - 1; j >= 0; j--)
        {
            if (((i / (int)pow(2, j)) % 2) == 1)
            {
                arrVariables[maxColumna - (j + 1)].setValor(true);
            }
            else
            {
                arrVariables[maxColumna - (j + 1)].setValor(false);
            }
        }
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
                cout << "1"
                     << "| ";
            }
            else
            {
                cout << "0"
                     << "| ";
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
            pilaExpresiones.push(expresion);
            expresion = "";
        }
        else
        {
            expresion += c;
        }
    }
    pilaExpresiones.push(expresion);
}

Variable solucion(string expresion)
{
    stack<Variable> pilaVariables;
    stack<char> pilaOperadores;

    for (char c : expresion)
    {
        if (c == '(')
        {
            pilaOperadores.push(c);
        }
        else if (c == ')')
        {
            while (!pilaOperadores.empty() && pilaOperadores.top() != '(')
            {
                char operador = pilaOperadores.top();
                pilaOperadores.pop();

                Variable variable2 = pilaVariables.top();
                pilaVariables.pop();

                if (operador == 239)
                { // NOT
                    variable2.invertirValores();
                }

                Variable variable1 = pilaVariables.top();
                pilaVariables.pop();

                if (operador == '*')
                { // AND
                    pilaVariables.push(operationAnd(variable1, variable2));
                }
                else if (operador == '+')
                { // OR
                    pilaVariables.push(operationOr(variable1, variable2));
                }
                else if (operador == '#')
                { // XOR
                    pilaVariables.push(operationXor(variable1, variable2));
                }
            }
            if (!pilaOperadores.empty())
            {
                pilaOperadores.pop(); // Quita el '(' de la pila
            }
        }
        else if (c == '*' || c == '+' || c == '#' || c == 239)
        {
            while (!pilaOperadores.empty() && pilaOperadores.top() != '(' &&
                   precedence(pilaOperadores.top()) >= precedence(c))
            {
                char operador = pilaOperadores.top();
                pilaOperadores.pop();

                Variable variable2 = pilaVariables.top();
                pilaVariables.pop();

                if (operador == 239)
                { // NOT
                    variable2.invertirValores();
                }

                Variable variable1 = pilaVariables.top();
                pilaVariables.pop();

                if (operador == '*')
                { // AND
                    pilaVariables.push(operationAnd(variable1, variable2));
                }
                else if (operador == '+')
                { // OR
                    pilaVariables.push(operationOr(variable1, variable2));
                }
                else if (operador == '#')
                { // XOR
                    pilaVariables.push(operationXor(variable1, variable2));
                }
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

        Variable variable2 = pilaVariables.top();
        pilaVariables.pop();

        if (operador == '\'')
        { // NOT
            variable2.invertirValores();
        }

        Variable variable1 = pilaVariables.top();
        pilaVariables.pop();

        if (operador == '*')
        { // AND
            pilaVariables.push(operationAnd(variable1, variable2));
        }
        else if (operador == '+')
        { // OR
            pilaVariables.push(operationOr(variable1, variable2));
        }
        else if (operador == '#')
        { // XOR
            pilaVariables.push(operationXor(variable1, variable2));
        }
    }

    return pilaVariables.top();
}

int precedence(char operador)
{
    if (operador == '*')
    { // AND
        return 2;
    }
    else if (operador == '+')
    { // OR
        return 1;
    }
    else if (operador == '#' || operador == 239)
    { // XOR y NOT
        return 3;
    }
    return 0;
}

Variable operationAnd(Variable variable1, Variable variable2)
{
    Variable outVariable;

    for (int i = 0; i < maxFila; i++)
    {
        if (variable1.getValor(i) && variable2.getValor(i))
        {
            outVariable.setValor(true);
        }
        else
        {
            outVariable.setValor(false);
        }
    }

    return outVariable;
}
Variable operationOr(Variable variable1, Variable variable2)
{
    Variable outVariable;

    for (int i = 0; i < maxFila; i++)
    {
        if (variable1.getValor(i) || variable2.getValor(i))
        {
            outVariable.setValor(true);
        }
        else
        {
            outVariable.setValor(false);
        }
    }
    return outVariable;
}
Variable operationXor(Variable variable1, Variable variable2)
{
    Variable outVariable;

    for (int i = 0; i < maxFila; i++)
    {
        if (variable1.getValor(i) != variable2.getValor(i))
        {
            outVariable.setValor(true);
        }
        else
        {
            outVariable.setValor(false);
        }
    }
    return outVariable;
}

Variable getVariables(char variableChar)
{
    for (int i = 0; i < maxColumna; i++)
    {
        char valueA = arrVariables[i].getNombre();

        if (valueA == variableChar)
        {
            return arrVariables[i];
        }
    }
    return false;
}

void imprimirSolucion(Variable variable, string variablesStr)
{
    cout << "Solucion: " << endl;
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

            cout.width((variablesStr.length() / 2) +1);
            cout << "1";
        }
        else
        {
            cout.width((variablesStr.length() / 2) + 1);
            cout << "0";
        }
        cout.width(variablesStr.length() / 2 +1);
        cout << "|";

        cout << endl;
    }
}