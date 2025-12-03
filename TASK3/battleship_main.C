#include <iostream>
#include <ctime>
#include <cstdlib>

#include "TASK3.H"

using namespace std;

int main() {

TASK3::World w;
TASK3::ShootResult res;
int shots = 0;

for (y = 1; y <= 10; y++){
   for (x = 1; x <=10; x++){
	res = w.shoot(x, y);
	shots++;

	cout <<"shot"<< shots << ": (" << x <<";" << y << ") ->" <<res<<endl;

	if (res == TASK3::GAME_OVER){
	cout << "game over"
