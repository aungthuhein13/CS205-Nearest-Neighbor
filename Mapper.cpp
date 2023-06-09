#include "Mapper.h"
#include <iostream> 
#include <sstream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <cmath>
using namespace std;

Mapper::Mapper(): datasetCols(0), datasetRows(0) {}

Mapper::Mapper(string filename) : datasetCols(0), datasetRows(0){
    loadFile(filename);
    this->normalize();
}

void Mapper::loadFile(string filename){
    size_t id = 0;
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }

    vector<string> tempDataset;
    string line;

    while (getline(file, line)) {
        tempDataset.push_back(to_string(id++) + line);
    }
    this->datasetRows = id;
    file.close();

    //cout << tempDataset.at(2) << endl;
    if(tempDataset.empty()){
        throw runtime_error("File"+ filename + "is empty");
    }

    stringstream ss(tempDataset.at(0));
    vector<string> tempColumns;
    string item;
    while (ss >> item) {
        tempColumns.push_back(item);
    }
    this->datasetCols = tempColumns.size();

    for(const auto& rowStr: tempDataset){
        vector<double> row = tokenizeAndconverToDouble(rowStr);
        if(!row.empty()){
            this->dataset.push_back(row);
        }
    }
}

vector<double> Mapper::tokenizeAndconverToDouble(const string& rowStr){
        stringstream ss(rowStr);
        vector<double> row;
        string item;
        while (ss >> item) {
           row.push_back(stod(item));
           //cout << row.at(i++) << endl;
           //cout << "Sucessfully opened and read the file.\n";
        }
    return row;
}

void Mapper::normalize(){
    vector<double> avg(this->datasetCols,0.0);
    vector<double> std(this->datasetCols,0.0);

    // calculate avgs
    for(int i = 2; i < this->datasetCols; ++i)
    {
        for(int j = 0; j < this->datasetRows; ++j)
        {
            avg[i] += this->dataset[j][i];
        }
        avg[i] /= this->datasetRows;
    }

    // calcualte standard deviations
    for(int i = 2; i < this->datasetCols; ++i){
        for(int j = 0; j < this->datasetRows; ++j) {
            std[i] += pow(this->dataset[j][i] - avg[i], 2);
        }
        std[i] = sqrt(std[i] / this->datasetRows);
    }

    for(int i = 2; i < this->datasetCols; ++i){
        for(int j = 0; j < this->datasetRows; ++j) {
            this->dataset[j][i] = (this->dataset[j][i] - avg[i] / std[i]);
        }
    }
}

void Mapper::Map(const string& algo){
    int maxCombos = 10; // maximum feature combinations to evaluate
    vector<int> selectedFeatures = (algo == "backwards") ? vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10} : vector<int>{};
    
    cout << "Beginning search." << endl << endl;

    vector<int> bestFeatures;
    double bestAccuracy = 0.0;

    for(int j = 0; j < maxCombos; ++j){
        double prevAccuracy = bestAccuracy;
        vector<vector<int>> featureSets = (algo == "backwards") ? backwards(selectedFeatures) : forwards(selectedFeatures);

        for(auto& features: featureSets){
            cout << "   Using feature(s) ";
            print(features);

            int correct = count_if(dataset.begin(), dataset.end(), [&](const vector<double>& instance) {
                return nearest_neighbor(features, features.size(), &instance - &dataset[0]) == instance[1];
            });

            double accuracy = static_cast<double>(correct) / datasetRows;
            cout << " accuracy is " << accuracy << "%" << endl;
            if(accuracy > bestAccuracy) {
                bestAccuracy = accuracy;
                selectedFeatures = features;
            }
        }

        cout << endl;

        if(bestAccuracy <= prevAccuracy) {
            cout << "Finished Search!! The best feature subset is ";
            print(bestFeatures);
            cout << ", with an accuracy of " << bestAccuracy * 100 << "%" << endl << endl;
            break;
        } 
        else {
            bestFeatures = selectedFeatures;
            cout << "Feature set: ";
            print(bestFeatures);
            cout << " was best, accuracy is " << bestAccuracy * 100 << "%" << endl;
        }
    }
}


void Mapper::print(const vector<int>& features)
{
    cout << "{";
    bool first = true;
    for(const auto& feature: features){
        if(!first){
            cout << ",";
        }
        cout << feature;
       first = false;
    }
    cout << "}";
}

vector<vector<int>> Mapper::forwards(vector<int> features)
{
    vector<vector<int>> set;

    for(int i = 0; i < this->datasetCols - 2; ++i){
        if(find(features.begin(), features.end(), i+1) == features.end()){
            vector<int> temp = features;
            temp.push_back(i + 1);
            set.push_back(temp);
        }
    }

    return set;
}

vector<vector<int>> Mapper::backwards(vector<int> features){
    vector<vector<int>> set;

    for(auto it = features.begin(); it != features.end(); ++it  ){
        vector<int> temp(features);
        temp.erase(temp.begin() + distance(features.begin(),it));
        set.push_back(temp);
    }

    return set;
}


double Mapper::nearest_neighbor(const vector<int> features, int num_features, int instance_ID){
    double minDist = numeric_limits<double>::max();
    double num = -1.0;

    for(int i = 0; i < this->datasetRows; ++i){
        if(i == instance_ID)
            continue;

        double dist = 0.0;
        for(int j = 0; j < num_features; ++j){
            double diff = dataset[i][features[j] + 1] - dataset[instance_ID][features[j] + 1];
            dist += diff * diff;
        }

        dist = sqrt(dist);
        
        if(dist < minDist){
            minDist = dist;
            num = dataset[i][1];
        }
    }
    return num;
}

int Mapper::getFeatures(){
    return this->datasetCols - 2;
}

int Mapper::getInstances(){
    return this->datasetRows;
}

double Mapper::fullSetAccuracy(){
    vector<int> set(getFeatures());
    iota(set.begin(),set.end(),1);

    int temp = 0;

    for(int i = 0; i < set.size(); ++i){
        if(nearest_neighbor(set, set.size(), i) == dataset[i][1])
        {
            ++temp;
        }
    }
    return static_cast<double>(temp) / datasetRows;
}