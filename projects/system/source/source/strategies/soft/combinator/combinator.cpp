#include "combinator.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace strategies
			{
				namespace soft
				{
					void Combinator::initialize()
					{
						LOGGER(logger);

						try
						{
							load_extractors();
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					void Combinator::uninitialize()
					{
						LOGGER(logger);

						try
						{
							m_thread_pool.join();
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					Combinator::levels_container_t Combinator::make_levels(frame_t frame) const
					{
						LOGGER(logger, false);

						try
						{
							const auto bias = 6ULL;

							levels_container_t levels;

							for (auto first = frame.first; first != std::prev(frame.second, 2 * bias); ++first)
							{
								auto [min_candle, max_candle] = std::minmax_element(
									first, std::next(first, 2U * bias + 1U),
										[](const auto & lhs, const auto & rhs)
										{
											return (lhs.price_close < rhs.price_close);
										});

								if (std::distance(first, min_candle) == bias)
								{
									levels.push_back({ min_candle->date_time_close, min_candle->price_close });
								}

								if (std::distance(first, max_candle) == bias)
								{
									levels.push_back({ max_candle->date_time_close, max_candle->price_close });
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

					void Combinator::load_extractors()
					{
						LOGGER(logger);

						try
						{
							// 5 red klines, 1 big green kline
							m_extractors.push_back([](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);
									const auto & input_3 = *std::prev(frame.second, 3);
									const auto & input_4 = *std::prev(frame.second, 4);
									const auto & input_5 = *std::prev(frame.second, 5);
									const auto & input_6 = *std::prev(frame.second, 6);

									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_2_6 = (input_2.price_close - input_6.price_open) / input_6.price_open;

									if ((deviation_1   > 0.0) &&
										(deviation_2   < 0.0) &&
										(deviation_2_6 < 0.0) &&
										(std::abs(deviation_1  ) > std::abs(deviation_2)) &&
										(std::abs(deviation_2_6) > std::abs(deviation_1)))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = deviation_2_6;
										pattern.at(3) = std::abs(deviation_1  ) / std::abs(deviation_2);
										pattern.at(4) = std::abs(deviation_2_6) / std::abs(deviation_1);
									}

									return pattern;
								});

							// 5 green klines, 1 big red kline
							m_extractors.push_back([](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);
									const auto & input_3 = *std::prev(frame.second, 3);
									const auto & input_4 = *std::prev(frame.second, 4);
									const auto & input_5 = *std::prev(frame.second, 5);
									const auto & input_6 = *std::prev(frame.second, 6);

									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_2_6 = (input_2.price_close - input_6.price_open) / input_6.price_open;

									if ((deviation_1   < 0.0) &&
										(deviation_2   > 0.0) &&
										(deviation_2_6 > 0.0) &&
										(std::abs(deviation_1  ) > std::abs(deviation_2)) &&
										(std::abs(deviation_2_6) > std::abs(deviation_1)))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = deviation_2_6;
										pattern.at(3) = std::abs(deviation_1  ) / std::abs(deviation_2);
										pattern.at(4) = std::abs(deviation_2_6) / std::abs(deviation_1);
									}

									return pattern;
								});

							// bullish engulfing
							m_extractors.push_back([](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);

									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_1_low = (input_1.price_low - input_1.price_open) / input_1.price_open;
									auto deviation_2_low = (input_2.price_low - input_2.price_open) / input_2.price_open;

									if ((deviation_1 > 0.0) && (deviation_2 < 0.0) &&
										(std::abs(deviation_1) > std::abs(deviation_2)))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = std::abs(deviation_1) / std::abs(deviation_2);
										pattern.at(3) = deviation_1_low;
										pattern.at(4) = deviation_2_low;
									}

									return pattern;
								});

							// bearish engulfing
							m_extractors.push_back([](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);

									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_1_high = (input_1.price_high - input_1.price_open) / input_1.price_open;
									auto deviation_2_high = (input_2.price_high - input_2.price_open) / input_2.price_open;

									if ((deviation_1 < 0.0) && (deviation_2 > 0.0) &&
										(std::abs(deviation_1) > std::abs(deviation_2)))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = std::abs(deviation_1) / std::abs(deviation_2);
										pattern.at(3) = deviation_1_high;
										pattern.at(4) = deviation_2_high;
									}

									return pattern;
								});

							// long bottom shadow
							m_extractors.push_back([](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);
									const auto & input_5 = *std::prev(frame.second, 5);

									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_2_5 = (input_2.price_close - input_5.price_open) / input_5.price_open;

									auto deviation_1_low = (input_1.price_low - input_1.price_open) / input_1.price_open;
									auto deviation_2_low = (input_2.price_low - input_2.price_open) / input_2.price_open;

									if ((deviation_2 < 0.0) && 
										(std::abs(deviation_1    ) < 0.001) &&
										(std::abs(deviation_1_low) > 0.010))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = deviation_1_low;
										pattern.at(3) = deviation_2_low;
										pattern.at(4) = deviation_2_5;
									}

									return pattern;
								});

							// long top shadow
							m_extractors.push_back([](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);
									const auto & input_5 = *std::prev(frame.second, 5);

									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_2_5 = (input_2.price_close - input_5.price_open) / input_5.price_open;

									auto deviation_1_high = (input_1.price_high - input_1.price_open) / input_1.price_open;
									auto deviation_2_high = (input_2.price_high - input_2.price_open) / input_2.price_open;

									if ((deviation_2 < 0.0) &&
										(std::abs(deviation_1     ) < 0.001) &&
										(std::abs(deviation_1_high) > 0.010))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = deviation_1_high;
										pattern.at(3) = deviation_2_high;
										pattern.at(4) = deviation_2_5;
									}

									return pattern;
								});

							// rebound from resistance level
							m_extractors.push_back([this](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);
									const auto & input_5 = *std::prev(frame.second, 5);
									
									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_2_5 = (input_2.price_close - input_5.price_open) / input_5.price_open;

									auto deviation_1_high = (input_1.price_high - input_1.price_open) / input_1.price_open;

									auto levels = make_levels(frame);

									auto rebounded_levels = 0ULL;

									for (const auto & level : levels)
									{
										if ((input_1.price_close < level.price) &&
											(input_1.price_high  > level.price))
										{
											++rebounded_levels;
										}
									}

									if ((deviation_1   < 0.0) && 
										(deviation_2   > 0.0) && 
										(deviation_2_5 > 0.0) && 
										(rebounded_levels > 0))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = deviation_1_high;
										pattern.at(3) = deviation_2_5;
										pattern.at(4) = static_cast < double > (rebounded_levels);
									}

									return pattern;
								});

							// rebound from support level
							m_extractors.push_back([this](frame_t frame)
								{
									pattern_t pattern(5, 0.0);

									const auto & input_1 = *std::prev(frame.second, 1);
									const auto & input_2 = *std::prev(frame.second, 2);
									const auto & input_5 = *std::prev(frame.second, 5);

									auto deviation_1 = (input_1.price_close - input_1.price_open) / input_1.price_open;
									auto deviation_2 = (input_2.price_close - input_2.price_open) / input_2.price_open;

									auto deviation_2_5 = (input_2.price_close - input_5.price_open) / input_5.price_open;

									auto deviation_1_high = (input_1.price_high - input_1.price_open) / input_1.price_open;

									auto levels = make_levels(frame);

									auto rebounded_levels = 0ULL;

									for (const auto & level : levels)
									{
										if ((input_1.price_close > level.price) &&
											(input_1.price_low   < level.price))
										{
											++rebounded_levels;
										}
									}

									if ((deviation_1   > 0.0) &&
										(deviation_2   < 0.0) &&
										(deviation_2_5 < 0.0) &&
										(rebounded_levels > 0))
									{
										pattern.at(0) = deviation_1;
										pattern.at(1) = deviation_2;
										pattern.at(2) = deviation_1_high;
										pattern.at(3) = deviation_2_5;
										pattern.at(4) = static_cast < double > (rebounded_levels);
									}

									return pattern;
								});
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					Strategy::State Combinator::run(frame_t frame, State current_state) const
					{
						LOGGER(logger, false);

						try
						{
							/*
							if (!m_database)
							{
								throw std::runtime_error("empty database");
							}

							std::vector < std::future < void > > futures;

							std::mutex mutex;

							std::vector < double > deviations;

							deviations.reserve(std::size(m_extractors));

							for (auto extractor : m_extractors)
							{
								std::packaged_task < void() > task([this, &inputs, extractor, &mutex, &deviations]()
								{
									const auto neighbors_limit = m_config.combinator_neighbors;

									std::vector < std::pair < double, double > > neighbors;

									auto alpha = extractor(make_frame(inputs));

									for (const auto & timeseries : *m_database)
									{
										for (auto i = m_config.skipped_timesteps; i < std::size(timeseries) - 1; ++i)
										{
											if (timeseries[i].date_time_close >= inputs.front().date_time_close)
											{
												break;
											}

											auto omega = extractor(make_frame(timeseries, i));

											auto error = compare_patterns(alpha, omega);

											auto deviation = (timeseries[i + 1].price_close -
												timeseries[i].price_close) / timeseries[i].price_close;

											if (std::size(neighbors) < neighbors_limit)
											{
												neighbors.emplace_back(error, deviation);
											}
											else
											{
												neighbors.back().first  = error;
												neighbors.back().second = deviation;
											}

											std::sort(std::begin(neighbors), std::end(neighbors),
												[](const auto & lhs, const auto & rhs)
													{ return (lhs.first < rhs.first); });
										}
									}

									auto deviation = 0.0;

									for (auto neighbor : neighbors)
									{
										deviation += ((1.0 - neighbor.first) * neighbor.second);
									}

									deviation /= neighbors_limit;

									std::scoped_lock lock(mutex);

									deviations.push_back(deviation);
								});

								futures.push_back(boost::asio::post(m_thread_pool, std::move(task)));
							}

							std::for_each(std::begin(futures), std::end(futures), [](auto & future) { future.wait(); });

							auto average_deviation = std::accumulate(std::begin(deviations),
								std::end(deviations), 0.0) / std::size(deviations);

							std::fstream fout("regression.data", std::ios::out | std::ios::app);

							fout << std::setprecision(6) << std::fixed << std::showpos << 
								average_deviation << std::endl;

							if (average_deviation > m_config.combinator_threshold_long)
							{
								return State::L;
							}

							if (average_deviation < m_config.combinator_threshold_short)
							{
								return State::S;
							}
							*/

							return current_state;
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					Combinator::frame_t Combinator::make_frame(const inputs_container_t & inputs) const
					{
						LOGGER(logger, false);

						try
						{
							return std::make_pair(std::begin(inputs), std::end(inputs));
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					Combinator::frame_t Combinator::make_frame(const inputs_container_t & inputs, std::size_t index) const
					{
						LOGGER(logger, false);

						try
						{
							return std::make_pair(
								std::next(std::begin(inputs), index + 1 - m_config.timesteps_prehistory),
								std::next(std::begin(inputs), index + 1));
						}
						catch (const std::exception & exception)
						{
							shared::catch_handler < strategy_exception > (logger, exception);
						}
					}

					double Combinator::compare_patterns(const pattern_t & alpha, const pattern_t & omega) const
					{
						LOGGER(logger, false);

						try
						{
							const auto epsilon = std::numeric_limits < double > ::epsilon();

							auto error = 0.0;

							for (auto i = 0ULL; i < std::size(alpha); ++i)
							{
								if ((std::abs(alpha[i]) < epsilon) || 
									(std::abs(omega[i]) < epsilon))
								{
									return 1.0;
								}
								else
								{
									error += std::min(std::abs(alpha[i] - omega[i]) /
										std::max(std::abs(alpha[i]), epsilon), 1.0);
								}
							}

							return (error / std::size(alpha));
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