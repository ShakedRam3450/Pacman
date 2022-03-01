#include "file.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                   working directory  
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int File::isSubstring(string end, string file_name)
{
    int M = (int)end.length();
    int N = (int)file_name.length();

    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++) 
    {
        int j;

        /* For current index i, check for pattern match */
        for (j = 0; j < M; j++)
            if (file_name[i + j] != end[j])
                break;
        
        if (j == M)
            return i;
    }

    return -1;
}
vector<string> File::createFileList()
{
    // Declaring Vector of String type
    vector<string> file_names_list;
    struct stat file_stats = { 0 };
    struct dirent* dent;
    DIR* dirp = opendir(".");
    
    if (dirp != NULL)    {
        do   {
            dent = readdir(dirp);

            if (dent)  {
                if (isSubstring(".screen", dent->d_name) != -1) 
                {
                    // Strings can be added at any time with push_back
                    file_names_list.push_back(dent->d_name);

                   // cout << "file: " << dent->d_name << endl;
                }
            }
        } while (dent);
    }
    closedir(dirp);

    return file_names_list;
}
vector<string> File::findFile(string file_name)
{
    // Declaring Vector of String type
    vector<string> file_names_list;
    struct stat file_stats = { 0 };
    struct dirent* dent;
    DIR* dirp = opendir(".");

    file_name += ".screen";
    if (dirp != NULL)
    {
        do
        {
            dent = readdir(dirp);

            if (dent)
            {
                
                if (isSubstring(file_name, dent->d_name) != -1)
                {

                    // Strings can be added at any time with push_back
                    file_names_list.push_back(dent->d_name);

                    cout << "file: " << dent->d_name << endl;
                    closedir(dirp);

                    return file_names_list;
                }
            }

        } while (dent);
    }

    closedir(dirp);

    return file_names_list;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                     result file  
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File::createResultFile(string file_name, vector<int> strike_time ,int points_of_time, int life)
{
    ofstream result_file(file_name);
    for (int i = 0; i < strike_time.size(); i++)
    {
        if (life == 0 && i == strike_time.size() - 1)
            result_file << strike_time[i] << " GameOver";
        else
            result_file << strike_time[i] << " Strike" << endl;
    }
    
    if (life != 0)
        result_file << points_of_time << " Win";
        
    result_file.close();
}
string File::changeName(string original, string add)
{
    while (original.find("screen") != string::npos)
        original.replace(original.find("screen"), 6, add);
    return original;
}
//int File::getResultPointsOfTime() 
//{
//
//}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                      save mode  
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File::createStepsFile(string file_name, Packman& packman, vector<Ghost>& ghosts, Fruit& fruit)
{
    ofstream steps_file(file_name);
    writePackmanSteps(steps_file, packman);
    writeGhostsSteps(steps_file, ghosts);
    writeFruitSteps(steps_file, fruit);
    initVectors(packman, ghosts, fruit);

    steps_file.close();
}

void File::writePackmanSteps(ofstream& steps_file, Packman& packman)
{
    // ~~~~~~~~~   appears   ~~~~~~~~~~
    steps_file << packman.getAppearTime().size() << endl;

    for (int i = 0; i < packman.getAppearTime().size(); i++)
        steps_file << packman.getAppearTime()[i] << " ";

    steps_file << endl;

  
    steps_file << packman.getSteps().size() << endl;
    for (int i = 0; i < packman.getSteps().size(); i++)
        steps_file << packman.getSteps()[i] << " ";

    steps_file << endl;
}
void File::writeGhostsSteps(ofstream& steps_file, vector<Ghost>& ghosts)
{
    int num_of_ghosts = (int)ghosts.size();
    vector<int> tmp_steps;

    for (int i = 0; i < num_of_ghosts; i++)
    {
        tmp_steps = ghosts[i].getSteps();
        steps_file << tmp_steps.size() << endl;
        for (int j = 0; j < tmp_steps.size(); j++)
            steps_file << tmp_steps[j] << " ";

        steps_file << endl;
    }
}
void File::writeFruitSteps(ofstream& steps_file, Fruit& fruit)
{
    int tmp_row, tmp_col;
    vector<Appears> fruitAppears = fruit.getAppears();
    vector<int> fruitSteps = fruit.getSteps();
    // ~~~~~~~~~   appears   ~~~~~~~~~~
    steps_file << fruitAppears.size() << endl;
    for (int i = 0; i < fruitAppears.size(); i++)
    {
        fruitAppears[i].loc.getRowCol(tmp_row, tmp_col);
        steps_file << fruitAppears[i].time << " " << tmp_row << " " << tmp_col << " " << fruitAppears[i].value << " ";
    }
    steps_file << endl;

    // ~~~~~~~~~   steps   ~~~~~~~~~~
    steps_file << fruitSteps.size() << endl;
    for (int i = 0; i < fruitSteps.size(); i++)
        steps_file << fruitSteps[i] << " ";
    steps_file << endl;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                       load mode 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File::readSteps(Packman& packman, vector<Ghost>& ghosts, Fruit& fruit, string file_name_original)
{
    ifstream steps_file;
    string filename;

    initVectors(packman, ghosts, fruit);

    filename = changeName(file_name_original, "steps");
    steps_file.open(filename); 
    
    readPackmanSteps(steps_file, packman);
    readGhostsSteps(steps_file, ghosts);
    readFruitSteps(steps_file, fruit);

    steps_file.close();   
}

void File::initVectors(Packman& packman, vector<Ghost>& ghosts, Fruit& fruit)
{
    packman.clearAppears();
    packman.clearSteps();
    packman.clearStepsCounter();

    for (int i = 0; i < ghosts.size(); i++)
    {
        ghosts[i].clearSteps();
        ghosts[i].clearStepsCounter();
    }

    fruit.clearAppears();
    fruit.clearSteps();
    fruit.clearStepsCounter();
}

void File::readPackmanSteps(ifstream& steps_file, Packman& packman)
{
    //string line;
    int step;
    int size, time;

    // ~~~~~~~~~   appears   ~~~~~~~~~~
    steps_file >> size;
    for (int i = 0; i < size; i++)
    {
        steps_file >> time;
        packman.addAppear(time);
    }

    // ~~~~~~~~~   steps   ~~~~~~~~~~
    steps_file >> size;
    for (int i = 0; i < size; i++)
    {
        steps_file >> step;
        packman.addStep(step);
    }
        
}
void File::readGhostsSteps(ifstream& steps_file, vector<Ghost>& ghosts)
{
    int step, size;
    for (int i = 0; i < ghosts.size(); i++)
    {
        steps_file >> size;
        for (int j = 0; j < size; j++)
        {
            steps_file >> step;
            ghosts[i].addStep(step);
        }
           
    }
}
void File::readFruitSteps(ifstream& steps_file, Fruit& fruit)
{
   
    int size, step;
    int x, y, time, value;

    // ~~~~~~~~~   appears   ~~~~~~~~~~
    steps_file >> size;
    for (int i = 0; i < size; i++)
    {
        steps_file >> time;
        steps_file >> x;
        steps_file >> y;
        steps_file >> value;

        fruit.addAppear({ time, Location(x, y) ,value });
    }

    // ~~~~~~~~~   steps   ~~~~~~~~~~
    steps_file >> size;
    for (int i = 0; i < size; i++)
    {
        steps_file >> step;
        fruit.addStep(step);
    }
       
}

vector<string> File::createFileStepsList()
{
    // Declaring Vector of String type
    vector<string> file_names_list;
    struct stat file_stats = { 0 };
    struct dirent* dent;
    DIR* dirp = opendir(".");

    if (dirp != NULL) {
        do {
            dent = readdir(dirp);

            if (dent) {
                if (isSubstring(".steps", dent->d_name) != -1)
                {
                    // Strings can be added at any time with push_back
                    file_names_list.push_back(dent->d_name);

                    // cout << "file: " << dent->d_name << endl;
                }
            }
        } while (dent);
    }
    closedir(dirp);

    return file_names_list;
}
bool File::compareRes(vector<Result> cur_game, string original_file_name)
{   
    ifstream result_file;
    string file_name;
    int tmp_time;
    string tmp_state;
    file_name = changeName(original_file_name, "result");
    result_file.open(file_name);
    bool res = true;

    for (int i = 0; i < cur_game.size(); i++)
    {
        result_file >> tmp_time >> tmp_state;
        if (cur_game[i].time != tmp_time)
            res = false;
        if (tmp_state.compare("Win") == 0 && cur_game[i].state != WON)
            res = false;
        if (tmp_state.compare("Strike") == 0 && cur_game[i].state != STRIKE)
            res = false;
        if (tmp_state.compare("GameOver") == 0 && cur_game[i].state != GAMEOVER)
            res = false;
    }

    result_file.close();
    return res;
}
int File::findEndGameTime(string file_name)
{
    bool notFound = true;
    int tmp_time;
    string tmp_state;
    ifstream result_file;
    file_name = changeName(file_name, "result");
    result_file.open(file_name);
    while(notFound)
    {
        result_file >> tmp_time >> tmp_state;
        if (tmp_state.compare("Win") == 0)
            notFound = false;
        else if (tmp_state.compare("GameOver") == 0 )
            notFound = false;
    }
    return tmp_time;
}