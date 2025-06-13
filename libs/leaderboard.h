#pragma once
#include <string>
#include <vector>

// This struct is to be sent over the web.

struct LeaderboardEntry {
    std::string name;
    int deaths;
    int time;
};


bool SubmitScore(const std::string& name, int deaths, float time);
std::vector<LeaderboardEntry> FetchLeaderboard(const std::string& sortBy);