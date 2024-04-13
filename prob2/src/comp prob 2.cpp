
#include <iostream>
using namespace std;

double calculateScore(int yardSize,int acornCount, int pileCount,int opperationCount){
	int numerator = 2 * acornCount * yardSize * yardSize* yardSize;
	int denominator = 3 * pileCount;
	double result = (double)numerator / denominator - opperationCount;
	return result;
}

void discoverPile(char yard[],int size,int i,int j){
	//set the current space to not pile
	yard[i*size+j] = '.';
	//check to space above
	if(i-1 >0 && yard[(i-1)*size+j] >= '1' && yard[(i-1)*size+j] <= '9'){
		discoverPile(yard,size,i-1,j);
	}
	
	//check the space to the left
	if(j-1 >0 && yard[i*size+j-1] >= '1' && yard[(i-1)*size+j] <= '9'){
		discoverPile(yard,size,i,j-1);
	}
	//check the space to the right
	if(i+1 <size && yard[(i+1)*size+j] >= '1' && yard[(i-1)*size+j] <= '9'){
		discoverPile(yard,size,i+1,j);
	}
	//check the space down
	if(j+1 <size && yard[i*size+j+1] >= '1' && yard[(i-1)*size+j] <= '9'){
		discoverPile(yard,size,i,j+1);
	}

}

int countPiles(int yardSize, char yard[]){
	char yardCopy[yardSize][yardSize];
	//copy the yard
	for(int i=0;i<yardSize;i++){
		for(int j=0;j<yardSize;j++){
			yardCopy[i][j] = yard[i*yardSize+j];
			discoverPile(yardCopy[0],yardSize,i,j);
		}
	}
	int piles =0;
	//loop through the yard to find piles
	for(int i=0;i<yardSize;i++){
		for(int j=0;j<yardSize;j++){
			//if the current selection is a pile
			if(yardCopy[i][j]>='1' && yardCopy[i][j]<='9'){
				piles++;
			}
		}
	}

	return piles;
}


int main() {
	int yardsize,totalAcorns,initialPiles;
	//get the inital data
	cin >> yardsize >> totalAcorns >> initialPiles;
	char yard[yardsize][yardsize];
	int xpos,ypos;

	//read in the initial yard;
	for(int i=0;i<yardsize;i++){
		for(int j=0;j<yardsize;j++){
			cin >> yard[i][j];
			if(yard[i][j]=='@'){
				xpos=i;
				ypos=j;
			}
		}
	}

	cout << calculateScore(yardsize,totalAcorns,initialPiles,0) << " " << countPiles(yardsize,yard[0])<<endl;
	return 0;
}