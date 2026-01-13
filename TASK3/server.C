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

class MySrv : public TCPserver{
public:
    MySrv(int port, int bufferSize) : TCPserver(port, bufferSize){};
protected:
    string myResponse(string input);
private:
    TASK3::World* w_ = nullptr;

};

int main(){
	srand(time(nullptr));
	MySrv srv(2023,25);
	srv.run();
}


string MySrv::myResponse(string input){

    if(input.compare(0,4,"INIT") == 0){
    //initalize a new game
        delete w_;
        w_ = new TASK3::World();

        return string("OKAY");
    }

    if(input.compare(0,6,"COORD(") == 0){
        //verarveite Koordinaten

        if(!w_) return "ERROR_NO_GAME";

        int start = input.find('(');
        int mitte = input.find(',');
        int ende  = input.find(')');

        string X_value = input.substr(start + 1, mitte - (start +1));
        string Y_value = input.substr(mitte + 1, ende - (mitte +1));

        int x = stoi(X_value);
        int y = stoi(Y_value);

        TASK3::ShootResult res = w_->shoot(x,y);

    switch(res){

        case TASK3::WATER: return "WATER";
        case TASK3::SHIP_HIT: return "SHIP_HIT";
        case TASK3::SHIP_DESTROYED: return "SHIP_DESTROYED";
        case TASK3::GAME_OVER: return "GAME_OVER";
        default: return "UNKNOWN";
    }

}
return string("UNKNWON_COMMAND");
}
