/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <iostream>
#include <unistd.h> //contains various constants

#include "SIMPLESOCKET.H"

using namespace std;

int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";

	c.conn(host, 2023);

	//INIT
	c.sendData("INIT");
	c.receive(32);

	for(int y = 1; y <= 10; y++ ){
        for(int x = 1; x <= 10; x++){
        string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);

        string res = c.receive(32);
        cout << req << " -> " << res << endl;

        if(res == "GAME_OVER"){
            cout << "Spiel beendet" << endl;
            return 0;
        }
        }

	}

}


