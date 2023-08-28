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
Variable operacionXor(Variable, Variable);
Variable operacionAnd(Variable, Variable);
Variable operationOr(Variable, Variable);

int maxFila = 0;
int maxColumna = 0;
stack<string> pilaExpresiones;


Variable arrVariables[5];

int main(int argc, char const *argv[])
{
	string archivo;
	string variablesStr = " ";
    int respuesta = 0;
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
			solucion(pilaExpresiones.top());
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
    if (arc.fail()) {
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

    string expresionAux = expresion;
    Variable solucionvariable;
    string newExpresion = "";
    if (expresion.find("(") != std::string::npos)
    {

        expresionAux = expresion.substr(expresion.find("(") + 1, expresion.find(")") - expresion.find("(") - 1);
        solucionvariable = solucion(expresionAux);
        if (expresion.find(")") + 1 == 239)
        {
            solucionvariable.invertirValores();
        }
        expresion = expresion.substr(0, expresion.find("(")) + "%" + expresion.substr(expresion.find(")") + 1, expresion.length() - expresion.find(")") - 1);
        cout << expresionAux << endl;
        cout << " ***********" << endl;
    }
    Variable opVariables[6];
    int nOperaciones = 0;

    while (expresion.size() > 1)
    {
        if (expresion.find("#"))
        {
            Variable variable1,variable2;
            int isANegative = 1;
            int isBNegative = 1;
            if (expresion[expresion.find("#") - 1] == 239)
            {
                isANegative++;
            }
            if (expresion[expresion.find("#") - 1] == 37) //%
            {
				variable1 = solucionvariable;
            }
            else if (isalpha(expresion[expresion.find("#") - isANegative]))
            {

                char aValue = expresion[expresion.find("#") - isANegative];
                cout << aValue << endl;
				variable1 = getVariables(aValue);
                isANegative == 2 ? variable1.invertirValores() :variable1.setAllValores(variable1.getAllValores());
            }
            else
            {
				variable1 = getVariableByList(expresion[expresion.find("#") - isANegative], opVariables);
            }

            if (expresion[expresion.find("#") + 1] == 37)
            {
				variable2 = solucionvariable;
            }
            else if (isalpha(expresion[expresion.find("#") + 1]))
            {
                char bValue = expresion[expresion.find("#") + 1];
                cout << bValue << endl;
				variable2 = getVariables(bValue);
            }
            else
            {
				variable2 = getVariableByList(expresion[expresion.find("#") + 1], opVariables);
            }
            if (expresion[expresion.find("#") + 2] == 239)
            {
				variable2.invertirValores();
                isBNegative++;
            }

            opVariables[nOperaciones] = operacionXor(variable1, variable2);
            opVariables[nOperaciones].setNombre(to_string(nOperaciones)[0]);
            expresion = expresion.substr(0, expresion.find("#") - isANegative) + to_string(nOperaciones)[0] + expresion.substr(expresion.find("#") + isBNegative + 1, expresion.length() - expresion.find("#") - isBNegative);
            nOperaciones++;
        }


    }
	for (int j = 0; j < nOperaciones; j++)
		cout<< expresion<<endl;
		{
			for (int i = 0; i < maxFila; i++)
			{
				cout << opVariables[nOperaciones - 1].getValor(i) << " ";
			}
			cout << endl;
		}
	
	return opVariables[nOperaciones - 1];
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
    }
    return outVariable;
}
Variable operacionXor(Variable variable1, Variable variable2)
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

Variable getVariableByList(char variable, Variable newArrVariables[])
{
    for (int i = 0; i < maxColumna; i++)
    {
        if (newArrVariables[i].getNombre() == variable)
        {

            return newArrVariables[i];
        }
    }
    return false;
}


