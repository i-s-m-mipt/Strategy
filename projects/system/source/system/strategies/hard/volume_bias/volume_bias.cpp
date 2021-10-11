#include "volume_bias.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				VOLUME_BIAS::State VOLUME_BIAS::handle(const inputs_container_t & inputs, 
					double transaction, const State & input_state) const
				{
					LOGGER(logger, false);

					try
					{
						State state = input_state;

						auto total_buy_volume  = 0.0;
						auto total_sell_volume = 0.0;

						for (const auto & input : inputs)
						{
							total_buy_volume  += input.volume_buy_base;
							total_sell_volume += input.volume_sell_base;
						}

						if (total_buy_volume / (total_buy_volume + total_sell_volume) > 0.48)
						{
							state.position = transaction;
						}
						else
						{
							state.position = -transaction;
						}

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