#include <stdio.h>
#include <malloc.h>

extern const int N;

double ** Create(double len) {
    double ** res = (double **) malloc(len * sizeof(*res));
    int i = 0;
    for (i = 0; i < len; ++i) {
        res[i] = (double *) malloc(N * sizeof(*res[i]));
    }
    return res;
}

void Copy(double **from, double ** to) {
    int i = 0;
    for (i = 0; i < N; ++i) {
        int j = 0;
        for (j = 0; j < N; ++j) {
            to[i][j] = from[i][j];
        }
    }
}


void Delete(double **a, int len) {
    int i = 0; 
    for (i = 0; i < len; ++i) {
        free(a[i]);
    }
    free(a);
}

void Add(double **a, double ** b) {
    int i = 0;
    for (i = 0; i < N; ++i) {
        int j = 0;
        for (j = 0; j < N; ++j) {
            a[i][j] += b[i][j];
        }
    }
}

void Mul(double **a, double **b, double **res) {
    int i = 0;
    for (i = 0; i < N; ++i) {
        int j = 0;
        for (j = 0; j < N; ++j) {
            res[i][j] = 0;
            int k = 0;
            for (k = 0; k < N; ++k) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void Muld(double **a, double x) {
    int i = 0;
    for (i = 0; i < N; ++i) {
        int j = 0;
        for (j = 0; j < N; ++j) {
            a[i][j] *= x;
        }
    }
}

void SetMinor(double **a, double **p, int ci, int cj, int len) {
    int i = 0;
    for (i = 0; i < len; ++i) {
        int j = 0;
        for (j = 0; j < len; ++j) {
            if (i == ci || j == cj) {
                continue;
            }
            int di = (i >= ci ? -1 : 0);
            int dj = (j >= cj ? -1 : 0);
            p[i + di][j + dj] = a[i][j];
        }
    }
}

void Print(double **a, int len) {
    printf("\n Mattrix:\n");
    int i = 0;
    for (i = 0; i < len; ++i) {
        int j = 0;
        for (j = 0; j < len; ++j) {
            printf("%lf ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Input(double **a, int len) {
    int i = 0;
    for (i = 0; i < len; ++i) {
        int j = 0;
        for (j = 0; j < len; ++j) {
            scanf("%lf", &a[i][j]);
        }
    }
}

double Det(double **a, int len) {
    if (len == 1) {
        return a[0][0];
    }
    int pLen = len - 1;
    double ** p = Create (pLen);
    double res = 0;
    
    int j = 0, k = 1;
    for (j = 0, k = 1; j < len; ++j) {
        SetMinor(a, p, 0, j, len);
        res += k * a[0][j] * Det(p, len - 1);
        k *= -1; 
    }
    Delete(p, pLen);
    return res;
}
