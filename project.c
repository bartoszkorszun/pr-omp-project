#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

int main() 
{
    int m, n;

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

        if (m < n) 
        {
            break;
        }

        printf("%s\n", "Lower bound must be less than the upper bound!");
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

    // Apply the Sieve of Eratosthenes to filter prime numbers
    int newSize = 0;
    sieveOfEratosthenes1(m, n, &numbersArray, &newSize);

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

    // Free allocated memory
    free(numbersArray);

    return 0;
}
