#include "pch.h"
#include "CppUnitTest.h"
#include "CDateTime.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DateTimeForArduino
{
	TEST_CLASS(DateTimeForArduino)
	{
	public:

		TEST_METHOD(CDateTimeInit)
		{
			CDateTime dt(1970, 1, 5, 23, 12, 34, 0);
			Assert::IsTrue(429154000ULL == dt.ToMilliSeconds());
			Assert::AreEqual<uint64_t>(429154000ULL, dt.ToMilliSeconds());
			Assert::IsTrue(429154 == dt.ToSeconds());
			Assert::IsTrue(((((5 - 1) * 24) + 23) * 60) + 12 == dt.ToMinutes());
			Assert::IsTrue(((5UL - 1) * 24) + 23 == dt.ToHours());
			Assert::AreEqual<uint32_t>(5 - 1, dt.ToDays());

			Assert::AreEqual<uint32_t>(0, dt.ToMonths());
			Assert::AreEqual<uint32_t>(0, dt.ToYears());

			Assert::AreEqual<uint32_t>(429154, dt.ToUNIXTime());
		}

		TEST_METHOD(CDateTime_CreatePart)
		{
			CDateTime dt(2022, 7, 13, 8, 1, 20, 0); // 1657699280
			Assert::AreEqual<uint32_t>(1657699280UL, dt.ToUNIXTime());
		}

		TEST_METHOD(CDateTime_CreateMillis)
		{
			CDateTime dt(1657587723333);
			Assert::AreEqual<uint64_t>(1657587723333ULL, dt.ToMilliSeconds());
			Assert::AreEqual<uint32_t>(333U, dt.GetMilliSecond());
			Assert::AreEqual<uint32_t>(1657587723UL, dt.ToSeconds());
			Assert::AreEqual<uint32_t>(1657587723UL, dt.ToUNIXTime());

		}

		TEST_METHOD(CDateTime_IsYearLeap)
		{
			Assert::IsFalse(CDateTime::IsYearLeap(1970));
			Assert::IsFalse(CDateTime::IsYearLeap(1971));
			Assert::IsTrue(CDateTime::IsYearLeap(1972));
			Assert::IsFalse(CDateTime::IsYearLeap(1973));
			Assert::IsFalse(CDateTime::IsYearLeap(1974));
			Assert::IsFalse(CDateTime::IsYearLeap(1975));
			Assert::IsTrue(CDateTime::IsYearLeap(1976));
			Assert::IsFalse(CDateTime::IsYearLeap(1977));
			Assert::IsFalse(CDateTime::IsYearLeap(1978));
			Assert::IsFalse(CDateTime::IsYearLeap(1979));
			Assert::IsTrue(CDateTime::IsYearLeap(1980));
			Assert::IsFalse(CDateTime::IsYearLeap(1900));
			Assert::IsFalse(CDateTime::IsYearLeap(2100));
			Assert::IsFalse(CDateTime::IsYearLeap(2200));
			Assert::IsFalse(CDateTime::IsYearLeap(1999));
			Assert::IsTrue(CDateTime::IsYearLeap(2000));
			Assert::IsFalse(CDateTime::IsYearLeap(2001));
		}

		TEST_METHOD(CDateTime_SecondInMonth)
		{
			//Assert::AreEqual<uint32_t>(30 * 24 * 60 * 60, CDateTime::SecondInMonth(0, 1970));
			Assert::AreEqual<uint32_t>(2678400, CDateTime::SecondInMonth(1, 1970));
			Assert::AreEqual<uint32_t>(2419200, CDateTime::SecondInMonth(2, 1970));
			Assert::AreEqual<uint32_t>(2505600, CDateTime::SecondInMonth(2, 1972));
			Assert::AreEqual<uint32_t>(2678400, CDateTime::SecondInMonth(3, 1970));
			Assert::AreEqual<uint32_t>(2592000, CDateTime::SecondInMonth(4, 1970));
			Assert::AreEqual<uint32_t>(2678400, CDateTime::SecondInMonth(5, 1970));
			Assert::AreEqual<uint32_t>(2592000, CDateTime::SecondInMonth(6, 1970));
			Assert::AreEqual<uint32_t>(2678400, CDateTime::SecondInMonth(7, 1970));
			Assert::AreEqual<uint32_t>(2678400, CDateTime::SecondInMonth(8, 1970));
			Assert::AreEqual<uint32_t>(2592000, CDateTime::SecondInMonth(9, 1970));
			Assert::AreEqual<uint32_t>(2678400, CDateTime::SecondInMonth(10, 1970));
			Assert::AreEqual<uint32_t>(2592000, CDateTime::SecondInMonth(11, 1970));
			Assert::AreEqual<uint32_t>(2678400, CDateTime::SecondInMonth(12, 1970));
			Assert::AreEqual<uint32_t>(0, CDateTime::SecondInMonth(13, 1970));

		}

		TEST_METHOD(CDateTime_GetYear)
		{
			CDateTime dt(0U, 0);
			Assert::AreEqual<uint32_t>(0, dt.ToUNIXTime());
			Assert::AreEqual<uint32_t>(1970U, dt.GetYear());

			CDateTime dt1(31536000U, 0);
			Assert::AreEqual<uint32_t>(1971U, dt1.GetYear());

			CDateTime dt2_1(315532799U, 0);
			Assert::AreEqual<uint32_t>(1979U, dt2_1.GetYear());

			CDateTime dt2(315532800U, 0);
			Assert::AreEqual<uint32_t>(1980U, dt2.GetYear());

			CDateTime dt3(315532801U, 0);
			Assert::AreEqual<uint32_t>(1980U, dt3.GetYear());

			CDateTime dt4(1640995200U, 0);
			Assert::AreEqual<uint32_t>(2022U, dt4.GetYear());
		}

		TEST_METHOD(CDateTime_GetMonth)
		{
			CDateTime dt(0U, 0);
			Assert::AreEqual<uint32_t>(1, dt.GetMonth());

			CDateTime dt1(2678400U, 0);
			Assert::AreEqual<uint32_t>(2, dt1.GetMonth());

			CDateTime dt2(1643673600U, 0);
			Assert::AreEqual<uint32_t>(2, dt2.GetMonth());

			CDateTime dt3(318211200U, 0);
			Assert::AreEqual<uint32_t>(2, dt3.GetMonth());

			CDateTime dt4(318211200U, 0);
			Assert::AreEqual<uint32_t>(2, dt4.GetMonth());
		}

		TEST_METHOD(CDateTime_GetDay)
		{
			CDateTime dt(0U, 0);
			Assert::AreEqual<uint32_t>(1, dt.GetDay());

			CDateTime dt1(86400U, 0);
			Assert::AreEqual<uint32_t>(2, dt1.GetDay());

			CDateTime dt2(1656979200U, 0);
			Assert::AreEqual<uint32_t>(5, dt2.GetDay());
		}

		TEST_METHOD(CDateTime_GetHour)
		{
			CDateTime dt(0U, 0);
			Assert::AreEqual<uint8_t>(0, dt.GetHour());

			CDateTime dt1(3600U, 0);
			Assert::AreEqual<uint8_t>(1, dt1.GetHour());

			CDateTime dt2(1657587723U, 0);
			Assert::AreEqual<uint8_t>(1, dt2.GetHour());
		}

		TEST_METHOD(CDateTime_GetMinute)
		{
			CDateTime dt(0U, 0);
			Assert::AreEqual<uint8_t>(0, dt.GetMinute());

			CDateTime dt1(60U, 0);
			Assert::AreEqual<uint8_t>(1, dt1.GetMinute());

			CDateTime dt2(1657587723U, 0);
			Assert::AreEqual<uint8_t>(2, dt2.GetMinute());
		}

		TEST_METHOD(CDateTime_GetSecond)
		{
			CDateTime dt(0U, 0);
			Assert::AreEqual<uint32_t>(0, dt.GetSecond());

			CDateTime dt1(59U, 0);
			Assert::AreEqual<uint32_t>(59, dt1.GetSecond());

			CDateTime dt2(1657587723U, 0);
			Assert::AreEqual<uint32_t>(3, dt2.GetSecond());
		}

		TEST_METHOD(CDateTime_GetMilliSecond)
		{
			CDateTime dt(0, 2000);
			Assert::AreEqual<uint32_t>(999U, dt.GetMilliSecond());

			CDateTime dt1(0, 0);
			Assert::AreEqual<uint32_t>(0U, dt1.GetMilliSecond());

			CDateTime dt2(1657587723U, 200);
			Assert::AreEqual<uint32_t>(200U, dt2.GetMilliSecond());
		}

		// =========== AddXXX methods
		TEST_METHOD(CDateTime_AddMilliSecond)
		{
			CDateTime dt(0, 2000);
			Assert::AreEqual<uint32_t>(0U, dt.ToUNIXTime());
			Assert::AreEqual<uint32_t>(999, dt.GetMilliSecond());

			dt.AddMilliSecond(1);
			Assert::AreEqual<uint32_t>(1U, dt.ToUNIXTime());
			Assert::AreEqual<uint32_t>(0U, dt.GetMilliSecond());
		}

		TEST_METHOD(CDateTime_AddSecond)
		{
			CDateTime dt(0, 2000);
			Assert::AreEqual<uint32_t>(0U, dt.ToUNIXTime());
			dt.AddSecond(1);
			Assert::AreEqual<uint32_t>(1U, dt.ToUNIXTime());

			CDateTime dt1(0, 2000);
			dt1.AddSecond(60);
			Assert::AreEqual<uint32_t>(60U, dt1.ToUNIXTime());

			CDateTime dt2(180U, 2000);
			dt2.AddSecond(-60);
			Assert::AreEqual<uint32_t>(120U, dt2.ToUNIXTime());

			CDateTime dt3(0U, 2000);
			dt3.AddSecond(-60);
			Assert::AreEqual<uint32_t>(4294967236U, dt3.ToUNIXTime());
		}

		TEST_METHOD(CDateTime_AddYear)
		{
			CDateTime dt(0, 2000);
			dt.AddYear(52); //2022/ 1640995200
			Assert::AreEqual<uint32_t>(2022, dt.GetYear());
			Assert::AreEqual<uint32_t>(1640995200U, dt.ToUNIXTime());

			CDateTime dt1(1999, 3, 15, 1, 2, 3, 2000);
			dt1.AddYear(1);
			Assert::AreEqual<uint32_t>(2000U, dt1.GetYear());
			Assert::AreEqual<uint32_t>(3U, dt1.GetMonth());
			Assert::AreEqual<uint32_t>(15U, dt1.GetDay());
			Assert::AreEqual<uint32_t>(1U, dt1.GetHour());

			CDateTime dt2(2001, 3, 15, 1, 2, 3, 2000);
			dt2.AddYear(-1);
			Assert::AreEqual<uint32_t>(2000U, dt2.GetYear());
			Assert::AreEqual<uint32_t>(3U, dt2.GetMonth());
			Assert::AreEqual<uint32_t>(15U, dt2.GetDay());
			Assert::AreEqual<uint32_t>(1U, dt2.GetHour());
			//Assert::AreEqual<uint32_t>(922755723UL, dt1.ToUNIXTime());
		}

		TEST_METHOD(CDateTime_AddMonth)
		{
			CDateTime dt(0, 2000);
			dt.AddMonth(1);
			Assert::AreEqual<uint8_t>(2U, dt.GetMonth());
			Assert::AreEqual<uint32_t>(2678400U, dt.ToUNIXTime());

			CDateTime dt1(1999, 3, 15, 1, 2, 3, 2000);
			dt1.AddMonth(1);
			Assert::AreEqual<uint32_t>(1999U, dt1.GetYear());
			Assert::AreEqual<uint32_t>(4U, dt1.GetMonth());
			Assert::AreEqual<uint32_t>(15U, dt1.GetDay());
			Assert::AreEqual<uint32_t>(1U, dt1.GetHour());

			CDateTime dt2(1999, 3, 31, 1, 2, 3, 2000);
			dt2.AddMonth(24);
			Assert::AreEqual<uint32_t>(2001U, dt2.GetYear());
			Assert::AreEqual<uint32_t>(3U, dt2.GetMonth());
			Assert::AreEqual<uint32_t>(31U, dt2.GetDay());
			Assert::AreEqual<uint32_t>(1U, dt2.GetHour());

			CDateTime dt3(1999, 1, 31, 1, 2, 3, 2000);
			dt3.AddMonth(-1);
			Assert::AreEqual<uint32_t>(1998U, dt3.GetYear());
			Assert::AreEqual<uint32_t>(12U, dt3.GetMonth());
			Assert::AreEqual<uint32_t>(31U, dt3.GetDay());
			Assert::AreEqual<uint32_t>(1U, dt3.GetHour());

			CDateTime dt4(1999, 1, 31, 1, 2, 3, 2000);
			dt4.AddMonth(-13);
			Assert::AreEqual<uint32_t>(1997U, dt4.GetYear());
			Assert::AreEqual<uint32_t>(12U, dt4.GetMonth());
			Assert::AreEqual<uint32_t>(31U, dt4.GetDay());
			Assert::AreEqual<uint32_t>(1U, dt4.GetHour());
		}
	};
}
