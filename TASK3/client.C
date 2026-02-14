#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

#include "SIMPLESOCKET.H"

using namespace std;

/*
    Diese Struktur speichert das Ergebnis eines kompletten Spiels:
    - shots: Anzahl der Züge/Schüsse bis GAME_OVER
    - hits : Anzahl der Treffer (SHIP_HIT oder SHIP_DESTROYED)
*/
struct Result {
    int shots;
    int hits;
};

/*
    Hilfsfunktion:
    Prüft, ob die Antwort des Servers als Treffer zählt.
    Treffer sind bei uns:
    - SHIP_HIT
    - SHIP_DESTROYED
*/
static bool isHit(const string& res){
    return (res == "SHIP_HIT" || res == "SHIP_DESTROYED");
}

/*
    STRATEGIE 1: SCAN (Reihenweise)
    --------------------------------
    Idee:
    - Wir schießen systematisch auf jedes Feld, von oben links nach unten rechts.
    - Also y=1..10 und in jeder Reihe x=1..10.
    Vorteil:
    - Sehr einfach und garantiert: irgendwann treffen wir alles.
    Nachteil:
    - Nicht besonders effizient, weil wir alles abklappern.
*/
Result playScan(TCPclient& c){
    // Neue Verbindung zum Server aufbauen (Port bleibt 2023)


    int shots = 0;  // Züge zählen
    int hits  = 0;  // Treffer zählen

    // Neues Spiel starten: INIT sagt dem Server "neue Welt erzeugen"
    c.sendData("INIT");
    c.receive(32); // Antwort z.B. "OKAY" (wird hier nicht weiter genutzt)

    // Reihenweise über das Spielfeld laufen (1..10)
    for(int y=1; y<=10; y++){
        for(int x=1; x<=10; x++){

            // Request-String bauen: COORD(x,y)
            string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";

            // Koordinate an Server senden
            c.sendData(req);
            shots++; // ein Zug gemacht

            // Antwort vom Server lesen
            string res = c.receive(32);

            // Treffer zählen
            if(isHit(res)) hits++;

            // Wenn Spiel vorbei ist, nur das Ergebnis ausgeben:
            // Ausgabeformat: STRATEGIE;ZUEGE
            if(res == "GAME_OVER"){
                cout << "SCAN;" << shots << endl;
                return {shots, hits};
            }
        }
    }

    // Falls aus irgendeinem Grund GAME_OVER nicht kommt (eigentlich unwahrscheinlich)
    cout << "SCAN;" << shots << endl;
    return {shots, hits};
}

/*
    STRATEGIE 2: RANDOM_SIMPLE (Zufall ohne Gedächtnis)
    --------------------------------------------------
    Idee:
    - Wir wählen immer zufällig ein Feld (x,y).
    - Wir merken uns NICHT, ob wir da schon geschossen haben.
    Vorteil:
    - Sehr einfach, kaum Code.
    Nachteil:
    - Es können doppelte Koordinaten vorkommen (ineffizient).
*/
Result playRandomSimple(TCPclient& c){


    int shots = 0;
    int hits  = 0;

    c.sendData("INIT");
    c.receive(32);

    while(true){
        // Zufällige Koordinate im Bereich 1..10
        int x = rand() % 10 + 1;
        int y = rand() % 10 + 1;

        // Request bauen und senden
        string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);
        shots++;

        // Antwort lesen
        string res = c.receive(32);

        // Treffer zählen
        if(isHit(res)) hits++;

        // Spielende -> nur Strategie + shots ausgeben
        if(res == "GAME_OVER"){
            cout << "RANDOM_SIMPLE;" << shots << endl;
            return {shots, hits};
        }
    }
}

/*
    STRATEGIE 3: RANDOM_MEMORY (Zufall mit Gedächtnis)
    -------------------------------------------------
    Idee:
    - Wir wählen zufällig Koordinaten (x,y).
    - Wir merken uns aber in einem 2D-Array used[x][y], ob wir dort schon waren.
    - Wenn used[x][y] schon true ist, wird neu gelost.
    Vorteil:
    - Keine doppelten Koordinaten => effizienter als Random ohne Memory.
    Nachteil:
    - Etwas mehr Code (Array + Kontrolle).
*/
Result playRandomMemory(TCPclient& c){


    int shots = 0;
    int hits  = 0;

    // used speichert, ob wir auf (x,y) schon geschossen haben.
    // Wir nutzen Indizes 1..10. 0 bleibt unbenutzt, daher Größe 11.
    bool used[11][11] = {false};

    c.sendData("INIT");
    c.receive(32);

    while(true){
        int x = rand() % 10 + 1;
        int y = rand() % 10 + 1;

        // Falls dieses Feld schon benutzt wurde, überspringen und neu wählen
        if(used[x][y]) continue;

        // Feld als benutzt markieren
        used[x][y] = true;

        string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
        c.sendData(req);
        shots++;

        string res = c.receive(32);
        if(isHit(res)) hits++;

        if(res == "GAME_OVER"){
            cout << "RANDOM_MEMORY;" << shots << endl;
            return {shots, hits};
        }
    }
}

/*
    STRATEGIE 4: CHECKERBOARD (Schachbrett)
    --------------------------------------
    Idee:
    - Schiffe haben eine Länge >= 2, daher reicht es oft, nur jedes zweite Feld zu testen.
    - Auf einem Schachbrettmuster: (x+y) % 2 == 0.
    - Falls das Spiel danach noch nicht vorbei ist, testen wir die restlichen Felder.
    Vorteil:
    - Häufig effizienter als Scan, weil man in Phase 1 nur 50 Felder prüft.
    Nachteil:
    - Ohne "Hunt/Target" nutzt man Treffer nicht optimal, aber immer noch ok.
*/
Result playCheckerboard(TCPclient& c){


    int shots = 0;
    int hits  = 0;

    c.sendData("INIT");
    c.receive(32);

    // Phase 1: nur Felder mit (x+y)%2==0
    for(int y=1; y<=10; y++){
        for(int x=1; x<=10; x++){
            if( (x+y) % 2 != 0 ) continue;

            string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
            c.sendData(req);
            shots++;

            string res = c.receive(32);
            if(isHit(res)) hits++;

            if(res == "GAME_OVER"){
                cout << "CHECKERBOARD;" << shots << endl;
                return {shots, hits};
            }
        }
    }

    // Phase 2: restliche Felder (x+y)%2==1
    for(int y=1; y<=10; y++){
        for(int x=1; x<=10; x++){
            if( (x+y) % 2 == 0 ) continue;

            string req = "COORD(" + to_string(x) + "," + to_string(y) + ")";
            c.sendData(req);
            shots++;

            string res = c.receive(32);
            if(isHit(res)) hits++;

            if(res == "GAME_OVER"){
                cout << "CHECKERBOARD;" << shots << endl;
                return {shots, hits};
            }
        }
    }

    // Fallback
    cout << "CHECKERBOARD;" << shots << endl;
    return {shots, hits};
}

/*
    MAIN:
    -----
    - Liest N als Kommandozeilenparameter.
      Beispiel: ./client 1000 > data.txt
    - Gibt eine Header-Zeile aus: strategie;shots
    - Führt jede Strategie N-mal aus (erst Strategie 1 N-mal, dann Strategie 2 N-mal, ...).
    - Jede Runde schreibt genau eine Zeile in stdout.
*/
int main(int argc, char* argv[]){

    TCPclient c;
    c.conn("localhost", 2023);

    // Zufallszahlengenerator initialisieren (für Random-Strategien)
    srand(time(NULL));

    // Standardwert, falls kein Argument übergeben wird
    int N = 1;

    // Wenn ein Argument da ist, als Zahl übernehmen
    if(argc >= 2){
        N = atoi(argv[1]);
        if(N < 1) N = 1;
    }

    // Header für Excel (Semikolon als Trennzeichen)
    cout << "strategie;shots" << endl;

    // Strategie 1: Scan N-mal
    for(int i=0; i<N; i++){
        playScan(c);
    }

    // Strategie 2: Random ohne Memory N-mal
    for(int i=0; i<N; i++){
        playRandomSimple(c);
    }

    // Strategie 3: Random mit Memory N-mal
    for(int i=0; i<N; i++){
        playRandomMemory(c);
    }

    // Strategie 4: Schachbrett N-mal
    for(int i=0; i<N; i++){
        playCheckerboard(c);
    }

    return 0;
}
