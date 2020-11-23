#include <pthread.h>

struct station {
	// FILL ME IN
	int passengersWaiting;
	int count;
	int passengersToBoard;
	pthread_mutex_t pc_mutex;
	pthread_cond_t trainAvailable, trainFull;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);