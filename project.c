#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>
#include <math.h>

bool isPrime(int number)
{
    for (int i = 2; i * i <= number; i++)
	{
		if (number % i == 0)
		{
			return false;
		}
	}
    return true;
}

void division(int m, int n, int** array)
{
    for (int i = m; i <= n; i++)
    {
        if (!isPrime(i))
        {
            (*array)[i - m] = 1;
        }
    }
}

void divisionParallel(int m, int n, int** array)
{
#pragma omp parallel for num_threads(8) schedule(dynamic, 1)
	for (int i = m; i <= n; i++)
	{
		if (!isPrime(i))
		{
			(*array)[i - m] = 1;
		}
	}
}

void sieveOfEratosthenesSequential(int m, int n, int** array) 
{
    for (int i = 2; i * i <= n; i++)
    {
        int start = (m > i * i) ? m : i * i;
        int multiple = (start % i == 0) ? start : (start / i + 1) * i;

        for (int j = multiple; j <= n; j += i)
        {
            if (j >= i && (*array)[j - m] != i)  
            {
                (*array)[j - m] = 1;
            }
        }
    }
}

void sieveOfEratosthenesParallelDomainwise(int m, int n, int** array)
{
#pragma omp parallel num_threads(8)
    {
        int numThreads = omp_get_num_threads();
        int domainSize = (n - m + 1) / numThreads;
        int threadIndex = omp_get_thread_num();
        int startIndex = m + threadIndex * domainSize;
        int endIndex = (threadIndex == numThreads - 1) ? n + 1 : startIndex + domainSize;

        for (int i = 2; i * i <= m + endIndex; i++)
        {
            if (i * i > endIndex)
            {
                break; 
            }

            int startMultiple = (startIndex > i * i) ? startIndex : i * i;
            int multiple = (startMultiple % i == 0) ? startMultiple : (startMultiple / i + 1) * i;

            for (int j = multiple; j < endIndex; j += i)
            {
                if ((*array)[j - m] != 1)
                {
                    (*array)[j - m] = 1;
                }
            }
        }
    }
}

void sieveOfEratosthenesParallelFunctionally(int m, int n, int** array)
{
    int root = sqrt(n);
#pragma omp parallel for num_threads(8)
    for (int i = 2; i <= root; i++)
    {
        int start = (m > i * i) ? m : i * i;
        int multiple = (start % i == 0) ? start : (start / i + 1) * i;

        for (int j = multiple; j <= n; j += i)
        {
            if (j >= i && (*array)[j - m] != i)
            {
                (*array)[j - m] = 1;
            }
        }
    }
}

void printArray(int* array, int size) 
{
    printf("Zmodyfikowana tablica z liczbami pierwszymi: \n[");
    for (int i = 0; i < size - 1; i++)
    {
        if (i != 0 && i % 10 == 0)
		{
			printf("\n");
		}
        printf("%d, ", array[i]);
    }
    if (size > 0)
    {
        printf("%d", array[size - 1]);
    }
    printf("]\n");
}

void changeArray(int m, int n, int** array, int* size) 
{
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
    int m, n, choice;
    clock_t start, end;

    while (1) 
    {
        printf("%s\n", "Wprowadz dolna granice:");
        scanf("%d", &m);

        if (m < 2) 
        {
            printf("%s\n", "Dolna granica musi byc wieksza, badz rowna 2!");
            continue;
        }

        printf("%s\n", "Wprowadz gorna granice:");
        scanf("%d", &n);

        if (m >= n)         
        {
            printf("%s\n", "Dolna granica musi byc mniejsza od gornej granicy!");
            continue;
        }

        printf("%s\n %s\n %s\n %s\n %s\n %s\n", "Wybierz sposob:", "1 - Dzielenie sekwencyjne", "2 - Dzielenie rownolegle", "3 - Usuwanie liczb zlozonych sekwencyjnie", "4 - Usuwanie liczb zlozonych rownolegle domenowo", "5 - Usuwanie liczb zlozonych rownolegle funkcyjnie");
        scanf("%d", &choice);

        if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5)
		{
            break;
		}

		printf("%s\n", "Nieprawidlowy wybor");
    }

    printf("Generowanie tablicy liczb od %d do %d...\n", m, n);
    int arraySize = n - m + 1;
    int* numbersArray = (int*)malloc(arraySize * sizeof(int));

    if (numbersArray == NULL) 
    {
        printf("Alokacja pamieci nie powiodla sie.\n");
        return 1;
    }

    for (int i = 0; i < arraySize; i++) 
    {
        numbersArray[i] = m + i;
    }

    printf("%s\n", "Tablica zostala wygenerowana...");

    int newSize = 0;
    
    printf("Sprawdzanie liczb pierwszych...\n");

    switch (choice)
    {
        case 1:
        {
            start = clock();
            division(m, n, &numbersArray);
            end = clock();

            break;
        }
        case 2:
        {
            start = clock();
            divisionParallel(m, n, &numbersArray);
            end = clock();

            break;
        }
        case 3:
        {
            start = clock();
            sieveOfEratosthenesSequential(m, n, &numbersArray);
            end = clock();

            break;
        }
        case 4:
        {
            start = clock();
            sieveOfEratosthenesParallelDomainwise(m, n, &numbersArray);
            end = clock();

            break;
        }
        case 5:
		{
            start = clock();
            sieveOfEratosthenesParallelFunctionally(m, n, &numbersArray);
            end = clock();

            break;
		}
        default:
        {
            printf("%s\n", "Ups cos poszlo nie tak!");
            return 1;
        }
    }

    changeArray(m, n, &numbersArray, &newSize);
    //printArray(numbersArray, newSize);

    printf("\nCzas wykonania algorytmu: %f sekund\n\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(numbersArray);

    return 0;
}
