#ifndef SIMULATE_PROGRESSION_H

#define SIMULATE_PROGRESSION_H

#include "random.h"
#include <unordered_map>
#include <utility>

typedef unsigned int u32;

typedef struct Score
{
public:
    u32 bitmap;
    u32 count;

    Score& operator=(const Score& rhs)
    {
        this->bitmap = rhs.bitmap;
        this->count = rhs.count;
        return *this;
    }
    
    bool operator==(const Score& rhs) const
    {
        if(rhs.bitmap == this->bitmap && rhs.count == this->count)
            return true;
        else
            return false;
    }
    

    struct Hash
    {
    public:
        unsigned operator()(const Score& score) const
        {
            return (std::hash<u32>()(score.bitmap) ^
                 (std::hash<u32>()(score.count) << 1) );
        }
    };

} Score;

enum Win
{
    A = 0,
    B = 1,
    Tie = 2
};

typedef struct WinProbs
{
    double Pa;
    double Pb;
} WinProbs;

typedef std::unordered_map<Score, u32, Score::Hash> HashMapScores;

Score simulate_progression(double La, double Lb);
void simulate_progression_data(double La, double Lb, HashMapScores& score_map, int num_simulations = 10000000);
enum Win win_from_behind(const Score& score);
WinProbs calculate_prob_win_from_behind(const HashMapScores& score_map);
WinProbs simulate(double La, double Lb, int num_simulations);

#endif


