// config.h
#ifndef CONFIG_H
#define CONFIG_H
// Define constants for sorting algorithm options
#define BUBBLE_SORT 0
#define INSERTION_SORT 1
#define SELECTION_SORT 2
#define MERGE_SORT 3
#define QUICK_SORT 4

// Configuration variables
 int elementarySortingAlgorithm=BUBBLE_SORT; // Variable to select elementary sorting algorithm
 int advancedSortingAlgorithm=MERGE_SORT;   // Variable to select advanced sorting algorithm
  int threshold=80;              	 // Threshold for switching between sorting and searching algorithms
#endif // CONFIG_H

