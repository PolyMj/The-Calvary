
#include <iostream>
#include <cstdbool>
#include <cmath>
using namespace std;

typedef struct game{
	int xpos;
	int ypos;
	char* yard;
	int yardSize;
	bool holding;
} game;

typedef struct postion{
	int x;
	int y;
}position;

double calculateScore(int yardSize,int acornCount, int pileCount,int opperationCount){
	int numerator = 2 * acornCount * yardSize * yardSize* yardSize;
	int denominator = 3 * pileCount;
	double result = (double)numerator / denominator - opperationCount;
	return result;
}

void discoverPile(char yard[],int size,int i,int j,char filler){
	//set the current space to not pile
	yard[i*size+j] = filler;
	//check to space above
	if(i-1 >=0 && yard[(i-1)*size+j] >= '1' && yard[(i-1)*size+j] <= '9'){
		discoverPile(yard,size,i-1,j,filler);
	}
	
	//check the space to the left
	if(j-1 >=0 && yard[i*size+j-1] >= '1' && yard[i*size+j-1] <= '9'){
		discoverPile(yard,size,i,j-1,filler);
	}
	//check the space to the right
	if(i+1 <size && yard[(i+1)*size+j] >= '1' && yard[(i+1)*size+j] <= '9'){
		discoverPile(yard,size,i+1,j,filler);
	}
	//check the space down
	if(j+1 <size && yard[i*size+j+1] >= '1' && yard[(i)*size+j+1] <= '9'){
		discoverPile(yard,size,i,j+1,filler);
	}

}

int countPiles(int yardSize, char yard[]){
	char yardCopy[yardSize][yardSize];
	//copy the yard
	for(int i=0;i<yardSize;i++){
		for(int j=0;j<yardSize;j++){
			yardCopy[i][j] = yard[i*yardSize+j];

		}
	}
	int piles =0;
	//loop through the yard to find piles
	for(int i=0;i<yardSize;i++){
		for(int j=0;j<yardSize;j++){
			//if the current selection is a pile
			if(yardCopy[i][j]>='1' && yardCopy[i][j]<='9'){
				piles++;
				discoverPile(yardCopy[0],yardSize,i,j,'.');
			}
		}
	}

	return piles;
}

void up(game &gameState){
	if(gameState.ypos>0){
		cout << 'N';
		gameState.ypos--;
	}
}

void down(game &gameState){
	if(gameState.ypos<gameState.yardSize-1){
		cout << 'S';
		gameState.ypos++;
	}
}

void left(game &gameState){
	if(gameState.xpos>0){
		cout << 'W';
		gameState.xpos--;
	}
}

void right(game &gameState){
	if(gameState.xpos<gameState.yardSize-1){
		cout << 'E';
		gameState.xpos++;
	}
}

void pickup(game &gameState){
	if(!gameState.holding){
		int i = gameState.xpos,j=gameState.ypos,size = gameState.yardSize;
		cout.flush();
		if(gameState.yard[i*size+j] >='1' && gameState.yard[i*size+j] <='9'){
			cout << 'P';
			int tileCount = gameState.yard[i*size+j]-'0';
			tileCount--;
			if(tileCount==0){
				gameState.yard[i*size+j]='.';
			}else{
				gameState.yard[i*size+j]=(char) '0'+tileCount;
			}
			gameState.holding=true;
		}
	}
}

void goPos(game &gameState, int destx, int desty){
	string inputline = "";
	int x = 0;
	int y = 0;
	x = destx - gameState.xpos;
	y = desty - gameState.ypos;
	if (y<0){
		y=abs(y);
		for (int j =0; j < y;j ++){
			up(gameState);
		}

	}
	else if (y>0){
		for (int j =0; j < y;j ++){
			down(gameState);
		}
	}
	if (x<0){
		x=abs(x);
		for (int i = 0; i < x; i ++){
			left(gameState);
		}

	}
	else if (x>0){
		for (int i = 0; i < x; i ++){
			right(gameState);
		}

	}
	while (gameState.xpos != destx){
		while(gameState.ypos != desty){
			//if need to go down
			if (gameState.ypos < desty){	
				down(gameState);

			}
			//if need to go up
			else if(gameState.ypos > desty){
				
				up(gameState);
			}

			//if need to go right
			if (gameState.xpos < destx){
				right(gameState);

			}
			//if need to go left
			else if(gameState.xpos > destx){
				left(gameState);
			}
		}
	}
	
}

void drop(game &gameState){
	if(gameState.holding){
		int i = gameState.xpos,j=gameState.ypos,size = gameState.yardSize;
		if(gameState.yard[i*size+j] !='9'){
			int tileCount = gameState.yard[i*size+j]-'0';
			tileCount++;
			cout << 'D';
			gameState.yard[i*size+j]=(char) '0'+tileCount;
			gameState.holding=false;
		}
	}
}

void generateCneterPileMask(game gameState,char outputmask[]){
	//copy the yard
	for(int i=0;i<gameState.yardSize;i++){
		for(int j=0;j<gameState.yardSize;j++){
			outputmask[i*gameState.yardSize+j] = gameState.yard[i*gameState.yardSize+j];
		}
	}
	//check if a part of a pile exsists at the center pos
	if(outputmask[gameState.yardSize/2*gameState.yardSize+gameState.yardSize/2] >='1' && outputmask[gameState.yardSize/2*gameState.yardSize+gameState.yardSize/2] <='9' ){
		//mask out the center pile using x
		discoverPile(outputmask,gameState.yardSize,gameState.yardSize/2,gameState.yardSize/2,'x');
	}

}

position findNearst(game gameState,char yard[],bool centerPile){
	position result;
	result.x=gameState.yardSize/2;
	result.y=gameState.yardSize/2;

	double distances[gameState.yardSize][gameState.yardSize];
	//calculate the distance of every possible correct tile from the squirl
	for(int i=0;i<gameState.yardSize;i++){
		for(int j=0;j<gameState.yardSize;j++){
			double dist;
			if(centerPile){
				//if the tile has what we are looking for on it
				if(yard[i*gameState.yardSize+j]=='x'){
					//calculate its distance
					dist = sqrt(pow(gameState.xpos-i,2)+pow(gameState.ypos-j,2));
				}else{
					//else set the distace to large number
					dist = 666666;
				}
			}else{
				//if the tile has what we are looking for on it
				if(yard[i*gameState.yardSize+j]>='1' && yard[i*gameState.yardSize+j]<='9'){
					//calculate its distance
					dist = sqrt(pow(gameState.xpos-i,2)+pow(gameState.ypos-j,2));
				}else{
					//else set the distace to large number
					dist = 666666;
				}
			}
			distances[i][j] = dist;
		}
	}

	double lowst=666666;
	//go through every tile
	for(int i=0;i<gameState.yardSize;i++){
		for(int j=0;j<gameState.yardSize;j++){
			//if the current tile is less distance the the last lowest one
			if(distances[i][j] < lowst){
				//then set the loest dist to this tile
				result.x=i;
				result.y=j;
				lowst=distances[i][j];
			}
		}
	}

	return result;
}


int main() {
	cout << "readdy"<<endl;
	int yardsize,totalAcorns,initialPiles;
	//get the inital data
	cin >> yardsize >> totalAcorns >> initialPiles;
	char yard[yardsize][yardsize];
	game gameState;
	gameState.yard = yard[0];
	gameState.holding=false;
	gameState.yardSize=yardsize;
	//read in the initial yard;
	for(int i=0;i<yardsize;i++){
		for(int j=0;j<yardsize;j++){
			cin >> yard[i][j];
			if(yard[i][j]=='@'){
				gameState.xpos=i;
				gameState.ypos=j;
			}
		}
	}
	cout << "readdy"<<endl;
	//action loop
	//while there are mre then 1 pile
	int pc;
	while((pc=countPiles(yardsize,yard[0]))>1){
		cout << "start "<<pc<<" piles"<<endl;

		position nearestPos;
		//generate center mask
		char centerMask[yardsize*yardsize];
		generateCneterPileMask(gameState,centerMask);
		cout << "generated"<<endl;

		//find nerst non center pile
		nearestPos = findNearst(gameState,centerMask,false);
		cout << "nearst pile pos" << endl << nearestPos.x << " "<<nearestPos.y<<endl;;

		//go to found pile
		goPos(gameState,nearestPos.x,nearestPos.y);
		cout <<endl<< "moved"<<endl;


		//pickup
		pickup(gameState);
		cout <<endl<< "picked up"<<endl;

		//find nearest center pile part
		nearestPos = findNearst(gameState,centerMask,true);
		int dx =  nearestPos.x-gameState.xpos;
		int dy = nearestPos.y-gameState.ypos;
		if(abs(dx) < abs(dy)){
			if(dx>0){
				nearestPos.x++;
			}else{
				nearestPos.x--;
			}
		}else{
			if(dy>0){
				nearestPos.y++;
			}else{
				nearestPos.y--;
			}
		}
		cout << "center pile" << endl << nearestPos.x << " "<<nearestPos.y<<endl;

		//go 1 tile away from the center part

		goPos(gameState,nearestPos.x,nearestPos.y);
		cout << endl<<"moved"<<endl;


		//drop
		drop(gameState);
		cout <<endl<< "dropped"<<endl;

	}


	cout << calculateScore(yardsize,totalAcorns,initialPiles,0) << endl << countPiles(yardsize,yard[0])<<endl<<endl;

	for(int i=0;i<yardsize;i++){
		for(int j=0;j<yardsize;j++){
			cout <<yard[i][j];
		}
		cout << endl;
	}


	return 0;
}
