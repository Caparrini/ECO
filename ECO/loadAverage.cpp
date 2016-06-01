//
//  LoadAverage.cpp
//  ECO
//
//  Created by Antonio Caparrini on 15/3/16.
//

#include "loadAverage.h"
#include <vector>
#include <math.h>
#include <string>
#include <iostream>

typedef std::vector<float> loads_vector; //Declaring a vector of loads

loadAverageTriplet::loadAverageTriplet(float l1,float l2,float l3){
    this->l1 = l1;
    this->l2 = l2;
    this->l3 = l3;
    buildSamples();
}

loadAverageTriplet::loadAverageTriplet(std::string uptime){
    std::vector<float> tri = parseResultUptime(uptime);
    l1 = tri[0];
    l2 = tri[1];
    l3 = tri[2];
    buildSamples();
}

void loadAverageTriplet::buildSamples(){
    alfa = ( 15 * l3 - 5 * l2 ) /10; //x1-10
    beta = ( 5 * l2 - l1 ) /4 ; //x11-14
    sigma = l1; //x15
    varxp = ( pow(alfa-l3,2) * 10 + pow(beta - l3,2) * 4 + pow(l1-l3,2) ) / 14;
    varxn = ( 12*pow(l3,2) + pow(10*alfa-l3,2) + pow(4*beta-l3,2) + pow(l1-l3,2) ) /14 ;
    samplesCalculated = true;
}

std::vector<float> loadAverageTriplet::parseResultUptime(std::string uptime){

    std::vector<float> tri(3);
    std::string delimiter = "load averages: ";
    std::string space = " ";
    std::string values = uptime.substr(uptime.find(delimiter)+delimiter.length());
    std::string token;
    size_t pos =0;
    int i =0;
    while ((pos = values.find(space)) != std::string::npos) {
        token = values.substr(0, pos);
        tri[i] = (float)atof(token.c_str());
        values.erase(0, pos + space.length());
        i++;
    }
    tri[i] = (float)atof(values.c_str());
    return tri;
}

double loadAverageTriplet::getSampleOfDistances(){
    if (samplesCalculated){
        return (90 * pow(alfa,2) + 12 * pow(beta,2) ) / 14;
    }else{
        return -1;
    }

}

double loadAverageTriplet::getVxn(){
    return varxn;
}

double loadAverageTriplet::getVxp(){
    return varxp;
}
