#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_DATA 1000 // Maximum number of data entries

// Structure to hold digester sensor data
typedef struct {
    double latitude;
    double longitude;
    double temperature;
    double pH;
    double gasProduction;
} DigesterData;

// Function to read CSV data
int readData(const char *filename, DigesterData data[], int maxData) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    int count = 0;
    while (fscanf(file, "%lf,%lf,%lf,%lf,%lf", 
                  &data[count].latitude, &data[count].longitude, 
                  &data[count].temperature, &data[count].pH, 
                  &data[count].gasProduction) == 5) {
        count++;
        if (count >= maxData) break;
    }
    fclose(file);
    return count;
}

// Function to calculate mean
double mean(double arr[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

// Function to calculate variance
double variance(double arr[], int size, double meanVal) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += pow(arr[i] - meanVal, 2);
    }
    return sum / size;
}

// Simple linear regression prediction for gas production
double predictGasProduction(double temperature, double pH) {
    // Assuming a simple linear model: GasProduction = (0.5 * temperature) + (0.3 * pH) + 2
    return (0.5 * temperature) + (0.3 * pH) + 2;
}

int main() {
    DigesterData data[MAX_DATA];
    int dataSize = readData("digester_data.csv", data, MAX_DATA);
    
    if (dataSize == 0) {
        printf("No data found!\n");
        return 1;
    }
    
    // Extract temperature and gas production for analysis
    double tempArray[MAX_DATA], gasArray[MAX_DATA];
    for (int i = 0; i < dataSize; i++) {
        tempArray[i] = data[i].temperature;
        gasArray[i] = data[i].gasProduction;
    }
    
    // Compute basic statistics
    double meanTemp = mean(tempArray, dataSize);
    double varTemp = variance(tempArray, dataSize, meanTemp);
    
    printf("Mean Temperature: %.2lf\n", meanTemp);
    printf("Temperature Variance: %.2lf\n", varTemp);
    
    // Predict gas production for a given input
    double testTemp = 35.0;
    double testPH = 7.0;
    double predictedGas = predictGasProduction(testTemp, testPH);
    printf("Predicted Gas Production for Temp=%.1lf and pH=%.1lf: %.2lf\n", testTemp, testPH, predictedGas);
    
    return 0;
}