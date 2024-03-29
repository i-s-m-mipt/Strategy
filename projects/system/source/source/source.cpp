#include "source.hpp"

namespace solution
{
	namespace system
	{
		void Source::Data::save_inputs(const inputs_container_t & inputs) const
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::inputs / m_config.asset;

				std::filesystem::create_directories(path);

				path /= File::inputs_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				const auto separator = ',';

				std::stringstream sout;

				for (auto i = m_config.skipped_timesteps; i < std::size(inputs); ++i)
				{
					const auto & input = inputs[i];

					sout << input.date_time_open << separator;

					for (auto j = 0U; j < 7U; ++j)
					{
						sout << std::noshowpos << (j == input.day ? 1 : 0) << separator;
					}

					for (auto j = 0U; j < 24U; ++j)
					{
						sout << std::noshowpos << (j == input.hour ? 1 : 0) << separator;
					}

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_open  << separator;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_high  << separator;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_low   << separator;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_close << separator;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_base  << separator;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_quote << separator;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_buy_base  << separator;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_buy_quote << separator;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_sell_base  << separator;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_sell_quote << separator;
				}

				fout << sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::Data::save_result(const Result & result) const
		{
			LOGGER(logger);

			try
			{
				save_reward(result);
				save_trades(result);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::Data::save_markup(const Result & result) const
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::markup / m_config.asset;

				std::filesystem::create_directories(path);

				path /= File::markup_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				std::vector < char > markup(std::size(result.rewards), '+');

				for (auto i = std::begin(result.rewards); i != std::end(result.rewards); ++i)
				{
					auto max = std::max_element(std::begin(result.rewards), std::next(i),
						[](const auto & lhs, const auto & rhs) { return (lhs.second < rhs.second); });

					auto min = std::min_element(max, std::next(i),
						[](const auto & lhs, const auto & rhs) { return (lhs.second < rhs.second); });

					if (max != min)
					{
						for (auto j = max; j != min; ++j)
						{
							markup[std::distance(std::begin(result.rewards), j)] = '-';
						}
					}
				}

				const auto separator = ',';

				auto index = 0ULL;

				for (const auto & [date_time, reward] : result.rewards)
				{
					fout << std::noshowpos << date_time << separator;

					fout << std::noshowpos << to_time_t(date_time) << separator;

					fout << markup[index++] << '\n';
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::Data::save_reward(const Result & result) const
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::result / m_config.asset;

				std::filesystem::create_directories(path);

				path /= File::reward_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				const auto separator = ',';

				for (const auto & [date_time, reward] : result.rewards)
				{
					fout << std::noshowpos << date_time << separator;

					fout << std::noshowpos << to_time_t(date_time) << separator;

					fout << std::setprecision(3) << std::fixed << std::showpos << reward << '\n';
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::Data::save_trades(const Result & result) const
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::result / m_config.asset;

				std::filesystem::create_directories(path);

				path /= File::trades_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				const auto separator = ',';

				for (const auto & trade : result.trades)
				{
					fout << std::noshowpos << trade.begin << separator;
					fout << std::noshowpos << trade.end << separator;

					fout << std::noshowpos << to_time_t(trade.begin) << separator;
					fout << std::noshowpos << to_time_t(trade.end) << separator;

					switch (trade.state)
					{
					case Strategy::State::L:
					{
						fout << "L" << separator;

						break;
					}
					case Strategy::State::S:
					{
						fout << "S" << separator;

						break;
					}
					default:
					{
						fout << "unknown" << separator;

						break;
					}
					}

					fout << std::setprecision(3) << std::fixed << std::showpos << trade.reward << '\n';
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::initialize()
		{
			LOGGER(logger);

			try
			{
				load();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::uninitialize()
		{
			LOGGER(logger);

			try
			{
				m_thread_pool.join();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::load()
		{
			LOGGER(logger);

			try
			{
				load_python();

				load_indicators();

				load_strategies();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::load_python()
		{
			LOGGER(logger);

			try
			{
				boost::python::exec("from system import make_report", m_python.global(), m_python.global());
				boost::python::exec("from system import get_klines",  m_python.global(), m_python.global());
				//boost::python::exec("from system import get_orders",  m_python.global(), m_python.global());
				//boost::python::exec("from system import get_trades",  m_python.global(), m_python.global());
			}
			catch (const boost::python::error_already_set &)
			{
				LOGGER_WRITE_ERROR(logger, shared::Python::exception());
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::load_indicators()
		{
			LOGGER(logger);

			try
			{
				m_indicators[source::indicators::APWVB::name] = 
					std::make_shared < source::indicators::APWVB > (m_config);

				m_indicators[source::indicators::EMA::name] =
					std::make_shared < source::indicators::EMA > (m_config);

				m_indicators[source::indicators::RSI::name] =
					std::make_shared < source::indicators::RSI > (m_config);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::load_strategies()
		{
			LOGGER(logger);

			try
			{
				m_strategies[source::Strategy::m_name] = 
					std::make_shared < source::Strategy > (m_config);

				m_strategies[source::strategies::hard::Accelerator::m_name] =
					std::make_shared < source::strategies::hard::Accelerator > (m_config);

				m_strategies[source::strategies::hard::Assimilator::m_name] = 
					std::make_shared < source::strategies::hard::Assimilator > (m_config);

				m_strategies[source::strategies::hard::Intersector::m_name] =
					std::make_shared < source::strategies::hard::Intersector > (m_config);

				m_strategies[source::strategies::hard::Subjugator::m_name] =
					std::make_shared < source::strategies::hard::Subjugator > (m_config);

				m_strategies[source::strategies::soft::Combinator::m_name] =
					std::make_shared < source::strategies::soft::Combinator > (m_config, m_database);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::reload_indicators()
		{
			LOGGER(logger);

			try
			{
				m_indicators.clear();

				load_indicators();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::reload_strategies()
		{
			LOGGER(logger);

			try
			{
				m_strategies.clear();

				load_strategies();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::handle()
		{
			LOGGER(logger);

			try
			{
				if (m_config.required_inputs)
				{
					handle_inputs();
				}

				if (m_config.required_backtest)
				{
					handle_backtest();
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::handle_inputs() const
		{
			LOGGER(logger);

			try
			{
				m_data.save_inputs(load_inputs(m_config.timeframe_strategy));
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::handle_backtest()
		{
			LOGGER(logger);

			try
			{
				if (m_config.required_backtest_fit)
				{
					handle_backtest_fit();
				}

				auto inputs_backtest = load_inputs(m_config.timeframe_backtest);
				auto inputs_strategy = load_inputs(m_config.timeframe_strategy);
				
				{
					Backtester backtester(m_config, 
						inputs_backtest, inputs_strategy,
							m_strategies.at(Strategy::m_name));

					m_result_BH = backtester.run();

					m_data.save_result(m_result_BH);
				}

				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::reward_data,
					Data::Directory::result / m_asset / Data::File::reward_BH_data);
				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::trades_data,
					Data::Directory::result / m_asset / Data::File::trades_BH_data);

				{
					Backtester backtester(m_config, 
						inputs_backtest, inputs_strategy,
							m_strategies.at(m_config.strategy));

					m_result_HS = backtester.run();

					m_data.save_result(m_result_HS);
				}

				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::reward_data,
					Data::Directory::result / m_asset / Data::File::reward_HS_data);
				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::trades_data,
					Data::Directory::result / m_asset / Data::File::trades_HS_data);

				make_report();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::handle_backtest_fit() // TODO
		{
			LOGGER(logger);

			try
			{
				auto max_reward = 0.0;

				struct Parameters
				{
					std::size_t parameter_1 = 0;
					std::size_t parameter_2 = 0;

					double      parameter_3 = 0.0;
				};

				Parameters best_parameters;

				auto index = 0ULL;

				for (auto parameter_1 = 20ULL; parameter_1 <= 50; parameter_1 += 1)
				{
					for (auto parameter_2 = 200ULL; parameter_2 <= 500; parameter_2 += 10)
					{
						for (auto parameter_3 = 0.00; parameter_3 <= 0.50; parameter_3 += 0.05)
						{
							m_config.timesteps_prehistory      = parameter_1;
							m_config.apwvb_timesteps_pwvb_long = parameter_1;
							m_config.apwvb_timesteps_sma_long  = parameter_2;
							m_config.apwvb_threshold_long      = parameter_3;

							reload_indicators();
							reload_strategies();

							Backtester backtester(m_config,
								load_inputs(m_config.timeframe_backtest),
								load_inputs(m_config.timeframe_strategy),
									m_strategies.at(m_config.strategy));

							Parameters parameters = { parameter_1, parameter_2, parameter_3 };

							auto reward = backtester.run().rewards.back().second;

							if (reward > max_reward)
							{
								max_reward = reward;

								best_parameters = parameters;
							}
						}
					}
				}

				m_config.timesteps_prehistory      = best_parameters.parameter_1;
				m_config.apwvb_timesteps_pwvb_long = best_parameters.parameter_1;
				m_config.apwvb_timesteps_sma_long  = best_parameters.parameter_2;
				m_config.apwvb_threshold_long      = best_parameters.parameter_3;

				reload_indicators();
				reload_strategies();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::inputs_container_t Source::load_inputs(const std::string & timeframe, bool all) const
		{
			LOGGER(logger);

			try
			{
				auto klines = load_klines(timeframe, all);
				auto orders = load_orders();
				auto trades = load_trades();

				return make_inputs(klines, orders, trades);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::klines_container_t Source::load_klines(const std::string & timeframe, bool all) const
		{
			LOGGER(logger);

			try
			{
				const auto epsilon = std::numeric_limits < double > ::epsilon();

				klines_container_t klines;

				auto inputs_first_year = (all ? 2015LL : m_config.inputs_first_year);

				for (auto year = inputs_first_year; year <= m_config.inputs_last_year; ++year)
				{
					for (std::time_t month = 1LL; month <= 12LL; ++month)
					{
						auto path = Data::Directory::klines;
						
						path /= make_klines_file_name(year, month, timeframe);

						if (!std::filesystem::exists(path))
						{
							LOGGER_WRITE_DEBUG(logger, "file " + path.string() + " doesn't exist");

							continue;
						}

						std::fstream fin(path.string(), std::ios::in);

						if (!fin)
						{
							throw source_exception("cannot open file " + path.string());
						}

						std::string line;

						while (std::getline(fin, line))
						{
							auto kline = parse_kline(line);

							if (kline.price_open < epsilon)
							{
								throw std::domain_error("required: (price_open > 0.0)");
							}

							if (kline.price_high < epsilon)
							{
								throw std::domain_error("required: (price_high > 0.0)");
							}

							if (kline.price_low < epsilon)
							{
								throw std::domain_error("required: (price_low > 0.0)");
							}

							if (kline.price_close < epsilon)
							{
								throw std::domain_error("required: (price_close > 0.0)");
							}

							klines.push_back(std::move(kline));
						}
					}
				}

				std::sort(std::begin(klines), std::end(klines),
					[](const auto & lhs, const auto & rhs)
						{ return (lhs.time_open < rhs.time_open); });

				return klines;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::orders_container_t Source::load_orders(bool all) const // TODO
		{
			LOGGER(logger);

			try
			{
				return {}; 
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::trades_container_t Source::load_trades(bool all) const
		{
			LOGGER(logger);

			try
			{
				trades_container_t trades;

				auto inputs_first_year = (all ? 2015LL : m_config.inputs_first_year);

				for (auto year = inputs_first_year; year <= m_config.inputs_last_year; ++year)
				{
					for (std::time_t month = 1LL; month <= 12LL; ++month)
					{
						auto path = Data::Directory::trades; 
						
						path /= make_trades_file_name(year, month);

						if (!std::filesystem::exists(path))
						{
							LOGGER_WRITE_DEBUG(logger, "file " + path.string() + " doesn't exist");

							continue;
						}

						std::fstream fin(path.string(), std::ios::in);

						if (!fin)
						{
							throw source_exception("cannot open file " + path.string());
						}

						std::string line;

						while (std::getline(fin, line))
						{
							trades.push_back(parse_trade(line));
						}
					}
				}

				std::sort(std::begin(trades), std::end(trades),
					[](const auto & lhs, const auto & rhs)
						{ return (lhs.time < rhs.time); });

				return trades;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		std::string Source::make_klines_file_name(std::time_t year, 
			std::time_t month, const std::string & timeframe) const
		{
			LOGGER(logger, false);

			try
			{
				const auto separator = '-';

				std::stringstream sout;

				sout <<
					m_asset << separator << timeframe << separator << year << separator <<
					std::setw(2) << std::setfill('0') << std::right << month << Extension::csv;

				return sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		std::string Source::make_orders_file_name(std::time_t year, std::time_t month) const
		{
			LOGGER(logger, false);

			try
			{
				const auto separator = '-';

				std::stringstream sout;

				sout << 
					m_asset << separator << "orders" << separator << year << separator <<
					std::setw(2) << std::setfill('0') << std::right << month << Extension::csv;

				return sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		std::string Source::make_trades_file_name(std::time_t year, std::time_t month) const
		{
			LOGGER(logger, false);

			try
			{
				const auto separator = '-';

				std::stringstream sout;

				sout << 
					m_asset << separator << "trades" << separator << year << separator <<
					std::setw(2) << std::setfill('0') << std::right << month << Extension::csv;

				return sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::Kline Source::parse_kline(const std::string & line) const
		{
			LOGGER(logger, false);

			try
			{
				detail::Kline_Parser < std::string::const_iterator > parser;

				auto first = std::begin(line);
				auto last  = std::end(line);

				Kline kline;

				auto result = boost::spirit::qi::phrase_parse(
					first, last, parser, boost::spirit::qi::blank, kline);

				if (result)
				{
					return kline;
				}
				else
				{
					throw source_exception("cannot parse line " + line);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::Order Source::parse_order(const std::string & line) const
		{
			LOGGER(logger, false);

			try
			{
				Order order;

				return order;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::Trade Source::parse_trade(const std::string & line) const
		{
			LOGGER(logger, false);

			try
			{
				detail::Trade_Parser < std::string::const_iterator > parser;

				auto first = std::begin(line);
				auto last  = std::end(line);

				Trade trade;

				auto result = boost::spirit::qi::phrase_parse(
					first, last, parser, boost::spirit::qi::blank, trade);

				if (result)
				{
					return trade;
				}
				else
				{
					throw source_exception("cannot parse line " + line);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::inputs_container_t Source::make_inputs(
			const klines_container_t & klines,
			const orders_container_t & orders,
			const trades_container_t & trades) const
		{
			LOGGER(logger);

			try
			{
				inputs_container_t inputs(std::size(klines));

				for (auto i = 0ULL; i < std::size(klines); ++i)
				{
					auto date_time_open  = from_time_t(klines[i].time_open,  true);
					auto date_time_close = from_time_t(klines[i].time_close, true);

					inputs[i].date_time_open  = date_time_open;
					inputs[i].date_time_close = date_time_close;

					inputs[i].day  = day_of_week(date_time_close);
					inputs[i].hour = date_time_close.hour;

					inputs[i].price_open  = klines[i].price_open;
					inputs[i].price_high  = klines[i].price_high;
					inputs[i].price_low   = klines[i].price_low;
					inputs[i].price_close = klines[i].price_close;

					inputs[i].volume_base  = klines[i].volume_base;
					inputs[i].volume_quote = klines[i].volume_quote;

					inputs[i].volume_buy_base  = klines[i].volume_buy_base;
					inputs[i].volume_buy_quote = klines[i].volume_buy_quote;

					inputs[i].volume_sell_base  = klines[i].volume_base  - klines[i].volume_buy_base;
					inputs[i].volume_sell_quote = klines[i].volume_quote - klines[i].volume_buy_quote;
				}

				update_indicators(inputs);

				//update_price_aggregated_trades(inputs, klines, trades);

				//update_movement_tags(inputs);

				return inputs;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::update_indicators(inputs_container_t & inputs) const
		{
			LOGGER(logger);

			try
			{
				for (const auto & [name, indicator] : m_indicators)
				{
					indicator->run(inputs);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::update_price_aggregated_trades(
			      inputs_container_t & inputs,
			const klines_container_t & klines, 
			const trades_container_t & trades) const // TODO
		{
			LOGGER(logger);

			try
			{
				/*
				const auto timesteps = m_config.timesteps_prehistory;

				if (std::size(klines) != std::size(inputs))
				{
					throw std::domain_error("invalid klines/inputs size");
				}

				for (auto i = timesteps - 1; i < std::size(klines); ++i)
				{
					const auto & kline = klines[i];

					auto price_high = std::max_element(
						std::next(std::begin(klines), i + 1 - timesteps),
						std::next(std::begin(klines), i + 1), 
						[](const auto & lhs, const auto & rhs)
							{ return (lhs.price_high < rhs.price_high); })->price_high;

					auto price_low = std::min_element(
						std::next(std::begin(klines), i + 1 - timesteps),
						std::next(std::begin(klines), i + 1),
						[](const auto & lhs, const auto & rhs)
							{ return (lhs.price_low < rhs.price_low); })->price_low;

					auto step = kline.price_close * m_config.price_aggregated_trade_step;

					inputs[i].price_aggregated_trades.resize(static_cast < std::size_t > (
						std::ceil((price_high - price_low) / step)), { 0.0, 0.0, 0.0, 0.0 });

					auto begin = std::lower_bound(std::begin(trades), std::end(trades),
						klines[i + 1 - timesteps].time_open, [](const auto & trade, auto time)
							{ return (trade.time < time); });

					auto end = std::upper_bound(std::begin(trades), std::end(trades),
						klines[i].time_close, [](auto time, const auto & trade)
							{ return (time < trade.time); });

					for (auto iterator = begin; iterator != end; ++iterator)
					{
						if (iterator->time < klines[i + 1 - timesteps].time_open ||
							iterator->time > klines[i].time_close)
						{
							LOGGER_WRITE_FATAL(logger, std::string("trade time: ") + std::to_string(iterator->time));
							LOGGER_WRITE_FATAL(logger, std::string("open  time: ") + std::to_string(kline.time_open));
							LOGGER_WRITE_FATAL(logger, std::string("close time: ") + std::to_string(kline.time_close));

							throw std::domain_error("invalid trade time");
						}

						auto index = static_cast < std::size_t > (std::floor((iterator->price - price_low) / step));

						if (iterator->is_buyer_maker == "True")
						{
							inputs[i].price_aggregated_trades[index].volume_buy_base  += iterator->volume_base;
							inputs[i].price_aggregated_trades[index].volume_buy_quote += iterator->volume_quote;
						}
						else
						{
							inputs[i].price_aggregated_trades[index].volume_sell_base  += iterator->volume_base;
							inputs[i].price_aggregated_trades[index].volume_sell_quote += iterator->volume_quote;
						}
					}
				}
				*/
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::update_movement_tags(inputs_container_t & inputs) const // TODO
		{
			LOGGER(logger);

			try
			{
				/*
				const auto epsilon = std::numeric_limits < double > ::epsilon();

				const auto prediction_range = m_config.timesteps_prediction;

				const auto min_deviation = m_config.min_movement;

				for (auto i = 0ULL; i < std::size(inputs) - prediction_range; ++i)
				{
					auto max_price = inputs[i + prediction_range].price_close;
					auto min_price = inputs[i + prediction_range].price_close;

					for (auto j = 1ULL; j < prediction_range; ++j)
					{
						max_price = std::max(max_price, inputs[i + j].price_close);
						min_price = std::min(min_price, inputs[i + j].price_close);
					}

					auto current_price_close = std::max(inputs[i].price_close, epsilon);

					auto deviation_L = std::abs((max_price - current_price_close) / current_price_close);
					auto deviation_S = std::abs((min_price - current_price_close) / current_price_close);

					if ((deviation_L < min_deviation) &&
						(deviation_S < min_deviation))
					{
						inputs[i].movement_tag = 0;
					}
					else
					{
						if (deviation_L > deviation_S)
						{
							inputs[i].movement_tag = +1;
						}
						else
						{
							inputs[i].movement_tag = -1;
						}
					}
				}
				*/
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::make_report() const
		{
			LOGGER(logger);

			try
			{
				try
				{
					m_python.global()["make_report"](
						(Data::Directory::result / m_asset / Data::File::reward_HS_data).string().c_str(),
						(Data::Directory::result / m_asset / Data::File::trades_HS_data).string().c_str(),
						(Data::Directory::result / m_asset / Data::File::reward_BH_data).string().c_str(),
						(Data::Directory::config / m_asset / Data::File::config_json   ).string().c_str());
				}
				catch (const boost::python::error_already_set &)
				{
					LOGGER_WRITE_ERROR(logger, shared::Python::exception());
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::State Source::handle(State current_state) const
		{
			LOGGER(logger);

			try
			{
				auto inputs = make_inputs(get_klines(), {}, {});

				auto prehistory = std::make_pair(std::prev(std::end(inputs), 
					m_config.timesteps_prehistory), std::end(inputs));

				return m_strategies.at(m_config.strategy)->run(prehistory, current_state);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::klines_container_t Source::get_klines() const
		{
			LOGGER(logger);

			try
			{
				try
				{
					const auto size = m_config.skipped_timesteps;

					const auto timeframe = m_config.timeframe_strategy;

					return make_klines(boost::python::extract < std::string > (
						m_python.global()["get_klines"](m_asset.c_str(),
							std::to_string(size).c_str(), timeframe.c_str())));
				}
				catch (const boost::python::error_already_set &)
				{
					LOGGER_WRITE_ERROR(logger, shared::Python::exception());
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::orders_container_t Source::get_orders() const // TODO
		{
			LOGGER(logger);

			try
			{
				try
				{
					return {};
				}
				catch (const boost::python::error_already_set &)
				{
					LOGGER_WRITE_ERROR(logger, shared::Python::exception());
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::trades_container_t Source::get_trades() const // TODO
		{
			LOGGER(logger);

			try
			{
				try
				{
					return {};
				}
				catch (const boost::python::error_already_set &)
				{
					LOGGER_WRITE_ERROR(logger, shared::Python::exception());
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::klines_container_t Source::make_klines(const std::string & data) const
		{
			LOGGER(logger);

			try
			{
				klines_container_t klines(m_config.skipped_timesteps);

				auto array = json_t::parse(data);

				for (auto i = 0ULL; i < std::size(klines); ++i)
				{
					const auto & element = array.at(i);

					klines[i].time_open        = element["time_open"       ].get < std::time_t > ();
					klines[i].price_open       = element["price_open"      ].get < double > ();
					klines[i].price_high       = element["price_high"      ].get < double > ();
					klines[i].price_low        = element["price_low"       ].get < double > ();
					klines[i].price_close      = element["price_close"     ].get < double > ();
					klines[i].volume_base      = element["volume_base"     ].get < double > ();
					klines[i].time_close       = element["time_close"      ].get < std::time_t > ();
					klines[i].volume_quote     = element["volume_quote"    ].get < double > ();
					klines[i].volume_buy_base  = element["volume_buy_base" ].get < double > ();
					klines[i].volume_buy_quote = element["volume_buy_quote"].get < double > ();
				}

				std::sort(std::begin(klines), std::end(klines),
					[](const auto & lhs, const auto & rhs)
						{ return lhs.time_open < rhs.time_open; });

				return klines;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::orders_container_t Source::make_orders(const std::string & data) const // TODO
		{
			LOGGER(logger);

			try
			{
				return {};
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::trades_container_t Source::make_trades(const std::string & data) const // TODO
		{
			LOGGER(logger);

			try
			{
				return {};
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::update_database(std::shared_ptr < database_t > database)
		{
			LOGGER(logger);

			try
			{
				m_database = database;

				reload_indicators();
				reload_strategies();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution