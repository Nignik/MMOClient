#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <iostream>

namespace net
{
	template <typename T>
	class client_interface
	{
	public:
		client_interface() {}
		virtual ~client_interface()
		{
			Disconnect();
		}

		// Connect to server with hostname/ip-address and port
		bool Connect(const std::string& host, const uint16_t port)
		{
			try
			{
				// Resolve hostname/ip-address into tangiable physical address
				asio::ip::tcp::resolver resolver(m_context);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

				// Create connection
				m_connection = std::make_unique<connection<T>>(connection<T>::owner::client, m_context, asio::ip::tcp::socket(m_context), m_qMessagesIn);

				// Tell the connection object to connect to server
				m_connection->ConnectToServer(endpoints);

				// Start Context Thread
				thrContext = std::thread([this]() { m_context.run(); });
			}
			catch (std::exception& e)
			{
				std::cerr << "Client Exception: " << e.what() << "\n";
				return false;
			}
			return true;
		}

		void Disconnect()
		{
			// If connection exists, and it's connected then...
			if (IsConnected())
			{
				// ...disconnect from server gracefully
				m_connection->Disconnect();
			}

			// Either way, we're also done with the asio context...				
			m_context.stop();
			// ...and its thread
			if (thrContext.joinable())
				thrContext.join();

			// Destroy the connection object
			m_connection.release();
		}

		// Check if client is actually connected to a server
		bool IsConnected()
		{
			if (m_connection)
				return m_connection->IsConnected();
			else
				return false;
		}

		// Send message to server
		void Send(const message<T>& msg)
		{
			if (IsConnected())
				m_connection->Send(msg);
		}

		// Retrieve queue of messages from server
		tsqueue<owned_message<T>>& Incoming()
		{
			return m_qMessagesIn;
		}

	protected:
		asio::io_context m_context;
		std::thread thrContext;
		std::unique_ptr<connection<T>> m_connection;

	private:
		tsqueue<owned_message<T>> m_qMessagesIn;
	};
}