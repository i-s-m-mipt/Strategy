#include "ema.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace indicators
			{
				void EMA::run(inputs_container_t & inputs) const
				{
					LOGGER(logger);

					try
					{
						run_implementation(inputs, m_config.ema_timesteps_fast, Key::fast);
						run_implementation(inputs, m_config.ema_timesteps_slow, Key::slow);
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void EMA::run_implementation(inputs_container_t & inputs,
					std::size_t timesteps, const std::string & key) const
				{
					LOGGER(logger);

					try
					{
						inputs.front().indicators[name][key] = inputs.front().price_close;

						auto k = 2.0 / (timesteps + 1.0);

						for (auto i = 1ULL; i < std::size(inputs); ++i)
						{
							inputs[i].indicators[name][key] = k * inputs[i].price_close +
								(1.0 - k) * inputs[i - 1].indicators[name][key];
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