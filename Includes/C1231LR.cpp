/*
    YACardEmu
    ----------------
    Copyright (C) 2020-2023 wutno (https://github.com/GXTX)


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

#include "C1231LR.h"

C1231LR::C1231LR(CardIo::Settings* cardSettings) : CardIo(cardSettings) {}

void C1231LR::UpdateRStatus()
{
	// We "grab" the card for the user
	if (m_localStatus == CardPosition::POS_EJECTED_NOT_REMOVED) {
		cardSettings->insertedCard = false;
		cardSettings->hasCard      = false;
		MoveCard(MovePositions::NO_CARD);
	}

	// We require the user to "insert" a card if we're waiting
	if (cardSettings->insertedCard && m_localStatus == CardPosition::NO_CARD) {
		MoveCard(MovePositions::READ_WRITE_HEAD);

		if (runningCommand && status.s == S::WAITING_FOR_CARD) {
			status.s = S::RUNNING_COMMAND;
		}
	}
}

bool C1231LR::HasCard()
{
	return (m_localStatus != CardPosition::NO_CARD &&
	        m_localStatus != CardPosition::POS_EJECTED_NOT_REMOVED);
}

void C1231LR::DispenseCard()
{
	if (m_localStatus != CardPosition::NO_CARD) {
		spdlog::info("Error dispensing card - card present?");
		SetSError(S::ILLEGAL_COMMAND);
	} else {
		spdlog::info("Dispensing new card");
		MoveCard(MovePositions::DISPENSER_THERMAL);
	}
}

void C1231LR::EjectCard()
{
	if (m_localStatus != CardPosition::NO_CARD) {
		spdlog::info("Ejecting card");
		MoveCard(MovePositions::EJECT);
	}
}

uint8_t C1231LR::GetRStatus()
{
	return static_cast<uint8_t>(m_localStatus);
}

void C1231LR::MoveCard(CardIo::MovePositions position)
{
	switch (position) {
	case MovePositions::NO_CARD:
		m_localStatus         = CardPosition::NO_CARD;
		cardSettings->hasCard = false;
		break;
	case MovePositions::READ_WRITE_HEAD:
		m_localStatus         = CardPosition::POS_MAG;
		cardSettings->hasCard = true;
		break;
	case MovePositions::THERMAL_HEAD:
		m_localStatus         = CardPosition::POS_THERM;
		cardSettings->hasCard = true;
		break;
	case MovePositions::DISPENSER_THERMAL:
		m_localStatus         = CardPosition::POS_THERM_DISP;
		cardSettings->hasCard = true;
		break;
	case MovePositions::EJECT:
		m_localStatus         = CardPosition::POS_EJECTED_NOT_REMOVED;
		cardSettings->hasCard = false;
		break;
	default: break;
	}
}

CardIo::MovePositions C1231LR::GetCardPos()
{
	switch (m_localStatus) {
	case CardPosition::NO_CARD: return MovePositions::NO_CARD;
	case CardPosition::POS_MAG: return MovePositions::READ_WRITE_HEAD;
	case CardPosition::POS_THERM: return MovePositions::THERMAL_HEAD;
	case CardPosition::POS_THERM_DISP:
		return MovePositions::DISPENSER_THERMAL;
	case CardPosition::POS_EJECTED_NOT_REMOVED: return MovePositions::EJECT;
	default: return MovePositions::NO_CARD;
	}
}
