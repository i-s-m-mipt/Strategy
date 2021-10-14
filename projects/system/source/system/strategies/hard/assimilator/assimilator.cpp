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

						if (m_config.awvb_timesteps_wvb != std::size(inputs))
						{
							throw std::domain_error("invalid prehistory size");
						}

						auto deviation = 0.0;

						for (auto i = 0ULL; i < std::size(inputs); ++i)
						{
							deviation += (inputs[i].volume_buy_base - inputs[i].volume_sell_base) *
								(inputs[i].price_high - inputs[i].price_low) / inputs[i].price_open;
						}

						auto awvb = inputs.back().indicators.at(indicators::AWVB::type).at("default");

						if (std::abs(deviation - awvb) / std::abs(awvb) >
							m_config.assimilator_min_deviation)
						{
							if (deviation > awvb)
							{
								state.position = +transaction;
							}

							if (deviation < awvb)
							{
								state.position = -transaction;
							}
						}
						else
						{
							state.position = 0.0;
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