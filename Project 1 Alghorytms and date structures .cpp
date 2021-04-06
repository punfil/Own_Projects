#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cstdlib> //malloc realloc

#pragma warning(disable: 6031) //Ignoring return value of scanf

#define PENDRIVE_COUNT 2
#define ALLOCATION_MULTIPLIER 2 //We make arrays twice as large as they were
#define DEFAULT_PENDRIVE_CAPACITY 4 //MUST BE MORE THAN 0, BECAUSE WE MULTIPLY IT LATER ON

typedef struct file {
	unsigned int number;
	unsigned int size;
}FileType;

typedef struct pendrive {
	unsigned int free_space;
	FileType* files;
	unsigned int max_file_count;
	unsigned int file_count;
}PendriveType;

void swap(FileType& x1, FileType& x2) {
	FileType temp = x1;
	x1 = x2;
	x2 = temp;
}

FileType* LoadFiles(int files_count) {
	FileType* all_files = (FileType*)malloc(files_count * sizeof(FileType));
	if (all_files == NULL) return NULL;
	for (int i = 0; i < files_count; i++) {
		scanf("%u %u\n", &(all_files[i].number), &(all_files[i].size));
	}
	return all_files;
}

void SetUpPendrives(PendriveType** pendrives, unsigned int pendrive_space) {
	for (int i = 0; i < PENDRIVE_COUNT; i++) {
		pendrives[i] = (PendriveType*)malloc(sizeof(PendriveType));
		if (pendrives[i] == NULL) {
			printf("Error allocating memory. :/");
			return;
		}
		pendrives[i]->file_count = 0;
		pendrives[i]->free_space = pendrive_space;
		pendrives[i]->max_file_count = DEFAULT_PENDRIVE_CAPACITY;
		pendrives[i]->files = (FileType*)malloc(DEFAULT_PENDRIVE_CAPACITY * sizeof(FileType));
		if (pendrives[i]->files == NULL) {
			printf("Error allocating memory. :/");
			return;
		}
	}
}

void EnlargePendrive(PendriveType* pendrive) {
	FileType* temp = (FileType*)realloc(pendrive->files, ALLOCATION_MULTIPLIER * pendrive->max_file_count * sizeof(FileType));
	if (temp == NULL) return;
	pendrive->files = temp;
	pendrive->max_file_count *= ALLOCATION_MULTIPLIER;
}

int PutFilesOnPendrive(FileType* all_files, PendriveType** all_pendrives, unsigned int files_count) {
	int put_files = 0;
	size_t j = 0;
	for (size_t i = 0; i < PENDRIVE_COUNT; i++) {
		for (; j < files_count; j++) {
			if (all_files[j].size > all_pendrives[i]->free_space) {
				break;
			}
			if (all_pendrives[i]->max_file_count < (all_pendrives[i]->file_count + 1)) {
				EnlargePendrive(all_pendrives[i]);
			}
			all_pendrives[i]->free_space -= all_files[j].size;
			swap(all_pendrives[i]->files[all_pendrives[i]->file_count], all_files[j]);
			all_pendrives[i]->file_count++;
			put_files++;
		}
	}
	return put_files;
}

void PrintPendrives(PendriveType** all_pendrives, unsigned int put_files) {
	printf("%u\n", put_files);
	for (int i = 0; i < PENDRIVE_COUNT; i++) {
		printf("%d:", i + 1);
		for (int j = all_pendrives[i]->file_count - 1; j > -1; j--) {
			printf(" %u", all_pendrives[i]->files[j].number);
		}
		printf("\n");
	}
}

void SortAscendingBySize(FileType* file_array, int beginning, int end) {
	if (end > beginning) {
		int i = beginning - 1, j = end + 1;
		unsigned int pivot = file_array[beginning].size;
		for (;;) {
			while (pivot > file_array[++i].size);
			while (pivot < file_array[--j].size);
			if (i <= j)
				swap(file_array[i], file_array[j]);
			else
				break;
		}
		if (j > beginning)//IF j is less equal beginning there is no need to sort the array. One element array is always sorted! Empty array does not need sorting
			SortAscendingBySize(file_array, beginning, j);
		if (i < end)//If j is greater equal end there is no need to sort the array. One element array is always sorted! Empty array does not need sorting.
			SortAscendingBySize(file_array, i, end);
	}
}

void SortAscendingByNumber(FileType* file_array, int beginning, int end) {
	if (end >= beginning) {
		unsigned int pivot = file_array[beginning].number;
		int i = beginning - 1, j = end + 1;
		for (;;) {
			while (pivot > file_array[++i].number);
			while (pivot < file_array[--j].number);
			if (i <= j)
				swap(file_array[i], file_array[j]);
			else
				break;
		}
		if (j > beginning)//IF j is less equal beginning there is no need to sort the array. One element array is always sorted! Empty array does not need sorting
			SortAscendingByNumber(file_array, beginning, j);
		if (i < end) //If j is greater equal end there is no need to sort the array. One element array is always sorted! Empty array does not need sorting.
			SortAscendingByNumber(file_array, i, end);
	}
}

int main() {
	int pendrive_size, number_of_files, put_files = 0;
	scanf("%d %d\n", &number_of_files, &pendrive_size);
	PendriveType** pendrives = (PendriveType**)malloc(PENDRIVE_COUNT * sizeof(PendriveType*));
	FileType* all_files = LoadFiles(number_of_files);
	if (pendrives == NULL && all_files != NULL) { free(all_files); return 0; }
	if (all_files == NULL && pendrives != NULL) { free(pendrives); return 0; }
	if (all_files == NULL && pendrives == NULL) { return 0; }
	SortAscendingBySize(all_files, 0, number_of_files - 1);
	SetUpPendrives(pendrives, pendrive_size);
	put_files = PutFilesOnPendrive(all_files, pendrives, number_of_files);
	for (size_t i = 0; i < PENDRIVE_COUNT; i++) {
		SortAscendingByNumber(pendrives[i]->files, 0, pendrives[i]->file_count - 1);
	}
	PrintPendrives(pendrives, put_files);
	for (int i = 0; i < PENDRIVE_COUNT; i++) {
		free(pendrives[i]);
	}
	free(pendrives);
	free(all_files);
}