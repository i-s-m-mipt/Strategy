#include "assimilator.hpp"

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
					Strategy::State Assimilator::run(const inputs_container_t & inputs, State current_state) const
					{
						LOGGER(logger, false);

						try
						{
							State required_state = current_state;

							if (std::size(inputs) != m_config.timesteps_prehistory)
							{
								throw strategy_exception("invalid inputs size");
							}

							auto deviation = 0.0;

							for (auto i = 0ULL; i < std::size(inputs); ++i)
							{
								deviation += indicators::AWVB::weighted_volume_bias(inputs[i]);
							}

							auto awvb = inputs.back().indicators.at(indicators::AWVB::name);

							if (std::abs(deviation - awvb) / std::abs(awvb) > m_config.assimilator_min_deviation)
							{
								if (deviation > awvb)
								{
									required_state = State::L;
								}

								if (deviation < awvb)
								{
									required_state = State::S;
								}
							}
							else
							{
								required_state = State::N;
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