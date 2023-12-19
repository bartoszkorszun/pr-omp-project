#include <stdio.h>
#include <stdlib.h>

int main() 
{
    int m, n;

    while (1)
    {
        printf("%s\n", "Wprowadz dolna granica:");
        scanf_s("%d", &m);

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

    if (numbersArray == NULL) {
        printf("Memory allocation failed.\n");
        return 1;  
    }

    for (int i = 0; i < arraySize; i++) {
        numbersArray[i] = m + i;
    }



    free(numbersArray);

    return 0; 
}