/* COP 3502C Programming Assignment 4
This program is written by: Zainab Syed */

#include <stdlib.h>
#include <stdio.h>

typedef struct garage
{
    int x;
    int y;
} garage;

typedef struct coordinates {
    int x;
    int y;
} Coordinates;

Coordinates myLocation;

/* Function to sort an array using insertion sort*/
// modify to take arr, start and end index of the array
void insertionSort(garage* arr, int start, int end)
{
    int i, j;
    garage item; // temp variable for the coordinates being moved

    for (i = start; i < end; i++)
    {
        item = arr[i];
        /* move elements of arr[start... i-1], that are greater than
        item, to one position ahead of their current position */
        for(j = i-1; j >= start; j--)
        {
            int dist_val = compareTo(&arr[j], &item);
            if(dist_val > 0) {
                arr[j+1] = arr[j];
            }
            else {
                break; }
        }
        arr[j+1] = item;
    }
}

// merges arrays
void merge(garage* arr, int l, int m, int r)
{
    // printf("reached merge\n");
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // create temp arrays
    garage *L = malloc(n1*sizeof(garage));
    garage *R = malloc(n2*sizeof(garage));

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1+ j];
    }

    // Merge the temp arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        int dist_val = compareTo(&L[i], &R[j]);

        // if the L[] coordinate is closer than the R[] coordinate
        if (dist_val < 0) {
            arr[k] = L[i];
            i++;
        }
        // if the L[] coordinate is further than the R[] coordinate
        else if (dist_val > 0) {
            arr[k] = R[j];
            j++;
        }
        // if the coordinates are pointing to the same spot
        else {
            arr[k] = L[i];
            i++;
            j++;
        }
        k++;
    }
    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

// conducting merge Sort until the threshold is met, then insertion sort is implemented
void mergeInsertionSort(garage* garages, int l, int r, int threshold)
{
    // threshold is met
    if ((r-l +1) <= threshold) {
        insertionSort(garages, l, r+1);
    }
    else if (l < r)
    {
        // get the mid point
        int m = (l+r)/2;
        // Sort first and second halves
        mergeInsertionSort(garages, l, m, threshold);
        mergeInsertionSort(garages, m+1, r, threshold);

        merge(garages, l, m, r);
    }
}

// calculates distance between my position and another coordinate
int dist(int x1, int y1, int x2, int y2) {
    return ((x2-x1) * (x2-x1)) + ((y2-y1)*(y2-y1));
}

int compareTo(garage* ptrPt1, garage* ptrPt2)
{
    // calculate the distance of ptr1 and ptr2
    int dist_1 = dist(myLocation.x, myLocation.y, ptrPt1->x, ptrPt1->y);
    int dist_2 = dist(myLocation.x, myLocation.y, ptrPt2->x, ptrPt2->y);

    // Pt1 is closer than Pt2
    if(dist_1 < dist_2) {
        return -1;
    }

    // Pt1 is further than Pt2
    else if(dist_1 > dist_2) {
        return 1;
    }

    // identical distance, but different location
    else if(dist_1 == dist_2) {
        if (ptrPt1->x < ptrPt2->x) {
            return -1;
        }
        else if(ptrPt1->x > ptrPt2->x) {
            return 1;
        }
        else if(ptrPt1->x == ptrPt2->x) {
            if(ptrPt1->y < ptrPt2->y) {
                return -1;
            }
            else if(ptrPt1->y > ptrPt2->y) {
                return 1;
            }
        }
    }
    // if the locations are identical
    return 0;
}

// reads all of the inputs and returns the array of garages
garage* ReadData(int* t, int* num_g, int* num_q, garage** query_points)
{
    scanf("%d %d %d %d %d", &myLocation.x, &myLocation.y, num_g, num_q, t);

    // allocates memory for the garages and queries
    garage* garages = malloc(*num_g *sizeof(garage));
    *query_points = malloc(*num_q * sizeof(garage));

    // inputs for garage coordinates
    for (int i = 0; i < *num_g; i++) {
        scanf("%d %d", &garages[i].x, &garages[i].y);
        printf("");
    }

    // inputs for query coordinates
    for (int i = 0; i < *num_q; i++) {
        scanf("%d %d", &(*query_points)[i].x, &(*query_points)[i].y);
        printf("");
    }

    return garages;
}

// binary Search of each query in the garage
int binSearch(garage* garages, int num_garages, garage q)
{
    int mid;
    int low = 0;
    int high = num_garages-1;
    while (low <= high)
    {
        mid = (low+high)/2;
        int comparison = compareTo(&garages[mid], &q);

        // if the garage is found
        if (comparison == 0) {
            printf("%d %d garage found at position %d in the order\n", q.x, q.y, mid+1);
            return mid;
        }
        // if q is closer than mid point
        else if(comparison > 0) {
            high = mid-1;
        }
        // if q is further than mid point
        else {
            low = mid + 1;
        }
    }
    // garage not found
    printf("%d %d no garage found\n", q.x, q.y);
    return -1;
}

// calls necessary sort functions
void wrapper(garage* garages, int num_g, int ths)
{
    if(num_g <= ths) {
        insertionSort(garages, 0, num_g);
    }
    mergeInsertionSort(garages, 0, num_g-1, ths);
}

int main()
{
    int threshold, num_garages, num_queries;
    garage* querie_points = NULL; // initialize the query array

    // creates array of garage stucts, and gets values for ths, num_g, & num_q
    garage* garages = ReadData(&threshold, &num_garages, &num_queries, &querie_points);

    wrapper(garages, num_garages, threshold);

    // prints sorted garages
    for(int i = 0; i < num_garages; i++) {
        printf("%d %d\n", garages[i].x, garages[i].y);
    }

    // searches for and prints the result of the queries
    for(int j = 0; j < num_queries; j++) {
        binSearch(garages, num_garages, querie_points[j]);
    }

    free(querie_points);
    free(garages);

    return 0;
}
