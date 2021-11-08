#include "subjugator.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace strategies
			{
				namespace hard
				{
					Strategy::State Subjugator::run(const inputs_container_t & inputs, State current_state) const
					{
						LOGGER(logger, false);

						try
						{
							State required_state = current_state;

							auto price_close = inputs.back().price_close;

							auto price_low = std::min_element(std::begin(inputs), std::end(inputs),
								[](const auto & lhs, const auto & rhs)
									{ return (lhs.price_low < rhs.price_low); })->price_low;

							auto step = price_close * m_config.price_aggregated_trade_step;

							const auto & price_aggregated_trades = inputs.back().price_aggregated_trades;

							if (auto index = std::distance(std::begin(price_aggregated_trades),
								std::max_element(std::begin(price_aggregated_trades),
									std::end(price_aggregated_trades), [](const auto & lhs, const auto & rhs)
									{ return (
										(lhs.volume_buy_base + lhs.volume_sell_base) <
										(rhs.volume_buy_base + rhs.volume_sell_base)); })); index == 
								static_cast < std::size_t > (std::floor((price_close - price_low) / step)))
							{
								if (price_aggregated_trades[index].volume_buy_base <
									price_aggregated_trades[index].volume_sell_base)
								{
									required_state = State::S;
								}
								else
								{
									required_state = State::L;
								}
							}

							return required_state;
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution