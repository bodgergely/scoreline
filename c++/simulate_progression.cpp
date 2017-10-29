#include "random.h"
#include "simulate_progression.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

Score simulate_progression(double La, double Lb)
{
    double T = 0.0;
    Score score;
    score.bitmap = 0;
    score.count = 0;

    u32 mask = 0x1;
    while(true)
    {
        T = T + random_exponential(La + Lb);
        if(T > 1 || (score.count > 20) )
            return score;
        if( random_uniform() < (La / (La + Lb)))
            score.bitmap |= mask;
        mask <<= 1;
        score.count++;
    }

    return score;
}

void simulate_progression_data(double La, double Lb, HashMapScores& score_map, int num_simulations)
{
    for(int i=0;i<num_simulations;i++)
    {
       Score score = simulate_progression(La, Lb);
       if(score_map.find(score) == score_map.end())
       {
           score_map[score] = 1;
       }
       else
       {
           score_map[score] += 1;
       }
    }
}


enum Win win_from_behind(const Score& score)
{
    int losing_a = 0;
    int losing_b = 0;
    int score_a = 0;
    int score_b = 0;
    const u32& count = score.count;
    u32 bitmap = score.bitmap;
    u32 i = 0;

    while(i < count)
    {
        if(bitmap & 0x1)
        {
            score_a++;
            if((score_b < score_a ) && !losing_b)
                losing_b = 1;
        }
        else
        {
            score_b++;
            if((score_a < score_b) && !losing_a)
                losing_a = 1;
        }
        i++;
        bitmap >>= 0x1;
    }

    if(losing_a && score_a > score_b)
        return Win::A;
    else if(losing_b && score_b > score_a)
        return Win::B;
    else
        return Win::Tie;

} 

WinProbs calculate_prob_win_from_behind(const HashMapScores& score_map)
{
    u32 win_a = 0;
    u32 win_b = 0;
    u32 totalsims = 0;
    for(auto it=score_map.cbegin();it!=score_map.cend();it++)
    {
        const u32& count = it->second;
        totalsims += count;
        const Score& score = it->first;
        Win res = win_from_behind(score);
        if(res == Win::A)
            win_a += count;
        else if(res == Win::B)
            win_b += count;
    }

    WinProbs probs;
    probs.Pa = win_a / (double)totalsims;
    probs.Pb = win_b / (double)totalsims;

    return probs;
}

static void _simulate(double La, double Lb, int num_simulations, WinProbs& winprobs)
{
    HashMapScores hashmap_scores;
    simulate_progression_data(La, Lb, hashmap_scores, num_simulations);
    winprobs = calculate_prob_win_from_behind(hashmap_scores);
}

WinProbs simulate(double La, double Lb, int num_simulations)
{
    unsigned num_cores = thread::hardware_concurrency();
    unsigned sims_per_thread = (unsigned)num_simulations / num_cores;  
    unsigned remainder_sims = (unsigned)num_simulations % num_cores;

    vector<WinProbs> winprobs(num_cores);
    vector<thread> threads; 
    for(unsigned i=0;i<num_cores;i++)
    {
        unsigned sims = sims_per_thread;
        if(i == num_cores - 1)
            sims = sims_per_thread + remainder_sims;
        threads.push_back(thread(_simulate, La, Lb, sims, std::ref(winprobs[i])));
    }

    for(thread& t : threads)
    {
        if(t.joinable())
            t.join();
    }

    double PaS = 0.0;
    double PbS = 0.0;
    for(const WinProbs& probs : winprobs)
    {
        PaS+=probs.Pa;
        PbS+=probs.Pb;
    }

    WinProbs winprob;
    winprob.Pa = PaS / winprobs.size();
    winprob.Pb = PbS / winprobs.size();

    return winprob;

}



