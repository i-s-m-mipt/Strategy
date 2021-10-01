#include "rsi.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			void RSI::initialize() const
			{
				LOGGER(logger);

				try
				{
					if (m_timesteps == 0ULL)
					{
						throw std::domain_error("required: (timesteps > 0)");
					}
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < indicator_exception > (logger, exception);
				}
			}

			void RSI::operator()(inputs_container_t & inputs) const
			{
				LOGGER(logger);

				try
				{
					const auto epsilon = std::numeric_limits < double > ::epsilon();

					const auto k = 2.0 / (m_timesteps + 1.0);

					auto ema_U = 0.0;
					auto ema_D = 0.0;

					for (auto i = 1U; i < std::size(inputs); ++i)
					{
						if (auto delta = inputs[i].price_close - inputs[i - 1U].price_close; delta > epsilon)
						{
							if (i == 1U)
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
							if (i == 1U)
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

						inputs[i].RSI[m_name] = value;
					}
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < indicator_exception > (logger, exception);
				}
			}

		} // namespace indicators

	} // namespace system

} // namespace solution