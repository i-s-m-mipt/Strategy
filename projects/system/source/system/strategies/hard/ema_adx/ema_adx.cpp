#include "ema_adx.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				EMA_ADX::State EMA_ADX::handle(const inputs_container_t & inputs, 
					double transaction, const State & input_state) const
				{
					LOGGER(logger, false);

					try
					{
						State state = input_state;

						const auto & current_input  = inputs.back();

						const auto & previous_input = inputs[std::size(inputs) - 2];

						if (previous_input.indicators.at(indicators::EMA::type).at("12") < 
							previous_input.indicators.at(indicators::EMA::type).at("120") && 
							current_input. indicators.at(indicators::EMA::type).at("12") >
							current_input. indicators.at(indicators::EMA::type).at("120"))
						{
							state.position = transaction;
						}

						if (previous_input.indicators.at(indicators::EMA::type).at("12") >
							previous_input.indicators.at(indicators::EMA::type).at("120") &&
							current_input. indicators.at(indicators::EMA::type).at("12") <
							current_input. indicators.at(indicators::EMA::type).at("120"))
						{
							state.position = -transaction;
						}

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