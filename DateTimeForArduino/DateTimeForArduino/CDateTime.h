#ifndef file_CDateTime
#define file_CDateTime

//#pragma once
#include <cstdint>


class CDateTime
{
public:
	CDateTime(uint16_t aYear, uint8_t aMonth, uint8_t aDay, uint8_t aHour, uint8_t aMinute, uint8_t aSecond, uint16_t aMilliSecond);
	CDateTime(uint32_t aUNIXtime, uint32_t aMilliSecond);
	CDateTime(uint64_t aMilliSecond = 0);

	uint16_t GetYear();
	uint8_t GetMonth();
	uint8_t GetDay();
	uint8_t GetHour();
	uint8_t GetMinute();
	uint8_t GetSecond();
	uint16_t GetMilliSecond();

	uint64_t ToMilliSeconds();
	uint32_t ToSeconds();
	uint32_t ToMinutes();
	uint32_t ToHours();
	uint32_t ToDays();
	uint32_t ToMonths();
	uint32_t ToYears();
	uint32_t ToUNIXTime();

	static bool IsYearLeap(uint16_t aYear);
	static uint32_t SecondInYear(uint16_t aYear);
	static uint32_t SecondInMonth(uint8_t aMonth, uint16_t aYear);
	static uint8_t DayInMonth(uint8_t aMonth, uint16_t aYear);

	
	bool AddYear(int cnt);
	bool AddMonth(int cnt);
	bool AddDay(int cnt);
	bool AddHour(int cnt);
	bool AddMinute(int cnt);
	bool AddSecond(int cnt);
	bool AddMilliSecond(int cnt);

private:
	uint32_t _dateTime;
	uint16_t _ms;

	uint32_t YearToSecond(uint16_t* aYear);
	uint32_t YearAndMonthToSecond(uint8_t* aMonth);
};

#endif

