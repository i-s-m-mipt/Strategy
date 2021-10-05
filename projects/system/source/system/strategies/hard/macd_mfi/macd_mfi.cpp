#include "macd_mfi.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			namespace hard
			{
				MACD_MFI::State MACD_MFI::handle(const inputs_container_t & inputs, double transaction) const
				{
					LOGGER(logger);

					try
					{
						State state;

						state.position = 0.0;

						const auto & input = inputs.back();

						if (input.indicators.at(indicators::MFI ::type).at("default") < 20.0 &&
							input.indicators.at(indicators::MACD::type).at("slow")    >  0.0)
						{
							state.position = transaction;
						}

						if (input.indicators.at(indicators::MFI ::type).at("default") > 80.0 &&
							input.indicators.at(indicators::MACD::type).at("slow")    <  0.0)
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