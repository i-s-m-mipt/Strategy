#include "rsi.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace indicators
			{
				void RSI::run(inputs_container_t & inputs) const
				{
					LOGGER(logger);

					try
					{
						run_implementation(inputs, m_config.rsi_timesteps_long,  Key::L);
						run_implementation(inputs, m_config.rsi_timesteps_short, Key::S);
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void RSI::run_implementation(inputs_container_t & inputs, 
					std::size_t timesteps, const std::string & key) const
				{
					LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						const auto k = 2.0 / (timesteps + 1.0);

						auto ema_U = 0.0;
						auto ema_D = 0.0;

						for (auto i = 1ULL; i < std::size(inputs); ++i)
						{
							if (auto delta = inputs[i].price_close - inputs[i - 1].price_close; delta > 0.0)
							{
								if (i == 1)
								{
									ema_U = delta;
								}
								else
								{
									ema_U = k * delta + (1.0 - k) * ema_U;

									ema_D *= (1.0 - k);
								}
							}
							else
							{
								if (i == 1)
								{
									ema_D = -delta;
								}
								else
								{
									ema_U *= (1.0 - k);

									ema_D = k * -delta + (1.0 - k) * ema_D;
								}
							}

							auto value = 0.0;

							if (ema_U + ema_D < epsilon)
							{
								value = 100.0;
							}
							else
							{
								value = 100.0 * ema_U / (ema_U + ema_D);
							}

							inputs[i].indicators[name][key] = value;
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