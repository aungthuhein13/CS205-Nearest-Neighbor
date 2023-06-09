#include <iostream>
#include <string>
#include <vector>
#include "Mapper.h"
#include <ctime>
using namespace std;

int main()
{
    int user = 0;
    string testFile;

    cout << "Welcome to Aung Thu Hein's Feature Selection Algorithm.\n"
         << "Type in the name of the file to test:  ";
    
    cin >> testFile;
    Mapper Mapper(testFile);
    
   cout << "Type the number of the algorithm to run:\n" 
         << "    1) Forward Selection\n" 
         << "    2) Backward Elimination\n";

    cin >> user;

    cout << "\nThis dataset has " << Mapper.getFeatures()
         << " features (not including the class attribute), with " 
         << Mapper.getInstances() << " instances.\n"
         << "Running nearest neighbor with all " <<  Mapper.getFeatures() 
         << " features, using 'leaving-one-out' evaluation, I getan accuracy of " << Mapper.fullSetAccuracy() << "%\n\n";

    clock_t start = clock();

    switch(user) {
        case 1:
            Mapper.Map("forwards");
            break;
        case 2:
            Mapper.Map("backwards");
            break;
        default:
            cout << "Invalid choice, exiting.\n";
            return -1;
    }
    double duration = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
    cout<<"Duration: "<< duration << " seconds" << endl;
    
    return 0;
}