
#include <iostream>
#include <cstdbool>
using namespace std;

typedef struct game{
	int xpos;
	int ypos;
	char* yard;
	int yardSize;
	bool holding;
} game;

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


int main() {
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
				gameState.xpos=j;
			}
		}
	}

	cout << calculateScore(yardsize,totalAcorns,initialPiles,0) << endl << countPiles(yardsize,yard[0])<<endl;


	return 0;
}
