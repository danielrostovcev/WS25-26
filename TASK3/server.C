/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> //contains various functions for manipulating date and time

#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses

#include <string>
#include<iostream>
#include "TASK3.H"

#include "SIMPLESOCKET.H"

//Klasse MySrv erbt von TCPserver. Sie erweitert den allgemeinen TCPserver um die Spiellogik von Battleship.

class MySrv : public TCPserver{
public:
    MySrv(int port, int bufferSize) : TCPserver(port, bufferSize){w_ = new TASK3::World();};
                        //Konstruktor übergibt port und puffergroesse an die Basisklasse TCPserver und erstellt eine neue Spielwelt
    ~MySrv(){
        delete w_;  //Destruktor gibt den reservierten Speicher wieder für die Spielwelt frei
    }
protected:
    string myResponse(string input);   //überschreibt die virtuelle methode der Basisklasse und verarbeitet eingehende Nachrichten vom Client
private:
    TASK3::World* w_ = nullptr;   //Zeiger auf die aktuelle Spielwelt, wird bei INIT neu erstellt

};

int main(){
	srand(time(nullptr));  //Zufallsgenerator
	MySrv srv(2023,25);    //Erstellen des Server-Objekts auf Port 2023
	srv.run();              //Startet die Server-Schleife
}


string MySrv::myResponse(string input){

    if(input.compare(0,4,"INIT") == 0){  //Wenn der Client INIT sendet, wird eine neue Spielwelt erzeugt
    //initalize a new game
        delete w_;                  //alte Spielwelt löschen und neue Spielwelt erzeugen
        w_ = new TASK3::World();

        return string("OKAY");      //Bestätigung an Client
    }

    if(input.compare(0,6,"COORD(") == 0){       //Wenn der Client eine Koordinate sendet im Format COORD(x,y)
        //verarveite Koordinaten

        if(!w_) return "ERROR_NO_GAME";         //Falls keine Spielwelt existiert -> Fehler

        int start = input.find('(');        //Position der Klammern und des Kommas ermitteln
        int mitte = input.find(',');
        int ende  = input.find(')');

        string X_value = input.substr(start + 1, mitte - (start +1));   //Teilstrings für X und Y extrahieren
        string Y_value = input.substr(mitte + 1, ende - (mitte +1));

        int x = stoi(X_value);          //Umwandlung der Strings in Integer
        int y = stoi(Y_value);

        TASK3::ShootResult res = w_->shoot(x,y);        //Aufruf der shoot-Funktion der Spielwelt

    switch(res){        //Auswertung der Ergebnisse und Rückgabe an den Client

        case TASK3::WATER: return "WATER";
        case TASK3::SHIP_HIT: return "SHIP_HIT";
        case TASK3::SHIP_DESTROYED: return "SHIP_DESTROYED";
        case TASK3::GAME_OVER: return "GAME_OVER";
        default: return "UNKNOWN";
    }

}
return string("UNKNWON_COMMAND");  //Falls keine bekannte Nachricht empfangen wurde
}
