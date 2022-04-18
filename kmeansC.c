#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void kmeans(int k, int iterNum, char input[], char output[]);
void discover_n_d(char input[], int n_d[2]);
void fill_mat(char input[], double** vectors);
int is_number(char s[]);
void update_u(double* new_centroid, int* cluster_of_vector, double** vectors, int d, int n, int clust_num);
int argmin(double x[], double** centroids, int n, int d);
double occlid_distance(double x1[], double x2[], int d);
void write_in_file(char output[], double** centroids, int k, int d);


int main(int argc, char* argv[]){
    if (argc!=4 && argc!=5) {  // wrong number of args
        printf("Invalid Input!");
        return 0;
    }
    if (!is_number(argv[1])) {  // k is not a number
        printf("Invalid Input!");
        return 0;
    }
    if (argc==4) {  //no max_iter given
        kmeans(atoi(argv[1]), 200, argv[2], argv[3]);
    }
    else {  //(argc==5) max_iter given
        if (!is_number(argv[2]) || atoi(argv[2]) <= 0) {  // max_iter not a number or not positive
            printf("Invalid Input!");
            return 0;
        }
        kmeans(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);
    }
    return 0;
}

void kmeans(int k, int max_iter, char input[], char output[]) {
    int n_d[2] = {0, 0};
    discover_n_d(input, n_d);
    int n = n_d[0];
    int d = n_d[1];
    double e = 0.001;
    if (k<=1 || k>=n) {  // 0 < k <= n
        printf("Invalid Input!");
        exit(1);
    }
    double** vectors = malloc(n*sizeof(double*));  //create matrix
    if(vectors == NULL) {
        printf("An Error Has Occurred");
        // add FREEEE!!!!
        exit(1);
    }
    for (int i =0; i <n; i++){
        vectors[i] = malloc(d*sizeof(double));
        if(vectors[i] == NULL) { // check if there's enough memory for vectors
            printf("An Error Has Occurred");
            // for(int j = i; j >= 0; j--) {
            //     free(vectors[j]);
            // }
            // free(vectors);
            exit(1);
        }
    }
    fill_mat(input, vectors);
    double** centroids = malloc(k*sizeof(double*));  //create centroids
    if(centroids == NULL) {
        printf("An Error Has Occurred");
        // add FREEEE!!!!
        exit(1);
    }
    for (int i =0; i < k; i++){
        centroids[i] = malloc(d*sizeof(double));
        if(centroids[i] == NULL) {
            printf("An Error Has Occurred");
            // add FREEEE!!!!
            exit(1);
        }
        for (int j =0; j <d; j++){ // copy vectors values
            centroids[i][j] = vectors[i][j];
        }
    }

    int* cluster_of_vector = malloc(n*sizeof(int)); //cluster_of_vector[i]==cluster of xi vector
    double* norms = malloc(k*sizeof(double));  // oclid distance between new and old centroid
    double* new_centroid = malloc(d*sizeof(double));
    if(cluster_of_vector == NULL || norms == NULL || new_centroid == NULL) {
        printf("An Error Has Occurred");
        // add FREEEE!!!!
        exit(1);
    }
    int num_of_loops = 0;
    while(1 == 1){
        for (int i=0; i<n; i++) {  // find closest cluster for each vector
            cluster_of_vector[i] = argmin(vectors[i], centroids, k, d);
        }
                
        for (int clust_num=0; clust_num<k; clust_num++) {  // calculate new centroid and norms
            for (int j=0; j<d; j++) {
                new_centroid[j] = 0.0;
            }
            update_u(new_centroid, cluster_of_vector, vectors, d, n, clust_num);
            norms[clust_num] = sqrt(occlid_distance(new_centroid, centroids[clust_num], d));  // oclid distance between new and old centroid
            for (int j=0; j<d; j++) {  // update centroid
                centroids[clust_num][j] = new_centroid[j];
            }
        }
        num_of_loops++;
        int is_bigger_than_e = 0;
        for (int i=0; i<k; i++) {  // check if any norm is bigger than e
            if (norms[i] >= e) {
                is_bigger_than_e = 1;
                break;
            }
        }
        if (num_of_loops >= max_iter || is_bigger_than_e==0) {
            break;
        }
    }

    for (int i =0; i <n; i++){  // free vectors matrix
        free(vectors[i]);
    }
    free(vectors);
    free(cluster_of_vector);
    free(norms);
    free(new_centroid);
    write_in_file(output, centroids, k, d);
    for (int i =0; i <k; i++){  // free centroids matrix
        free(centroids[i]);
    }
    free(centroids);
    return;
}

    
void discover_n_d(char input[], int n_d[2]) {
    FILE* inputFile;
    inputFile = fopen(input, "r");
    if (inputFile == NULL) {
        printf("An Error Has Occurred");
        exit(1);
    }
    char line[1000];  // check length of max line
    int n = 0;
    int d = 0;
    while (fgets(line, 1000, inputFile)) {  //searching for n and d
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
    return;
}

void fill_mat(char input[], double** vectors) {
    FILE* inputFile;
    char line[1000];  // check max length
    int i = 0;
    int j = 0;
    inputFile = fopen(input, "r");
    if (inputFile == NULL) {
        printf("An Error Has Occurred");
        // add FREEEE!!!!
        exit(1);
    }
    while (fgets(line, 1000, inputFile)) {
        char *token = strtok(line, ",");
        while (token != NULL) {
            sscanf(token, "%lf", &vectors[i][j]);
            j++;
            token = strtok(NULL, ",");
        }
        i++;
        j = 0;
    }
    fclose(inputFile);
    return;
}

int is_number(char s[]) {  // check if string is int
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

int argmin(double x[], double** centroids, int k, int d) {  // returns the index of the cluster
    int index = 0;
    double min_distance = INFINITY;
    double distance = 0;
    for (int j=0; j<k; j++) {  // find closest centroid
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
    for (int i=0; i<n; i++) {  // finds all vectors in cluster
        if (cluster_of_vector[i]==clust_num) {
            num_vectors_in_cluster++;
            for (int j=0; j<d; j++) {
                new_centroid[j] +=vectors[i][j];
            }
        }
    }
    for (int i=0; i<d; i++) {
        new_centroid[i] = new_centroid[i] / (double)num_vectors_in_cluster;
    }
    return;
}

void write_in_file(char output[], double** centroids, int k, int d) {
    FILE * output_file = fopen(output, "w");
    if (output_file == NULL) {
        printf("An Error Has Occurred");
        // add FREEEE!!!!
        exit(1);
    }
    for (int i = 0; i<k; i++) {
        for (int j=0; j<d-1; j++) {
            fprintf(output_file, "%.4f,", centroids[i][j]);
        }
        fprintf(output_file, "%.4f\n", centroids[i][d-1]);
    }
    fclose(output_file);
}