#pragma once
#include <string>
#include "../Structures.h"

std::string HostSolver(std::string hostname);
std::string urlencode(const std::string& url);

std::string SendAnswer(std::string ip,std::string token,Answer answer);
Problem GetProblem(std::string host,std::string token,int ProblemID);

