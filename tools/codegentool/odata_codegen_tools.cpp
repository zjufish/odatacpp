﻿#include "odata_codegen_initializer.h"
#include "odata_codegen_writer.h"
#include "odata/client/odata_client.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/json.h"
#include "cpprest/http_client.h"
#include "cpprest/containerstream.h"
#include <iostream>
#include <chrono>
#include <string>

using namespace ::odata::codegen::tools;
using namespace concurrency;
using namespace concurrency::streams;
using namespace web::http;
using namespace web::http::client;
using namespace ::pplx;
using namespace ::web::http::client;
using namespace ::web::http;

#ifdef WIN32
int _tmain(int argc, ::utility::char_t* argv[])
#else
int main(int argc, ::utility::char_t* argv[])
#endif
{
	if (argc != 3 && argc != 5)
	{
		std::cout << "cmd argument error";
		return -1;
	}

	::utility::string_t metadata_url = *++argv;
	::utility::string_t file_name = *++argv;
	
	::utility::string_t user_name;
	::utility::string_t password;
	if (argc > 3)
	{
		user_name = *++argv;
		password = *++argv;
	}

	auto initializer = std::make_shared<odata_codegen_initializer>();
	initializer->begin_initialize(metadata_url, file_name, user_name, password).then(
	   [initializer] (int) -> int
	   {
		   auto writer = std::make_shared<odata_codegen_writer>(initializer);
		   return writer->begin_generate_file().wait();
	   }).wait();

	return 0;
}

