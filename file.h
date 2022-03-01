#ifndef _FILE
#define _FILE

#include "Location.h"
#include "Ghost.h"
#include "Packman.h"
#include "Fruit.h"
#include <fstream>


class File 
{
public:

    // ~~~~~~~~~~~~~~~~~~~  working directory  ~~~~~~~~~~~~~~~~~~~~~~~~~
    int isSubstring(string end, string file_name);
    vector<string> createFileList();
    vector<string> findFile(string file_name);

    // ~~~~~~~~~~~~~~~~~~~~  save mode  ~~~~~~~~~~~~~~~~~~~~~~~~~~
    void createStepsFile(string file_name, Packman& packman, vector<Ghost>& ghosts, Fruit& fruit);
    
    void writePackmanSteps(ofstream& steps_file, Packman& packman);
    void writeGhostsSteps(ofstream& steps_file, vector<Ghost>& ghosts);
    void writeFruitSteps(ofstream& steps_file, Fruit& fruit);
    
    // ~~~~~~~~~~~~~~~~~~~~  load mode ~~~~~~~~~~~~~~~~~~~~~~~~~~
    void readSteps(Packman& packman, vector<Ghost>& ghosts, Fruit& fruit, string file_name_original);
    
    void initVectors(Packman& packman, vector<Ghost>& ghosts, Fruit& fruit);

    void readPackmanSteps(ifstream& steps_file, Packman& packman);
    void readGhostsSteps(ifstream& steps_file, vector<Ghost>& ghosts);
    void readFruitSteps(ifstream& steps_file, Fruit& fruit);

    void createResultFile(string file_name, vector<int> strike_time, int points_of_time, int life);
    string changeName(string original, string add);
    vector<string> createFileStepsList();
    bool compareRes(vector<Result> cur_game, string original_file_name);
    int findEndGameTime(string file_name);
};

#endif
