#pragma once
#include <Windows.h>
#include "WinInet.h"
#pragma comment(lib, "Wininet.lib")
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <map>
#include <sstream>
#include <regex>
#include <future>

#ifdef DELETE
#undef DELETE
#endif

namespace HttpService
{
	struct Response
	{
		int statusCode = -1;
		std::string content = "";

		inline bool IsSuccess()
		{
			return statusCode >= 200 && statusCode <= 299;
		}
	};

	namespace Exceptions
	{
		class ConnectionException : public std::exception
		{
			private:
			const char* m_msg;

			public:
			ConnectionException(const char* msg) : m_msg(msg) {};

			virtual char const* what()
			{
				return m_msg;
			}
		};
	}

	namespace Method
	{
		struct Method_t
		{
			private:
			const char* value;
			public:
			inline Method_t(const char* str)
			{
				this->value = str;
			}

			inline operator const char* () const
			{
				return value;
			}

			inline operator std::string() const
			{
				return std::string(value);
			}
		};

		const Method_t GET("GET");
		const Method_t POST("POST");
		const Method_t PATCH("PATCH");
		const Method_t PUT("PUT");
		const Method_t DELETE("DELETE");
	}

	namespace ContentType
	{
		struct ContentType_t
		{
			private:
			const char* value;
			public:
			inline ContentType_t(const char* str)
			{
				this->value = str;
			}

			inline operator const char* () const
			{
				return value;
			}

			inline operator std::string() const
			{
				return std::string(value);
			}
		};

		const ContentType_t json("application/json");
		const ContentType_t xml("application/xml");
		const ContentType_t url_encoded("x-www-form-urlencoded");
		const ContentType_t text_plain("text/plain");
		const ContentType_t text_xml("text/xml");
	}

	using namespace Exceptions;
	class HttpHandle
	{
		private:
		HINTERNET hConnect;
		HINTERNET hRequest;

		void Close()
		{
			if(this->hRequest)
				InternetCloseHandle(this->hRequest);

			if (this->hConnect)
				InternetCloseHandle(this->hConnect);
		}

		public:
		HttpHandle(HINTERNET hInternet, std::string url, const std::string& method, int port = 0)
		{
			static std::regex url_regex(R"((https?:\/\/)?([^\/:]+)(\/.*)?)");

			std::string scheme;
			std::string domain;
			std::string path;

			std::smatch match;
			if (std::regex_search(url, match, url_regex))
			{
				scheme = match[1].str();
				domain = match[2].str();
				path = match[3].str();
			}

			DWORD requestFlag = 0;

			if(port == 0 && scheme == "http://")
			{
				port = INTERNET_DEFAULT_HTTP_PORT;
			}
			else if(port == 0 && scheme == "https://")
			{
				requestFlag = INTERNET_FLAG_SECURE;
				port = INTERNET_DEFAULT_HTTPS_PORT;
			}

			this->hConnect = InternetConnectA(hInternet, domain.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
			if (!hConnect) 
			{
				Close();
				throw new ConnectionException("Failed to connect host");
			} 

			this->hRequest = HttpOpenRequestA(hConnect, method.c_str(), path.c_str(), NULL, NULL, NULL, requestFlag, 0);
			if (!hRequest)
			{
				Close();
				throw new ConnectionException("Failed to open request");
			}
		}

		~HttpHandle()
		{
			Close();
		}

		inline const HINTERNET GetRequestHandle()
		{
			return this->hRequest;
		}

		inline bool SendRequest(const std::string& data = "", const std::string& header = "")
		{
			return HttpSendRequest(this->hRequest, header.c_str(), header.size(), (void*)data.c_str(), data.size());
		}

		int GetStatusCode()
		{
			DWORD statusCode = -1;
			DWORD length = sizeof(statusCode);
			HttpQueryInfo(this->hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, nullptr);

			return statusCode;
		}

		std::string ReadResponseContent()
		{
			constexpr int BUFFER_SIZE = 1024;
			char buffer[BUFFER_SIZE];

			DWORD bytesRead = 0;
			std::string content;
			while (InternetReadFile(this->hRequest, buffer, BUFFER_SIZE, &bytesRead) && bytesRead > 0)
			{
				content.append(buffer, bytesRead);
			}

			return content;
		}
	};

	class Client
	{
		private:
		int port = 0;
		HINTERNET hInternet = nullptr;
		std::string m_url = "";
		std::map<std::string, std::string> m_headers;

		public:
		Client(const char* url)
		{
			hInternet = InternetOpen(
				"HttpServive Client/1.0",
				INTERNET_OPEN_TYPE_DIRECT,
				NULL,
				NULL,
				0
			);

			if (!hInternet)
			{
				throw new ConnectionException("Failed to open internet");
			}

			this->m_url = url;
		}

		Client(const char* url, int port)
		{
			hInternet = InternetOpen(
				"HttpServive Client/1.0",
				INTERNET_OPEN_TYPE_DIRECT,
				NULL,
				NULL,
				0
			);

			if (!hInternet)
			{
				throw new ConnectionException("Failed to open internet");
			}

			this->m_url = url;
			this->port = port;
		}

		Client()
		{
			hInternet = InternetOpen(
				"HttpServive Client/1.0",
				INTERNET_OPEN_TYPE_DIRECT,
				NULL,
				NULL,
				0
			);

			if (!hInternet)
			{
				throw new ConnectionException("Failed to open internet");
			}
		}

		~Client()
		{
			InternetCloseHandle(hInternet);
		}

		inline void AddHeader(const std::string& name, const std::string& data)
		{
			m_headers.insert({name, data});
		}

		inline void AddHeaders(std::map<std::string, std::string> headers)
		{
			for (const auto& v : headers)
			{
				m_headers.insert({v.first, v.second});
			}
		}

		inline void ClearHeaders()
		{
			m_headers.clear();
		}

		inline Response Get(const std::string& path)
		{
			return Request(Method::GET, path, "", "");
		}

		inline Response Get()
		{
			return Get("/");
		}

		inline Response Post(const std::string& path, const ContentType::ContentType_t& type, const std::string& data)
		{
			return Request(Method::POST, path, type, data);
		}

		inline Response Post(const ContentType::ContentType_t& type, const std::string& data)
		{
			return Post("/", type, data);
		}

		Response Request(const Method::Method_t& method, const std::string& path, const std::string& type, const std::string& data)
		{
			std::map<std::string, std::string> additonalHeaders = {};

			if (!type.empty())
			{
				additonalHeaders["Contnet-Length"] = data.size();
				additonalHeaders["Content-Type"] = type;
			}

			HttpHandle handle(hInternet, this->m_url + path, method, this->port);
			handle.SendRequest(data, BuildHeaders(additonalHeaders));

			return Response{handle.GetStatusCode(), handle.ReadResponseContent()};
		}

		private:
		std::string BuildHeaders(const std::map<std::string, std::string>& additionalHeaders = {})
		{
			std::stringstream headerStream;

			for (const auto& v : m_headers)
			{
				headerStream << v.first << ":" << v.second << std::endl;
			}

			for (const auto& v : additionalHeaders)
			{
				headerStream << v.first << ":" << v.second << std::endl;
			}

			return headerStream.str();
		}
	};
}