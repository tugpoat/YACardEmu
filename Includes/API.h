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

#ifndef API_H
#define API_H

#include <vector>
#include <filesystem>

#include "C1231LR.h"
#include "base64.h"

#include "httplib.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"

class API
{
public:
	API(int port, C1231LR::Settings *card);
	~API();

	void startHttp();
	bool initalized{true};
private:
	enum class Actions {
		Cards,
		InsertedCard,
		Dispenser,
	};

	int desiredPort{8080};
	C1231LR::Settings *cardSettings{nullptr};

	void Methods(std::string_view method, httplib::Response &res);
	void Resources(std::string_view resource, httplib::Response &res);
	std::string generateCardListJSON(std::string basepath)
};

#endif //API_H
