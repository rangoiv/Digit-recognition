
using namespace std;

class NeuronLayer {
    private:

    public:
        NeuronLayer(int size) :
            size(size),
            values(size),
            valuesCost(size),
            unactivatedValues(size)
        {
            values.assign(size, 0);
            valuesCost.assign(size, 0);
            unactivatedValues.assign(size, 0);
        }
        int size;
        vector<double>values;
        vector<double>unactivatedValues;
        vector<double>valuesCost;

        display(){
            displayVector(values);
        }
};

