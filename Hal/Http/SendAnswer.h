#pragma once
#include <string>
#include "../Structures.h"

std::string HostSolver(std::string hostname);

std::string SendAnswer(std::string ip,std::string token,std::string host = "");
Problem GetProblem(std::string ip,std::string token,int ProblemID,std::string host = "");
