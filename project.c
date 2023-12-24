#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

bool isPrime1(int number)
{
    for (int i = 2; i * i <= number; i++)
	{
		if (number % i == 0)
		{
			return false;
		}
	}
}

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
        printf("%s\n", "Wprowadz dolna granice:");
        scanf_s("%d", &m);

        if (m < 2) 
        {
            printf("%s\n", "Dolna granica musi byc wieksza, badz równa 2!");
            continue;
        }

        printf("%s\n", "Wprowadz gorna granice:");
        scanf_s("%d", &n);

        if (m >= n)         
        {
            printf("%s\n", "Dolna granica musi byc mniejsza od gornej granicy!");
            continue;
        }

        printf("%s\n %s\n %s\n %s\n %s\n %s\n", "Wybierz sposob:", "1 - Dzielenie sekwencyjne", "2 - Dzielenie rownolegle", "3 - Usuwanie liczb zlozonych sekwencyjnie", "4 - Usuwanie liczb zlozonych rownolegle domenowo", "5 - Usuwanie liczb zlozonych rownolegle funkcyjnie");
        scanf_s("%d", &choice);

        if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5)
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
        bool coma = false;
        start = clock();
        printf("Modified array with prime numbers: [");
        for (int i = m; i <= n; i++)
        {
            if (i != 0 && isPrime1(i) && coma)
            {
                printf(", ");
            }
            if (isPrime1(i))
			{
                printf("%d", i);
                if (!coma)
                {
					coma = true;
                }
			}
        }
        printf("]\n");
        end = clock();
    }
    else if (choice == 2)
    {
        printf("%d\n", choice);
    }
    else if (choice == 3) 
	{
        // Apply the Sieve of Eratosthenes to filter prime numbers
        start = clock();
        sieveOfEratosthenes1(m, n, &numbersArray, &newSize);
        end = clock();

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
	}
	else if (choice == 4) 
	{
        // Apply the Sieve of Eratosthenes to filter prime numbers
        start = clock();
#pragma omp parallel for
        for (int i = 0; i < arraySize; i += 16) 
		{
            sieveOfEratosthenes2(i, numbersArray[i], numbersArray[i + 16], &numbersArray, &newSize);
		}

        numbersArray = realloc(numbersArray, newSize * sizeof(int));
        end = clock();

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
	}
    else if (choice == 5)
    {
        printf("%d\n", choice);
    }
	else 
	{
		printf("%s\n", "Oops something went wrong!");
		return 1;
	}

    // Display the execution time
    printf("\nExecution time: %f seconds\n\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Free allocated memory
    free(numbersArray);

    return 0;
}
