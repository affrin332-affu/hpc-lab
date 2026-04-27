#include <stdio.h>
#include <omp.h>

double f(double x) {
    return x * x;
}

int main() {
    double a, b, total_integral;
    int n, choice;

    printf("Enter lower limit (a): ");
    scanf("%lf", &a);
    printf("Enter upper limit (b): ");
    scanf("%lf", &b);
    printf("Enter number of intervals (n): ");
    scanf("%d", &n);

    double h = (b - a) / n;

    printf("\n--- Trapezoidal Rule Menu ---\n");
    printf("1. Parallel For\n");
    printf("2. Reduction Clause\n");
    printf("3. Critical Section\n");
    printf("4. Exit\n");
    while(1)
    {
        printf("Enter Choice: ");
        scanf("%d", &choice);

        if (choice == 4) break;

        total_integral = (f(a) + f(b)) / 2.0;

        if (choice == 1) {
             printf("Parallel for Result\n");
            #pragma omp parallel
            {
                double my_sum = 0.0;
                #pragma omp for
                for (int i = 1; i < n; i++) {
                    my_sum += f(a + i * h);
                }
                #pragma omp critical
                total_integral += my_sum;
            }
        }
        else if (choice == 2) {
            printf("Reduction Clause\n");
            #pragma omp parallel for reduction(+:total_integral)
            for (int i = 1; i < n; i++) {
                total_integral += f(a + i * h);
            }
        }
        else if (choice == 3) {
           printf("Critical section\n");
            #pragma omp parallel
            {
                double local_sum = 0.0;
                #pragma omp for
                for (int i = 1; i < n; i++) {
                    local_sum += f(a + i * h);
                }
                #pragma omp critical
                total_integral += local_sum;
            }
        }
        else {
            printf("Invalid!\n");
            continue;
        }

        printf("Final Result: %f\n", total_integral * h);
    }

    return 0;
}
