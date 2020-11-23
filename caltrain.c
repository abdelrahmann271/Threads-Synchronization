#include <pthread.h>
#include "caltrain.h"
#include <stdio.h>
#include <unistd.h>
void
station_init(struct station *station)
{
	station->passengersWaiting=0;
	station->passengersToBoard=0;
	station->count=0;
	pthread_mutex_init(&station->pc_mutex, NULL);
	pthread_cond_init(&station->trainFull, NULL);
	pthread_cond_init(&station->trainAvailable, NULL);
}

void
station_load_train(struct station *station, int count)
{
	//Train must leave if there is no passengers or seats.
/*	if(count<=0 || station->passengersWaiting==0){
		return;
	}*/
	pthread_mutex_lock(&station->pc_mutex);
	//CS-Begins
	station->count=count;
		//Signal all waiting Threads of passengers who found no train.

	while( (station->passengersWaiting >0 && station->count>0) || station->passengersToBoard>0 ){
		pthread_cond_broadcast(&station->trainAvailable);
		pthread_cond_wait(&station->trainFull,&station->pc_mutex);
	}
	station->count=0;
	//CS-Ends
	pthread_mutex_unlock(&station->pc_mutex);
	return;
}

void
station_wait_for_train(struct station *station)
{//For a Single Passenger
	//printf("\npassenger gah!\n");
	pthread_mutex_lock(&station->pc_mutex);
	//CS-Begins
	station->passengersWaiting++;
		//Wait for a train with available seat for this passenger.
	while( station->passengersToBoard == station->count ){
		pthread_cond_wait(&station->trainAvailable, &station->pc_mutex);
	}
	station->passengersToBoard++;
	station->passengersWaiting--;
	//CS-Ends
	pthread_mutex_unlock(&station->pc_mutex);
	return;
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->pc_mutex);
	//CS begins.
	station->count--;
	station->passengersToBoard--;
	if(station->passengersToBoard == 0 || station->count ==0 ){
		pthread_cond_signal(&station->trainFull);
	}
	//CS ends.
	pthread_mutex_unlock(&station->pc_mutex);
	return;
}
