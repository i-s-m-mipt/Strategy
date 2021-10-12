#include "assimilator.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				Strategy::State Assimilator::handle(const inputs_container_t & inputs,
					const State & input_state, double transaction) const
				{
					LOGGER(logger, false);

					try
					{
						State state = input_state;

						auto total_buy_volume  = 0.0;
						auto total_sell_volume = 0.0;

						for (const auto& input : inputs)
						{
							total_buy_volume  += input.volume_buy_base;
							total_sell_volume += input.volume_sell_base;
						}

						std::fstream fout("tmp.txt", std::ios::out | std::ios::app);

						if (total_buy_volume > total_sell_volume)
						{
							fout << inputs.back().date_time << " : +\n";
						}
						else
						{
							fout << inputs.back().date_time << " : -\n";
						}

						if (std::abs(total_buy_volume - total_sell_volume) /
							(total_buy_volume + total_sell_volume) > 0.01)
						{
							if (total_buy_volume > total_sell_volume)
							{
								state.position = transaction;
							}
							else
							{
								state.position = -transaction;
							}
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