#include <stdio.h>

int m, n;
int* tab;

void generateTable(int m, int n, int* tab)
{
    int i = 0;
    for (int j = m; j < n; j++)
    {
        printf("%d, ", j);
        tab[i] = j;
        i++;
    }
}

int main() 
{
    while(1)
    {
        printf("%s\n", "Wprowadz dolna granice:");
        scanf("%d", &m);
        printf("%s\n", "Wprowadz gorna granice:");
        scanf("%d", &n);

        if (m < n)
        {
            break;
        }
        
        printf("%s\n", "Dolna granica musi byc mniejsza od gornej!");
    }
    
    printf("Przedzial od %d do %d\n", m, n);

    generateTable(m, n, tab);

    // for (int i = 0; i < sizeof(tab) / sizeof(tab[0]); i++)
    // {
    //     printf("%d, \n", tab[i]);
    // }

    return 0;
}