#include "headers.h"
#include "kafka_connector.h"

namespace calc_module {

KafkaClient::KafkaClient(const std::string& kafka, const std::string& inTopic, const std::string& outTopic) :
	m_kafka(kafka),
	m_inTopic(inTopic),
	m_outTopic(outTopic)
{
	BOOST_LOG_TRIVIAL(info) << "KafkaClient constructed. "
		"Kafka broker: " << kafka
		<< ", input topic: " << inTopic
		<< ", output topic: " << outTopic;
}

void KafkaClient::SendMessage(const std::string& key, const std::string& value)
{
	BOOST_LOG_TRIVIAL(info) << "Sending message: " << value << ", key:" << key;

	try
	{
		static cppkafka::Configuration config = {{"metadata.broker.list", m_kafka}};
		static cppkafka::Producer producer(config);
		static cppkafka::MessageBuilder builder(m_outTopic);

		builder.key(key).payload(value);
		producer.produce(builder);
	}
	catch (const std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "SendMessage() exception: " << e.what();
	}

	BOOST_LOG_TRIVIAL(info) << "Sending message succeeded";
}

void KafkaClient::WaitMessage(std::string& key, std::string& value)
{
	BOOST_LOG_TRIVIAL(debug) << "Waiting message...";

	cppkafka::Configuration config =
	{
		// The list of brokers we'll use
		{ "metadata.broker.list", m_kafka },

		// Disable auto commit
		{ "enable.auto.commit", false },

		// Client group session timeout
		{ "session.timeout.ms", 60000 },

		// Our consumer's group
		{ "group.id", "calc-module" },
	};
	cppkafka::Consumer consumer(config);
	consumer.subscribe({m_inTopic});
	while (true)
	{
		cppkafka::Message msg = consumer.poll();
		if (!msg)
			continue;

		// Messages can contain error notifications rather than actual data
		if (msg.get_error())
		{
			if (!msg.is_eof())
			{
				BOOST_LOG_TRIVIAL(warning) << "Error message recieved";
			}
			continue;
		}

		key = msg.get_key();
		value = msg.get_payload();

		BOOST_LOG_TRIVIAL(info) << "Got message: " << value << ", key: " << key;

		return;
	}
}

} // calc_module
