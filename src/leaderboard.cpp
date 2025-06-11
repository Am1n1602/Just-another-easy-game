#include "leaderboard.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool SubmitScore(const std::string& name, int deaths, float time) {
    json body = {
        {"name", name},
        {"deaths", deaths},
        {"time", time}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{ "YOUR-URL" },
        cpr::Header{ {"Content-Type", "application/json"} },
        cpr::Body{ body.dump() }
    );

    return r.status_code == 200;
}

std::vector<LeaderboardEntry> FetchLeaderboard(const std::string& sortBy) {
    cpr::Response r = cpr::Get(
        cpr::Url{ "YOUR-URL" },
        cpr::Parameters{ {"sortBy", sortBy} }
    );

    std::vector<LeaderboardEntry> list;

    if (r.status_code == 200) {
        auto data = json::parse(r.text);
        for (const auto& entry : data) {
            list.push_back({
                entry["name"],
                entry["deaths"],
                entry["time"]
                });
        }
    }

    return list;
}
