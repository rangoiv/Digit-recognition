#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <math.h>
#include "vectors.h"
#include "ConnectionLayer.h"
#include "NeuronLayer.h"
#include "DigitImage.h"
using namespace std;

#define ROWS 28
#define COLUMNS 28

#define LAYERS 3

const int LAYER_SIZES[LAYERS] = {ROWS*COLUMNS, 40, 10};
const int MINIBATCHES = 10;
const double SLOPE = 1.2;
bool DEBUG = false;

class NeuralNetwork {
    private:

        vector<NeuronLayer> neuronLayers;
        vector<ConnectionsLayer> connectionsLayers;
        vector<double> desiredValues;
        char currentLabel;
        double cost;
        ifstream trainImagesFile;
        ifstream trainLabelsFile;
        DigitImage digitImage = DigitImage(ROWS, COLUMNS, &trainImagesFile);

        //DONE
        double D_cost_value (int L, int k) {
            if (L==LAYERS-1) {
                return 2 * (neuronLayers[LAYERS-1].values[k]- desiredValues[k]);
            }

            double s = 0;
            for (int j = 0; j<LAYER_SIZES[L+1]; j++) {
                s += D_unactivatedValue_value(L, k, j) * D_value_unactivatedValue(L+1, j) * neuronLayers[L+1].valuesCost[j];
            }
            return s;
        }
        //DONE
        double D_value_unactivatedValue (int L, int k) {
            return derivedActivationFunction(neuronLayers[L].unactivatedValues[k]);
        }
        //DONE
        double D_unactivatedValue_weight (int L, int k, int j) {
            return neuronLayers[L].values[k];
        }
        //DONE
        double D_unactivatedValue_value (int L, int k, int j) {
            return connectionsLayers[L].weights[j][k];
        }

        //DONE
        double D_cost_weight (int L, int k, int j) {
            return D_unactivatedValue_weight(L,k,j) * D_value_unactivatedValue(L+1, j) * neuronLayers[L+1].valuesCost[j];
        }
        //DONE, takes place after calculating D_cost_value(L,k)
        double D_cost_bias (int L, int k) {
            return D_value_unactivatedValue(L, k) * neuronLayers[L].valuesCost[k];
        }



    public:
        NeuralNetwork() {
            srand (time(NULL));
            trainImagesFile.open("train-images.idx3-ubyte", ios::binary);
            trainLabelsFile.open("train-labels.idx1-ubyte", ios::binary);



            for (int i=0; i<LAYERS; i++) {
                neuronLayers.push_back(NeuronLayer(LAYER_SIZES[i]));
            }
            for (int i=0; i<LAYERS-1; i++) {
                connectionsLayers.push_back(ConnectionsLayer(LAYER_SIZES[i], LAYER_SIZES[i+1]));
            }
        }
        ~NeuralNetwork() {
            trainImagesFile.close();
        }

        void begin(){
            char ch;
            for (int i=0; i<16; i++) {
                trainImagesFile.read (&ch, 1);
            }
            for (int i=0; i<8; i++) {
                trainLabelsFile.read (&ch, 1);
            }

        }
        void load() {
            digitImage.load();
            digitImage.assignToVector(&neuronLayers[0].values);

            trainLabelsFile.read (&currentLabel, 1);
            desiredValues.assign(10, 0);
            desiredValues[currentLabel] = 1;
        }
        void evaluate() {
            for (int i = 0; i < LAYERS-1; i++) {
                multiplyVector(neuronLayers[i].values, connectionsLayers[i].weights, &neuronLayers[i+1].unactivatedValues);
                addVector(connectionsLayers[i].biases, neuronLayers[i+1].unactivatedValues, &neuronLayers[i+1].unactivatedValues);
                activateVector(neuronLayers[i+1].unactivatedValues, &neuronLayers[i+1].values);
            }
            cost = 0;
            for (int i = 0; i < LAYER_SIZES[LAYERS-1]; i++) {
                cost += pow(neuronLayers[LAYERS-1].values[i] - desiredValues[i],2);
            }
        }
        void displayLastStep() {
            digitImage.display();
            printf("Desired label: %d\n", currentLabel);
            printf("Network label: %d\n", max(neuronLayers[LAYERS-1].values));

            printf("Network output:");
            displayVector(neuronLayers[LAYERS-1].values);
            cout << "Desired values: ";
            displayVector(desiredValues);
        };

        void backpropagateLayer(int L) {
            for (int j = 0; j<LAYER_SIZES[L+1]; j++) {
                neuronLayers[L+1].valuesCost[j] = D_cost_value(L+1, j);
            }
            for (int j = 0; j<LAYER_SIZES[L+1]; j++) {
                connectionsLayers[L].biasesCost[j] +=  D_cost_bias(L+1, j);
            }
            for (int k = 0; k<LAYER_SIZES[L]; k++) {
                for (int j=0; j<LAYER_SIZES[L+1]; j++){
                    connectionsLayers[L].weightsCost[j][k] +=  D_cost_weight(L, k, j);
                }
            }
        }

        void addCosts(){
            for (int i=0; i<LAYERS-1; i++){
                multiplyVectorMatrix(-SLOPE / MINIBATCHES, connectionsLayers[i].weightsCost, &connectionsLayers[i].weightsCost);
                multiplyVector(-SLOPE / MINIBATCHES, connectionsLayers[i].biasesCost, &connectionsLayers[i].biasesCost);

                addVectorMatrix(connectionsLayers[i].weights, connectionsLayers[i].weightsCost, &connectionsLayers[i].weights);
                addVector(connectionsLayers[i].biases, connectionsLayers[i].biasesCost, &connectionsLayers[i].biases);

                multiplyVectorMatrix(0, connectionsLayers[i].weightsCost, &connectionsLayers[i].weightsCost);
                multiplyVector(0, connectionsLayers[i].biasesCost, &connectionsLayers[i].biasesCost);

            }
        }

        void backpropagate() {
            for (int i=LAYERS-2; i>=0; i--) {
                backpropagateLayer(i);
            }
        };

        void test(int x) {
            cout << "Testing: " << x << endl;
            for (int i = 0; i<x; i++) {
                load();
                evaluate();
                displayLastStep();

            }
        }
        void train(int x) {
            int totalCorrect = 0;

            cout << "Training: " << x << "x" << MINIBATCHES << endl;
            for (int i = 0; i<x; i++) {
                for (int j = 0; j<MINIBATCHES; j++) {
                    load();

                    evaluate();

                    if (max(neuronLayers[LAYERS-1].values) == currentLabel) {totalCorrect += 1;};

                    if (DEBUG) {
                        displayLastStep();
                    }

                    backpropagate();

                }
                addCosts();

            }
            cout << "Correct answers: " << totalCorrect << "/" << x*MINIBATCHES << endl;
        }

        void display() {
            for (int L = 0; L < LAYERS-1; L++) {
                cout << endl << "Weights: ";
                displayVectorMatrix(connectionsLayers[L].weights);
                cout << "Biases: ";
                displayVector(connectionsLayers[L].biases);
                cout << endl;
            }
        }
};

int main () {
    DEBUG = false;
    NeuralNetwork network;
    network.begin();

    for (int i=0; i<20; i++) {
        network.train(100);
    }

    while (true) {
        char command;
        int x;

        scanf("%c %d", &command, &x);
        if (command == 'X') {
            return 0;
        }
        if (command == 'T') {
            network.train(x);
        }
        if (command == 'E') {
            network.test(x);
        }
    }
    return 0;
}
