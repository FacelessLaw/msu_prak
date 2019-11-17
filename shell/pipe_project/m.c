#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    int n;
    scanf("%d", &n);
    printf("%d\n", n + 1);
    return 0;
}
