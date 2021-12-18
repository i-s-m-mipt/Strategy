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
					Strategy::State Subjugator::run(frame_t frame, State current_state) const
					{
						LOGGER(logger, false);

						try
						{
							auto required_state = current_state;

							auto levels = make_levels(frame);

							if (levels.empty())
							{
								return required_state;
							}

							std::sort(std::begin(levels), std::end(levels),
								[](const auto & lhs, const auto & rhs)
									{ return (lhs.input->price_close < rhs.input->price_close); });

							const auto & last_input = *std::prev(frame.second);

							auto last_deviation = std::abs(last_input.price_close -
								last_input.price_open) / last_input.price_open;

							auto counter = 0ULL;

							for (auto input = frame.first; input != frame.second; ++input)
							{
								if (std::abs(input->price_close - input->price_open) / 
										input->price_open < last_deviation)
								{
									++counter;
								}
							}

							auto has_pulse = ((1.0 * counter / 
								m_config.timesteps_prehistory) > m_config.pulse_threshold);

							if ((last_input.price_close  > levels.back().price_high) &&
								(last_input.price_open  <= levels.back().price_high)/* &&
									!is_violated(levels.back(), frame)*/ && !has_pulse)
							{
								required_state = State::L;
							}

							if ((last_input.price_close  < levels.front().price_low) &&
								(last_input.price_open  >= levels.front().price_low)/* &&
									!is_violated(levels.front(), frame)*/ && !has_pulse)
							{
								required_state = State::S;
							}

							return required_state;
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					Subjugator::levels_container_t Subjugator::make_levels(frame_t frame) const
					{
						LOGGER(logger, false);

						try
						{
							levels_container_t levels;

							for (auto bias = m_config.level_min_bias; bias <= m_config.level_max_bias; ++bias)
							{
								for (auto first = frame.first; first != std::prev(frame.second, 2 * bias); ++first)
								{
									auto [min_candle, max_candle] = std::minmax_element(
										first, std::next(first, 2 * bias + 1), 
											[](const auto & lhs, const auto & rhs)
											{
												return (lhs.price_close < rhs.price_close);
											});

									if (std::distance(first, min_candle) == bias)
									{
										make_level(levels, min_candle, bias);
									}

									if (std::distance(first, max_candle) == bias)
									{
										make_level(levels, max_candle, bias);
									}
								}
							}

							levels.shrink_to_fit();

							return levels;
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					void Subjugator::make_level(levels_container_t & levels, 
						iterator_t extremum, std::size_t locality) const
					{
						LOGGER(logger, false);

						try
						{
							if (auto iterator = std::find_if(std::begin(levels), std::end(levels), 
									[extremum](const auto & level) 
									{ 
										return (level.input->date_time_close == extremum->date_time_close); 
									}); 
								iterator == std::end(levels))
							{
								levels.push_back(Level { extremum, locality,
									extremum->price_close * (1.0 - m_config.level_deviation),
									extremum->price_close * (1.0 + m_config.level_deviation) });
							}
							else
							{
								iterator->locality = locality;
							}
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					bool Subjugator::is_violated(const Level & level, frame_t frame) const
					{
						LOGGER(logger, false);

						try
						{
							for (auto input  = std::next(level.input,  4 + 1);
								      input != std::prev(frame.second, 4); ++input)
							{
								if (((input->price_close  > level.price_high)  &&
									 (input->price_open  <= level.price_high)) ||
									((input->price_close  < level.price_low )  &&
									 (input->price_open  >= level.price_low )))
								{
									return true;
								}
							}

							return false;
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