#include "awvb.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			void AWVB::initialize() const
			{
				LOGGER(logger);

				try
				{
					if (m_timesteps_wvb == 0)
					{
						throw std::domain_error("required: (timesteps_wvb > 0)");
					}

					if (m_timesteps_sma == 0)
					{
						throw std::domain_error("required: (timesteps_sma > 0)");
					}
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < indicator_exception > (logger, exception);
				}
			}

			void AWVB::operator()(inputs_container_t & inputs) const
			{
				LOGGER(logger);

				try
				{
					for (auto i = m_timesteps_wvb + m_timesteps_sma - 2; i < std::size(inputs); ++i)
					{
						auto sma = 0.0;

						for (auto j = i + 1 - m_timesteps_sma; j <= i; ++j)
						{
							auto deviation = 0.0;

							for (auto k = j + 1 - m_timesteps_wvb; k <= j; ++k)
							{
								deviation += (inputs[k].volume_buy_base - inputs[k].volume_sell_base) *
									(inputs[k].price_high - inputs[k].price_low) / inputs[k].price_open;
							}

							sma += deviation;
						}

						sma /= m_timesteps_sma;
						
						inputs[i].indicators[type][m_name] = sma;
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