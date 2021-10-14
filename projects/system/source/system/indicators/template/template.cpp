#include "template.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			void TEMPLATE::initialize() const
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

			void TEMPLATE::operator()(inputs_container_t & inputs) const
			{
				LOGGER(logger);

				try
				{
					// ...
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < indicator_exception > (logger, exception);
				}
			}

		} // namespace indicators

	} // namespace system

} // namespace solution