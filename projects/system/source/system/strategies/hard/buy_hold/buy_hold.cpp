#include "buy_hold.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				BUY_HOLD::State BUY_HOLD::handle(const inputs_container_t & inputs, 
					double transaction, const State & input_state) const
				{
					LOGGER(logger, false);

					try
					{
						State state;

						state.position = transaction;

						return state;
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