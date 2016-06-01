//
//  main.cpp
//  ECO
//
//  Created by Antonio Caparrini on 15/3/16.
//  Copyright (c) 2016 Antonio Caparrini. All rights reserved.
//

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>

#include "loadAverage.h"
#include "pstream.h"

using namespace std;




/** 
 *  Executes the command uptime
 *  return: string with the output of the uptime command
 */
string LA(){
    //Execute command uptime and send the exit text to stdin
    redi::ipstream uptime("uptime", redi::opstream::pstdin);

    string result;

    // Read from stdin
    getline(uptime.out(), result);

    return result;
}

/**
 *  Plots the graph using the script in R
 */
void plotGraph(double s,double l,int divs,std::string drawr){
    //Execute command r for script dragraph.r
    std::string command = "r -f "+drawr+"drawGraph.r --args " +to_string(s)+ " " +to_string(l)+ " " + to_string(divs);
    std::system(command.c_str());
}

/**
 *  return: the average of the samples of distances of all the LAs in the vector v
 */
double averageVector(std::vector<loadAverageTriplet> v){
    double sum =0;
    int i;
    for (i=0; i<v.size();i++){
        sum += v[i].getSampleOfDistances();
    }
    return sum/i;
}

/**
 *  Writes a .dat file with data for draw the graph
 *  return: true if success
 */
bool writeResultFile(vector<loadAverageTriplet> myLATs){
    string ruta = "";
    string tab = "\t";
    ofstream outfile;

    outfile.open(ruta+"results.dat");
    outfile << "Vxp"+tab+"Vxn"+tab+"D"+"\n";
    outfile << setprecision(2) << fixed;
    if(outfile.is_open()){

        for (vector<loadAverageTriplet>::iterator it = myLATs.begin() ; it != myLATs.end(); ++it){
            outfile << it->getVxp() << tab;
            outfile << it->getVxn() << tab;
            outfile << it->getSampleOfDistances() <<"\n";
        }

    } else {

        outfile.close();
        return false;

    }

    outfile.close();
    return true;
}

int main(int argc, const char * argv[]) {

    system("pwd");
    int samples;
    int timeDelay;
    double landa;
    std::string uptime;
    std::string drawr;
    vector<loadAverageTriplet> myLATs;


    cout << "How many samples would I take?" << endl;
    cin >> samples; //Comprobar entrada TODO
    cout << "What is the time interval do you want them taken?" << endl;
    cin >> timeDelay; //Comprobar entrada TODO
    cout << "What is your threshold level of stability?" << endl;
    cin >> landa; //Comprobar entrada TODO
    cout << "What is the localization of the drawGraph.r?" << endl;
    cin >> drawr;

    for(int i=0; i<samples; i++){
        uptime = LA();
        loadAverageTriplet LAT = *new loadAverageTriplet(uptime);
        cout << i << "    " << uptime <<" diferencia de las var = " << LAT.getSampleOfDistances() << endl;

        myLATs.push_back(LAT);
        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(timeDelay));

    }

    cout << "La media de las diferencias de las var es : ";
    cout << averageVector(myLATs);
    cout << endl;

    cout << "Comparativa de la media con landa : ";
    cout << landa-averageVector(myLATs);
    cout << endl;

    if(!writeResultFile(myLATs)){
        cout << "Error al escribir archivo results.dat.\n";
    }

    plotGraph(averageVector(myLATs),landa,samples,drawr);

    return 0;
}
