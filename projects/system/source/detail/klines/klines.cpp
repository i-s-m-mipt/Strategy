#include "klines.hpp"

namespace solution
{
	namespace system
	{
		namespace detail
		{
			void Kline::update_date_time() noexcept
			{
				date_time_open. from_time_t(time_open,  true);
				date_time_close.from_time_t(time_close, true);
			}

		} // namespace detail

	} // namespace system

} // namespace solution