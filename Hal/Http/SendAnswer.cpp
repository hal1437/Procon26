
#include "SendAnswer.h"
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

namespace asio = boost::asio;
namespace ip = asio::ip;

#include <boost/function_output_iterator.hpp>
#include <boost/bind.hpp>
#include <iterator>
#include <iomanip>


std::string HostSolver(std::string hostname){
	asio::io_service io_service;
	
	//TCPソケットを作成する
	ip::tcp::socket sock(io_service);

	ip::tcp::resolver resolver(io_service);
	ip::tcp::resolver::query query(hostname, "http");

	//名前解決
	return 	ip::tcp::endpoint(*resolver.resolve(query)).address().to_string();
}

std::string SendAnswer(std::string host,std::string token,Answer answer){
	try{

		asio::io_service io_service;
		ip::tcp::socket sock(io_service);

		//boost::system::error_code error;
		sock.connect(ip::tcp::endpoint(ip::address::from_string(HostSolver(host)), 80));

		std::string answer_str;
		std::stringstream ss;
		ss << answer;
		answer_str = ss.str();


		//メッセージを送信
		asio::streambuf request;
		std::ostream request_ostream(&request);
		request_ostream << "POST /answer?token=" << token << " HTTP/1.0\r\n";
		request_ostream << "Host:" << host << "\r\n\r\n";
		request_ostream << "answer=" << answer_str << "\r\n";
		std::cout << answer_str << std::endl;
		asio::write(sock, request);

		//メッセージを受信
		asio::streambuf buffer;
		boost::system::error_code error;
		asio::read(sock, buffer, asio::transfer_all(), error);
		if (error && error != asio::error::eof){
			std::cout << "receive failed: " << error.message() << std::endl;
		}else{
			std::cout << &buffer;
		}
		return "SendAnswer successfull";
	}catch (std::exception& e){
		return std::string("SendAnswer exception!:\n") + e.what();
	}
	
	return "";
}
Problem GetProblem(std::string host,std::string token,int ProblemID){
	try{
		boost::asio::io_service io_service;

		//TCPソケットを作成する
		ip::tcp::socket sock(io_service);

		//ホスト情報を設定する
		ip::tcp::endpoint endpoint(asio::ip::address::from_string(HostSolver(host)),80);

		//ソケットへ接続
		sock.connect(endpoint);

		//メッセージを送信
		//if(!host.empty())host = std::string("\r\nHost:") + host;
		asio::streambuf request;
		std::ostream request_ostream(&request);
		request_ostream << "GET  /quest" << ProblemID << ".txt?token=" << token << " HTTP/1.0\r\n";
		request_ostream << "Host:" << host << "\r\n\r\n";
		asio::write(sock, request);

		//メッセージを受信
		asio::streambuf buffer;
		boost::system::error_code error;
		asio::read(sock, buffer, asio::transfer_all(), error);
		if (error && error != asio::error::eof){
			std::cout << "receive failed: " << error.message() << std::endl;
			return Problem();
		}else{
			std::cout << "receive successfull:" << std::endl;
			std::ofstream tmp("QuestTMP.txt");
			int enable_index = 0;
			std::string str = (std::stringstream() << &buffer).str();
			
			for(enable_index = 0;(str[enable_index]!='\n' || str[enable_index+2]!='\n');enable_index++);
			str.erase(0,enable_index+2);
			for(enable_index = 0;(str[enable_index]=='\r' || str[enable_index]=='\n');enable_index++);
			str.erase(0,enable_index);
			tmp << str;
			tmp.close();
			return Problem("QuestTMP.txt");
		}
	}catch (std::exception& e){
		std::cout << std::string("SendAnswer exception!:\n") + e.what();
		return Problem();
	}
	
	return Problem();
}
