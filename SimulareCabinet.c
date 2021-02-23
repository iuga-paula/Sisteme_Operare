#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <semaphore.h>

#define NR_MEDICI 5
#define NR_PACIENTI 10
#define TIMP_LIMITA 60//minute
#define ORA_SOSIRE_MAXIMA 960// 16*60 minute

int contor = 0;
pthread_mutex_t mtx;
FILE *fp;

struct Doctor{

int id;
int timp_ocupat;//pt prioritizarea algoritmului


};

/*bool sortDoctors(struct Doctor a, struct Doctor b)
{
	return (a.timp_ocupat<b.timp_ocupat);//iau doctorul cu timpul ocupat cel mai mic
}*/

struct argv_pacient{
int  nr_ordine;
int durata_consultatie;
int sosire;
};

int timp;
struct Doctor doctori[NR_MEDICI];///array de medici



void swap(struct Doctor *A, struct Doctor *B)
{
	struct Doctor aux  = *A;
	*A = *B;
	*B = aux;
}



void *asteapta_la_medic(void * v)
{
struct argv_pacient *args = (struct argv_struct *)v;//iau argumntele threadului
int nr_ordine, sosire, timp_asteptat;

nr_ordine = args->nr_ordine;
sosire = args->sosire;
timp_asteptat = 0;

pthread_mutex_lock(&mtx);


struct Doctor d; //sectiune critica patrunde doar un thread
//vad  la care doctor poate  sa intre threadul => cel care are cel mai mic timp ocupat;
//qsort(doctori, doctori + NR_MEDICI, sizeof(doctori),  sortDoctors);
for(int i = 1; i < NR_MEDICI - 1; i++)//sortex dupa timp doctorii
{
	for(int j = 0; j< NR_MEDICI - i; j++)
	{
		if(doctori[j].timp_ocupat >  doctori[j+1].timp_ocupat)
		{
			d = doctori[j];
			doctori[j] = doctori[j+1];
			doctori[j+1] = d;

		//	swap(&doctori[i], &doctori[j]);
		}
	}
}

for(int i = 0; i < NR_MEDICI; i++)
{
          fprintf(fp,"%d ", doctori[i].timp_ocupat);
	  printf("%d ", doctori[i].timp_ocupat); 

}

if(sosire/60  <= doctori[0].timp_ocupat)
{


	timp_asteptat = doctori[0].timp_ocupat - sosire/60;
}
else
{
		timp_asteptat =  doctori[0].timp_ocupat;

}


doctori[0].timp_ocupat += timp_asteptat + args->durata_consultatie; 


printf("Pacientul %d soseste la %d, asteapta %d minute si intra la medicul %d, timp de consulatie: %d \n", nr_ordine, sosire/60,  timp_asteptat, doctori[0].id, args->durata_consultatie);
fprintf(fp,"Pacientul %d soseste la %d, asteapta %d minute si intra la medicul %d, timp de consulatie: %d \n", nr_ordine, sosire/60,  timp_asteptat, doctori[0].id, args->durata_consultatie);
pthread_mutex_unlock(&mtx);


}


int main()
{
fp = fopen("coadaCabinet.txt", "w+");

rewind(fp);

if(fp<0)
{
perror("Nu s-a gasit fisierul de  scriere");
return errno;
}

//fputs("Test", fp);

if(pthread_mutex_init(&mtx, NULL)) //creez mutex - ul ca sa blochez o resursa de tip doctor;
{

	perror("Eroare la creerea mutextului");
	return errno;

}



pthread_t pacienti[NR_PACIENTI];//threaduri pacienti

for(int i = 0; i < NR_MEDICI; i++)
{
	doctori[i].id = contor;
	contor++;
	doctori[i].timp_ocupat = 0;
}

srand(time(0));

for(int i = 0; i < NR_PACIENTI; i++)//pornesc pacientii simultan
{
	struct argv_pacient *argv =(struct argv_pacient*)malloc(sizeof(struct argv_pacient));

	argv->durata_consultatie = rand()%TIMP_LIMITA;
	argv->nr_ordine = i;

	if(i != 0)//daca nu e primul pacient, pacientul anterior ar un  timp de sosire mai mare decat pacientul anterior
	{
		argv->sosire = (rand()% (ORA_SOSIRE_MAXIMA - timp + 1)) + timp;

		timp = argv->sosire;
	}
	else
	{
		timp = 0;
		argv->sosire = 0;
	}

        if(pthread_create(&pacienti[i], NULL, asteapta_la_medic, argv))
	{
		perror("Eroare la creerea unui pacient");
		return errno;
	}

	sleep(1);
}


void *rezultat;

for(int i = 0; i < NR_PACIENTI; i++)
{

	if(pthread_join(pacienti[i], &rezultat)) //astept terminarea consultatiei pt fiecare pacient
	{
		perror(NULL);
		return errno;
	}
}


fclose(fp);

	return 0;
}



