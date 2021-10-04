#include "mfi.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			void MFI::initialize() const
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

			void MFI::operator()(inputs_container_t & inputs) const
			{
				LOGGER(logger);

				try
				{
					const auto epsilon = std::numeric_limits < double > ::epsilon();

					std::vector < double > typical_prices(std::size(inputs), 0.0);

					std::transform(std::begin(inputs), std::end(inputs), 
						std::begin(typical_prices), [](const auto & input)
						{
							return ((input.price_high + input.price_low + input.price_close) / 3.0);
						});

					for (auto i = m_timesteps + 1U; i <= std::size(inputs); ++i)
					{
						auto positive_money_flow = 0.0;
						auto negative_money_flow = 0.0;

						for (auto j = i - m_timesteps; j < i; ++j)
						{
							auto money_flow = typical_prices[j] * inputs[j].volume_base;

							if (typical_prices[j] - typical_prices[j - 1U] > epsilon)
							{
								positive_money_flow += money_flow;
							}
							else
							{
								negative_money_flow += money_flow;
							}
						}

						inputs[i - 1U].indicators[type][m_name] = 100.0 - 100.0 / (1.0 + 
							(positive_money_flow / std::max(negative_money_flow, epsilon)));
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