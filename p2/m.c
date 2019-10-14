#include <stdio.h>
#include "lib/mattrix.h"
#include <malloc.h>

const int N = 3;

int main()
{
    double ** a = Create(N);
    double ** b = Create(N);
    
    Input(a, N);
    printf("\nStart...\nA:");
    Print(a, N);
    Input(b, N);

    printf("B:");
    Print(b, N);
    double x;
    scanf("%lf", &x);
    printf("X:\n%lf\n\n", x);
    
    printf("|A|:: %lf\n\n", Det(a, N));
    printf("|B|:: %lf\n\n", Det(b, N));
    
    double ** res = Create(N);
    
    Copy(a, res);
    Add(res, b);
    printf("Sum::");
    Print(res, N);
    
    
    Copy(a, res);
    Muld(res, x);
    printf("A * x::");
    Print(res, N);
    
    Mul(a, b, res); 
    printf("Mul::");
    Print(res, N);
    
    Delete(a, N);
    Delete(b, N);
    Delete(res, N);
    
    return 0;
}
