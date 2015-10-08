#pragma once

#include "../Structure/Problem.h"
#include "../Structure/Answer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

Problem GetProblem(std::string host,std::string filename,std::string token);
void SendAnswer(std::string host,std::string filename,std::string token);

