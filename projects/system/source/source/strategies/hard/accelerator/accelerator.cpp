#include "accelerator.hpp"

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
					Strategy::State Accelerator::run(frame_t frame, State current_state) const
					{
						LOGGER(logger, false);

						try
						{
							auto required_state = current_state;

							const auto & input_r = *std::prev(frame.second, 1);
							const auto & input_c = *std::prev(frame.second, 2);
							const auto & input_l = *std::prev(frame.second, 3);

							auto previous_deviation_s = 0.0;

							for (auto i = 4; true; ++i)
							{
								const auto & input = *std::prev(frame.second, i);

								auto deviation = (input.price_close - input.price_open) / input.price_open;

								if (((previous_deviation_s <= 0.0) && (deviation < 0.0)) ||
									((previous_deviation_s >= 0.0) && (deviation > 0.0)))
								{
									previous_deviation_s += deviation;
								}

								if (((previous_deviation_s > 0.0) && (deviation < 0.0)) ||
									((previous_deviation_s < 0.0) && (deviation > 0.0)))
								{
									break;
								}
							}

							auto deviation_r = (input_r.price_close - input_r.price_open) / input_r.price_open;
							auto deviation_c = (input_c.price_close - input_c.price_open) / input_c.price_open;
							auto deviation_l = (input_l.price_close - input_l.price_open) / input_l.price_open;
							auto deviation_s = (input_r.price_close - input_l.price_open) / input_l.price_open;

							auto deviation_low_r = (input_r.price_low - input_r.price_open) / input_r.price_open;
							auto deviation_low_c = (input_c.price_low - input_c.price_open) / input_c.price_open;
							auto deviation_low_l = (input_l.price_low - input_l.price_open) / input_l.price_open;

							auto deviation_high_r = (input_r.price_high - input_r.price_open) / input_r.price_open;
							auto deviation_high_c = (input_c.price_high - input_c.price_open) / input_c.price_open;
							auto deviation_high_l = (input_l.price_high - input_l.price_open) / input_l.price_open;

							auto acceleration_k_lc = deviation_c / deviation_l;
							auto acceleration_k_cr = deviation_r / deviation_c;

							auto has_rrr = (deviation_r < 0.0) && (deviation_c < 0.0) && (deviation_l < 0.0);
							auto has_ggg = (deviation_r > 0.0) && (deviation_c > 0.0) && (deviation_l > 0.0);

							auto has_support = (has_rrr && std::find_if(
								std::prev(frame.second, 3 + 24),
								std::prev(frame.second, 3),
								[&input_r](const auto & input)
								{
									return (input.price_close < input_r.price_close);
								}) != std::prev(frame.second, 3));

							auto has_acceleration = (
								(std::abs(deviation_r) > std::abs(deviation_c)) &&
								(std::abs(deviation_c) > std::abs(deviation_l)));

							auto has_correct_acceleration = (acceleration_k_cr >= acceleration_k_lc);

							auto has_valid_deviation_s = (
								(std::abs(deviation_s) > m_config.accelerator_min_deviation_s) &&
								(std::abs(deviation_s) < m_config.accelerator_max_deviation_s));

							auto has_flat_deviation_l = (std::abs(deviation_l) < m_config.accelerator_flat_deviation);

							auto has_long_lower_shadow_r = (deviation_low_r  / deviation_r > 2.0);
							auto has_long_lower_shadow_c = (deviation_low_c  / deviation_c > 2.0);
							auto has_long_lower_shadow_l = (deviation_low_l  / deviation_l > 2.0);

							auto has_long_upper_shadow_r = (deviation_high_r / deviation_r > 2.0);
							auto has_long_upper_shadow_c = (deviation_high_c / deviation_c > 2.0);
							auto has_long_upper_shadow_l = (deviation_high_l / deviation_l > 2.0);

							auto has_rrr_divergence = (has_rrr && (previous_deviation_s >
								std::abs(deviation_l + deviation_c)));

							auto has_ggg_divergence = (has_ggg && (std::abs(previous_deviation_s) >
								deviation_l + deviation_c));

							auto has_rrr_overlap = (has_rrr && (std::abs(duration(
								m_last_rrr_date_time, input_r.date_time_close)) < 4 * 15 * 60));

							auto has_ggg_overlap = (has_ggg && (std::abs(duration(
								m_last_ggg_date_time, input_r.date_time_close)) < 4 * 15 * 60));

							auto has_stop_loss = ((std::abs(input_r.price_close - 
								m_last_position_price) / m_last_position_price) > 
									m_config.accelerator_stop_loss);

							auto has_rrr_timeout = (std::abs(duration(m_last_rrr_date_time, 
								input_r.date_time_close)) >= 32 * 15 * 60);

							auto has_ggg_timeout = (std::abs(duration(m_last_ggg_date_time, 
								input_r.date_time_close)) >= 32 * 15 * 60);

							if (((current_state == State::S) && (has_stop_loss || has_rrr_timeout)) ||
								((current_state == State::L) && (has_stop_loss || has_ggg_timeout)))
							{
								required_state = State::N;
							}

							if (has_rrr && has_acceleration)
							{
								m_last_rrr_date_time = input_r.date_time_close;

								if (/*has_correct_acceleration &&*/
									!has_flat_deviation_l &&
									 has_valid_deviation_s &&
									!has_rrr_overlap &&
									!has_long_lower_shadow_r &&
									!has_rrr_divergence &&
									!has_support)
								{
									m_last_position_price = input_r.price_close;

									required_state = State::S;
								}
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