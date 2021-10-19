#include "detail.hpp"

namespace solution
{
	namespace system
	{
		bool operator== (const Date_Time & lhs, const Date_Time & rhs)
		{
			return (
				(lhs.year   == rhs.year  ) &&
				(lhs.month  == rhs.month ) &&
				(lhs.day    == rhs.day   ) &&
				(lhs.hour   == rhs.hour  ) &&
				(lhs.minute == rhs.minute) &&
				(lhs.second == rhs.second));
		}

		bool operator!= (const Date_Time & lhs, const Date_Time & rhs)
		{
			return !(lhs == rhs);
		}

		bool operator< (const Date_Time & lhs, const Date_Time & rhs)
		{
			return (to_time_t(lhs) < to_time_t(rhs));
		}

		bool operator<= (const Date_Time & lhs, const Date_Time & rhs)
		{
			return !(lhs > rhs);
		}

		bool operator> (const Date_Time & lhs, const Date_Time & rhs)
		{
			return (to_time_t(lhs) > to_time_t(rhs));
		}

		bool operator>= (const Date_Time & lhs, const Date_Time & rhs)
		{
			return !(lhs < rhs);
		}

		std::ostream & operator<< (std::ostream & stream, const Date_Time & date_time)
		{
			const char date_delimeter = '/';
			const char time_delimeter = ':';

			stream <<
				std::setw(4) << std::right << std::setfill('0') << std::noshowpos << date_time.year  << date_delimeter <<
				std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.month << date_delimeter <<
				std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.day   << ' ';

			stream <<
				std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.hour   << time_delimeter <<
				std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.minute << time_delimeter <<
				std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.second;

			return stream;
		}

		std::time_t to_time_t(const Date_Time & date_time) noexcept
		{
			std::tm time = {
				static_cast < int > (date_time.second),
				static_cast < int > (date_time.minute),
				static_cast < int > (date_time.hour  ),
				static_cast < int > (date_time.day   ),
				static_cast < int > (date_time.month ) - 1,
				static_cast < int > (date_time.year  ) - 1900 };

			return std::mktime(&time);
		}

		Date_Time from_time_t(std::time_t time, bool has_milliseconds) noexcept
		{
			if (has_milliseconds)
			{
				time /= 1000LL;
			}

			auto tm = localtime(&time);

			Date_Time date_time;

			date_time.year   = static_cast < unsigned int > (tm->tm_year + 1900);
			date_time.month  = static_cast < unsigned int > (tm->tm_mon  + 1);
			date_time.day    = static_cast < unsigned int > (tm->tm_mday);
			date_time.hour   = static_cast < unsigned int > (tm->tm_hour);
			date_time.minute = static_cast < unsigned int > (tm->tm_min);
			date_time.second = static_cast < unsigned int > (tm->tm_sec);

			return date_time;
		}

		std::time_t duration(const Date_Time & date_time_1, const Date_Time & date_time_2) noexcept
		{
			return (to_time_t(date_time_1) - to_time_t(date_time_2));
		}

		unsigned int day_of_week(const Date_Time & date_time) noexcept
		{
			std::tm tm = { 0, 0, 0,
				static_cast < int > (date_time.day  ),
				static_cast < int > (date_time.month) - 1,
				static_cast < int > (date_time.year ) - 1900 };

			auto time = std::mktime(&tm);

			return static_cast < unsigned int > (std::localtime(&time)->tm_wday);
		}

	} // namespace system

} // namespace solution