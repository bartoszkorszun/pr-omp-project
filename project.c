#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

void sieveOfEratosthenes1(int m, int n, int** array, int* size) 
{
    for (int i = 2; i * i <= n; i++) 
	{
		for (int j = 0; j < n - m + 1;)
		{
			if ((*array)[j] % i == 0 && (*array)[j] != i)
			{
				(*array)[j] = 1;
                j += i;
			}
			else
			{
				j++;
			}
		}
	}

    for (int i = 0; i < n - m + 1; i++)
    {
        if ((*array)[i] == 1)
		{
			(*array)[i] = 0;
		}
    }

    int j = 0;
    for (int i = 0; i < n - m + 1; i++) 
    {
        if ((*array)[i])
        {
            (*array)[j++] = m + i;
        }
    }

    *array = realloc(*array, j * sizeof(int));
    *size = j;
}

void sieveOfEratosthenes2(int index, int m, int n, int** array, int* size)
{
    for (int i = 2; i * i <= n; i++)
    {
        for (int j = index; j < n - m + 1;)
        {
            if ((*array)[j] % i == 0 && (*array)[j] != i)
            {
                (*array)[j] = 1;
                j += i;
            }
            else
            {
                j++;
            }
        }
    }

    for (int i = index; i < n - m + 1; i++)
    {
        if ((*array)[i] == 1)
        {
            (*array)[i] = 0;
        }
    }

    int j = 0;
#pragma omp critical
    for (int i = index; i < n - m + 1; i++)
    {
        if ((*array)[i])
        {
            (*array)[j++] = m + i;
        }
    }

#pragma omp atomic
    *size += j;
}

int main() 
{
    int m, n, choice;
    clock_t start, end;

    // Input
    while (1) 
    {
        printf("%s\n", "Enter the lower bound:");
        scanf_s("%d", &m);

        if (m < 2) 
        {
            printf("%s\n", "Lower bound must be greater than 1!");
            continue;
        }

        printf("%s\n", "Enter the upper bound:");
        scanf_s("%d", &n);

        if (m >= n)         
        {
            printf("%s\n", "Lower bound must be less than the upper bound!");
            continue;
        }

        printf("%s\n %s\n %s\n", "Choose:", "1 - Sequential", "2 - Parallel");
        scanf_s("%d", &choice);

        if (choice == 1 || choice == 2) 
		{
            break;
		}

		printf("%s\n", "Invalid choice!");
    }

    // Generate an array of all numbers from m to n
    int arraySize = n - m + 1;
    int* numbersArray = (int*)malloc(arraySize * sizeof(int));

    if (numbersArray == NULL) 
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < arraySize; i++) 
    {
        numbersArray[i] = m + i;
    }

    int newSize = 0;
    
    if (choice == 1) 
	{
		printf("%s\n", "Sequential");
        // Apply the Sieve of Eratosthenes to filter prime numbers
        start = clock();
        sieveOfEratosthenes1(m, n, &numbersArray, &newSize);
        end = clock();
	}
	else if (choice == 2) 
	{
		printf("%s\n", "Parallel");
        start = clock();
        // Apply the Sieve of Eratosthenes to filter prime numbers
#pragma omp parallel for
        for (int i = 0; i < arraySize/16; i += 16) 
		{
            sieveOfEratosthenes2(i, numbersArray[i], numbersArray[i + 16], &numbersArray, &newSize);
		}

        numbersArray = realloc(numbersArray, newSize * sizeof(int));
        end = clock();
	}
	else 
	{
		printf("%s\n", "Oops something went wrong!");
		return 1;
	}

    // Display the modified array with prime numbers
    printf("Modified array with prime numbers: [");
    for (int i = 0; i < newSize - 1; i++) 
    {
        printf("%d, ", numbersArray[i]);
    }
    if (newSize > 0) 
    {
        printf("%d", numbersArray[newSize - 1]);
    }
    printf("]\n");

    // Display the execution time
    printf("Execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Free allocated memory
    free(numbersArray);

    return 0;
}
