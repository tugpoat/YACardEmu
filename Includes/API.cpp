/*
    YACardEmu
    ----------------
    Copyright (C) 2020-2022 wutno (https://github.com/GXTX)


    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "API.h"

API::API(int port, C1231LR::Settings *card)
{
	desiredPort = port;
	cardSettings = card;
}

void API::startHttp()
{
	httplib::Server svr;
	svr.set_mount_point("/", "public");

	svr.Post(R"(/api/v2/(\w+))", [&](const httplib::Request &req, httplib::Response &res) {
		auto match = req.matches[1];
		std::string str(match);

		// FIXME: We want to be able to stop the main application from here.
		if (str.compare("stop") == 0) {
			spdlog::info("Stopping application...");
			svr.stop();
		}

		spdlog::info("Method: {}", str);
		Methods(str, res);
	});

	svr.Get(R"(/api/v2/(\w+))", [&](const httplib::Request &req, httplib::Response &res) {
		auto match = req.matches[1];
		std::string str(match);

		spdlog::info("Resource: {}", str);
		Resources(str, res);
	});

	if (!svr.listen("0.0.0.0", !desiredPort ? 8080 : desiredPort)) {
		spdlog::critical("Failed to start HTTP server!\n");
		initalized = false;
	}
}

void API::Methods(std::string_view method, httplib::Response &res)
{
	//GenerateHeader(res);
	spdlog::info("meth");
}

void API::Resources(std::string_view resource, httplib::Response &res)
{
	//GenerateHeader(res);
	spdlog::info("res");

	switch 



}

std::string API::generateCardListJSON(std::string basepath)
{
	std::string list{"["};

	for (const auto &entry: ghc::filesystem::directory_iterator(basepath)) {
		std::string card{entry.path().string()};

		auto find = card.find(".track_0");

		if (find != std::string::npos) {
			card.replace(find, 8, "");
			list.append("{\"name\":\"");
#ifdef _WIN32
			list.append(card.substr(card.find_last_of("\\") + 1));
#else
			list.append(card.substr(card.find_last_of("/") + 1));
#endif
			list.append("\",\"image\":\"");

			auto find2 = card.find(".bin");
			if (find2 != std::string::npos)
				card.replace(find2, 4, ".png");
			
			std::string base64{};

			if (ghc::filesystem::exists(card)) {
				std::ifstream img(card.c_str(), std::ifstream::in | std::ifstream::binary);

				base64.resize(ghc::filesystem::file_size(card));

				img.read(reinterpret_cast<char *>(&base64[0]), base64.size());
				img.close();
			}

			std::string encoded = base64_encode(base64, false);
			list.append("data:image/png;base64, ");
			list.append(encoded);
			list.append("\"},");
		}
	}

	// remove the errant comma
	if (list.compare("[") != 0) {
		list.pop_back();
	}

	list.append("]");
	return list;
}
