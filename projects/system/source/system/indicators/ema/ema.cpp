#include "ema.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			void EMA::initialize() const
			{
				LOGGER(logger);

				try
				{
					if (m_timesteps == 0)
					{
						throw std::domain_error("required: (timesteps > 0)");
					}
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < indicator_exception > (logger, exception);
				}
			}

			void EMA::operator()(inputs_container_t & inputs) const
			{
				LOGGER(logger);

				try
				{
					inputs.front().indicators[type][m_name] = inputs.front().price_close;

					auto k = 2.0 / (m_timesteps + 1.0);

					for (auto i = 1U; i < std::size(inputs); ++i)
					{
						inputs[i].indicators[type][m_name] = k * inputs[i].price_close +
							(1.0 - k) * inputs[i - 1U].indicators.at(type).at(m_name);
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