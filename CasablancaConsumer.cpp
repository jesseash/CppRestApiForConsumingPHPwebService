// CasablancaConsumer.cpp : Defines the entry point for the console application.
//Reference: Marius Bancila's Blog https://mariusbancila.ro/blog/2017/11/19/revisited-full-fledged-client-server-example-with-c-rest-sdk-2-10/

#include "stdafx.h"  //used for windows
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

pplx::task<int> Post();
void make_request(http_client & client, method mtd, json::value const & jvalue);
void display_field_map_json(json::value & jvalue);
pplx::task<http_response> make_task_request(http_client & client,
	method mtd,
	json::value const & jvalue);
int _tmain(int argc, _TCHAR* argv[])
{
	http_client client(U("http://yourUrlAddress/WebService.php"));
	wcout << L"\npost values (POST)\n";
	///hash keys and values for json object here
	std::vector<std::pair<utility::string_t, json::value>> putvalue;
	putvalue.push_back(make_pair(L"name", json::value(L"Roy Rogers")));
	make_request(client, methods::POST, web::json::value::object(putvalue));
	

	return 0;
}




void display_field_map_json(json::value & jvalue)
{
	if (!jvalue.is_null())
	{
		for (auto const & e : jvalue.as_object())
		{
			wcout << e.first << L" : " << e.second.as_string() << endl;
			//parse data here 

		}
	}
}

pplx::task<http_response> make_task_request(http_client & client,
	method mtd,
	json::value const & jvalue)
{
	return (mtd == methods::GET || mtd == methods::HEAD) ?
		client.request(mtd, L"/restdemo") :
		client.request(mtd, L"/restdemo", jvalue);
}

void make_request(http_client & client, method mtd, json::value const & jvalue)
{
	make_task_request(client, mtd, jvalue)
		.then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			return response.extract_json();
		}
		return pplx::task_from_result(json::value());
	})
		.then([](pplx::task<json::value> previousTask)
	{
		try
		{
			display_field_map_json(previousTask.get());
		}
		catch (http_exception const & e)
		{
			wcout << e.what() << endl;
		}
	})
		.wait();
}
