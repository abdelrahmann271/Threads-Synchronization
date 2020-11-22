#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	// FILL ME IN
	station->passengers=0;
	pthread_mutex_init(&station->pc_mutex, NULL);
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN

	pthread_mutex_lock(&station->pc_mutex);
	//CS
	while( (station->passengers) < count ) {
		pthread_cond_wait(&station->cond_availSpace, &station->pc_mutex);
	}
	//Decrease count and passengers and make the train leaves.
	pthread_mutex_unlock(&station->pc_mutex);
}

void
station_wait_for_train(struct station *station)
{//For a Single Passenger

	pthread_mutex_lock(&station->pc_mutex);
	//CS
	station->passengers++;
/*	while (){

	}*/
	pthread_mutex_unlock(&station->pc_mutex);
	// FILL ME IN
	station_on_board(station);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN

}
