#include <sstream>
#include <iostream>
#include "message.h"

// anonymous
namespace {

template <typename T>
std::vector<T> GetVector(const pt::ptree& tree, const pt::ptree::key_type& key)
{
	std::vector<T> r;
	for (auto& item : tree.get_child(key))
		r.push_back(item.second.get_value<T>());
	return r;
}

}

namespace calc_module {

Message::Message(const std::string& input)
{
	std::stringstream ss(input);
	std::cout << ss.str() << std::endl;
	pt::read_json(ss, m_ptree);

	m_command = m_ptree.get<std::string>("command");
	m_ptree.erase("command");
	try
	{
		m_params = GetVector<std::string>(m_ptree, "params");
		m_ptree.erase("params");
	}
	catch(...)
	{
		std::cout << "Message: GetVector error" << std::endl;
	}
}

std::string Message::GetString() const
{
	std::stringstream ss;
	pt::write_json(ss, m_ptree);
	return ss.str();
}

void Message::SetText(const std::string& text)
{
	m_ptree.put("text", text);
}

std::vector<std::string> Message::GetParams() const
{
	return m_params;
}

std::string Message::GetCommand() const
{
	return m_command;
}

} // calc_module
