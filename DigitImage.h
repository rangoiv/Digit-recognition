
using namespace std;

const char *SHADES = ".,-~:;=!*#$@"; //12

class DigitImage {
    private:
        vector<vector<unsigned char> > matrix;
        ifstream *myfile;
        int rows;
        int columns;
    public:
        DigitImage(int rows, int columns, ifstream *myfile) :
        rows(rows), columns(columns),
        myfile(myfile),
        matrix(rows, vector<unsigned char>(columns)) {   }

        void load() {
            for (int i = 0; i<rows; i++) {
                for (int j = 0; j<columns; j++) {
                    char ch;
                    myfile->read (&ch, 1);
                    matrix[i][j] = ch;
                }
            }
        }
        void display() {
            for (int i = 0; i<rows; i++) {
                for (int j = 0; j<columns; j++) {
                    unsigned char shade = matrix[i][j];

                    printf("%c", SHADES[shade/22]);
                }
                printf("\n");
            }
        }
        void assignToVector(vector<double> *A) {
            if (rows*columns != A->size()) {
                cout << "wrong size!";
                return;
            }

            for (int i = 0; i<rows; i++) {
                for (int j = 0; j<columns; j++) {
                    A->at(i*columns + j) = (double)matrix[i][j]/255;
                }
            }
        }
};

