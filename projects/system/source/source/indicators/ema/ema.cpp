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
						inputs.front().indicators[name] = inputs.front().price_close;

						auto k = 2.0 / (m_config.ema_timesteps + 1.0);

						for (auto i = 1ULL; i < std::size(inputs); ++i)
						{
							inputs[i].indicators[name] = k * inputs[i].price_close +
								(1.0 - k) * inputs[i - 1].indicators[name];
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