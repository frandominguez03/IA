#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <cmath>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

/*
	Función para calcular la mejor columna a seleccionar
	Va comprobando si para cada posición en horizontal se encuentran fichas seguidas del jugador. Si se encuentran dos,
	la puntuación de esa columna se aumenta en 2². Si se encuentran 3 seguidas, la puntuación se aumenta en 2³. Si
	solo hay una ficha, se le suma 2¹.

	Si se encuentra una ficha del enemigo, se resta a la puntuaócin de esa columna lo correspondiente, dependiendo
	del número de fichas seguidas que lleve hasta el momento, con los mismos criterios anteriormente hablados.
 */
int mejorColumna(const Environment& estado, int jugador){
	int puntosCol = 0, fichasSeguidas = 0, bombaJugador, bombaEnemigo;

	if(jugador == 1){
		bombaJugador = 4;
		bombaEnemigo = 5;
	}

	else{
		bombaEnemigo = 4;
		bombaJugador = 5;
	}

	for(int i=0; i<7; i++){
		for(int j=0; j<7; j++){
			if(estado.See_Casilla(i, j) == jugador || estado.See_Casilla(i, j) == bombaJugador){
				fichasSeguidas++;

				if(fichasSeguidas == 2){
					puntosCol += pow(2, fichasSeguidas);
				}

				else if(fichasSeguidas == 3){
					puntosCol += pow(2, fichasSeguidas);
				}

				else{
					puntosCol += pow(2, fichasSeguidas);
				}
			}

			else if(estado.See_Casilla(i, j) != jugador || estado.See_Casilla(i, j) == bombaEnemigo){
				if(fichasSeguidas == 2){
					puntosCol -= pow(2, fichasSeguidas);
				}

				else if(fichasSeguidas == 3){
					puntosCol -= pow(2, fichasSeguidas);
				}

				else{
					puntosCol -= pow(2, fichasSeguidas);
				}

				fichasSeguidas = 0;
			}

			else{
				fichasSeguidas = 0;
			}
		}

		fichasSeguidas = 0;
	}

	return puntosCol;
}

/*
	Función para calcular la mejor fila a seleccionar
	Va comprobando si para cada posición en vertical se encuentran fichas seguidas del jugador. Si se encuentran dos,
	la puntuación de esa fila se aumenta en 2². Si se encuentran 3 seguidas, la puntuación se aumenta en 2³. Si
	solo hay una ficha, se le suma 2¹.

	Si se encuentra una ficha del enemigo, se resta a la puntuaócin de esa fila lo correspondiente, dependiendo
	del número de fichas seguidas que lleve hasta el momento, con los mismos criterios anteriormente hablados.
 */
int mejorFila(const Environment& estado, int jugador){
	int puntosFil = 0, fichasSeguidas = 0, bombaJugador, bombaEnemigo;

	if(jugador == 1){
		bombaJugador = 4;
		bombaEnemigo = 5;
	}

	else{
		bombaEnemigo = 4;
		bombaJugador = 5;
	}

	for(int i=0; i<7; i++){
		for(int j=0; j<7; j++){
			if(estado.See_Casilla(j, i) == jugador || estado.See_Casilla(j, i) == bombaJugador){
				fichasSeguidas++;

				if(fichasSeguidas == 2){
					puntosFil += pow(2, fichasSeguidas);
				}

				else if(fichasSeguidas == 3){
					puntosFil += pow(2, fichasSeguidas);
				}

				else{
					puntosFil += pow(2, fichasSeguidas);
				}
			}

			else if(estado.See_Casilla(j, i) != jugador || estado.See_Casilla(j, i) == bombaEnemigo){
				if(fichasSeguidas == 2){
					puntosFil -= pow(2, fichasSeguidas);
				}

				else if(fichasSeguidas == 3){
					puntosFil -= pow(2, fichasSeguidas);
				}

				else{
					puntosFil -= pow(2, fichasSeguidas);
				}

				fichasSeguidas = 0;
			}

			else{
				fichasSeguidas = 0;
			}
		}

		fichasSeguidas = 0;
	}

	return puntosFil;
}

double Heuristica(int jugador, const Environment &estado){
	int min;

	if(jugador == 1){
		min = 2;
	}

	else{
		min = 1;
	}

    int mejorColMAX = mejorColumna(estado, jugador);
    int mejorColMIN = mejorColumna(estado, min);
    int mejorFilaMAX = mejorFila(estado, jugador);
    int mejorFilaMIN = mejorFila(estado, min);

    return ((mejorColMIN + mejorFilaMIN) - (mejorColMAX + mejorColMIN));
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
	int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Heuristica(jugador, estado);
}


// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

double Poda_AlfaBeta(const Environment& actual_, int jugador, int profundidad, double alfa, double beta,
	Environment::ActionType& accion, bool maximizando){
	
	int ultimaAccion = -1;
	bool aplicables[8];
	int numHijos = actual_.possible_actions(aplicables);
	Environment::ActionType accionAnterior;
	double valor;
	Environment hijo = actual_.GenerateNextMove(ultimaAccion);

	if(profundidad == 0 || actual_.JuegoTerminado()){
		return Valoracion(actual_, jugador);
	}

	// Jugador 1
	if(maximizando){
		for(int i=0; i<numHijos; i++){
			valor = Poda_AlfaBeta(hijo, jugador, profundidad-1, alfa, beta, accionAnterior, false);

			if(valor > alfa){
				alfa = valor;
				accion = static_cast <Environment::ActionType>(ultimaAccion);
			}

			if(alfa >= beta){
				break;
			}

			hijo = actual_.GenerateNextMove(ultimaAccion);
		}

		return alfa;
	}

	// Jugador 2
	else{
		for(int i=0; i<numHijos; i++){
			valor = Poda_AlfaBeta(hijo, jugador, profundidad-1, alfa, beta, accionAnterior, true);

			if(valor < beta){
				beta = valor;
				accion = static_cast <Environment::ActionType>(ultimaAccion);
			}

			if(alfa >= beta){
				break;
			}

			hijo = actual_.GenerateNextMove(ultimaAccion);
		}

		return beta;		
	}
}


// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

    /*
    //--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //--------------------- COMENTAR Hasta aqui
	*/

    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    alpha = menosinf;
    beta = masinf;
    valor = Poda_AlfaBeta(actual_, jugador_, PROFUNDIDAD_ALFABETA, alpha, beta, accion, true);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}

