#include "headers.h"

#include <iostream>

#include "kafka_connector.h"
#include "message.h"

namespace {

std::string ExecuteShellCommand(const std::string& cmd)
{
	char buffer[128];
	std::string result = "";
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try 
	{
		while (!feof(pipe)) 
		{
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (const std::exception& e)
	{
		pclose(pipe);
		throw e;
	}
	pclose(pipe);
	return result;
}

// trim from end (in place)
static inline void RTrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

void InitLogging()
{
	boost::log::core::get()->set_filter
	(
		boost::log::trivial::severity >= boost::log::trivial::info
	);
}

} // anonymous namespace

int main()
{
	InitLogging();
	calc_module::KafkaClient kafka("evg.veretennikov.pserver.ru:9092", "to-module", "to-connector");
	BOOST_LOG_TRIVIAL(info) << "Service started...";

	while(true)
	{
		std::string key; std::string value;
		kafka.WaitMessage(key, value);

		calc_module::Message msg(value);
		if (msg.GetCommand() != "bc")
		{
			continue;
		}
		else
		{
			BOOST_LOG_TRIVIAL(info) << "Calc message. Processing...";
			const std::vector<std::string> Params = msg.GetParams();
			std::string concatParams;
			for (auto it = Params.begin(); it != Params.end(); it++)
				concatParams += *it;

			const std::string Command = "echo \"" + concatParams + "\" | bc -l";
			std::string output = ExecuteShellCommand(Command);
			RTrim(output);
			BOOST_LOG_TRIVIAL(info) << "Calculating result: \"" << output << "\"";
			msg.SetText(output);
			value = msg.GetString();
			kafka.SendMessage(key, value);
		}
	}
}
