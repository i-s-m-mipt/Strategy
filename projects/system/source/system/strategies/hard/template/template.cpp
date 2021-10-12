#include "template.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				Strategy::State TEMPLATE::handle(const inputs_container_t & inputs, 
					const State & input_state, double transaction) const
				{
					LOGGER(logger, false);

					try
					{
						State state = input_state;

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