#include "simulate_progression.h"
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

vector<string> parse_line(string line)
{
    size_t delim_pos = 0;
    vector<string> tokenz;
    string token;
    while((delim_pos = line.find(',')) != string::npos )
    {
        token = line.substr(0, delim_pos);
        tokenz.push_back(token);
        line.erase(0, delim_pos + 1);
    }
    tokenz.push_back(line);
    return tokenz;
}


int process_file(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("usage: scoreline <filename> <numsimulations>");
        return -1;
    }
    ifstream input_file(argv[1], ifstream::in);
    int num_simulations = atoi(argv[2]);
    string line;
    while(getline(input_file, line))
    {
        vector<string> data = parse_line(line);
        for(string s : data)
        {
            cout << s << endl;
        }
        double La = atof(data[0].c_str());
        double Lb = atof(data[1].c_str());
        double Pa = atof(data[2].c_str());
        double Pb = atof(data[3].c_str());
        HashMapScores score_map = simulate_progression_data(La, Lb, num_simulations);
        WinProbs winprobs = calculate_prob_win_from_behind(score_map);
        printf("Expected prob A: %f and calculated prob A: %f, Diff: %f\n", Pa, winprobs.Pa, Pa - winprobs.Pa);
        printf("Expected prob B: %f and calculated prob B: %f, Diff: %f\n", Pb, winprobs.Pb, Pb - winprobs.Pb);
        printf("\n");
    }

    return 0;

}

void debug_win_from_behind()
{
    Score score;
    score.count = 5;
    //  b11010
    score.bitmap = 0x1A;
    Win win = win_from_behind(score);
    cout << score.bitmap << " Won by: " << win << endl;
}

void debug_simulate_progression()
{
    double La = 2.462;
    double Lb = 2.812;
    for(int i=0;i<10;i++)
    {
        Score score = simulate_progression(La,Lb);
        cout << "Score: " << score.bitmap << " Score count: " << score.count << endl;
    }
}

int main(int argc, char** argv)
{
    return process_file(argc, argv);
    //debug_simulate_progression(); 
    return 0;
}




