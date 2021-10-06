#include "macd_mfi.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				MACD_MFI::State MACD_MFI::handle(const inputs_container_t & inputs, 
					double transaction, const State & input_state) const
				{
					LOGGER(logger, false);

					try
					{
						State state = input_state;

						const auto & current_input  = inputs.back();

						const auto & previous_input = inputs[std::size(inputs) - 2];

						if (current_input. indicators.at(indicators::MACD::type).at("slow") *
							previous_input.indicators.at(indicators::MACD::type).at("slow") < 0.0)
						{
							state.position = 0.0;
						}

						if (current_input.indicators.at(indicators::MFI ::type).at("default") < 20.0 &&
							current_input.indicators.at(indicators::MACD::type).at("slow")    >  0.0)
						{
							state.position = transaction;
						}

						if (current_input.indicators.at(indicators::MFI ::type).at("default") > 80.0 &&
							current_input.indicators.at(indicators::MACD::type).at("slow")    <  0.0)
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