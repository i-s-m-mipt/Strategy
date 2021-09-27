#include "detail.hpp"

namespace solution
{
	namespace system
	{
		namespace detail
		{
			std::time_t Date_Time::to_time_t() const noexcept
			{
				std::tm time = {
					static_cast < int > (second),
					static_cast < int > (minute),
					static_cast < int > (hour),
					static_cast < int > (day),
					static_cast < int > (month) - 1,
					static_cast < int > (year)  - 1900 };

				return std::mktime(&time);
			}

			void Date_Time::from_time_t(std::time_t time, bool has_milliseconds) noexcept
			{
				if (has_milliseconds)
				{
					time /= 1000LL;
				}

				auto tm = localtime(&time);

				year   = static_cast < unsigned int > (tm->tm_year + 1900);
				month  = static_cast < unsigned int > (tm->tm_mon  + 1);
				day    = static_cast < unsigned int > (tm->tm_mday);
				hour   = static_cast < unsigned int > (tm->tm_hour);
				minute = static_cast < unsigned int > (tm->tm_min);
				second = static_cast < unsigned int > (tm->tm_sec);
			}

			bool operator== (const Date_Time & lhs, const Date_Time & rhs)
			{
				return (
					(lhs.year   == rhs.year)   &&
					(lhs.month  == rhs.month)  &&
					(lhs.day    == rhs.day)    &&
					(lhs.hour   == rhs.hour)   &&
					(lhs.minute == rhs.minute) &&
					(lhs.second == rhs.second));
			}

			bool operator!= (const Date_Time & lhs, const Date_Time & rhs)
			{
				return !(lhs == rhs);
			}

			bool operator< (const Date_Time & lhs, const Date_Time & rhs)
			{
				return (lhs.to_time_t() < rhs.to_time_t());
			}

			bool operator<= (const Date_Time & lhs, const Date_Time & rhs)
			{
				return !(lhs > rhs);
			}

			bool operator> (const Date_Time & lhs, const Date_Time & rhs)
			{
				return (lhs.to_time_t() > rhs.to_time_t());
			}

			bool operator>= (const Date_Time & lhs, const Date_Time & rhs)
			{
				return !(lhs < rhs);
			}

			std::ostream & operator<< (std::ostream & stream, const Date_Time & date_time)
			{
				const char delimeter = '/';

				stream <<
					std::setw(4) << std::right << std::setfill('0') << std::noshowpos << date_time.year  << delimeter <<
					std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.month << delimeter <<
					std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.day;

				return stream;
			}

			std::time_t duration(const Date_Time & date_time_1, const Date_Time & date_time_2)
			{
				return (date_time_1.to_time_t() - date_time_2.to_time_t());
			}

		} // namespace detail

	} // namespace system

} // namespace solution