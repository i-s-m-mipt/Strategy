#ifndef SOLUTION_SYSTEM_DETAIL_KLINES_HPP
#define SOLUTION_SYSTEM_DETAIL_KLINES_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../detail.hpp"

namespace solution
{
	namespace system
	{
		namespace detail
		{
			struct Kline
			{
				std::time_t time_open  = 0LL;
				
				double price_open  = 0.0;
				double price_high  = 0.0;
				double price_low   = 0.0;
				double price_close = 0.0;

				double volume_base = 0.0;

				std::time_t time_close = 0LL;

				double volume_quote = 0.0;

				std::size_t n_trades = 0ULL;

				double volume_taker_base  = 0.0;
				double volume_taker_quote = 0.0;

				double ignore = 0.0; // ignored
			};

			template < typename Iterator >
			class Kline_Parser : public boost::spirit::qi::grammar <
				Iterator, Kline(), boost::spirit::qi::blank_type >
			{
			private:

				using rule_t = boost::spirit::qi::rule <
					Iterator, Kline(), boost::spirit::qi::blank_type > ;

			public:

				Kline_Parser() : Kline_Parser::base_type(start)
				{
					start %=
						boost::spirit::qi::long_long  >> separator >> // time_open
						boost::spirit::qi::double_    >> separator >> // price_open
						boost::spirit::qi::double_    >> separator >> // price_high
						boost::spirit::qi::double_    >> separator >> // price_low
						boost::spirit::qi::double_    >> separator >> // price_close
						boost::spirit::qi::double_    >> separator >> // volume_base
						boost::spirit::qi::long_long  >> separator >> // time_close
						boost::spirit::qi::double_    >> separator >> // volume_quote
						boost::spirit::qi::ulong_long >> separator >> // n_trades
						boost::spirit::qi::double_    >> separator >> // volume_taker_base
						boost::spirit::qi::double_    >> separator >> // volume_taker_quote
						boost::spirit::qi::double_;                   // ignore
						
				}

				~Kline_Parser() noexcept = default;

			private:

				static inline const auto separator = ',';

			private:

				rule_t start;
			};

			using klines_container_t = std::vector < Kline > ;

		} // namespace detail

	} // namespace system

} // namespace solution

BOOST_FUSION_ADAPT_STRUCT
(
	solution::system::detail::Kline,
		(std::time_t, time_open)
		(double,      price_open)
		(double,      price_high)
		(double,      price_low)
		(double,      price_close)
		(double,      volume_base)
		(std::time_t, time_close)
		(double,      volume_quote)
		(std::size_t, n_trades)
		(double,      volume_taker_base)
		(double,      volume_taker_quote)
		(double,      ignore)
)

#endif // #ifndef SOLUTION_SYSTEM_DETAIL_KLINES_HPP