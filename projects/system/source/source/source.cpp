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
				auto path = Directory::inputs / m_config.inputs_asset;

				std::filesystem::create_directory(path);

				path /= File::inputs_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				const auto delimeter = ',';

				std::stringstream sout;

				for (auto i = m_config.skipped_timesteps; i < std::size(inputs); ++i)
				{
					const auto & input = inputs[i];

					for (auto j = 0U; j < 7U; ++j)
					{
						sout << std::noshowpos << (j == input.day ? 1 : 0) << delimeter;
					}

					for (auto j = 0U; j < 24U; ++j)
					{
						sout << std::noshowpos << (j == input.hour ? 1 : 0) << delimeter;
					}

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_open  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_high  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_low   << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.price_close << delimeter;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_base  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_quote << delimeter;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_buy_base  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_buy_quote << delimeter;

					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_sell_base  << delimeter;
					sout << std::setprecision(6) << std::fixed << std::noshowpos << input.volume_sell_quote << delimeter;

					for (const auto & [name, value] : input.indicators)
					{
						sout << std::setprecision(6) << std::fixed << // ?
							std::showpos << value << delimeter;
					}

					// TODO (save price_aggregated_trades)

					sout << std::showpos << input.movement_tag << '\n';
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

		void Source::Data::save_reward(const Result & result) const
		{
			LOGGER(logger);

			try
			{
				auto path = Directory::result / m_config.inputs_asset;

				std::filesystem::create_directory(path);

				path /= File::reward_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				const auto delimeter = ',';

				for (const auto & [date_time, reward] : result.rewards)
				{
					fout << std::noshowpos << date_time << delimeter;

					fout << std::noshowpos << to_time_t(date_time) << delimeter;

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
				auto path = Directory::result / m_config.inputs_asset;

				std::filesystem::create_directory(path);

				path /= File::trades_data;

				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				const auto delimeter = ',';

				for (const auto & trade : result.trades)
				{
					fout << std::noshowpos << trade.begin << delimeter;
					fout << std::noshowpos << trade.end << delimeter;

					fout << std::noshowpos << to_time_t(trade.begin) << delimeter;
					fout << std::noshowpos << to_time_t(trade.end) << delimeter;

					switch (trade.state)
					{
					case Strategy::State::L:
					{
						fout << "L" << delimeter;

						break;
					}
					case Strategy::State::S:
					{
						fout << "S" << delimeter;

						break;
					}
					default:
					{
						fout << "unknown" << delimeter;

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

				if (m_config.required_inputs)
				{
					handle_inputs();
				}

				if (m_config.required_backtest)
				{
					handle_backtest();
				}

				if (m_config.required_research)
				{
					handle_research();
				}
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
				load_indicators();

				load_strategies();
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
				m_indicators[source::indicators::AWVB::name] = 
					std::make_shared < source::indicators::AWVB > (m_config);
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

				m_strategies[source::strategies::hard::Assimilator::m_name] = 
					std::make_shared < source::strategies::hard::Assimilator > (m_config);
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

		void Source::handle_inputs() const
		{
			LOGGER(logger);

			try
			{
				m_data.save_inputs(load_inputs());
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

				auto inputs = load_inputs();

				{
					Backtester backtester(m_config, inputs, m_strategies.at(Strategy::m_name));

					m_data.save_result(backtester.run());
				}

				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::reward_data,
					Data::Directory::result / m_asset / Data::File::reward_BH_data);
				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::trades_data,
					Data::Directory::result / m_asset / Data::File::trades_BH_data);

				{
					Backtester backtester(m_config, inputs,
						m_strategies.at(m_config.test_hard_strategy));

					m_data.save_result(backtester.run());
				}

				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::reward_data,
					Data::Directory::result / m_asset / Data::File::reward_HS_data);
				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::trades_data,
					Data::Directory::result / m_asset / Data::File::trades_HS_data);

				/*
				{
					Backtester backtester(m_config, inputs,
						m_strategies.at(m_config.test_soft_strategy));

					m_data.save_result(backtester.run());
				}

				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::reward_data,
					Data::Directory::result / m_asset / Data::File::reward_SS_data);
				std::filesystem::rename(
					Data::Directory::result / m_asset / Data::File::trades_data,
					Data::Directory::result / m_asset / Data::File::trades_SS_data);
				*/

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
				std::vector < std::future < void > > futures;

				std::mutex mutex;

				auto max_reward = 0.0;

				struct Parameters
				{
					std::size_t parameter_1 = 0ULL;
					std::size_t parameter_2 = 0ULL;
					double      parameter_3 = 0.00;
				};

				Parameters best_parameters;

				for (auto parameter_1 = 1ULL; parameter_1 <= 10; ++parameter_1)
				{
					for (auto parameter_2 = 10ULL; parameter_2 <= 100; parameter_2 += 10)
					{
						for (auto parameter_3 = 0.05; parameter_3 <= 0.50; parameter_3 += 0.05)
						{
							m_config.timesteps_prehistory      = parameter_1;
							m_config.awvb_timesteps_wvb        = parameter_1;
							m_config.awvb_timesteps_sma        = parameter_2;
							m_config.assimilator_min_deviation = parameter_3;

							reload_indicators();
							reload_strategies();

							Backtester backtester(m_config, load_inputs(),
								m_strategies.at(m_config.test_hard_strategy));

							Parameters parameters = { parameter_1, parameter_2, parameter_3 };

							std::packaged_task < void() > task([backtester = std::move(backtester),
								&mutex, &max_reward, &best_parameters, parameters]()
								{
									auto reward = backtester.run().rewards.back().second;

									std::scoped_lock lock(mutex);

									if (reward > max_reward)
									{
										max_reward = reward;

										best_parameters = parameters;
									}
								});

							futures.push_back(boost::asio::post(m_thread_pool, std::move(task)));
						}
					}
				}

				std::for_each(std::begin(futures), std::end(futures), [](auto & future) { future.wait(); });

				m_config.timesteps_prehistory      = best_parameters.parameter_1;
				m_config.awvb_timesteps_wvb        = best_parameters.parameter_1;
				m_config.awvb_timesteps_sma        = best_parameters.parameter_2;
				m_config.assimilator_min_deviation = best_parameters.parameter_3;

				reload_indicators();
				reload_strategies();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::handle_research() const
		{
			LOGGER(logger);

			try
			{
				auto inputs = load_inputs();

				research_volumes(inputs);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::inputs_container_t Source::load_inputs() const
		{
			LOGGER(logger);

			try
			{
				auto klines = load_klines();
				auto orders = load_orders();
				auto trades = load_trades();

				return make_inputs(klines, orders, trades);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::klines_container_t Source::load_klines() const
		{
			LOGGER(logger);

			try
			{
				const auto epsilon = std::numeric_limits < double > ::epsilon();

				klines_container_t klines;

				for (auto year = m_config.inputs_year_begin; year <= m_config.inputs_year_end; ++year)
				{
					for (std::time_t month = 1LL; month <= 12LL; ++month)
					{
						auto path = Data::Directory::klines;
						
						path /= make_klines_file_name(year, month);

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

		Source::orders_container_t Source::load_orders() const
		{
			LOGGER(logger);

			try
			{
				return {}; // TODO
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		Source::trades_container_t Source::load_trades() const
		{
			LOGGER(logger);

			try
			{
				trades_container_t trades;

				for (auto year = m_config.inputs_year_begin; year <= m_config.inputs_year_end; ++year)
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

		std::string Source::make_klines_file_name(std::time_t year, std::time_t month) const
		{
			LOGGER(logger, false);

			try
			{
				const auto delimeter = '-';

				std::stringstream sout;

				sout <<
					m_asset << delimeter << m_config.inputs_timeframe <<
					m_config.inputs_timeframe_type << delimeter << year << delimeter <<
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
				const auto delimeter = '-';

				std::stringstream sout;

				sout << 
					m_asset << delimeter << "orders" << delimeter << year << delimeter <<
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
				const auto delimeter = '-';

				std::stringstream sout;

				sout << 
					m_asset << delimeter << "trades" << delimeter << year << delimeter <<
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

					inputs[i].day  = day_of_week(date_time_open);
					inputs[i].hour = date_time_open.hour;

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

				update_price_aggregated_trades(inputs, klines, trades);

				update_movement_tags(inputs);

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
			const trades_container_t & trades) const
		{
			LOGGER(logger);

			try
			{
				if (std::size(klines) != std::size(inputs))
				{
					throw std::domain_error("invalid klines/inputs size");
				}

				const auto epsilon = std::numeric_limits < double > ::epsilon();

				for (auto i = m_config.timesteps_prehistory - 1; i < std::size(klines); ++i)
				{
					auto begin = std::lower_bound(std::begin(trades), std::end(trades),
						klines[i + 1 - m_config.timesteps_prehistory].time_open,
							[](const auto & trade, auto time) { return (trade.time < time); });

					auto end = std::upper_bound(std::begin(trades), std::end(trades), klines[i].time_close,
						[](auto time, const auto & trade) { return (time < trade.time); });

					inputs[i].lower_price_aggregated_trades.resize(
						m_config.price_aggregated_trades_depth, { 0.0, 0.0, 0.0, 0.0 });

					inputs[i].upper_price_aggregated_trades.resize(
						m_config.price_aggregated_trades_depth, { 0.0, 0.0, 0.0, 0.0 });

					for (auto iterator = begin; iterator != end; ++iterator)
					{
						if (iterator->time < klines[i + 1 - m_config.timesteps_prehistory].time_open ||
							iterator->time > klines[i].time_close)
						{
							LOGGER_WRITE_FATAL(logger, std::string("trade time: ") + std::to_string(iterator->time));
							LOGGER_WRITE_FATAL(logger, std::string("open  time: ") + std::to_string(klines[i].time_open));
							LOGGER_WRITE_FATAL(logger, std::string("close time: ") + std::to_string(klines[i].time_close));

							throw std::domain_error("invalid trade time");
						}

						if (auto index = static_cast < std::size_t > (std::floor(
								std::abs(iterator->price - inputs[i].price_close) /
									std::max(inputs[i].price_close, epsilon) /
										m_config.price_aggregated_trade_step));
							index < m_config.price_aggregated_trades_depth)
						{
							if (iterator->price < inputs[i].price_close)
							{
								if (iterator->is_buyer_maker == "True")
								{
									inputs[i].lower_price_aggregated_trades[index].volume_buy_base  += iterator->volume_base;
									inputs[i].lower_price_aggregated_trades[index].volume_buy_quote += iterator->volume_quote;
								}
								else
								{
									inputs[i].lower_price_aggregated_trades[index].volume_sell_base  += iterator->volume_base;
									inputs[i].lower_price_aggregated_trades[index].volume_sell_quote += iterator->volume_quote;
								}
							}
							else
							{
								if (iterator->is_buyer_maker == "True")
								{
									inputs[i].upper_price_aggregated_trades[index].volume_buy_base  += iterator->volume_base;
									inputs[i].upper_price_aggregated_trades[index].volume_buy_quote += iterator->volume_quote;
								}
								else
								{
									inputs[i].upper_price_aggregated_trades[index].volume_sell_base  += iterator->volume_base;
									inputs[i].upper_price_aggregated_trades[index].volume_sell_quote += iterator->volume_quote;
								}
							}
						}
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::update_movement_tags(inputs_container_t & inputs) const
		{
			LOGGER(logger);

			try
			{
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
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

		void Source::make_report() const // TODO
		{
			LOGGER(logger);

			shared::Python python;

			try
			{
				boost::python::exec("from system import make_report", python.global(), python.global());

				python.global()["make_report"](
					(Data::Directory::result / m_asset / Data::File::reward_HS_data).string().c_str(),
					(Data::Directory::result / m_asset / Data::File::trades_HS_data).string().c_str(),
					(Data::Directory::result / m_asset / Data::File::reward_BH_data).string().c_str(),
					(Data::Directory::config / m_asset / Data::File::config_json   ).string().c_str());
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

		void Source::research_volumes(const inputs_container_t & inputs) const
		{
			LOGGER(logger);

			try
			{
				const auto timesteps_wvb = m_config.awvb_timesteps_wvb;
				const auto timesteps_sma = m_config.awvb_timesteps_sma;

				const auto delimeter = ',';

				auto path = Data::Directory::result / m_asset / Data::File::research_data;

				std::fstream fout(path, std::ios::out);

				if (!fout)
				{
					throw source_exception("cannot open file " + path.string());
				}

				std::vector < std::pair < Date_Time, double > > deviations;

				deviations.reserve(std::size(inputs));

				for (auto i = timesteps_wvb - 1; i < std::size(inputs); ++i)
				{
					auto deviation = 0.0;

					for (auto j = i + 1 - timesteps_wvb; j <= i; ++j)
					{
						deviation += (inputs[j].volume_buy_base - inputs[j].volume_sell_base) *
							(inputs[j].price_high - inputs[j].price_low) / inputs[j].price_open;
					}

					deviations.push_back(std::make_pair(inputs[i].date_time_open, deviation));
				}

				for (auto i = timesteps_sma - 1; i < std::size(deviations); ++i)
				{
					auto sma = 0.0;

					for (auto j = i + 1 - timesteps_sma; j <= i; ++j)
					{
						sma += deviations[j].second;
					}

					sma /= timesteps_sma;

					char state = 'N';

					if (std::abs(deviations[i].second - sma) / std::abs(sma) >
						m_config.assimilator_min_deviation)
					{
						if (deviations[i].second < sma)
						{
							state = 'S';
						}

						if (deviations[i].second > sma)
						{
							state = 'L';
						}
					}

					fout << deviations[i].first << delimeter <<
						std::setw(9) << std::setfill(' ') << std::right <<
						std::setprecision(3) << std::fixed << std::showpos << deviations[i].second << delimeter <<
						std::setw(9) << std::setfill(' ') << std::right <<
						std::setprecision(3) << std::fixed << std::showpos << sma << delimeter << state << "\n";
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < source_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution