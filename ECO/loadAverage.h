//
//  LoadAverage.h
//  ECO
//
//  Created by Antonio Caparrini on 15/3/16.
//

#include <stdio.h>
#include <string>
#include <vector>



//Class for saving the LAT
class loadAverageTriplet
{
    private:
        float l1;
        float l2;
        float l3;
        bool samplesCalculated;
        float alfa;
        float beta;
        float sigma;
        double varxn;
        double varxp;
        std::vector<float> parseResultUptime(std::string uptime);
    
    public:
        double getVxn();
        double getVxp();
        loadAverageTriplet(std::string uptime); //Build the LAT from the string output of a uptime command
        loadAverageTriplet(float l1,float l2,float l3);//Build the LAT from 3 values
        void buildSamples(); //Builds the alfa,beta and sigma values from the l1,l2,l3
        double getSampleOfDistances(); //Calculates the sample of distances between the variances of the lesssample and the most sample
    
    
};