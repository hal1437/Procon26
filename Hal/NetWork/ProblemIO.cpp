
#include "ProblemIO.h"

Problem GetProblem(std::string host,std::string filename,std::string token){
	const std::string query = std::string("curl 'http://") + host + "/" + filename + "?token=" + token + "'>GetProblem.log";
	std::system(query.c_str());
	std::cout << std::ifstream("GetProblem.log").rdbuf();
	return Problem("GetProblem.log");
}

void SendAnswer(std::string host,std::string filename,std::string token){
	const std::string query = std::string("curl 'http://") + host + "/answer' "
							  "--form-string 'token=" + token + "' "
							  "-F 'answer=@" + filename + "' "
							  ">SendAnswer.log";
	std::system(query.c_str());
	std::cout << std::ifstream("SendAnswer.log").rdbuf();
}


