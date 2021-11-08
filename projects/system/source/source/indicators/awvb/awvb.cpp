#include "awvb.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace indicators
			{
				void AWVB::run(inputs_container_t & inputs) const
				{
					LOGGER(logger);

					try
					{
						const auto timesteps_wvb = m_config.awvb_timesteps_wvb;
						const auto timesteps_sma = m_config.awvb_timesteps_sma;

						for (auto i = timesteps_wvb + timesteps_sma - 2; i < std::size(inputs); ++i)
						{
							auto sma = 0.0;

							for (auto j = i + 1 - timesteps_sma; j <= i; ++j)
							{
								auto deviation = 0.0;

								for (auto k = j + 1 - timesteps_wvb; k <= j; ++k)
								{
									deviation += weighted_volume_bias(inputs[k]);
								}

								sma += deviation;
							}

							sma /= timesteps_sma;

							inputs[i].indicators[name] = sma;
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				double AWVB::weighted_volume_bias(const Input & input) noexcept
				{
					double weight = std::pow((input.price_high - input.price_low) / input.price_open, 1.0);

					return (input.volume_buy_base - input.volume_sell_base) * weight;
				}

			} // namespace indicators

		} // namespace source

	} // namespace system

} // namespace solution