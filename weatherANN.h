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

#ifndef __WEATHERANN__H__
#define __WEATHERANN__H__

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <map>
#include <time.h>
#include <cmath>
#include <algorithm>

enum feature
{
	TAVG = 0,
	HAVG,
	WAVG,
	GAVG,
	BAVG,
	RAVG,
	UVAVG,
	TMIN,
	HMIN,
	WMIN,
	GMIN,
	BMIN,
	RMIN,
	UVMIN,
	TMAX,
	HMAX,
	WMAX,
	GMAX,
	BMAX,
	RMAX,
	UVMAX
};


//void import_parameters (ifstream &fin, parameters &params);
using namespace std;
/*====================== global struct definitions ===================*/

struct w_date
{
	int month;
	int day;
	int year;
	char *disdate;
};

struct w_temp
{
	double avg;
	double min;
	double max;
};

struct w_humi
{
	double avg;
	double min;
	double max;
};

struct w_wind
{
	double avg;
	double min;
	double max;
};

struct w_gust
{
	double avg;
	double min;
	double max;
};

struct w_baro
{
	double avg;
	double min;
	double max;
};

struct w_rain
{
	double avg;
	double min;
	double max;
};

struct w_uv
{
	double avg;
	double min;
	double max;
};

struct weatherday
{
	w_date date;
	w_temp temp;
	w_humi humi;
	w_wind wind;
	w_gust gust;
	w_baro baro;
	w_rain rain;
	w_uv uv;
};


struct paramline
{
	char oneline[512];
};

struct parameters
{
	char weightfile[512];
	int epochs;
	double learn_rate;
	double momentum;
	double threshold;
	int layers;
	vector<int> num_nodes;
	char datafile[512];
	int num_input_features;
	int training_days;
	vector<char*> in_features;
	int num_output_features;
	vector<char*> out_features;
};

struct weight_struct
{
    int layers;
    int* nodesperlayer;
    double*** weights;
    
};
//struct parameters
//{
//	char* weightfile;
//	int epochs;
//	double learn_rate;
//	double momentum;
//	double threshold;
//	int layers;
//	vector<int> num_nodes;
//	char* datafile;
//	int num_input_features;
//	int training_days;
//	vector<feature> in_features;
//	int num_output_features;
//	vector<feature> out_features;
//};




struct neuralnet
{
	double learnrate;
	double momentum;
	double threshold;
	int layers;
	int nodecount;
	int weightcount;
	int biasweightcount;
	double* nodevalues;
	int* numofnodes;
	double*** weights;
	double* biasweights;
	double SE;
    double MSE;
    double RMS;
    int traincount;
};



struct trainingset
{
    weatherday* traindays;
    weatherday predictday;
};


void import_weather_data (char *filename, vector<weatherday> &vec);

weatherday import_helper (ifstream &fin);

void initialize_neural_net(neuralnet &net, double learnrate, double momentum, double threshold, int layers, int* numofnodes);

void train_sample(neuralnet &net, double* desiredvals, double* inputvals);

void normalize_data( vector<weatherday> &vec );

void import_parameters (ifstream &fin, parameters &params);

void import_weights (ifstream &fin, weight_struct &weights);

double get_feature_val (char* fture, weatherday day);

void write_weights (ofstream &fout, weight_struct &weights);

#endif