
using namespace std;

class ConnectionsLayer {
    private:
        double initialWeight(){
            return ((double)rand()/RAND_MAX)*2-1;
        }
        double initialBias(){
            return ((double)rand()/RAND_MAX)*2-1;
        }
    public:
        ConnectionsLayer(int prevLayerSize, int nextLayerSize) :
            prevLayerSize(prevLayerSize), nextLayerSize(nextLayerSize),
            weights(nextLayerSize, vector<double>(prevLayerSize)),
            weightsCost(nextLayerSize, vector<double>(prevLayerSize)),
            biases(nextLayerSize),
            biasesCost(nextLayerSize)
        {
            for (int i=0; i<nextLayerSize; i++) {
                biases[i] = initialBias();
            }
            for (int i = 0; i<nextLayerSize; i++) {
                for (int j = 0; j<prevLayerSize; j++) {
                    weights[i][j] = initialWeight();
                }
            }
            biasesCost.assign(nextLayerSize, 0);
            for (int i = 0; i<nextLayerSize; i++) {
                weightsCost[i].assign(prevLayerSize, 0);
            }

        }

        int prevLayerSize;
        int nextLayerSize;
        vector<vector<double> > weights;
        vector<vector<double> > weightsCost;
        vector<double> biases;
        vector<double> biasesCost;

        void display() {
            printf("Biases: \n");
            displayVector(biases);
            printf("\n");
            printf("Weights:\n");
            displayVectorMatrix(weights);
        }
};
