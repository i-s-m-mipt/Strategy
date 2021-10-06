#include "template.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				TEMPLATE::State TEMPLATE::handle(const inputs_container_t & inputs, double transaction) const
				{
					LOGGER(logger);

					try
					{
						State state;

						// TODO

						return state;
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < strategy_exception > (logger, exception);
					}
				}

			} // namespace hard

		} // namespace strategies

	} // namespace system

} // namespace solution