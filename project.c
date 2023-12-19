#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void sieveOfEratosthenes1(int m, int n, int* array) 
{
    bool* isPrime = (bool*)malloc((n - m + 1) * sizeof(bool));

    printf("Rozmiar: %d\n", sizeof(isPrime));

    for (int i = 0; i < sizeof(isPrime)/sizeof(bool); i++) 
    {
        isPrime[i] = true;
        printf("%d, ", isPrime[i]);
    }

    printf("\n");

    for (int p = 2; p * p <= n; p++) 
    {
        if (isPrime[p] == true) 
        {
            for (int i = p * p; i <= n; i += p) 
            {
                isPrime[i] = false;
            }
        }
    }

    int j = 0; 
    for (int i = 2; i <= n; i++) 
    {
        if (isPrime[i]) 
        {
            array[j++] = i;
        }
    }

    array = realloc(array, j * sizeof(int));

    free(isPrime);
}

int main() 
{
    int m, n;

    while (1)
    {
        printf("%s\n", "Wprowadz dolna granica:");
        scanf_s("%d", &m);

        if (m < 2) 
		{
			printf("%s\n", "Dolna granica musi byc wieksza od 1!");
			continue;
		}

        printf("%s\n", "Wprowadz gorna granice:");
        scanf_s("%d", &n);

        if (m < n) 
        {
            break;  
        }

        printf("%s\n", "Dolna granica musi byc mniejsza od gornej granicy!");
    }

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

    // Sekwencyjnie
    sieveOfEratosthenes1(m, n, numbersArray);

    printf("Modified array with prime numbers: [");
    for (int i = 0; i < n - 2; i++) 
    {
        printf("%d, ", numbersArray[i]);
    }
    printf("%d]\n", numbersArray[n - 2]);

    free(numbersArray);

    return 0; 
}