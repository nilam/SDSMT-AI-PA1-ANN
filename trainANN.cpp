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

/*====================== global symbolic constants ===================*/
/*====================== global type definitions =====================*/
/*======================= function prototypes ========================*/



/*=====================================================================
Function:
Description:
Parameters:
======================================================================*/

#include "weatherANN.h"


//Initialize the weights of networks (More information)
//Choose a random training sample, and assign input vector to the input neurons
//Propagate all neurons in the forward direction to obtain output at the output layer
//Evaluate error values at each neuron in the output layer as the difference between obtained output and the desired output of the training sample choosen
//Evaluate Mean Squared Error value. This value reflects the effectiveness of training done so far.
//Backpropagate the errors, all the way upto the input layer
//Calculate delta (weight update) for all synapses
//Update the weights all synapses such that the sum-squared value of error is minimized.
//Now, choose another random sample and repeat the process. In this fashion, train all samples in some random order. A training epoch is a cycle through all the samples in the training set. Typically, many training epochs are required to train a backpropagation network.
//Check if the stopping criterion has reached. If not, continue with the next training epoch.



using namespace std;

int main(int argc, char** argv)
{

    ifstream paramfile;
    ifstream weightfile;
    ifstream datafile;
    ofstream outweight;
    weight_struct weights;
    //weight_struct bestweights;
    if (argc != 2)
    {
        cout << "\nUsage: trainANN parameterfile";
        return 1;
    }

    char pfile[100];

    strcpy(pfile,argv[1]);

    paramfile.open(pfile);

    if (!paramfile)
    {
        cout << "\nUnable to open the parameter file " << pfile;
        return 1;
    }

    parameters params;

    import_parameters(paramfile, params);
    params.layers++;
    datafile.open(params.datafile);

    if(!datafile)
    {
        cout << "\nUnable to open the data file " << params.datafile;
        return 1;
    }
    datafile.close();

    vector<weatherday> wdata;

    import_weather_data(params.datafile, wdata);

    if (wdata.size() < 1)
        return 1;

    weights.layers = params.layers-1;
    weights.nodesperlayer = new (nothrow) int[weights.layers-1];
    for (int i = 0; i < params.num_nodes.size(); i++)
    {
        weights.nodesperlayer[i] = params.num_nodes[i];
    }
    weightfile.open(params.weightfile);


    neuralnet net;

    initialize_neural_net(net, params.learn_rate, params.momentum, params.threshold, params.layers, weights.nodesperlayer);
    if(weightfile)
    {
         import_weights(weightfile, weights);
         for (int i = 0; i < params.layers-1; i++)
         {
            //weights.weights[i] = new (nothrow) double*[weights.nodesperlayer[i]];
            for (int j = 0; j < weights.nodesperlayer[i]; j++)
            {
                //weights.weights[i][j] = new (nothrow) double[weights.nodesperlayer[i+1]];
                for (int k = 0; k < weights.nodesperlayer[i+1]; k++)
                {
                    //double w;
                   // fin >> w;
                    net.weights[i][j][k] = weights.weights[i][j][k];
                }
            }
         }
    }
    else
    {
        //new weight file
    }
    weightfile.close();
    vector<trainingset> trainers;

    //normalize data
    normalize_data(wdata);

    for (int i = 0; i < wdata.size() - params.training_days - 1; i++)
    {
        trainingset tset;
        tset.traindays = new (nothrow) weatherday[params.training_days];

        for (int j = 0; j < params.training_days; j++)
        {
            tset.traindays[j] = wdata[i + j];
        }
        tset.predictday = wdata[i + params.training_days];
        trainers.push_back(tset);
    }

    srand( time(NULL) );
    vector<trainingset>::iterator it;
    
    double minRMS = 100.0;

    for (int i = 0; i < params.epochs; i++)
    {
        random_shuffle(trainers.begin(), trainers.end());

        for (int x = 0; x < trainers.size(); x++)
        {
            double* inputs = new (nothrow) double[params.num_nodes[0]];
            int pos = 0;
            for (int k = 0; k < params.training_days; k++)
            {
                for (int j = 0; j < params.num_input_features; j++)
                {
                    inputs[pos] = get_feature_val(params.in_features[j], trainers[x].traindays[k]);
           //         cout << inputs[pos] << ' ';
                    pos++;
                }
            }
         //   cout << '\n';
            double* outputs = new (nothrow) double[params.num_output_features];
            for (int k = 0; k < params.num_output_features; k++)
            {
                outputs[k] = get_feature_val(params.out_features[k], trainers[x].predictday);
            }
            
            train_sample(net, outputs, inputs);
        }

        if (net.RMS < minRMS)
        {
            minRMS = net.RMS;
            weights.weights = new (nothrow) double**[params.layers-1];
            for (int a = 0; a < params.layers-1; a++)
            {
                weights.weights[a] = new (nothrow) double*[weights.nodesperlayer[a]];
                for (int b = 0; b < weights.nodesperlayer[a]; b++)
                {
                    weights.weights[a][b] = new (nothrow) double[weights.nodesperlayer[a+1]];
                    for (int c = 0; c < weights.nodesperlayer[a+1]; c++)
                    {
                       weights.weights[a][b][c] = net.weights[a][b][c];
                    }
                }
            }
        }

        if (i % 10 == 0)
        {
            cout << "Epoch      " << i << ":  RMS error = " << net.RMS << '\n';
        }
    }

    outweight.open(params.weightfile);
    if (outweight)
    {
        write_weights (outweight, weights);
    }

    cout << '\n';
    outweight.close();
    for (int a = 0; a < params.layers-1; a++)
    {
        for (int b = 0; b < weights.nodesperlayer[a]; b++)
        {
            delete[] weights.weights[a][b];
        }
        delete[] weights.weights[a];
    }
    delete[] weights.weights;
    return 0;

}

double get_feature_val (char* fture, weatherday day)
{
    if (strcmp(fture, "TAVG") == 0)
    {
        return day.temp.avg;
    }
    else if (strcmp(fture, "HAVG") == 0)
    {
        return day.humi.avg;
    }
    else if (strcmp(fture, "WAVG") == 0)
    {
        return day.wind.avg;
    }
    else if (strcmp(fture, "GAVG") == 0)
    {
        return day.gust.avg;
    }
    else if (strcmp(fture, "BAVG") == 0)
    {
        return day.baro.avg;
    }
    else if (strcmp(fture, "RAVG") == 0)
    {
        return day.rain.avg;
    }
    else if (strcmp(fture, "UVAVG") == 0)
    {
        return day.uv.avg;
    }
    else if (strcmp(fture, "TMIN") == 0)
    {
        return day.temp.min;
    }
    else if (strcmp(fture, "HMIN") == 0)
    {
        return day.humi.min;
    }
    else if (strcmp(fture, "WMIN") == 0)
    {
        return day.wind.min;
    }
    else if (strcmp(fture, "GMIN") == 0)
    {
        return day.gust.min;
    }
    else if (strcmp(fture, "BMIN") == 0)
    {
        return day.baro.min;
    }
    else if (strcmp(fture, "RMIN") == 0)
    {
        return day.rain.min;
    }
    else if (strcmp(fture, "UVMIN") == 0)
    {
        return day.uv.min;
    }
    else if (strcmp(fture, "TMAX") == 0)
    {
        return day.temp.max;
    }
    else if (strcmp(fture, "HMAX") == 0)
    {
        return day.humi.max;
    }
    else if (strcmp(fture, "WMAX") == 0)
    {
        return day.wind.max;
    }
    else if (strcmp(fture, "GMAX") == 0)
    {
        return day.gust.max;
    }
    else if (strcmp(fture, "BMAX") == 0)
    {
        return day.baro.max;
    }
    else if (strcmp(fture, "RMAX") == 0)
    {
        return day.rain.max;
    }
    else if (strcmp(fture, "UVMAX") == 0)
    {
        return day.uv.max;
    }
    else
    {
        return 0;
    }
}

void normalize_data( vector<weatherday> &vec )
{
    double tempmax = 0.0;
    double tempmin = 100.0;

    double humimax = 0.0;
    double humimin = 100.0;

    double windmax = 0.0;
    double windmin = 100.0;

    double gustmax = 0.0;
    double gustmin = 100.0;

    double baromax = 0.0;
    double baromin = 100.0;

    double rainmax = 0.0;
    double rainmin = 100.0;

    double uvmax = 0.0;
    double uvmin = 100.0;
    

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].temp.max > tempmax)
        {
            tempmax = vec[i].temp.max;
        }

        if (vec[i].temp.min < tempmin)
        {
            tempmin = vec[i].temp.min;
        }

        if (vec[i].humi.max > humimax)
        {
            humimax = vec[i].humi.max;
        }

        if (vec[i].humi.min < humimin)
        {
            humimin = vec[i].humi.min;
        }

        if (vec[i].wind.max > windmax)
        {
            windmax = vec[i].wind.max;
        }

        if (vec[i].wind.min < windmin)
        {
            windmin = vec[i].wind.min;
        }

        if (vec[i].gust.max > gustmax)
        {
            gustmax = vec[i].gust.max;
        }

        if (vec[i].gust.min < gustmin)
        {
            gustmin = vec[i].gust.min;
        }

        if (vec[i].baro.max > baromax)
        {
            baromax = vec[i].baro.max;
        }

        if (vec[i].baro.min < baromin)
        {
            baromin = vec[i].baro.min;
        }

        if (vec[i].rain.max > rainmax)
        {
            rainmax = vec[i].rain.max;
        }

        if (vec[i].rain.min < rainmin)
        {
            rainmin = vec[i].rain.min;
        }

        if (vec[i].uv.max > uvmax)
        {
            uvmax = vec[i].uv.max;
        }

        if (vec[i].uv.min < uvmin)
        {
            uvmin = vec[i].uv.min;
        }

    }

    for (int i = 0; i < vec.size(); i++)
    {
        vec[i].temp.avg = (vec[i].temp.avg - tempmin ) / (tempmax - tempmin);
        vec[i].temp.max = (vec[i].temp.max - tempmin ) / (tempmax - tempmin);
        vec[i].temp.min = (vec[i].temp.min - tempmin ) / (tempmax - tempmin);

        vec[i].humi.avg = (vec[i].humi.avg - humimin ) / (humimax - humimin);
        vec[i].humi.max = (vec[i].humi.max - humimin ) / (humimax - humimin);
        vec[i].humi.min = (vec[i].humi.min - humimin ) / (humimax - humimin);

        vec[i].wind.avg = (vec[i].wind.avg - windmin ) / (windmax - windmin);
        vec[i].wind.max = (vec[i].wind.max - windmin ) / (windmax - windmin);
        vec[i].wind.min = (vec[i].wind.min - windmin ) / (windmax - windmin);

        vec[i].gust.avg = (vec[i].gust.avg - gustmin ) / (gustmax - gustmin);
        vec[i].gust.max = (vec[i].gust.max - gustmin ) / (gustmax - gustmin);
        vec[i].gust.min = (vec[i].gust.min - gustmin ) / (gustmax - gustmin);

        vec[i].baro.avg = (vec[i].baro.avg - baromin ) / (baromax - baromin);
        vec[i].baro.max = (vec[i].baro.max - baromin ) / (baromax - baromin);
        vec[i].baro.min = (vec[i].baro.min - baromin ) / (baromax - baromin);

        vec[i].rain.avg = (vec[i].rain.avg - rainmin ) / (rainmax - rainmin);
        vec[i].rain.max = (vec[i].rain.max - rainmin ) / (rainmax - rainmin);
        vec[i].rain.min = (vec[i].rain.min - rainmin ) / (rainmax - rainmin);

        vec[i].uv.avg = (vec[i].uv.avg - uvmin ) / (uvmax - uvmin);
        vec[i].uv.max = (vec[i].uv.avg - uvmin ) / (uvmax - uvmin);
        vec[i].uv.min = (vec[i].uv.avg - uvmin ) / (uvmax - uvmin);
    }

}