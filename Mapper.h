#ifndef Mapper_H
#define Mapper_H

#include <string>
#include <vector>
using namespace std;
class Mapper
{
    private:
        vector<vector<double>> dataset;
        int datasetCols;
        int datasetRows;
    public:
        Mapper();
        Mapper(string);
        void loadFile(string);
        void normalize();
        void Map(const string&);
        double nearest_neighbor(const vector<int>, int, int);
        vector<vector<int>> backwards(vector<int>);
        vector<vector<int>> forwards(vector<int>);
        void print(const vector<int>&);
        double fullSetAccuracy();
        vector<double> tokenizeAndconverToDouble(const string&);
        int getFeatures(); 
        int getInstances(); 
};
#endif