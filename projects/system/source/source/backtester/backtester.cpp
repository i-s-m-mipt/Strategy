#include "backtester.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			void Backtester::initialize()
			{
				LOGGER(logger);

				try
				{
					// ...
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			void Backtester::uninitialize()
			{
				LOGGER(logger);

				try
				{
					// ...
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			Backtester::Result Backtester::run() const
			{
				LOGGER(logger);

				try
				{
					if (m_strategy->name() == Strategy::m_name)
					{
						return handle_investor();
					}
					else
					{
						return handle_manager();
					}					
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			Backtester::Result Backtester::handle_investor() const
			{
				LOGGER(logger);

				try
				{
					const auto begin = make_begin();

					Result result;

					result.rewards.reserve(std::size(m_inputs));

					auto position = m_config.transaction;

					result.trades.push_back(Result::Trade());

					result.trades.back().begin = m_inputs[begin].date_time_close;
					result.trades.back().state = State::L;

					auto total_reward = 0.0;
					auto trade_reward = 0.0;

					for (auto i = begin; i < std::size(m_inputs) - 1; ++i)
					{
						auto price_deviation = (m_inputs[i + 1].price_close -
							m_inputs[i].price_close) / m_inputs[i].price_close;

						auto delta = price_deviation * position;

						position += delta;

						total_reward += delta;
						trade_reward += delta;

						result.rewards.push_back(std::make_pair(m_inputs[i + 1].date_time_close, total_reward));
					}

					result.trades.back().end = m_inputs.back().date_time_close;
					result.trades.back().reward = trade_reward;

					return result;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			Backtester::Result Backtester::handle_manager() const
			{
				LOGGER(logger);

				try
				{
					const auto begin = make_begin();

					const auto commission = m_config.commission;

					const auto stop_loss_relaxation = m_config.stop_loss_relaxation;

					Result result;

					result.rewards.reserve(std::size(m_inputs));

					auto transaction = m_config.transaction;

					auto total_reward = 0.0;
					auto trade_reward = 0.0;

					auto current_state = State::N;

					auto current_stop_loss_relaxation = 0ULL;

					auto position = 0.0;

					auto counter = 0ULL;

					for (auto i = begin; i < std::size(m_inputs) - 1; ++i)
					{
						update_transaction(transaction, total_reward);

						auto required_state = m_strategy->run(
							make_prehistory(m_inputs, i), current_state);

						if (((required_state == State::N) && (current_state != State::N)) ||
							((required_state == State::L) && (current_state == State::S)) ||
							((required_state == State::S) && (current_state == State::L)) ||
							((trade_reward < -1.0 * m_config.stop_loss * transaction) && 
								(current_state != State::N)))
						{
							if (trade_reward < -1.0 * m_config.stop_loss * transaction)
							{
								current_stop_loss_relaxation = stop_loss_relaxation;

								++counter;
							}

							auto delta = -commission * std::abs(position);

							position = 0.0;

							current_state = State::N;

							result.trades.back().end = m_inputs[i].date_time_close;
							result.trades.back().reward = trade_reward + delta;

							trade_reward = 0.0;

							total_reward += delta;
						}
						
						if (((required_state == State::L) && (current_state == State::N) && 
								(current_stop_loss_relaxation == 0)) ||
							((required_state == State::S) && (current_state == State::N) && 
								(current_stop_loss_relaxation == 0)))
						{
							position = (required_state == State::L ? +transaction : -transaction);

							current_state = required_state;

							auto delta = -commission * std::abs(position);

							result.trades.push_back(Result::Trade());

							result.trades.back().begin = m_inputs[i].date_time_close;
							result.trades.back().state = current_state;

							trade_reward += delta;
							total_reward += delta;
						}

						if (current_state != State::N)
						{
							auto price_deviation = (m_inputs[i + 1].price_close - 
								m_inputs[i].price_close) / m_inputs[i].price_close;

							auto delta = price_deviation * position;

							position += delta;

							total_reward += delta;
							trade_reward += delta;
						}

						result.rewards.push_back(std::make_pair(m_inputs[i + 1].date_time_close, total_reward));

						if (current_stop_loss_relaxation > 0)
						{
							--current_stop_loss_relaxation;
						}
					}

					if (current_state != State::N)
					{
						result.trades.back().end = m_inputs.back().date_time_close;
						result.trades.back().reward = trade_reward;
					}

					verify(result);

					LOGGER_WRITE(logger, std::to_string(counter));

					return result;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			std::size_t Backtester::make_begin() const
			{
				LOGGER(logger, false);

				try
				{
					auto begin = m_config.skipped_timesteps;

					while (m_inputs[begin].date_time_close.day != 1)
					{
						++begin;
					}

					return begin;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			void Backtester::update_transaction(double & transaction, double total_reward) const noexcept
			{
				if (m_config.has_reinvestment)
				{
					transaction = m_config.transaction + total_reward;
				}
			}

			Backtester::inputs_container_t Backtester::make_prehistory(
				const inputs_container_t & inputs, std::size_t index) const
			{
				LOGGER(logger, false);

				try
				{
					return inputs_container_t(
						std::next(std::begin(m_inputs), index + 1 - m_config.timesteps_prehistory),
						std::next(std::begin(m_inputs), index + 1));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			void Backtester::verify(const Result & result) const
			{
				LOGGER(logger, false);

				try
				{
					auto total_reward_by_trades = 0.0;

					for (const auto & trade : result.trades)
					{
						total_reward_by_trades += trade.reward;
					}

					LOGGER_WRITE(logger, std::to_string(total_reward_by_trades));
					LOGGER_WRITE(logger, std::to_string(result.rewards.back().second));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

		} // namespace source

	} // namespace system

} // namespace solution