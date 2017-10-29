#include "random.h"
#include "simulate_progression.h"
#include <iostream>

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

HashMapScores simulate_progression_data(double La, double Lb, int num_simulations)
{
    HashMapScores score_map;
    srand(time(NULL));
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
    return score_map;
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
            if(!losing_b)
                losing_b = 1;
            score_a++;
        }
        else
        {
            if(!losing_a)
                losing_a = 1;
            score_b++;
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

