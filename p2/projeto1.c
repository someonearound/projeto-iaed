/*
 * projeto1.c
 *
 * Rui Amaral - ist1103155
 *
 * File containing main() and other primary functions for the program.
 * 
 */

#include "projeto1.h"
#include <stdio.h>
#include <string.h>


int main(){
	while (command_listener(getchar())) {}
	return 0;
}

/*
 * Listens for commands entered by the user until it
 * receives the 'q' command.
 */
int command_listener(char command) {
	switch (command) {
		case 'q':
			return 0;
		case 'a':
			exec_add_airport();
			return 1;
		case 'l':
			exec_list_airports();
			return 1;
		case 'v':
			exec_add_flight();
			return 1;
		case 'p':
			exec_list_departing_flts();
			return 1;
		case 'c':
			exec_list_arriving_flts();
			return 1;
		case 't':
			exec_set_time();
			return 1;
	}
	return 1;
}

/*
 * Handles the 'a' command.
 * Adds a new airport to the system.
 */
void exec_add_airport() {
	char id[AP_ID_LENGTH], country[AP_COUNTRY_LENGTH], city[AP_CITY_LENGTH];

	scanf("%s %s %[^\n]", id, country, city);

	if (invalid_ap_args(id)) {
		return;
	}
	add_airport(id, country, city);
	printf("airport %s\n", id);
}

/*
 * Handles the 'l' command.
 * Fetches and lists info about given airports.
 * With no arguments prints info of every airport in the system.
 */
void exec_list_airports() {
	char c = getchar(), id[AP_IDLENGTH];

	if (c == ' ') {
		while (c != '\n' && c != EOF) {
			fgets(id, AP_ID_LENGTH, stdin);

			if (!is_airport(id)) {
				printf("%s: no such airport ID\n", id);	
			} else {
				print_ap_info(find_airport(id));
			}
			c = getchar();
		}
	} else {
		sort_airports();
		list_airports();
	}
}

/*
 * Handles the 'v' command.
 * Adds a flight to the system, or lists every flight
 * if no arguments are given.
 */
void exec_add_flight() {
	char c = getchar();
	flight new_f;
	timestamp dep_dt;

	if (c == ' ') { /* checks if there are more arguments */
		scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d",
			new_f.code, new_f.origin, new_f.destin, &dep_dt.d,
			&dep_dt.mth, &dep_dt.y, &dep_dt.h, &dep_dt.min,
			&new_f.dura.h, &new_f.dura.min, &new_f.cap);

		if (invalid_flt_args(new_f, dep_dt)) {
			return;
		}
		add_flight(new_f, dep_dt);
	} else {
		list_flights();
	}
}

/*
 * Handles the 'p' command.
 * Lists every flight departing from a given airport sorted
 * from oldest to most recent.
 */
void exec_list_departing_flts() {
	char id[AP_ID_LENGTH];
	scanf("%s", id);

	if (!is_airport(id)) {
		printf("%s: no such airport ID\n", id);	
		return;
	}
	get_flts_departing(id);
	sort_flights();
	list_departing_flights();
}

/*
 * Handles the 'c' command.
 * Lists every flight arriving at a given airport, sorted
 * from oldest to most recent.
 */
void exec_list_arriving_flts() {
	char id[AP_ID_LENGTH];
	scanf("%s", id);

	if (!is_airport(id)) {
		printf("%s: no such airport ID\n", id);
		return;
	}
	get_flts_arriving(id);
	sort_flights();
	list_arriving_flights();
}

/*
 * Handles the 't' command.
 * Updates the system date.
 */
void exec_set_time() {
	timestamp new_date;

	scanf("%d-%d-%d", &new_date.d, &new_date.mth, &new_date.y);

	new_date.h = 0;  /* initialize hour and min */
	new_date.min = 0;

	if (invalid_date(new_date)) {
		printf("invalid date\n");
		return;
	}
	global_date = new_date;
	printf("%02d-%02d-%d\n", global_date.d, global_date.mth, global_date.y);
}

void exec_add_reservation(info global_info) {
	char flt_code[FLIGHT_CODE_LENGTH], res_code[MAX_CMD_LEN];
	int day, month, year, pass_n, error;

	scanf("%s %d-%d-%d", flt_code, day, month, year);
	if (getchar() == ' ') {
		error = list_reservations(flt_code, day, month, year);
	} else {
		scanf("%s%ld", res_code, pass_n);
		error = add_reservation(flt_code, day, month, year, res_code, pass_n);
	}
	if (error) {
		print_res_error(error, flt_code, res_code);
	}
}

/*
 * Auxiliary function that receives a string and returns 1 if
 * all characters in the string are part of the uppercase alphabet.
 * Returns 0 otherwise.
 */
int isupper_str(char str[]) {
	char c;
	int i, len = strlen(str);

	for (i = 0; i < len; i++) {
		c = str[i];
		if (c < 'A' || c > 'Z') {
	  		return FALSE;
		}
	}
	return TRUE;
}

void *my_alloc(unsigned size) {
	void *ptr = malloc(size);
	if (!ptr) {
		printf("No memory.");
		exit(1);
	}
	return ptr;
}