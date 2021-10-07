#include "backtester.hpp"

namespace solution
{
	namespace system
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
				const auto epsilon = std::numeric_limits < double > ::epsilon();

				const auto transaction = m_config.transaction;
				const auto commission  = m_config.commission;

				Result result;

				LOGGER_WRITE(logger, std::to_string(std::size(m_inputs)));
				LOGGER_WRITE(logger, std::to_string(result.rewards.max_size()));

				result.rewards.reserve(std::size(m_inputs));

				auto total_reward = 0.0;
				auto trade_reward = 0.0;

				Strategy::State current_state;

				for (auto i = m_config.skipped_timesteps + m_config.timesteps_prehistory;
					i < std::size(m_inputs) - 1; ++i)
				{
					auto current_reward = 0.0;

					if (!current_state.is_null() && trade_reward < -transaction * m_config.stop_loss)
					{
						current_reward = -commission * std::abs(current_state.position);

						current_state = Strategy::State();

						result.trades.back().end = m_inputs[i].date_time;
						result.trades.back().reward = trade_reward - current_reward;

						trade_reward = 0.0;
					}

					inputs_container_t prehistory(
						std::next(std::begin(m_inputs), i + 1 - m_config.timesteps_prehistory),
						std::next(std::begin(m_inputs), i + 1));

					auto required_state = m_strategy->handle(prehistory, transaction, current_state);

					if (required_state.is_null() && !current_state.is_null())
					{
						current_reward = -commission * std::abs(current_state.position);

						current_state = required_state;

						result.trades.back().end = m_inputs[i].date_time;
						result.trades.back().reward = trade_reward - current_reward;

						trade_reward = 0.0;
					}

					if ((required_state.is_long()  && !current_state.is_long()) ||
						(required_state.is_short() && !current_state.is_short()))
					{
						if (!current_state.is_null())
						{
							current_reward = -commission * std::abs(current_state.position);

							result.trades.back().end = m_inputs[i].date_time;
							result.trades.back().reward = trade_reward - current_reward;

							trade_reward = 0.0;
						}

						current_reward -= commission * std::abs(required_state.position);

						current_state = required_state;
						
						result.trades.push_back(Result::Trade());

						result.trades.back().begin = m_inputs[i].date_time;
						result.trades.back().type = (current_state.is_long() ?
							Result::Trade::Type::L : Result::Trade::Type::S);
					}

					if (!current_state.is_null())
					{
						auto price_deviation = (m_inputs[i + 1].price_close -
							m_inputs[i].price_close) / std::max(m_inputs[i].price_close, epsilon);

						current_reward         += price_deviation * current_state.position;
						current_state.position += price_deviation * current_state.position;
					}

					total_reward += current_reward;
					trade_reward += current_reward;

					result.rewards.push_back(std::make_pair(m_inputs[i].date_time, total_reward));
				}

				return result;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < backtester_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution