#include "sha3.h"
//https://keccak.team/keccak_specs_summary.html
// y [0..4] en fonction x[0..4]
int constanteOffsetRotation [5][5] ={{0,1,62,28,27},{36,44,6,55,20},{3,10,43,25,39},{41,45,15,21,8},{18,2,61,56,14}};

etat finalData[64]; // represente S

int main (int argc, char **argv){
	char x,y,z;
	x='0';
	y='0';
	z='1';
	printf("%c\n", ('1'^'0'^'1'^'0')+'0' );
	printf("%c\n", ('0'^'0'^'1'^'0')+'0' );
	printf("%d\n", '0'^'1'^'1'^'1' );
	printf("%d\n", '1'^'1'^'1'^'1' );
	int a = -1;
	printf("%d\n", a % 5);
	for (int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++)
			printf("%d ",constanteOffsetRotation[i][j]);
		printf("\n");
	}
	formaterChaineSha3("OK");

	//keccak();
	//
}