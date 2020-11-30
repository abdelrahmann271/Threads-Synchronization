#include <pthread.h>
#include "caltrain.h"
#include <stdio.h>
#include <unistd.h>
void
station_init(struct station *station)
{
	//Number of passengers waiting for a train.
	station->passengersWaiting=0;
	//Number of passengers who found a train but not yet have boarded.
	station->passengersToBoard=0;
	//Number of Seats Available for the current train at the station.
	station->count=0;
	//Mutex for control sections where we change the station variables.
	pthread_mutex_init(&station->pc_mutex, NULL);
	//Condition that the train is full.
	pthread_cond_init(&station->trainFull, NULL);
	//The condition that there is a train Available.
	pthread_cond_init(&station->trainAvailable, NULL);
}

void
station_load_train(struct station *station, int count)
{
	pthread_mutex_lock(&station->pc_mutex);
	//CS-Begins
	//Updating the available seats at the station.
	station->count=count;
	while( (station->passengersWaiting >0 && station->count>0) || station->passengersToBoard>0 ){
		//Signal all waiting Threads of passengers who found no train.
		pthread_cond_broadcast(&station->trainAvailable);
		//The train must wait until it's full and all passengers have boarded.
		pthread_cond_wait(&station->trainFull,&station->pc_mutex);
	}
	//The train leaves the station , so update seats count with 0.
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
	//Increment Number of passengers waiting.
	station->passengersWaiting++;
	while( station->passengersToBoard == station->count ){
		//Wait for a train with available seat for this passenger.
		pthread_cond_wait(&station->trainAvailable, &station->pc_mutex);
	}
	//The passenger now have stopped waiting and "On-board" function should be called.
	station->passengersToBoard++;
	//Decrement No. of waiting passengers.
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
	//A passenger has boarded and a seat is not available now.
	station->count--;
	station->passengersToBoard--;
	//Signal the train to left under the conditions:
	// 1- All waiting passengers have boarded , so there is no waiting passengers and the train must leave.
	if(station->passengersToBoard == 0 ){
		pthread_cond_signal(&station->trainFull);
	}
	//CS ends.
	pthread_mutex_unlock(&station->pc_mutex);
	return;
}
