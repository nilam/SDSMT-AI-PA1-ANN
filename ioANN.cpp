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

using namespace std;




/*====================== global symbolic constants ===================*/
/*====================== global type definitions =====================*/

/*====================== global enum definitions =====================*/
//enum feature
//{
//	TAVG = 0,
//	HAVG,
//	WAVG,
//	GAVG,
//	BAVG,
//	RAVG,
//	UVAVG,
//	TMIN,
//	HMIN,
//	WMIN,
//	GMIN,
//	BMIN,
//	RMIN,
//	UVMIN,
//	TMAX,
//	HMAX,
//	WMAX,
//	GMAX,
//	BMAX,
//	RMAX,
//	UVMAX
//};


/*====================== global struct definitions ===================*/

//struct w_date
//{
//	int month;
//	int day;
//	int year;
//	char *disdate;
//};
//
//struct w_temp
//{
//	double avg;
//	double min;
//	double max;
//};
//
//struct w_humi
//{
//	double avg;
//	double min;
//	double max;
//};
//
//struct w_wind
//{
//	double avg;
//	double min;
//	double max;
//};
//
//struct w_gust
//{
//	double avg;
//	double min;
//	double max;
//};
//
//struct w_baro
//{
//	double avg;
//	double min;
//	double max;
//};
//
//struct w_rain
//{
//	double avg;
//	double min;
//	double max;
//};
//
//struct w_uv
//{
//	double avg;
//	double min;
//	double max;
//};
//
//struct weatherday
//{
//	w_date date;
//	w_temp temp;
//	w_humi humi;
//	w_wind wind;
//	w_gust gust;
//	w_baro baro;
//	w_rain rain;
//	w_uv uv;
//};
//
//struct parameters
//{
//	char weightfile[512];
//	int epochs;
//	double learn_rate;
//	double momentum;
//	double threshold;
//	int layers;
//	vector<int> num_nodes;
//	char datafile[512];
//	int num_input_features;
//	int training_days;
//	vector<feature> in_features;
//	int num_output_features;
//	vector<feature> out_features;
//};


/*======================= function prototypes ========================*/


//int main()
//{
//	vector<weatherday> wv;
//	import_weather_data ("weather2008.dat", wv);
//
//	return 0;
//}
/*=====================================================================
Function:
Description:
Parameters:
======================================================================*/
void import_weather_data ( char* filename, vector<weatherday> &vec )
{
	
	char buf[100];
	ifstream fin;

	fin.open( filename );
	
	if ( !fin )
	{
		return;
	}

	//get rid of first 2 lines
	fin.getline(buf,100);
	fin.getline(buf, 100);
	
	while ( !fin.eof() )
	{
		weatherday wtemp;
		wtemp = import_helper ( fin );
		vec.push_back( wtemp );
		//fin >> buf;
	}
	return;
}


/*=====================================================================
Function:
Description:
Parameters:
======================================================================*/
//vector<weatherday> import_weather_data ( char* filename, w_date startday, w_date endday )
//{
//
//}


/*=====================================================================
Function:
Description:
Parameters:
======================================================================*/
weatherday import_helper ( ifstream &fin )
{
	int i;
	//char avgline[100];
	//char minline[100];
	//char maxline[100];
	char buffer[50];
	//char *tokenized[10];
	char *token;
	weatherday wdat;

	
	/*fin.getline( avgline, 100 );
	fin.getline( minline, 100 );
	fin.getline( maxline, 100 );

	i = 0;
	token = strtok(avgline, " ");
	while ( token != NULL )
	{
		tokenized[i] = new char[50];

	}*/

	fin >> buffer;
	i = 0;
	token = strtok(buffer, "/");
	while ( token != NULL )
	{
		if (i == 0)
		{
			wdat.date.month = atoi( token );
		}
		else if (i == 1)
		{
			wdat.date.day = atoi( token );
		}
		else
		{
			wdat.date.year = atoi( token);
		}
		i++;
		token = strtok(NULL, "/");
	}
	wdat.date.disdate = new char[10];
	strcpy(wdat.date.disdate, buffer);
	fin >> buffer;
	fin >> wdat.temp.avg;
	fin >> wdat.humi.avg;
	fin >> wdat.wind.avg;
	fin >> wdat.gust.avg;
	fin >> wdat.baro.avg;
	fin >> wdat.rain.avg;
	fin >> wdat.uv.avg;

	fin >> buffer;
	fin >> buffer;
	

	fin >> wdat.temp.min;
	fin >> wdat.humi.min;
	fin >> wdat.wind.min;
	fin >> wdat.gust.min;
	fin >> wdat.baro.min;
	fin >> wdat.rain.min;
	fin >> wdat.uv.min;

	fin >> buffer;
	fin >> buffer;
	

	fin >> wdat.temp.max;
	fin >> wdat.humi.max;
	fin >> wdat.wind.max;
	fin >> wdat.gust.max;
	fin >> wdat.baro.max;
	fin >> wdat.rain.max;
	fin >> wdat.uv.max;

	return wdat;
}


/*=====================================================================
Function:
Description:
Parameters:
======================================================================*/
void import_parameters (ifstream &fin, parameters &params)
{
	//if line starts with '#','%' ignore the whole line
	//if line contains '#','%' then ignore everything after on the line
	vector<paramline> lines;
	vector<paramline> infolines;
	paramline gline;

	char weightfile[30];
	char layer[512];

	int i = 0;
	int j = 0;
	int k = 0;
	int num = 0;

	//get all lines of param file in lines vector
	while (!fin.eof())
	{
		paramline buf;
		fin.getline(buf.oneline,512);
		lines.push_back(buf);
	}

	//strip comment lines and empty lines by comparing first character
	for (i = 0; i < lines.size(); i++)
	{	
		//need \0 because lines read in as strings already
		if (lines[i].oneline[0] != '#' && lines[i].oneline[0] != '%' 
		&& lines[i].oneline[0] != '\n' && lines[i].oneline[0] != ' '
		&& lines[i].oneline[0] != '\0')
		{			
			j = 0;
			//strip inline comments
			while(lines[i].oneline[j] != '\n' && lines[i].oneline[j] != '#'
			&& lines[i].oneline[j] != '%' && lines[i].oneline[j] != '\0')
			{
				gline.oneline[j] = lines[i].oneline[j];
				j++;
			}
			//strcpy(gline.oneline, lines[i].oneline);
			gline.oneline[j] = '\0';
			infolines.push_back(gline);
		}
	}

	//following lines add data to the parameters struct
	//features are hardcoded as enum, so skipped here

	i = 0;
	//char weightfile[100];
	strcpy(params.weightfile, infolines[i].oneline);
	i++;
	//int epochs;
	params.epochs = atoi(infolines[i].oneline);
	i++;
	//double learn_rate;
	params.learn_rate = atof(infolines[i].oneline);
	i++;
	//double momentum;
	params.momentum = atof(infolines[i].oneline);
	i++;
	//double threshold;
	params.threshold = atof(infolines[i].oneline);
	i++;
	//int layers;
	params.layers = atoi(infolines[i].oneline);
	i++;
	//vector<int> num_nodes;

	// j iterates through line
	// k iterates through layer string
	j = 0;
	while(num <= params.layers)
	{	
		k = 0;
		while(infolines[i].oneline[j] != ' ')
		{
			layer[k] = infolines[i].oneline[j];
			j++;
			k++;
		}
		layer[k] = '\0';
		params.num_nodes.push_back(atoi(layer));
		num++;
		j++;
	}
	i++;
	//char datafile[100];
	strcpy(params.datafile, infolines[i].oneline);
	i++;
	//int num_input_features;
	params.num_input_features = atoi(infolines[i].oneline);
	i++;
	//int training_days;
	params.training_days = atoi(infolines[i].oneline);
	i++;

    for (int z = 0; z < params.num_input_features; z++)
    {
        
        char* feat = new char[512];
        strcpy(feat, infolines[i].oneline);
        params.in_features.push_back(feat);
        i++;
    }
	//vector<feature> in_features;
	//i += params.num_input_features;
	//int num_output_features;
	params.num_output_features = atoi(infolines[i].oneline);
	i++;
	//vector<feature> out_features;
    for (int z = 0; z < params.num_output_features; z++)
    {
        
        char* feat = new char[512];
        strcpy(feat, infolines[i].oneline);
        params.out_features.push_back(feat);
        i++;
    }
	//i += params.num_output_features;
}


/*=====================================================================
Function:
Description:
Parameters:
======================================================================*/
void import_weights (ifstream &fin, weight_struct &weights)
{
    int lyrs;
    fin >> lyrs;
    for (int i = 0; i <= lyrs; i++)
    {
        int nodecount;
        fin >> nodecount;
        if (weights.nodesperlayer[i] != nodecount)
        {
            return;
        }
    }
    int temp;
    fin >> temp;
    weights.weights = new (nothrow) double**[lyrs];
    for (int i = 0; i < lyrs; i++)
    {
        weights.weights[i] = new (nothrow) double*[weights.nodesperlayer[i]];
        for (int j = 0; j < weights.nodesperlayer[i]; j++)
        {
            weights.weights[i][j] = new (nothrow) double[weights.nodesperlayer[i+1]];
            for (int k = 0; k < weights.nodesperlayer[i+1]; k++)
            {
                double w;
                fin >> w;
                weights.weights[i][j][k] = w;
            }
        }
    }
}

void write_weights (ofstream &fout, weight_struct &weights)
{
    fout << weights.layers;
    for (int i = 0; i < weights.layers+1; i++)
    {
        fout << " " << weights.nodesperlayer[i] ;
    }
    fout << '\n';

    for (int i = 0; i < weights.layers; i++)
    {
        for (int j = 0; j < weights.nodesperlayer[i]; j++)
        {
            for (int k = 0; k < weights.nodesperlayer[i+1]; k++)
            {
                fout << weights.weights[i][j][k] << " ";
            }
        }
    }
}
//void import_parameters (ifstream &fin, parameters &params)
//{
//	//if line starts with '#','%' ignore the whole line
//	//if line contains '#','%' then ignore everything after on the line
//	vector<char[512]> lines;
//	vector<char[512]> infolines;
//	char curline[512];
//
//	
//
//
//
//	while (!fin.eof())
//	{
//		char buf[512];
//		fin.getline(buf,512);
//		lines.push_back(buf);
//	}
//
//		//	struct parameters
//		//{
//		//	char weightfile[100];
//		//	int epochs;
//		//	double learn_rate;
//		//	double momentum;
//		//	double threshold;
//		//	int layers;
//		//	vector<int> num_nodes;
//		//	char datafile[100];
//		//	int num_input_features;
//		//	int training_days;
//		//	vector<feature> in_features;
//		//	int num_output_features;
//		//	vector<feature> out_features;
//		//};
//
//	
//	for (int i = 0; i < lines.size(); i++)
//	{
//		if (lines[i][0] != '#' && lines[i][0] != '%' && lines[i][0] != '\n' && lines[i][0] != ' ')
//		{
//			char gline[512];
//			strcpy_s(gline, lines[i]);
//			infolines.push_back(gline);
//		}
//	}
//	
//	//get weight file name
//	strcpy_s(curline,infolines[0]);
//	int p = 0;
//	while (p < 512 && curline[p] != '\0' && curline[p] != '#' && curline[p] != ' ')
//	{
//		p++;
//	}
//
//	curline[p] = '\0';
//	strcpy_s(params.weightfile, curline);
//
//
//}