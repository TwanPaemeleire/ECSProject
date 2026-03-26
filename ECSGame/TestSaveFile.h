#pragma once
#include <json.hpp>

struct TestSaveFile
{
	float Health = 20.0f;
	int Level = 20;
	int Coins = 50000;
};

inline void to_json(nlohmann::json& j, const TestSaveFile& p)
{
    j = nlohmann::json{
        { "Health", p.Health },
        { "Level", p.Level },
        { "Coins", p.Coins }
    };
}

inline void from_json(const nlohmann::json& j, TestSaveFile& p)
{
    j.at("Health").get_to(p.Health);
    j.at("Level").get_to(p.Level);
    j.at("Coins").get_to(p.Coins);
}