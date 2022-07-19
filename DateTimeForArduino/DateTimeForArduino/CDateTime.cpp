#include "pch.h"
#include "CDateTime.h"

CDateTime::CDateTime(uint16_t aYear, uint8_t aMonth, uint8_t aDay, uint8_t aHour, uint8_t aMinute, uint8_t aSecond, uint16_t aMilliSecond)
{
	aSecond = aSecond > 59 ? 59 : aSecond;
	aMinute = aMinute > 59 ? 59 : aMinute;
	aHour = aHour > 24 ? 24 : aHour;
	if (aDay == 0)
	{
		aDay = 1;
	}
	else if (aDay > DayInMonth(aMonth, aYear))
	{
		aDay = DayInMonth(aMonth, aYear);
	} 
	if (aMonth == 0)
	{
		aMonth = 1;
	}
	else if (aMonth > 12)
	{
		aMonth = 12;
	}


	_dateTime = 0;

    for (uint16_t y = 1970; y < aYear; y++)
	{
		_dateTime += SecondInYear(y);
	}

	uint8_t maxMonths = aMonth > 12 ? 12 : aMonth;
	for (uint8_t m = 1; m < maxMonths; m++)
	{
		_dateTime += SecondInMonth(m, aYear);
	}
	_dateTime += (uint32_t)aSecond + (60UL * (aMinute + 60UL * (aHour + 24UL * (aDay - 1))));

	_ms = aMilliSecond>999?999: aMilliSecond;
};

CDateTime::CDateTime(uint32_t aUNIXtime, uint32_t aMilliSecond)
{
	_dateTime = aUNIXtime;
	_ms = aMilliSecond > 999 ? 999 : aMilliSecond;
};

CDateTime::CDateTime(uint64_t aMilliSecond)
{
	_dateTime =(uint32_t) (aMilliSecond / 1000);
	_ms = (uint16_t)(aMilliSecond % 1000);
};


bool CDateTime::IsYearLeap(uint16_t aYear)
{
	if (aYear % 400 == 0)
	{
		return true;
	};

	if (aYear % 100 == 0)
	{
		return false;
	};

	if (aYear % 4 == 0)
	{
		return true;
	};

	return false;
};

uint32_t CDateTime::SecondInYear(uint16_t aYear)
{
	return (IsYearLeap(aYear) ? 366UL : 365UL) * 24 * 60 * 60;
};

uint32_t CDateTime::SecondInMonth(uint8_t aMonth, uint16_t aYear)
{
	return DayInMonth(aMonth, aYear) * 24 * 60 * 60;
}

uint8_t CDateTime::DayInMonth(uint8_t aMonth, uint16_t aYear)
{
	switch (aMonth)
	{
	case 9:
	case 4:
	case 6:
	case 11:
		return 30 ;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
		break;
	case 2:
		return (IsYearLeap(aYear) ? 29 : 28);
		break;
	default:
		return 0;
		break;
	};
};

uint32_t CDateTime::YearToSecond(uint16_t* aYear)
{
	uint32_t allsec = 0;
	uint16_t curYear = 1970;
	
	while (allsec + SecondInYear(curYear) <= _dateTime)
	{
		allsec += SecondInYear(curYear);
		curYear ++ ;
	};
	*aYear = curYear;
	return allsec;
}

uint32_t CDateTime::YearAndMonthToSecond(uint8_t* aMonth)
{
	uint32_t allsec = 0;
	uint16_t curYear = 1970;
	allsec = YearToSecond(&curYear);

	uint8_t curMon = 1;

	while (allsec + SecondInMonth(curMon, curYear) <= _dateTime)
	{
		allsec += SecondInMonth(curMon++, curYear);
	}
	*aMonth = curMon;

	return allsec;
}

uint16_t CDateTime::GetYear()
{
	uint32_t allsec = 0;
	uint16_t curYear = 1970;

	allsec = YearToSecond(&curYear);

	return curYear;
};

uint8_t CDateTime::GetMonth()
{
	uint8_t curMonth = 1;	
	uint32_t allsec = YearAndMonthToSecond(&curMonth);
	return curMonth;
};

uint8_t CDateTime::GetDay()
{
	uint8_t curMonth = 1;	
	uint32_t allsec = YearAndMonthToSecond(&curMonth);
	return (_dateTime - allsec)/(24*60*60)+1;
};

uint8_t CDateTime::GetHour()
{
	uint8_t curMonth = 1;
	uint32_t allsec = YearAndMonthToSecond(&curMonth);
	uint32_t hourless = (_dateTime - allsec) % (24 * 60 * 60);
	return hourless / (60 * 60);
};

uint8_t CDateTime::GetMinute()
{
	uint8_t curMonth = 1;
	uint32_t allsec = YearAndMonthToSecond(&curMonth);
	uint32_t minutesless = (_dateTime - allsec) % (60 * 60);
	return minutesless / 60;
};

uint8_t CDateTime::GetSecond()
{
	uint8_t curMonth = 1;
	uint32_t allsec = YearAndMonthToSecond(&curMonth);
	return (_dateTime - allsec) % 60;
};

uint16_t CDateTime::GetMilliSecond()
{
	return _ms;
};

//=======  ToXXX metods ===================

uint64_t CDateTime::ToMilliSeconds()
{
	uint64_t res = (uint64_t)_dateTime * 1000ULL + (uint64_t)_ms;
	return res;
};

uint32_t CDateTime::ToSeconds()
{
	return _dateTime;
};

uint32_t CDateTime::ToMinutes()
{
	return _dateTime / 60UL;
};

uint32_t CDateTime::ToHours()
{
	return _dateTime / 3600UL;
};

uint32_t CDateTime::ToDays()
{
	return _dateTime / (60UL * 60UL * 24UL);
};

uint32_t CDateTime::ToMonths()
{
	return 0UL;
};

uint32_t CDateTime::ToYears()
{
	return 0;
}

uint32_t CDateTime::ToUNIXTime()
{
	return _dateTime;
};

// ======= AddXXX Methods ==================

bool CDateTime::AddYear(int cnt)
{
	uint16_t Y = GetYear();
	uint8_t M = GetMonth();
	uint8_t D = GetDay();
	uint8_t h = GetHour();
	uint8_t m = GetMinute();
	uint8_t s = GetSecond();
	uint16_t ms = GetMilliSecond();
	CDateTime newDT(Y + cnt, M, D, h, m, s, ms);
	_dateTime = newDT.ToUNIXTime();
	return true;
};

bool CDateTime::AddMonth(int cnt = 1)
{
	uint16_t allMonth = GetYear() * 12 + GetMonth() + cnt;
	uint16_t Y = allMonth / 12;
	uint8_t M = allMonth % 12;
	uint8_t D = GetDay();
	uint8_t h = GetHour();
	uint8_t m = GetMinute();
	uint8_t s = GetSecond();
	uint16_t ms = GetMilliSecond();
	CDateTime newDT(Y, M, D, h, m, s, ms);
	_dateTime = newDT.ToUNIXTime();
	return true;
};

bool CDateTime::AddDay(int cnt = 1)
{
	AddSecond(cnt * 24 * 60 * 60);
	return true;
};

bool CDateTime::AddHour(int cnt = 1)
{
	AddSecond(cnt*60*60);
	return true;
};

bool CDateTime::AddMinute(int cnt = 1)
{
	AddSecond(cnt * 60);
	return true;
};

bool CDateTime::AddSecond(int cnt = 1)
{
	_dateTime += cnt;
	return true;
};

bool CDateTime::AddMilliSecond(int cnt = 1)
{
	uint32_t allms = GetMilliSecond() + cnt;
	AddSecond(allms / 1000);
	_ms = allms % 1000;
	return true;
};

