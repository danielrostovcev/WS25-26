/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
/*
//Reihenweise
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

	//INIT
	c.sendData("INIT");
	c.receive(32);

	for(int y = 1; y <= 10; y++ ){
        for(int x = 1; x <= 10; x++){
        string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);
        shots++;

        string res = c.receive(32);
        if(res == "SHIP_HIT" || res == "SHIP_DESTROYED"){
            hits++;
        }
        cout << req << " -> " << res << endl;

        if(res == "GAME_OVER"){
            float shots_f = shots;
            float quote = (hits/shots_f);
            cout << "Spiel beendet" << endl << "benötigte Züge: " << shots << endl << "davon getroffen: " << hits << endl;
            cout << "Trefferquote: " << quote * 100 <<"%" << endl;
            return 0;
        }
        }

	}

}




//Random ohne Memory

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

	//INIT
	c.sendData("INIT");
	c.receive(32);

	while(true){

	int x = rand() % 10 +1;
	int y = rand() % 10 +1;


	string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);
        shots++;

        string res = c.receive(32);
        if(res == "SHIP_HIT" || res == "SHIP_DESTROYED"){
            hits++;
        }

        cout << req << " -> " << res << endl;

        if(res == "GAME_OVER"){
            float shots_f = shots;
            float quote = (hits/shots_f);
            cout << "Spiel beendet" << endl << "benötigte Züge: " << shots << endl;
            cout << "davon getroffen: " << hits << endl;
            cout << "Trefferquote: " << quote * 100 <<"%" << endl;
            break;
        }

	}

}




//Random mit Memory

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
	bool used[11][11] = {false};

	//INIT
	c.sendData("INIT");
	c.receive(32);

	while(true){

        int x = rand() %10 +1;
        int y = rand() %10 +1;

        if(used[x][y]){
         continue;
         }

        used[x][y] = true;
        shots++;

    string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);
        string res = c.receive(32);

        if(res == "SHIP_HIT" || res == "SHIP_DESTROYED"){
            hits++;
        }

        cout << req << " -> " << res << endl;

        if(res == "GAME_OVER"){
            float shots_f = shots;
            float quote = (hits/shots_f);
            cout << "Spiel beendet" << endl << "benötigte Züge: " << shots << endl;
            cout << "davon getroffen: " << hits << endl;
            cout << "Trefferquote: " << quote * 100 <<"%" << endl;
            break;
        }

	}
}

    //Schachbrett

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

	//INIT
	c.sendData("INIT");
	c.receive(32);

	for(int y = 1; y <= 10; y++ ){
        for(int x = 1; x <= 10; x++){
        int mod = (x+y) % 2;
        if( mod == 0){
        string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);
        shots++;

        string res = c.receive(32);
        if(res == "SHIP_HIT" || res == "SHIP_DESTROYED"){
            hits++;
        }
        cout << req << " -> " << res << endl;
        }

        if(res == "GAME_OVER"){
            float shots_f = shots;
            float quote = (hits/shots_f);
            cout << "Spiel beendet" << endl << "benötigte Züge: " << shots << endl << "davon getroffen: " << hits << endl;
            cout << "Trefferquote: " << quote * 100 <<"%" << endl;
            return 0;
        }
        }

	}

}
*/
