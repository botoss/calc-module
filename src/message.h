#pragma once
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

namespace calc_module {

class Message
{
public:
	Message(const std::string& input);
	std::string GetString() const;
	void SetText(const std::string& text);
	std::vector<std::string> GetParams() const;
	std::string GetCommand() const;

private:
	pt::ptree m_ptree;

	std::vector<std::string> m_params;
	std::string m_text;
	std::string m_command;
};

} // calc_module
