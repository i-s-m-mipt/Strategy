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
									deviation += (inputs[k].volume_buy_base - inputs[k].volume_sell_base) *
										(inputs[k].price_high - inputs[k].price_low) / inputs[k].price_open;
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

			} // namespace indicators

		} // namespace source

	} // namespace system

} // namespace solution