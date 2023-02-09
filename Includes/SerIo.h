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

#ifndef SERIO_H
#define SERIO_H

#ifdef _WIN32
#	include <windows.h>
#endif

#include <iostream>
#include <vector>

#include <libserialport.h>

#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/spdlog.h"

class SerIo {
public:
	enum class Status {
		Okay,
		Timeout,
		ReadError,
		WriteError,
		ZeroSizeError,
	};

	struct Settings {
		int baudRate           = 9600;
		sp_parity parity       = SP_PARITY_NONE;
		std::string devicePath = "";
	};

	bool m_isPipe            = false;
	Settings* m_portSettings = nullptr;

	SerIo(SerIo::Settings* settings);
	~SerIo();

	bool Open();
	SerIo::Status Read(std::vector<uint8_t>& buffer);
	SerIo::Status Write(std::vector<uint8_t>& buffer);
	void SendAck();

private:
#ifdef _WIN32
	HANDLE m_pipeHandle = INVALID_HANDLE_VALUE;
#endif

	sp_port* m_portHandle        = nullptr;
	sp_port_config* m_portConfig = nullptr;

	std::vector<uint8_t> serialBuffer{};
};

#endif
