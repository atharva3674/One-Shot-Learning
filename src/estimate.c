#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// method headers
double **multiply(double **matrixA, double **matrixB, int dimRowA, int dimColA, int dimRowB, int dimColB);
double **transpose(double **matrixA, int dimRow, int dimCol, double **transposeMatrix); 
double **inverse(double **matrixA, int dimRow, int dimCol);
void printFinal(double **matrix, int dimRow, int dimCol);
void freeMatrix(double **matrix, int rows);

double **multiply(double **matrixA, double **matrixB, int dimRowA, int dimColA, int dimRowB, int dimColB){
    double **productMatrix = malloc(sizeof(double*) * dimRowA);
    int i, j, k;
    double temp = 0.0;
    for(i = 0; i < dimRowA; i++){
        productMatrix[i] = malloc(sizeof(double) * dimColB);
    }
    for(i = 0; i < dimRowA; i++){
        for(j = 0; j < dimColB; j++){
            for(k = 0; k < dimColA; k++){
                temp = temp + matrixA[i][k] * matrixB[k][j];
            }
            productMatrix[i][j] = temp;
            temp = 0;
        }
    }
    return productMatrix;
}

double **transpose(double **matrixA, int dimRow, int dimCol, double **transposeMatrix){
    int i, j;
    for(i = 0; i < dimRow; i++){
        for(j = 0; j < dimCol; j++){
            transposeMatrix[j][i] = matrixA[i][j]; 
        }
    }
    return transposeMatrix;
}

double **inverse(double **matrixA, int dimRow, int dimCol){
    double **identityMatrix = malloc(sizeof(double*) * dimRow);
    int i, j, k, p;
    double f;

    for(i = 0; i < dimRow; i++){
        identityMatrix[i] = malloc(sizeof(double) * dimCol);
    }
    for(i = 0; i < dimRow; i++){
        for(j = 0; j < dimRow; j++){
            if(i == j){
                identityMatrix[i][j] = 1.0;
            }
            else{
                identityMatrix[i][j] = 0.0;
            }
        }
    }

    for(p = 0; p < dimRow; p++){
        f = matrixA[p][p];

        // divide matrix A and identity matrix by f
        for(j = 0; j < dimRow; j++){
            matrixA[p][j] = matrixA[p][j] / f;
            identityMatrix[p][j] = identityMatrix[p][j] / f;
        }
        for(i = p + 1; i < dimRow; i++){
            f = matrixA[i][p];

            // subtract product of Ap and f from Ai
            for(k = 0; k < dimRow; k++){
                matrixA[i][k] = matrixA[i][k] - (matrixA[p][k] * f);
                identityMatrix[i][k] = identityMatrix[i][k] - (identityMatrix[p][k] * f);  
            }
        }
    }

    for(p = dimRow - 1; p >= 0; p--){
        for(i = p - 1; i >= 0; i--){
            f = matrixA[i][p];

            // subtract product of Ap and f from Ai
            for(j = 0; j < dimRow; j++){
                matrixA[i][j] = matrixA[i][j] - (matrixA[p][j] * f);
                identityMatrix[i][j] = identityMatrix[i][j] - (identityMatrix[p][j] * f);   
            }
        }
    }


    return identityMatrix;
}

void printFinal(double **matrix, int dimRow, int dimCol){
    int i, j;
    for(i = 0; i < dimRow; i++){
        for(j = 0; j < dimCol; j++){
            printf("%.0f\n", matrix[i][j]);
            if(j + 1 != dimCol){
                printf(" ");
            }
        }    
    }
}

void freeMatrix(double **matrix, int rows){
    int i;
    for(i = 0; i < rows; i++){
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char* argv[]){

    // loop variables 
    int i = 0;
    int j = 0;
    
    // read input files and check to make sure not null
    FILE *inputFileTrain;
    FILE *inputFileData;
    inputFileTrain = fopen(argv[1], "r");
    inputFileData = fopen(argv[2], "r");
    
    // buffer to get rid of the word train and data
    char bufferTrain[1000];
    char bufferData[1000];
    fgets(bufferTrain, 1000, inputFileTrain);
    fgets(bufferData, 1000, inputFileData);
    // fgets(bufferData, 100, inputFileData);
    
    // check if files are null
    if (inputFileTrain  == NULL || inputFileData  == NULL )
        return EXIT_FAILURE;
    
    // read in dimensions of training data and data
    int attributes = 0;
    int houses = 0;
    int attributesData = 0;
    int housesData = 0;
    
    //read in attributes and houses
    fscanf(inputFileTrain, "%d\n", &attributes);
    fscanf(inputFileTrain, "%d\n", &houses);
    fscanf(inputFileData, "%d\n", &attributesData);
    fscanf(inputFileData, "%d\n", &housesData);

    /*
    printf("%d", attributes);
    printf("\n");
    printf("%d", houses);
    printf("\n");
    */
    
    // get dimensions of training data X and Y and for actual data
    int dimRowX = houses;
    int dimColX = attributes + 1;
    int dimRowY = houses;
    int dimColY = 1;
    int dimRowXData = housesData;
    int dimColXData = attributesData + 1;

    // create matrix X and Y for training data and allocate memory and create X and Y for Data
    double **trainX = malloc(sizeof(double *) * dimRowX);
    double **trainY = malloc(sizeof(double *) *  dimRowY);
    double **dataX = malloc (sizeof(double *) * dimRowXData);

    for(i = 0; i < dimRowX; i++){
        trainX[i] = malloc(dimColX * sizeof(double));
    }
    for(i = 0; i < dimRowY; i++){
        trainY[i] = malloc(dimColY * sizeof(double));
    }
    for(i = 0; i < dimRowXData; i++){
        dataX[i] = malloc(dimColXData * sizeof(double *));
    }
   
    // initalize X with 1s for training and actual data
    for(i = 0; i < dimRowX; i++){
        trainX[i][0] = 1;
    }
    for(i = 0; i < dimRowXData; i++){
        dataX[i][0] = 1;
    }

    // read in values for matrix X and Y from training data and actual data
    for(i = 0; i < houses; i++){
        for(j = 0; j < attributes; j++){ // 0 1 2 3 4 and exits at 5. On 4 check to put in matrix Y
            fscanf(inputFileTrain,"%lf", &trainX[i][j + 1]);
        }
        fscanf(inputFileTrain,"%lf", &trainY[i][0]);
        fscanf(inputFileTrain, "\n");
    }

    for(i = 0; i < housesData; i++){
        for(j = 0; j < attributesData; j++){ 
            fscanf(inputFileData,"%lf", &dataX[i][j + 1]);
        }
        // fscanf(inputFileData,"%lf", &dataY[i][0]);
        fscanf(inputFileData, "\n");
    }

    // printMatrix(trainX, dimRowX, dimColX);
    // printf("\n");
     
    // Entire Prcedure to find W

    // tranpose matrix X
    double **transposeMatrix = malloc(sizeof(double*) * dimColX);
    for(i = 0; i < dimColX; i++){
        transposeMatrix[i] = malloc(sizeof(double) * dimRowX );   // heap buffer
    }

    double **transposeX = transpose(trainX, dimRowX, dimColX, transposeMatrix);
    int dimRowTransposeX = dimColX;
    int dimColTransposeX = dimRowX;

    // printMatrix(transposeX, dimRowTransposeX, dimColTransposeX);
    // printf("\n");

    // Multiply X transpose and X
    double **productXTransposeX = multiply(transposeX, trainX, dimRowTransposeX, dimColTransposeX, dimRowX, dimColX);
    int dimProductXTransposeXRow = dimRowTransposeX;
    int dimProductXTransposeXCol = dimColX;

    // printf("%d", dimProductXTransposeXRow);
    // printf("%d", dimProductXTransposeXCol);
    // printf("\n");
    // printMatrix(productXTransposeX, dimProductXTransposeXRow, dimProductXTransposeXCol);
    // printf("\n");

    // inverse
    double **inverseMatrix = inverse(productXTransposeX, dimProductXTransposeXRow, dimProductXTransposeXCol);
    int dimInverseRow = dimProductXTransposeXRow;
    int dimInverseCol = dimProductXTransposeXCol;

    // printMatrix(inverseMatrix, dimInverseRow, dimInverseCol);
    // printf("\n");

    // inverse times X transpose
    double **inverseTranspose = multiply(inverseMatrix, transposeX, dimInverseRow, dimInverseCol, dimRowTransposeX, dimColTransposeX);
    int dimInverseTransposeRow = dimInverseRow;
    int dimInverseTransposeCol = dimColTransposeX;

    // printMatrix(inverseTranspose, dimInverseTransposeRow, dimInverseTransposeCol);
    // printf("\n");
     
    // inverse transpose times Y to finally get matrix W
    double **W = multiply(inverseTranspose, trainY, dimInverseTransposeRow, dimInverseTransposeCol, dimRowY, dimColY);
    int dimRowW = dimInverseTransposeRow;
    int dimColW = dimColY;

    // printf("\n");
    // printMatrix(W, dimRowW, dimColW);
    // printf("\n");
    // printMatrix(dataX, dimRowXData, dimColXData); 
    
    // W times X to get matrix Y 
    double **Y = multiply(dataX, W, dimRowXData, dimColXData, dimRowW, dimColW);
    int dimRowYData = dimRowXData;
    int dimColYData = dimColW;
     
    // print out Y
    printFinal(Y, dimRowYData, dimColYData);
    
    // free matrices
    freeMatrix(transposeMatrix, dimRowTransposeX);
    freeMatrix(productXTransposeX, dimProductXTransposeXRow);
    freeMatrix(inverseMatrix, dimInverseRow);
    freeMatrix(inverseTranspose, dimInverseTransposeRow);
    freeMatrix(W, dimRowW);
    freeMatrix(Y, dimRowYData);
    freeMatrix(dataX, dimRowXData);
    freeMatrix(trainX, dimRowX);
    freeMatrix(trainY, dimRowY);
   
    // close files
    fclose(inputFileTrain);
    fclose(inputFileData);

    return 0;

}