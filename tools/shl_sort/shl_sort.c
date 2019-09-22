#include <stdio.h>

void ShellSort(int a[], int size) {
    for (int step = size / 2; step > 0; step /= 2) {
        for (int i = step; i < size; ++i) {
            for (int j = i - step; j >= 0 && a[j] > a[j + step]; j -= step) {
                int tmp = a[j];
                a[j] = a[j + step];
                a[j + step] = tmp;
            }
        }
    }
}

enum {
    MAX_AR_SIZE = 200
};

void print_array(int a[], int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void shl_sort() {
    int n;
    scanf("%d", &n);
    int a[MAX_AR_SIZE];
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
    print_array(a, n);
    ShellSort(a, n);
    print_array(a, n);
}