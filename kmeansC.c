#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void kmeans(int k, int iterNum, char input[], char output[]);
char *strtok(char *str, const char *delim);
void discover_n_d(char input[], int n_d[2]);
void fill_mat(char input[], double** vectors);
int is_number(char s[]);
void update_u(double* new_centroid, int* cluster_of_vector, double** vectors, int d, int n, int clust_num);
int argmin(double x[], double** centroids, int n, int d);
double occlid_distance(double x1[], double x2[], int d);




int main(int argc, char* argv[]){
    printf("hi\n");
    printf("k = %s\n", argv[1]);
    if (argc!=4 && argc!=5) {
        printf("Invalid Input!");
        return 0;
    }
    if (!is_number(argv[1])) {
        printf("Invalid Input!");
        return 0;
    }
    if (argc==4) { //check validation
        kmeans(atoi(argv[1]), 200, argv[2], argv[3]);
    }
    else { //(argc==5) 
        if (!is_number(argv[2])) {
            printf("Invalid Input!");
            return 0;
        }
        kmeans(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);
    }
    // finding out n,d values
    return 0;
}

// double** kmeans(int k, int iterNum, char input[], char output[]) {
void kmeans(int k, int max_iter, char input[], char output[]) {
    printf("k = %d\n", k);
    int n_d[2];
    discover_n_d(input, n_d);
    int n = n_d[0];
    int d = n_d[1];
    double e = 0.001;
    if (k<=1 || k>=n) {
        printf("Invalid Input!");
        // add FREEEE!!!!
        exit(1);
    }
    double** vectors = malloc(n*sizeof(double*)); //create matrix
    for (int i =0; i <n; i++){
        vectors[i] = malloc(d*sizeof(double));
    }
    fill_mat(input, vectors);
    double** centroids = malloc(k*sizeof(double*)); //create centroids
    for (int i =0; i < k; i++){
        centroids[i] = malloc(d*sizeof(double));
        for (int j =0; j <d; j++){
            centroids[i][j] = vectors[i][j];
        }
    }

    int* cluster_of_vector = malloc(n*sizeof(int)); //cluster_of_vector[i]==cluster of xi vector
    int num_of_loops = 0;
    while(1==1){
        for (int i=0; i<n; i++) {
            cluster_of_vector[i] = argmin(vectors[i], centroids, k, d);
        }
        
        double* norms = malloc(k*sizeof(double));
        double* new_centroid = malloc(d*sizeof(double));
        for (int clust_num=0; clust_num<k; clust_num++) {
            update_u(new_centroid, cluster_of_vector, vectors, d, n, clust_num);
            norms[clust_num] = sqrt(occlid_distance(new_centroid, centroids[clust_num], d));
            for (int j=0; j<d; j++) {
                centroids[clust_num][j] = new_centroid[j];
            }
        }
        num_of_loops++;
        int is_bigger_than_e = 0;
        for (int i=0; i<k; i++) {
            if (norms[i] >= e) {
                is_bigger_than_e = 1;
                break;
            }
        }
        if (num_of_loops >= max_iter || is_bigger_than_e==0) {
            break;
        }
    }

    // print all centroids:
    // int row, columns;
    // for (row=0; row<k; row++)
    // {
    //     for(columns=0; columns<d; columns++)
    //     {
    //         printf("%f     ", centroids[row][columns]);
    //     }
    //     printf("\n");
    // }

    // for (int i=0; i<n; i++)
    // {
    //     printf("%d     ,", cluster_of_vector[i]);
    // }

    printf("iteration number: %d\n", num_of_loops);
    printf("max iter: %d\n", max_iter);


    printf("BYEE");
}

// print all vectors
    // int row, columns;
    // for (row=0; row<k; row++)
    // {
    //     for(columns=0; columns<d; columns++)
    //     {
    //         printf("%.4f     ", centroids[row][columns]);
    //     }
    //     printf("\n");
    // }
    


void discover_n_d(char input[], int n_d[2]) {
    FILE* inputFile;
    inputFile = fopen(input, "r");
    char line[1000];
    int n;
    int d=0;
    while (fgets(line, 1000, inputFile)) { //searching for n and d
        d=0;
        char *token = strtok(line, ",");
        while (token != NULL) {
            d++;
            token = strtok(NULL, ",");
        }
        n++;
    }
    fclose(inputFile);
    n_d[0] = n;
    n_d[1] = d;
    // printf("d=%d\n", d);
    // printf("n=%d\n", n);
    return;
}

void fill_mat(char input[], double** vectors) {
    FILE* inputFile;
    char line[1000];
    int i = 0;
    int j = 0;  
    inputFile = fopen(input, "r");
    while (fgets(line, 1000, inputFile)) {
        char *token = strtok(line, ",");
        while (token != NULL) {
            sscanf(token, "%lf", &vectors[i][j]);
            // scanf(token, "%d", &vectors[i][j]);
            j++;
            token = strtok(NULL, ",");
        }
        i++;
        j=0;
    }
    fclose(inputFile);
    return;
}


int is_number(char s[]) {
    for (int i = 0; i < strlen(s); i++)
        if (isdigit(s[i]) == 0)
            return 0;
    return 1;
}

double occlid_distance(double x1[], double x2[], int d) {
    double distance = 0;
    for (int i=0; i < d ;i++) {
        distance += pow(x1[i]-x2[i], 2);
    }
    return distance;
}

int argmin(double x[], double** centroids, int k, int d) {

    // int row, columns;
    // for (row=0; row<n; row++)
    // {
    //     for(columns=0; columns<d; columns++)
    //     {
    //         printf("%f     ", centroids[row][columns]);
    //     }
    //     printf("\n");
    // }

    int index = 0;
    // double min_distance = occlid_distance(x, centroids[0], d);
    double min_distance = INFINITY;
    double distance = 0;
    for (int j=0; j<k; j++) {
        distance = occlid_distance(x, centroids[j], d);
        if (distance < min_distance) {
            min_distance = distance;
            index = j;
        }
    }
    return index;
}

void update_u(double* new_centroid, int* cluster_of_vector, double** vectors, int d, int n, int clust_num) {
    int num_vectors_in_cluster = 0;
    for (int i=0; i<n; i++) {
        if (cluster_of_vector[i]==clust_num) {
            num_vectors_in_cluster++;
            for (int j=0; j<d; j++) {
                new_centroid[j] +=vectors[i][j];
            }
        }
    }
    for (int i=0; i<d; i++) {
        new_centroid[i] = new_centroid[i] / num_vectors_in_cluster;
    }
    // printf("In cluster %d we have %d vectors.\n", clust_num, num_vectors_in_cluster);
    return;
}