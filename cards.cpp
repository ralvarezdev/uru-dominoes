#include"clases.h"
#include"ioTerminal.h"
#include<iostream>
#include<vector>
#include<random>
#include<string>
#include<chrono>
#include<thread>

using std::random_device; using std::uniform_int_distribution;
using std::cout;
using std::vector;
using std::string;

bool esUnAs(int carta){ // Evalúa si la carta es un As
    if(carta%13==0){
        return true;
    }else{
        return false;
    }
}

void puntajeCarta(int carta, int& numeroAsParticipante, int& sumaCartasParticipante, bool& outParticipante){ // Devuelve la cantidad de puntos de la carta
    bool r = esUnAs(carta);

    if(r==true){
        numeroAsParticipante++;
        sumaCartasParticipante += 11;        
    }else{
        int numero = carta%13+1;

        if(numero!=1&&numero<11){
            sumaCartasParticipante += numero;
        }else{
            sumaCartasParticipante += 10;
        }
    } 

    while(sumaCartasParticipante>21&&numeroAsParticipante>=1){
        sumaCartasParticipante-=10;
        numeroAsParticipante--;
    };

    if(sumaCartasParticipante>21){
        outParticipante = true;
    }
}

void imprimirCarta(bool nuevaLinea, int carta){ // Según el número de la carta, imprime cuál es
    int x;

    if(nuevaLinea==true){
        cout<<"\n";
    }

    x = carta/13;
    carta -= 13*x;
    switch(x){
        case 0:
            cout<<"- C";
            break;
        case 1:
            cout<<"- D";
            break;
        case 2:
            cout<<"- T";
            break;
        case 3:
            cout<<"- P";
            break;
    }

    if(carta==0){
        cout<<"A";
    }else{
        switch(carta){
            case 10:
                cout<<"J";
                break;
            case 11:
                cout<<"Q";
                break;
            case 12:
                cout<<"K";
                break;
            default:
                cout<<carta+1;
        }
    }
}

void chequearMazo(vector<int>& mazo, int& cartasUsadas, int& numeroMazo){ // Chequea si quedan cartas en el mazo, de no haber generado un nuevo mazo
    int x;
    
    if(mazo.size()==0){
        cartasUsadas = 0;
        numeroMazo++;

        for(x=0;x<52;x++){
            mazo.push_back(x);
        }
    }
}

int revolverCarta(int numero, vector<int>& mazo, int& cartasUsadas, int& numeroMazo){ // Saca una carta aleatoria del mazo
    int carta = mazo[numero];
    mazo.erase(mazo.begin() + numero);
    cartasUsadas++;
    chequearMazo(mazo, cartasUsadas, numeroMazo);

    return carta;
}

void mensajeFinal(string mensajeInicial, string mensajeParaAleatorizar){
    random_device rd;

    char c;
    int mensajeAleatorioSize = 0;
    int x, n, numero, vectorSize, mensajeSize, mensajeAleatorioDiff;
    string mensajeAleatorio;

    mensajeSize = mensajeParaAleatorizar.size();

    vector<char> alfabeto{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    vector<char> simbolos{' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '+', '-', '*', '/', '!', '@', '$', '%', '&', '^', '?'};
    vector<char> caracteres, caracteresAleatorio;

    vectorSize = alfabeto.size();
    for(x=0;x<vectorSize;x++){
        alfabeto.push_back(toupper(alfabeto[x]));
    }

    caracteres.insert(caracteres.begin(), alfabeto.begin(), alfabeto.end());
    caracteres.insert(caracteres.end(), simbolos.begin(), simbolos.end());
    alfabeto.clear(); simbolos.clear();
    vectorSize = caracteres.size();

    for(x=0;x<vectorSize;x++){
        n = x+1;
        uniform_int_distribution<int> dist(0,vectorSize-n);
        numero = dist(rd);
        c = caracteres[numero];
        caracteres.erase(caracteres.begin()+numero);
        caracteresAleatorio.push_back(c); 
    }

    mensajeAleatorioDiff = mensajeSize;
    do{
        for(x=0;x<vectorSize;x++){
            c = caracteresAleatorio[x];
            system("cls");
            cout<<mensajeInicial<<"--- "<<mensajeAleatorio<<c<<string(mensajeAleatorioDiff-1, '#')<<" ---";

            if(c==mensajeParaAleatorizar[mensajeAleatorioSize]){
                mensajeAleatorioSize++;
                mensajeAleatorioDiff--;
                mensajeAleatorio.append(string(1, c));
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        if(mensajeAleatorioDiff==0){
            break;
        }
    }while(true);
}
