#pragma once
#include <string>
#include <cppkafka/producer.h>
#include <cppkafka/consumer.h>

namespace calc_module {

class KafkaClient
{
public:
	KafkaClient(const std::string& kafka, const std::string& inTopic, const std::string& outTopic);
	void SendMessage(const std::string& key, const std::string& value);
	void WaitMessage(std::string& key, std::string& value);

private:
	const std::string m_kafka;
	const std::string m_inTopic;
	const std::string m_outTopic;
};

} // calc_module
