#include "tete.h"


// 
int main (int argc, char **argv){
	int nombreDetours;
	mpz_t sum1,sum0,ch,maj,tmp1,tmp2,sig1,sig0;
	mpz_inits(sum1,sum0,ch,maj,tmp1,tmp2,sig1,sig0,NULL);

	mpz_inits(a,b,c,d,e,f,g,h,NULL);

	char * chaine = malloc (32*sizeof(char));
	initialiserHashStructure(valeurH);
	initialiserK_valeurConstante(k_values);
	// test ici (à enlever après)
	if (argc ==2 )
		initialiserBlockData(argv[1]);
	else
		initialiserBlockData("Groupe14");
	//initialiserBlockData("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	
	nombreDetours  = strlen(chaineAtraiter)/512;

	for (int j = 0 ; j < nombreDetours; j++ ){
		copieConstante();
		for (int i = 0; i < 16; i++)
			recuppererBlockPartie(chaineAtraiter,i+(j*16),blockData[i]); //  512 Taille d'un bloc
		for (int i = 16; i < 64; i++){
			sigmaUn(sig1,blockData[i-2]);
			sigmaZero(sig0,blockData[i-15]);
			
			mpz_add(blockData[i],sig1,sig0);
			mpz_add(blockData[i],blockData[i],blockData[i-7]);
			mpz_add(blockData[i],blockData[i],blockData[i-16]);
			if (mpz_sizeinbase(blockData[i],16) > 8){
				chaine = paddingChaine(mpz_get_str(chaine,16,blockData[i]),8);  //!!!   penser toujours à mettre en padding 32 bits 1 jour 
				mpz_set_str(blockData[i],chaine,16);
			}
			
		}

		for (int i = 0; i < 64; i++){
			// t1 = h + Σ 1 (e) + Ch(e, f, g) + K i + W i
			fonction_sommeUn(sum1,valeurH[4]);
			fonction_ch(ch,valeurH[4],valeurH[5],valeurH[6]);
			mpz_add(tmp1,valeurH[7],sum1);
			mpz_add(tmp1,tmp1,ch);
			mpz_add(tmp1,tmp1,k_values[i+1]);
			mpz_add(tmp1,tmp1,blockData[i]);
			//***t2
			//t
			fonction_sommeZero(sum0,valeurH[0]);
			fonction_Maj(maj,valeurH[0],valeurH[1],valeurH[2]);
			mpz_add(tmp2,sum0,maj);
			mpz_add(tmp2,tmp2,tmp1);
			// nouvelles valeurs
			mpz_set(valeurH[7] , valeurH[6]);
			mpz_set(valeurH[6] , valeurH[5]);
			mpz_set(valeurH[5] , valeurH[4]);
			
			mpz_add(tmp1,tmp1,valeurH[3]);
			// padding 16
			if (mpz_sizeinbase(tmp1,16) > 8) //!!!   penser toujours à mettre en padding 32 bits 1 jour 
				mpz_set_str(tmp1,paddingChaine(mpz_get_str(chaine,16,tmp1),8),16);

			mpz_set(valeurH[4],tmp1);
			mpz_set(valeurH[3] , valeurH[2]);
			mpz_set(valeurH[2] , valeurH[1]);
			mpz_set(valeurH[1] , valeurH[0]);
			if (mpz_sizeinbase(tmp2,16) > 8)
				mpz_set_str(tmp2,paddingChaine(mpz_get_str(chaine,16,tmp2),8),16);
			
			mpz_set(valeurH[0],tmp2);

		}
		/*for (int i = 0; i < 8; i++)
				gmp_printf("%s   ", mpz_get_str(chaine,16,valeurH[i]));
		printf("\n");*/

		nouvelleValeur();

		/*printf("après\n");
		for (int i = 0; i < 8; i++)
				gmp_printf("%s   ", mpz_get_str(chaine,16,valeurH[i]));
		printf("****\n");*/

	}
	//exit(0);
	printf( "sha2 256 **\n");
	for (int i = 0; i < 8; i++){
		gmp_printf("%s   ", mpz_get_str(chaine,16,valeurH[i]));
	}

	return 0;
}
