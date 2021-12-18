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
					const auto begin = make_backtest_begin();

					Result result;

					result.rewards.reserve(std::size(m_inputs_backtest));

					auto position = m_config.investment;

					result.trades.push_back(Result::Trade());

					result.trades.back().begin = m_inputs_backtest[begin].date_time_close;
					result.trades.back().state = State::L;

					auto total_reward = 0.0;
					auto trade_reward = 0.0;

					for (auto i = begin; i < std::size(m_inputs_backtest) - 1; ++i)
					{
						auto price_deviation = (m_inputs_backtest[i + 1].price_close -
							m_inputs_backtest[i].price_close) / m_inputs_backtest[i].price_close;

						auto delta = price_deviation * position;

						position += delta;

						total_reward += delta;
						trade_reward += delta;

						result.rewards.push_back(std::make_pair(m_inputs_backtest[i + 1].date_time_close, total_reward));
					}

					result.trades.back().end = m_inputs_backtest.back().date_time_close;
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
					const auto commission = m_config.commission;

					auto investment = m_config.investment;

					auto total_reward = 0.0;
					auto trade_reward = 0.0;

					auto index = make_strategy_begin();

					Result result;

					result.rewards.reserve(std::size(m_inputs_backtest));

					auto current_state = State::N;

					auto position = 0.0;

					auto price_trade     = 0.0;
					auto price_delta_tsl = 0.0;
					auto price_tsl       = 0.0;
					auto price_tp        = 0.0;
					auto price_delta_ttp = 0.0;

					for (auto i = make_backtest_begin(); i < std::size(m_inputs_backtest) - 1; ++i)
					{
						if (current_state != State::N)
						{
							if (((current_state == State::L) && (m_inputs_backtest[i].price_close > price_tp)) ||
								((current_state == State::S) && (m_inputs_backtest[i].price_close < price_tp)))
							{
								price_delta_tsl = price_delta_ttp;
							}

							if (current_state == State::L)
							{
								price_tsl = std::max(price_tsl, m_inputs_backtest[i].price_close - price_delta_tsl);
							}
							else
							{
								price_tsl = std::min(price_tsl, m_inputs_backtest[i].price_close + price_delta_tsl);
							}

							if (((current_state == State::L) && (m_inputs_backtest[i].price_close < price_tsl)) ||
								((current_state == State::S) && (m_inputs_backtest[i].price_close > price_tsl)))
							{
								auto delta = -commission * std::abs(position);

								position = 0.0;

								current_state = State::N;

								result.trades.back().end = m_inputs_backtest[i].date_time_close;
								result.trades.back().reward = trade_reward + delta;

								trade_reward = 0.0;

								total_reward += delta;

								price_trade     = 0.0;
								price_delta_tsl = 0.0;
								price_tsl       = 0.0;
								price_tp        = 0.0;
								price_delta_ttp = 0.0;
							}
						}

						if (m_inputs_backtest[i].date_time_close >= m_inputs_strategy[index].date_time_close)
						{
							update_transaction(investment, total_reward);

							auto required_state = m_strategy->run(make_prehistory(index), current_state);

							if (((required_state == State::N) && (current_state != State::N)) ||
								((required_state == State::L) && (current_state == State::S)) ||
								((required_state == State::S) && (current_state == State::L)))
							{
								auto delta = -commission * std::abs(position);

								position = 0.0;

								current_state = State::N;

								result.trades.back().end = m_inputs_strategy[index].date_time_close;
								result.trades.back().reward = trade_reward + delta;

								trade_reward = 0.0;

								total_reward += delta;
							}

							if (((required_state == State::L) && (current_state == State::N)) ||
								((required_state == State::S) && (current_state == State::N)))
							{
								position = (required_state == State::L ? +investment : -investment);

								current_state = required_state;

								auto delta = -commission * std::abs(position);

								result.trades.push_back(Result::Trade());

								result.trades.back().begin = m_inputs_strategy[index].date_time_close;
								result.trades.back().state = current_state;

								trade_reward += delta;
								total_reward += delta;

								price_trade = m_inputs_strategy[index].price_close;

								if (current_state == State::L)
								{
									price_delta_tsl = price_trade * m_config.stop_loss_long;
									price_delta_ttp = price_trade * m_config.profit_rollback_long;

									price_tsl = price_trade - price_delta_tsl;
									price_tp = price_trade * (1.0 + m_config.take_profit_long);
								}
								else
								{
									price_delta_tsl = price_trade * m_config.stop_loss_short;
									price_delta_ttp = price_trade * m_config.profit_rollback_short;

									price_tsl = price_trade + price_delta_tsl;
									price_tp = price_trade * (1.0 - m_config.take_profit_short);
								}
							}

							++index;
						}

						if (current_state != State::N)
						{
							auto price_deviation = (m_inputs_backtest[i + 1].price_close -
								m_inputs_backtest[i].price_close) / m_inputs_backtest[i].price_close;

							auto delta = price_deviation * position;

							position += delta;

							total_reward += delta;
							trade_reward += delta;
						}

						result.rewards.push_back(std::make_pair(m_inputs_backtest[i + 1].date_time_close, total_reward));
					}

					if (current_state != State::N)
					{
						result.trades.back().end = m_inputs_strategy.back().date_time_close;
						result.trades.back().reward = trade_reward;
					}

					verify(result);

					return result;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < backtester_exception > (logger, exception);
				}
			}

			std::size_t Backtester::make_strategy_begin() const
			{
				LOGGER(logger, false);

				try
				{
					auto begin = m_config.skipped_timesteps;

					while (m_inputs_strategy[begin].date_time_close.day != 1)
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

			std::size_t Backtester::make_backtest_begin() const
			{
				LOGGER(logger, false);

				try
				{
					auto begin = m_config.skipped_timesteps * 15; // TODO

					while (m_inputs_backtest[begin].date_time_close.day != 1)
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
					transaction = m_config.investment + total_reward;
				}
			}

			Backtester::frame_t Backtester::make_prehistory(std::size_t index) const
			{
				LOGGER(logger, false);

				try
				{
					return std::make_pair(
						std::next(std::begin(m_inputs_strategy), index + 1 - m_config.timesteps_prehistory),
						std::next(std::begin(m_inputs_strategy), index + 1));
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