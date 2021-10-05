#include "macd_mfi.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				MACD_MFI::State MACD_MFI::handle(const inputs_container_t & inputs, State input_state) const
				{
					LOGGER(logger);

					try
					{

					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < strategy_exception > (logger, exception);
					}
				}

			} // namespace hard

		} // namespace strategies

	} // namespace system

} // namespace solution