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




//Executes the command uptime and returns and string with the result
string LA(){
    //Execute command uptime and send the exit text to stdin
    redi::ipstream uptime("uptime", redi::opstream::pstdin);

    string result;

    // Read from stdin
    getline(uptime.out(), result);

    return result;
}

void plotGraph(double s,double l,int divs){
    //Execute command r for script dragraph.r
    std::string command = "r -f /Users/Capa/College/LAStability/LAStability/LAStability/drawGraph.r --args " +to_string(s)+ " " +to_string(l)+ " " + to_string(divs);
    std::system(command.c_str());
}

double averageVector(std::vector<loadAverageTriplet> v){
    double sum =0;
    int i;
    for (i=0; i<v.size();i++){
        sum += v[i].getSampleOfDistances();
    }
    return sum/i;
}

bool writeResultFile(vector<loadAverageTriplet> myLATs){
    string ruta = "/Users/Capa/College/LAStability/LAStability/LAStability/";
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
    vector<loadAverageTriplet> myLATs;


    cout << "How many samples would I take?" << endl;
    cin >> samples; //Comprobar entrada TODO
    cout << "What is the time interval do you want them taken?" << endl;
    cin >> timeDelay; //Comprobar entrada TODO
    cout << "What is your threshold level of stability?" << endl;
    cin >> landa; //Comprobar entrada TODO

    for(int i=0; i<samples; i++){
        uptime = LA();
        loadAverageTriplet LAT = *new loadAverageTriplet(uptime);
        cout << i << "    " << uptime <<" media = " << LAT.getSampleOfDistances() << endl;

        myLATs.push_back(LAT);
        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(timeDelay));

    }

    cout << "La media es : ";
    cout << averageVector(myLATs);
    cout << endl;

    cout << "Comparativa con landa : ";
    cout << landa-averageVector(myLATs);
    cout << endl;

    if(!writeResultFile(myLATs)){
        cout << "Error al escribir archivo results.dat.\n";
    }

    plotGraph(averageVector(myLATs),landa,samples);

    return 0;
}
