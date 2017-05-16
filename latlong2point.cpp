#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////
// Converts a list of space delimited lat-long pairs to pixel point coordinates  //
//							                         //
//     *Created to create an output file usable by the heatmap_gen program*      //
//										 //
// Saves output to points.txt							 //
//										 //
// The first two arguments should be the height and width of the expected output //
///////////////////////////////////////////////////////////////////////////////////


using namespace std;

int main(int argc, char** argv) //If not given a list, will look for data.txt in the same directory
{
   vector<float> latList;
   vector<float> longList;
   int height = 0, width = 0;
   float latLowBound = 0.0, latHighBound = 0.0, longLowBound = 0.0, longHighBound = 0.0;
   

   if(argc > 2)
   {
      if(!(width = stoi(argv[1])) || !(height = stoi(argv[2])))
      {
         cerr << "Error reading arguments. The first two arguments must be the width and height" << endl;
      }
      if(!(latLowBound = stof(argv[3])) || !(latHighBound = stof(argv[4])))
      {

      }
      if(!(longLowBound = stof(argv[5])) || !(longHighBound = stof(argv[6])))
      {

      }
      for(int i = 7 ; i < argc ;)
      {
         latList.push_back(stof(argv[i++]));

         longList.push_back(stof(argv[i++]));
      }
   }

   ifstream dataFile; 
   
   dataFile.open("data.txt", ios::in);
   if(!dataFile.is_open())
   {
      cerr << "Error opening data.txt" << endl;
   }
   else
   {
      string line;

      while (getline(dataFile,line))
      {
         size_t delim = line.find(" ");
	 if(width == 0 && height == 0)
	 {
	    width = stoi(line.substr(0,delim));
	    height = stoi(line.substr(delim + 1));
	 }
         else if (latLowBound == 0.0 && latHighBound == 0.0)
         {
	    latLowBound = stof(line.substr(0,delim));
            latHighBound = stof(line.substr(delim + 1));
         }
         else if (longLowBound == 0.0 && longHighBound == 0.0)
         {
	    longLowBound = stof(line.substr(0,delim));
            longHighBound = stof(line.substr(delim + 1));
         }
	 else
	 {
	    latList.push_back(stof(line.substr(0,delim)));
            longList.push_back(stof(line.substr(delim + 1)));
	 } 
      }
   dataFile.close();
   }


   float minLat = latList[0], minLong = longList[0], maxLat = latList[0], maxLong = longList[0]; 
   int size = latList.size();
   for(int i = 1 ; i < size ; i++)
   {
      if(latList[i] < minLat)
      {
         minLat = latList[i];
      }
      else if(latList[i] > maxLat)
      {
         maxLat = latList[i];
      }

      if(longList[i] < minLong)
      {
         minLong = longList[i];
      }
      else if(longList[i] > maxLong)
      {
         maxLong = longList[i];
      }
   }
   
   cout << "longLowBound " << longLowBound << endl;

   for(int i = 0 ; i < size ; i++)
   {
      latList[i] = round((minLat - latLowBound) + (((latList[i] - latLowBound)/(latHighBound - latLowBound)))*width);
      longList[i] = round((minLong - longLowBound) + (((longList[i] - longLowBound)/(longHighBound - longLowBound)))*height);
   }

   ofstream outputFile;
   outputFile.open("points.txt", ios::out);
   if(!outputFile.is_open())
   {
      cerr << "Error opening points.txt" << endl;
   }
   else
   {
      for(int i = 0 ; i < size ; i++)
      { 
	 outputFile << latList[i] << " " << longList[i] << endl;
      }
   }
   outputFile.close();

   return 0;
}
