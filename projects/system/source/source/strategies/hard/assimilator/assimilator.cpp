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
					Strategy::State Assimilator::run(frame_t frame, State current_state) const
					{
						LOGGER(logger, false);

						try
						{
							/*
							auto bias = 0.0;

							for (auto input = frame.first; input != frame.second; ++input)
							{
								bias += (input->volume_sell_base - input->volume_buy_base);
							}

							bias /= std::distance(frame.first, frame.second);

							auto pattern = frame;

							auto last = std::prev(frame.second);

							for (auto first = std::prev(last); ; --first)
							{
								if ((last ->price_close > last->price_open ) &&
									(first->price_close < last->price_close) &&
									(first->price_open  > last->price_close) ||
									(last ->price_close < last->price_open ) &&
									(first->price_close > last->price_close) &&
									(first->price_open  < last->price_close) || (first == frame.first))
								{
									pattern.first = first;

									break;
								}
							}

							auto size = std::distance(pattern.first, pattern.second);
							
							if ((size < 48) || (size > 480))
							{
								return current_state;
							}

							auto volume_delta = 0.0;

							for (auto input = pattern.first; input != pattern.second; ++input)
							{
								volume_delta += (input->volume_sell_base - input->volume_buy_base);
							}

							volume_delta /= size;

							if (volume_delta * m_previous_volume_delta < 0.0)
							{
								std::fstream fout("patterns.data", std::ios::out | std::ios::app);

								const auto separator = ',';

								fout << last->date_time_open << separator;

								fout << volume_delta << std::endl;
							}

							m_previous_volume_delta = volume_delta;
							*/

							/*
							auto bias = 0.0;

							for (auto input = frame.first; input != frame.second; ++input)
							{
								bias += (input->volume_sell_base - input->volume_buy_base);
							}

							bias /= std::distance(frame.first, frame.second);

							auto volume_buy  = 0.0;
							auto volume_sell = 0.0;

							auto counter = 0ULL;

							double k = 1.0;

							for (auto input = std::prev(frame.second, 96); input != frame.second; ++input)
							{
								if (input->price_close > input->price_open)
								{
									volume_buy += k * input->volume_buy_base;
								}

								if (input->price_close < input->price_open)
								{
									volume_sell += k * input->volume_sell_base;

									++counter;
								}

								k *= 1.05;
							}

							auto indicator = (volume_sell - volume_buy) - bias * counter;

							std::fstream fout("bias.txt", std::ios::out | std::ios::app);

							const auto & input = *std::prev(frame.second);

							auto required_state = State::N;

							auto delta = std::abs(indicator - m_previous_value);

							if (m_previous_value * indicator < 0.0 && delta > 50000.0)
							{
								fout <<
									input.date_time_close << ',' <<
									input.price_close     << ',' << delta << "\n";
							}

							m_previous_value = indicator;

							if (indicator > 5000.0)
							{
								required_state = State::S;

								//fout << 
								//	input.date_time_close << ',' << 
								//	input.price_close     << ',' << indicator << ",S\n";
							}

							if (indicator < -5000.0)
							{
								required_state = State::L;

								//fout <<
								//	input.date_time_close << ',' <<
								//	input.price_close     << ',' << indicator << ",L\n";
							}
							*/

							/*
							auto pwvb_long = 0.0;

							for (auto i = std::size(inputs) - m_config.apwvb_timesteps_pwvb_long; i < std::size(inputs); ++i)
							{
								pwvb_long += indicators::APWVB::price_weighted_volume_bias(inputs[i]);
							}

							auto pwvb_short = 0.0;

							for (auto i = std::size(inputs) - m_config.apwvb_timesteps_pwvb_short; i < std::size(inputs); ++i)
							{
								pwvb_short += indicators::APWVB::price_weighted_volume_bias(inputs[i]);
							}

							const auto permitted_state = m_config.permitted_state;

							if (!permitted_state.empty())
							{
								auto apwvb = inputs.back().indicators.at(
									indicators::APWVB::name).at(permitted_state);

								if (permitted_state == Indicator::Key::L)
								{
									if ((std::abs(pwvb_long - apwvb) / std::abs(apwvb) >
										m_config.apwvb_threshold_long) && (pwvb_long > apwvb))
									{
										required_state = State::L;
									}
									else
									{
										required_state = State::N;
									}
								}

								if (permitted_state == Indicator::Key::S)
								{
									if ((std::abs(pwvb_short - apwvb) / std::abs(apwvb) >
										m_config.apwvb_threshold_short) && (pwvb_short < apwvb))
									{
										required_state = State::S;
									}
									else
									{
										required_state = State::N;
									}
								}
							}
							else
							{
								auto apwvb_long = inputs.back().indicators.at(
									indicators::APWVB::name).at(Indicator::Key::L);
								
								auto required_long = false;

								if ((std::abs(pwvb_long - apwvb_long) / std::abs(apwvb_long) >
									m_config.apwvb_threshold_long) && (pwvb_long > apwvb_long))
								{
									required_long = true;
								}

								auto apwvb_short = inputs.back().indicators.at(
									indicators::APWVB::name).at(Indicator::Key::S);

								auto required_short = false;

								if ((std::abs(pwvb_short - apwvb_short) / std::abs(apwvb_short) >
									m_config.apwvb_threshold_short) && (pwvb_short < apwvb_short))
								{
									required_short = true;
								}

								if (required_long && required_short)
								{
									required_state = State::N;
								}
								else
								{
									required_state = State::N;

									if (required_long)
									{
										required_state = State::L;
									}
									
									if (required_short)
									{
										required_state = State::S;
									}
								}
							}

							if (required_state != current_state)
							{
								if (required_state == State::L)
								{
									auto rsi_long = inputs.back().indicators.at(
										indicators::RSI::name).at(Indicator::Key::L);

									if (rsi_long > m_config.rsi_threshold_long)
									{
										required_state = State::N;
									}
								}
								
								if (required_state == State::S)
								{
									auto rsi_short = inputs.back().indicators.at(
										indicators::RSI::name).at(Indicator::Key::S);

									if (rsi_short < m_config.rsi_threshold_short)
									{
										required_state = State::N;
									}
								}
							}
							*/

							return State::N;
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