#include "apwvb.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace indicators
			{
				void APWVB::run(inputs_container_t & inputs) const
				{
					LOGGER(logger);

					try
					{
						run_implementation(inputs,
							m_config.apwvb_timesteps_pwvb_long,
							m_config.apwvb_timesteps_sma_long,  Key::L);

						run_implementation(inputs,
							m_config.apwvb_timesteps_pwvb_short,
							m_config.apwvb_timesteps_sma_short, Key::S);
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void APWVB::run_implementation(inputs_container_t & inputs,
					std::size_t timesteps_pwvb, std::size_t timesteps_sma, const std::string & key) const
				{
					LOGGER(logger);

					try
					{
						for (auto i = timesteps_pwvb + timesteps_sma - 2; i < std::size(inputs); ++i)
						{
							auto sma = 0.0;

							for (auto j = i + 1 - timesteps_sma; j <= i; ++j)
							{
								auto deviation = 0.0;

								for (auto k = j + 1 - timesteps_pwvb; k <= j; ++k)
								{
									deviation += price_weighted_volume_bias(inputs[k]);
								}

								sma += deviation;
							}

							sma /= timesteps_sma;

							inputs[i].indicators[name][key] = sma;
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				double APWVB::price_weighted_volume_bias(const Input & input) noexcept
				{
					double weight = (input.price_high - input.price_low) / input.price_open;

					return weight * (input.volume_buy_base - input.volume_sell_base);
				}

			} // namespace indicators

		} // namespace source

	} // namespace system

} // namespace solution