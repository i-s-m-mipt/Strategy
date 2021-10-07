#include "adx.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			void ADX::initialize() const
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

			void ADX::operator()(inputs_container_t & inputs) const
			{
				LOGGER(logger);

				try
				{
					const auto epsilon = std::numeric_limits < double > ::epsilon();

					const auto k = 2.0 / (m_timesteps + 1.0);

					auto pdi = 0.0, ndi = 0.0;

					std::vector < double > dx(std::size(inputs) - 1U, 0.0);

					for (auto i = 1U; i < std::size(inputs); ++i)
					{
						auto tr = std::max(epsilon,
							std::max(inputs[i].price_high, inputs[i - 1U].price_close) -
							std::min(inputs[i].price_low,  inputs[i - 1U].price_close));

						auto pm = inputs[i     ].price_high - inputs[i - 1U].price_high;
						auto nm = inputs[i - 1U].price_low  - inputs[i     ].price_low;

						auto pdm = 0.0;
						auto ndm = 0.0;

						if (pm - nm > epsilon && pm > epsilon)
						{
							pdm = pm;
						}

						if (nm - pm > epsilon && nm > epsilon)
						{
							ndm = nm;
						}

						auto pdm_div_tr = pdm / tr;
						auto ndm_div_tr = ndm / tr;

						if (i == 1U)
						{
							pdi = pdm_div_tr;
							ndi = ndm_div_tr;
						}
						else
						{
							pdi = k * pdm_div_tr + (1.0 - k) * pdi;
							ndi = k * ndm_div_tr + (1.0 - k) * ndi;
						}

						dx[i - 1U] = 100.0 * std::abs(pdi - ndi) / std::max((pdi + ndi), epsilon);
					}

					std::vector < double > adx(std::size(dx), 0.0);

					adx.front() = dx.front();

					for (auto i = 1U; i < std::size(adx); ++i)
					{
						adx[i] = k * dx[i] + (1.0 - k) * adx[i - 1U];
					}

					for (auto i = 1U + m_timesteps; i < std::size(inputs); ++i)
					{
						inputs[i].indicators[type][m_name] = adx[i - 1U];
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