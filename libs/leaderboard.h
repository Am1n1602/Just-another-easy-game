#pragma once
#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string name;
    int deaths;
    int time;
};

bool SubmitScore(const std::string& name, int deaths, float time);
std::vector<LeaderboardEntry> FetchLeaderboard(const std::string& sortBy);