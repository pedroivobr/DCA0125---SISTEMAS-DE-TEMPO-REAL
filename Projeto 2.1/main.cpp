#include <iostream>
#include "PWM/PWM.h"
#include "ADC/Adc.h"
#include "unistd.h"
#include "BlackGPIO/BlackGPIO.h"
#include <pthread.h>
#include <stdlib.h>

using namespace BlackLib;

//variaveis
ADC adc1(AINx::AIN5);
ADC adc2(AINx::AIN4);
ADC adc3(AINx::AIN3);
pthread_mutex_t m1; //mutex
pthread_mutex_t m2; //mutex
float valor_adc1 = 0.0;
float valor_adc2 = 0.0;
float valor_adc3 = 0.0;
float tempo1 = 0.0;
float tempo2 = 0.0;
float tempo3 = 0.0;

//saidas
BlackGPIO l1(GPIO_66,output); 
BlackGPIO l2(GPIO_67,output);
BlackGPIO l3(GPIO_69,output);
BlackGPIO l4(GPIO_68,output);
BlackGPIO l5(GPIO_26,output);
BlackGPIO l6(GPIO_47,output);
BlackGPIO l7(GPIO_46,output);
BlackGPIO l8(GPIO_27,output);

//funcoes
void L(int trilho);
void *trem_1_function(void *arg);
void *trem_2_function(void *arg);
void *trem_3_function(void *arg);
void *ler_ADCs(void *arg);

int main(int argc, char * argv[]){
	int res;
	pthread_t trem_1,trem_2,trem_3,adcs;
	void *thread_result;

	res = pthread_create(&adcs, NULL,*ler_ADCs,NULL);
	if(res != 0){
		perror("Iniciação da thread ler_adcs falhou.\n");
		exit(EXIT_FAILURE);
	}
	res = pthread_create(&trem_1, NULL,*trem_1_function,NULL);
	if(res != 0){
		perror("Iniciação da thread trem_1 falhou.\n");
		exit(EXIT_FAILURE);
	}
	res = pthread_create(&trem_2, NULL,*trem_2_function,NULL);
	if(res != 0){
		perror("Iniciação da thread trem_2 falhou.\n");
		exit(EXIT_FAILURE);
	}
	res = pthread_create(&trem_3, NULL,*trem_3_function,NULL);
	if(res != 0){
		perror("Iniciação da thread trem_3 falhou.\n");
		exit(EXIT_FAILURE);
	}

	res = pthread_join(adcs, &thread_result);
	if(res != 0){
		perror("Erro no join da thread adcs\n");
	}
	res = pthread_join(trem_1, &thread_result);
	if(res != 0){
		perror("Erro no join da thread trem_1\n");
	}
	res = pthread_join(trem_2, &thread_result);
	if(res != 0){
		perror("Erro no join da thread trem_2\n");
	}
	res = pthread_join(trem_2, &thread_result);
	if(res != 0){
		perror("Erro no join da thread trem_3\n");
	}

	return 0;

}

void *ler_ADCs(void *arg){
	while(1){
	valor_adc1  = adc1.getPercentValue();
	sleep(0.1);
	valor_adc2  = adc2.getPercentValue();
	sleep(0.1);
	valor_adc3  = adc3.getPercentValue();
	sleep(0.1);
	tempo1 = valor_adc1/10.0;
	tempo2 = valor_adc2/10.0;
	tempo3 = valor_adc3/10.0;
	}
}

void *trem_1_function(void *arg){
	while(1){
		L(1);sleep(tempo1);
		pthread_mutex_lock(&m1);
		L(2);sleep(tempo1);
		pthread_mutex_unlock(&m1);
	}
}


void *trem_2_function(void *arg){
	while(1){
		L(4);sleep(tempo2);
		pthread_mutex_lock(&m1);
		L(3);sleep(tempo2);
		pthread_mutex_unlock(&m1);
		L(5);sleep(tempo2);
		pthread_mutex_lock(&m2);
		L(6);sleep(tempo2);
		pthread_mutex_unlock(&m2);
	}
}


void *trem_3_function(void *arg){
	while(1){
		L(8);sleep(tempo3);
		pthread_mutex_lock(&m2);
		L(7);sleep(tempo3);
		pthread_mutex_unlock(&m2);
	}
}

void L(int trilho){
	if(trilho == 1){
		printf("Trem 1 - Trilho 1\n");
		l1.setValue(high); 
		l2.setValue(low); 
	}else if(trilho == 2){
		printf("Trem 1 - Trilho 2\n");
		l2.setValue(high); 
		l1.setValue(low);
	}else if(trilho == 3){
		printf("Trem 2 - Trilho 3\n");
		l3.setValue(high);
		l4.setValue(low);
	}else if(trilho == 4){
		printf("Trem 2- Trilho 4\n");
		l4.setValue(high);
		l6.setValue(low);
	}else if(trilho == 5){
		printf("Trem 2 - Trilho 5\n");
		l5.setValue(high);
		l3.setValue(low);
	}else if(trilho == 6){
		printf("Trem 2 - Trilho 6\n");
		l6.setValue(high);
		l5.setValue(low);
	}else if(trilho == 7){
		printf("Trem 3 - Trilho 7\n");
		l7.setValue(high);
		l8.setValue(low);
	}else if(trilho == 8){
		printf("Trem 3 - Trilho 8\n");
		l8.setValue(high);
		l7.setValue(low);
	}else{
		printf("Chamada de trilho errada!");
		exit(0);
	}
}
