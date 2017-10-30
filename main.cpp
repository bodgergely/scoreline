#include "simulate_progression.h"
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "rdtsc.h"

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
        printf("usage: scoreline <filename> <num_simulations>");
        return -1;
    }
    ull tsc_per_sec = estimate_tsc_per_sec();
    printf("************************************************************\n");
    printf("Detected number of cycles per second: %llu (%.2f Ghz)\n", tsc_per_sec, tsc_per_sec / (double)1000000000);
    printf("************************************************************\n");
    ifstream input_file(argv[1], ifstream::in);
    int num_simulations = atoi(argv[2]);
    string line;
    while(getline(input_file, line))
    {
        vector<string> data = parse_line(line);
        double La = atof(data[0].c_str());
        double Lb = atof(data[1].c_str());
        double Pa = atof(data[2].c_str());
        double Pb = atof(data[3].c_str());
        ull start = rdtsc();
        WinProbs winprobs = simulate(La, Lb, num_simulations);
        ull took = rdtsc() - start;
        printf("Expected prob A: %f and calculated prob A: %f, Diff: %f\n", Pa, winprobs.Pa, Pa - winprobs.Pa);
        printf("Expected prob B: %f and calculated prob B: %f, Diff: %f\n", Pb, winprobs.Pb, Pb - winprobs.Pb);
        printf("Seconds to calculate: %f\n", took / (double)tsc_per_sec);
    }

    return 0;

}

void debug_win_from_behind(unsigned bitmap, unsigned count)
{
    Score score;
    score.count = count;
    //  b11010
    score.bitmap = bitmap;
    Win win = win_from_behind(score);
    cout << "Score: " << score.bitmap << " goal count: " << score.count << " Won by: " << win << endl;
}

void debug_win_from_behind()
{
    debug_win_from_behind(0x1A, 5);
    // b0000000
    debug_win_from_behind(0x0, 7);
    // b100000
    debug_win_from_behind(0x20, 6);
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
}





