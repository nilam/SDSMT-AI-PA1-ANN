/*=====================================================================
Program:
Author:
Class:
Instructor:
Date:
Description:
Input:
Output:
Compilation instructions:
Usage:
Modifications:
Date Comment
---- ------------------------------------------------
======================================================================*/

#include "weatherANN.h"


//using namespace std;


/*====================== global symbolic constants ===================*/
/*====================== global type definitions =====================*/

/*====================== global enum definitions =====================*/

/*====================== global struct definitions ===================*/

//struct neuralnet
//{
//	double learnrate;
//	double momentum;
//	double threshold;
//	int layers;
//	int* numofnodes;
//	double* weights;
//	double* biasweights;
//};


void initialize_neural_net(neuralnet &net, double learnrate, double momentum, double threshold, int layers, int* numofnodes)
{
	int nodecount = 0;
	int weightcount = 0;
	int biascount = 0;
    
	double*** weights;
	double* biasweights;
    weights = new (nothrow) double**[layers-1];
	int* copyofnon = new (nothrow) int[layers];
	for (int i = 0; i < layers; i++)
	{
		if (i < layers-1)
		{
            weights[i] = new (nothrow) double*[numofnodes[i]];
			weightcount = numofnodes[i] * numofnodes[i+1];
            for (int j = 0; j < numofnodes[i]; j++)
            {
                weights[i][j] = new (nothrow) double[numofnodes[i+1]];
            }
			
		}
		
		copyofnon[i] = numofnodes[i];
		nodecount = nodecount + numofnodes[i];
		
	}

	
	biasweights = new (nothrow) double[biascount];

	


	srand( time(NULL) );

	//int halfRAND_MAX = RAND_MAX / 2;
    for (int i = 0; i < layers-1; i++)
    {
        for (int j = 0; j < numofnodes[i]; j++)
        {
            for (int k = 0; k < numofnodes[i+1]; k++)
            {
                int rnum = rand() % 1000;
		        rnum = rnum - 500;
		        double w = (double) rnum / 500.0;
                weights[i][j][k] = w;
            }
        }
    }
	/*for (int i = 0; i < weightcount; i++)
	{
		int rnum = rand() % 1000;
		rnum = rnum - 500;
		double w = (double) rnum / 500.0;
		weights[i] = w;

		if (i < biascount)
		{
			rnum = rand() % 1000;
			rnum = rnum - 500;
			w = (double) rnum / 500.0;
			biasweights[i] = w;
		}
	}*/


	net.biasweights = biasweights;
	net.layers = layers;
	net.learnrate = learnrate;
	net.momentum = momentum;
	net.numofnodes = copyofnon;
	net.threshold = threshold;
	net.weights = weights;
	net.nodecount = nodecount;
	net.weightcount = weightcount;
	net.biasweightcount = biascount;
    net.MSE = 0.0;
    net.RMS = 0.0;
    net.SE = 0.0;
    net.traincount = 0;
}

double sigmoid_fun(double x)
{
	return  1.0 / (1.0 + exp(-x));
}

double sfderiv(double fx)
{
	return fx * ( 1 - fx);
}






void train_sample(neuralnet &net, double* desiredvals, double* inputvals)
{
	
	double ***updatedweights;
    double **erroramt;

	//output layer training
    updatedweights = new (nothrow) double**[net.layers-1];
	int outputlayernum = net.layers-1;
	int weightposition = 0;	
	int outputnodes = net.numofnodes[outputlayernum];
    double **nodeoutputs = new (nothrow) double*[net.layers];
    erroramt = new (nothrow) double*[net.layers];
    //updatedweights[outputlayernum-1] = new (nothrow) double*[net.numofnodes[outputlayernum-1]];
    nodeoutputs[0] = new (nothrow) double[net.numofnodes[0]];
    for (int i = 0; i < net.numofnodes[0]; i++)
    {
        nodeoutputs[0][i] = inputvals[i];
     //   cout << inputvals[i] << ' ';
    }
   // cout << '\n';
    for (int i = 0; i < net.layers-1; i++)
    {
        updatedweights[i] = new (nothrow) double*[net.numofnodes[i]];
    }
    for (int j = 0; j < net.numofnodes[outputlayernum-1]; j++)
    {
        updatedweights[outputlayernum-1][j] = new (nothrow) double[net.numofnodes[outputlayernum]];
    }

    for (int layer = 0; layer < net.layers-1; layer++)
    {
        
        nodeoutputs[layer+1] = new (nothrow) double[net.numofnodes[layer+1]];
        
        for (int j = 0; j < net.numofnodes[layer+1]; j++)
        {
            
            double summation = 0.0;
            for (int i = 0; i < net.numofnodes[layer]; i++)
            {
                
                summation = summation + nodeoutputs[layer][i] * net.weights[layer][i][j];
                
            }
       //     cout << summation;
            nodeoutputs[layer+1][j] = sigmoid_fun(summation);
       //     cout << sigmoid_fun(summation);
        }
       
    }

    erroramt[outputlayernum] = new (nothrow) double[net.numofnodes[outputlayernum]];
    for (int j = 0; j < net.numofnodes[outputlayernum]; j++)
    {
        
        double yj = nodeoutputs[outputlayernum][j];
       // cout << yj;
        double dj = desiredvals[j];

        net.SE = net.SE + (dj - yj) * (dj - yj);
        double deltaj = (dj - yj) * yj * ( 1 - yj);
        erroramt[outputlayernum][j] = deltaj;
        for (int i = 0; i < net.numofnodes[outputlayernum-1]; i++)
        {
            double updatedweight = net.weights[outputlayernum-1][i][j] + net.learnrate * deltaj * nodeoutputs[outputlayernum][j];
            updatedweights[outputlayernum-1][i][j] = updatedweight;
        }
    }
    
    for (int lyr = outputlayernum-1; lyr > 0; lyr--)
    {
        erroramt[lyr] = new (nothrow) double[net.numofnodes[lyr]];
        updatedweights[lyr-1] = new (nothrow) double*[net.numofnodes[lyr-1]];
        for (int b = 0; b < net.numofnodes[lyr-1]; b++)
        {
            updatedweights[lyr-1][b] = new (nothrow) double[net.numofnodes[lyr]];
        }
        for (int j = 0; j < net.numofnodes[lyr]; j++)
        {
            
            double errorsum = 0.0;
            for (int i = 0; i < net.numofnodes[lyr+1]; i++)
            {
                errorsum = errorsum + erroramt[lyr+1][i] * net.weights[lyr][j][i];
                /*for (int i = 0; i < net.numofnodes[lyr+1]; i++)
                {
                    errorsum = errorsum + erroramt[lyr+1][i] * net.weights[lyr][i]
                }*/
            }
            
            double deltaj = errorsum * nodeoutputs[lyr][j] * (1 - nodeoutputs[lyr][j]);
            erroramt[lyr][j] = deltaj;
           // updatedweights[lyr-1][j]  = new (nothrow) double[net.numofnodes[lyr]];
            for (int i = 0; i < net.numofnodes[lyr-1]; i++)
            {
                double updatedweight = net.weights[lyr-1][i][j] + net.learnrate * deltaj * nodeoutputs[lyr-1][i];
                updatedweights[lyr-1][i][j] = updatedweight;
            }
        }
        delete[] nodeoutputs[lyr];
        delete[] erroramt[lyr+1];
    }
    delete[] nodeoutputs[0];

    delete[] nodeoutputs;
    delete[] erroramt;
	// update all the weights
    for (int lyr = 0; lyr < outputlayernum; lyr++)
    {
        for (int j = 0; j < net.numofnodes[lyr]; j++)
        {
            for (int i = 0; i < net.numofnodes[lyr+1]; i++)
            {
                net.weights[lyr][j][i] = updatedweights[lyr][j][i];
            }
            delete[] updatedweights[lyr][j];
        }
        delete[] updatedweights[lyr];
    }

    delete[] updatedweights;
    net.traincount = net.traincount+1;

    net.MSE = net.SE / (net.numofnodes[outputlayernum] * net.traincount);
    net.RMS = sqrt(net.MSE);

    
    /*double *outdesired = new double[outputnodes];
    double **nodeoutputs = new (nothrow) double*[net.layers];*/
	/*double ***updatedweights;
    double **erroramt;*/
}