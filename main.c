#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void print_sensor(sensor *Senzori, int index)
{
	if (Senzori[index].sensor_type == TIRE) {
		tire_sensor *tire_senzor = Senzori[index].sensor_data;
		/* am folosit o variabila auxiliara pentru a respecta
		conditiile impuse de coding style */
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", tire_senzor->pressure);
		printf("Temperature: %.2f\n", tire_senzor->temperature);
		printf("Wear Level: %d%%\n",  tire_senzor->wear_level);
		if (tire_senzor->performace_score == 0) {
			printf("Performance Score: Not Calculated\n");
		} else {
			printf("Performance Score: %d\n",  tire_senzor->performace_score);
		}
	} else if (Senzori[index].sensor_type == PMU) {
		power_management_unit *pmu_senzor = Senzori[index].sensor_data;
		/* am folosit o variabila auxiliara pentru a respecta
		conditiile impuse de coding style */
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", pmu_senzor->voltage);
		printf("Current: %.2f\n", pmu_senzor->current);
		printf("Power Consumption: %.2f\n", pmu_senzor->power_consumption);
		printf("Energy Regen: %d%%\n", pmu_senzor->energy_regen);
		printf("Energy Storage: %d%%\n", pmu_senzor->energy_storage);
	}
}  // functie care printeaza senzorul de la pozitia index in formatul dorit
void get_operations(void **operations);  // antetul functiei din operations.c

void analyze_sensor(sensor *Senzori, int idx, void (*operations[])(void *))
{
	for (int i = 0; i < Senzori[idx].nr_operations; i++) {
	(*operations[Senzori[idx].operations_idxs[i]])(Senzori[idx].sensor_data);
	}
}  // functie care executa toate operatiile senzorului dat ca argument

void free_sensor(sensor *Senzori, int index)
{
	if (Senzori[index].operations_idxs) {
		free(Senzori[index].operations_idxs);
	}
	if (Senzori[index].sensor_data) {
		free(Senzori[index].sensor_data);
	}
}  // functie care elibereaza memoria pentru un senzor

void free_sensors(sensor *Senzori, int nr_sensors)
{
	for (int i = 0; i < nr_sensors; i++) {
		if (Senzori[i].operations_idxs) {
			free(Senzori[i].operations_idxs);
		}
		if (Senzori[i].sensor_data) {
			free(Senzori[i].sensor_data);
		}
	}
	free(Senzori);
}  // functie care elibereaza memoria pentru toti senzorii

int clear_sensors(sensor *Senzori, int nr_sensors)
{
	for (int i = 0; i < nr_sensors; i++) {
		// daca senzorul nu respecta valorile normale este eliminat
		// se elibereaza memoria pentru datele lui
		if (Senzori[i].sensor_type == TIRE) {
			tire_sensor *tire_senzor = Senzori[i].sensor_data;
			/* am folosit o variabila auxiliara pentru a respecta
			conditiile impuse de coding style */
			if (tire_senzor->pressure < 19 || tire_senzor->pressure > 28) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			} else if (tire_senzor->temperature < 0 ||
				tire_senzor->temperature > 120) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			} else if (tire_senzor->wear_level < 0 ||
				tire_senzor->wear_level > 100) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			}
		} else if (Senzori[i].sensor_type == PMU) {
			power_management_unit *pmu_senzor = Senzori[i].sensor_data;
			/* am folosit o variabila auxiliara pentru a respecta
			conditiile impuse de coding style */
			if (pmu_senzor->voltage < 10 || pmu_senzor->voltage > 20) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			} else if (pmu_senzor->current < -100 ||
				pmu_senzor->current > 100) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			} else if (pmu_senzor->power_consumption < 0 ||
				pmu_senzor->power_consumption > 1000) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			} else if (pmu_senzor->energy_regen < 0 ||
				pmu_senzor->energy_regen > 100) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			} else if (pmu_senzor->energy_storage < 0 ||
				pmu_senzor->energy_storage > 100) {
				free_sensor(Senzori, i);
				for (int j = i; j < nr_sensors-1; j++) {
					Senzori[j] = Senzori[j+1];
				}
				nr_sensors--;
				i--;
			}
		}
	}
	return nr_sensors;
}  /* functie care elimina vectorii care contin valori eronate si returneaza
nr de senzori ramasi pentru a putea realoca si elibera corect memoria la final*/
int sort_sensors(const void *p, const void *q)
{
	int tip_senzor1 = ((sensor *)p)->sensor_type;
	int tip_senzor2 = ((sensor *)q)->sensor_type;
	if (tip_senzor1 == PMU && tip_senzor2 == TIRE)
		return -1;
	if (tip_senzor1 == TIRE && tip_senzor2 == PMU)
		return 1;
	return 0;
}  // functie de sortare in functie de tipul senzorului pentru qsort



int main(int argc, char const *argv[])
{
	FILE *fileptr;
	int nr_sensors;
	if (argc < 2) {
		exit(1);
	}

	fileptr = fopen(argv[1], "rb");
	if (fileptr == NULL) {
		printf("Eroare!\n");
		exit(1);
	}
	fread(&nr_sensors, sizeof(int), 1, fileptr);
	sensor *Senzori = malloc(nr_sensors * sizeof(sensor));
	// s-a alocat memorie pentru vectorul de senzori
	int tip_senzor;
	int nr_operatii;
	for (int i = 0; i < nr_sensors; i++) {
		fread(&tip_senzor, sizeof(int), 1, fileptr);
		if (tip_senzor == 0) {
			tire_sensor *tire = malloc(sizeof(tire_sensor));
			Senzori[i].sensor_type = TIRE;
			fread(&tire->pressure, sizeof(float), 1, fileptr);
			fread(&tire->temperature, sizeof(float), 1, fileptr);
			fread(&tire->wear_level, sizeof(int), 1, fileptr);
			fread(&tire->performace_score, sizeof(int), 1, fileptr);
			Senzori[i].sensor_data = tire;
		} else if (tip_senzor == 1) {
			power_management_unit *pmu = malloc(sizeof(power_management_unit));
			Senzori[i].sensor_type = PMU;
			fread(&pmu->voltage, sizeof(float), 1, fileptr);
			fread(&pmu->current, sizeof(float), 1, fileptr);
			fread(&pmu->power_consumption, sizeof(float), 1, fileptr);
			fread(&pmu->energy_regen, sizeof(int), 1, fileptr);
			fread(&pmu->energy_storage, sizeof(int), 1, fileptr);
			Senzori[i].sensor_data = pmu;
		}
		fread(&nr_operatii, sizeof(int), 1, fileptr);
		Senzori[i].nr_operations = nr_operatii;
		Senzori[i].operations_idxs = malloc(nr_operatii * sizeof(int));
		// s-a alocat memorie pentru vectorul de operatii
		for (int j = 0; j < Senzori[i].nr_operations; j++) {
			fread(&Senzori[i].operations_idxs[j], sizeof(int), 1, fileptr);
		}
	}  // s-a format vectorul de senzori cu datele citite din fisierul binar
	qsort(Senzori, nr_sensors, sizeof(sensor), sort_sensors);
	// s-a ordonat vectorul dupa prioritatea tipului de senzor
	void(*operations[8])(void *);
	// se foloseste un vector de pointeri la functii pentru a le apela
	get_operations((void **)operations);
	// se initializeaza pointerii la operatii cu functiile date
	char command[50];
	int index;
	int new_nr = 0;
	/* se foloseste aceasta variabila pentru a retine nr de senzori
	dupa apelarea functiei de clear*/
	scanf("%s", command);
	while (strncmp(command, "exit", 5) != 0) {
		if (strncmp(command, "print", 5) == 0) {
			scanf("%d", &index);
			if (index < 0 || index > nr_sensors) {
				printf("Index not in range!\n");
			} else {
				print_sensor(Senzori, index);
			}
		}
		if (strncmp(command, "analyze", 7) == 0) {
			scanf("%d", &index);
			if (index < 0 || index > nr_sensors) {
				printf("Index not in range!\n");
			} else {
				analyze_sensor(Senzori, index, operations);
			}
		}
		if (strncmp(command, "clear", 5) == 0) {
			new_nr = clear_sensors(Senzori, nr_sensors);
			Senzori = realloc(Senzori, new_nr * sizeof(sensor));
			// nr de senzori ramasi dupa eliminarea celor cu valori eronate
		}
		scanf("%s", command);
	}
	if (new_nr == 0) {
		new_nr = nr_sensors;
	}
	/* daca nu s-a apelat functia de clear,
	numarul de senzori este cel initial*/
	if (strncmp(command, "exit", 5) == 0) {
		free_sensors(Senzori, new_nr);
		fclose(fileptr);
		return 0;
	}
}
