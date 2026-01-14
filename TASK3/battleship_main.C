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

	int shots = 0;
	int hits = 0;
	int run = 1;

	//INIT
	c.sendData("INIT");
	c.receive(32);

	while(run == 1){

	int x = rand() % 10 +1;
	int y = rand() % 10 +1;




	string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);
        shots++;

        string res = c.receive(32);
        if(res == "SHIP_HIT" || res == "SHIP_DESTROYED"){
            hits++;

        cout << req << " -> " << res << endl;

        if(res == "GAME_OVER"){
            float shots_f = shots;
            float quote = (hits/shots_f);
            cout << "Spiel beendet" << endl << "benötigte Züge: " << shots << endl << "davon getroffen: " << hits << endl;
            cout << "Trefferquote: " << quote * 100 <<"%" << endl;
            run = 0;
        }


        }
	}

