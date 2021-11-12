#include "subjugator.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace strategies
			{
				namespace hard
				{
					Strategy::State Subjugator::run(const inputs_container_t & inputs, State current_state) const
					{
						LOGGER(logger, false);

						try
						{
							State required_state = current_state;

							auto ema_previous = inputs[std::size(inputs) - 2].indicators.at(indicators::EMA::name);
							auto ema_current  = inputs[std::size(inputs) - 1].indicators.at(indicators::EMA::name);

							if (ema_previous > ema_current)
							{
								required_state = State::S;
							}
							else
							{
								required_state = State::L;
							}

							return required_state;
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

				} // namespace hard

			} // namespace strategies

		} // namespace source

	} // namespace system

} // namespace solution