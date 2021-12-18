#ifndef SOLUTION_SYSTEM_DETAIL_TRADES_HPP
#define SOLUTION_SYSTEM_DETAIL_TRADES_HPP

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
			struct Trade
			{
				std::size_t id;

				double price;

				double volume_base;
				double volume_quote;

				std::time_t time;

				std::string is_buyer_maker;

				std::string is_best_match;
			};

			template < typename Iterator >
			class Trade_Parser : public boost::spirit::qi::grammar <
				Iterator, Trade(), boost::spirit::qi::blank_type >
			{
			private:

				using rule_t = boost::spirit::qi::rule <
					Iterator, Trade(), boost::spirit::qi::blank_type > ;

			public:

				Trade_Parser() : Trade_Parser::base_type(start)
				{
					start %=
						boost::spirit::qi::ulong_long         >> separator >> // id
						boost::spirit::qi::double_            >> separator >> // price
						boost::spirit::qi::double_            >> separator >> // volume_base
						boost::spirit::qi::double_            >> separator >> // volume_quote
						boost::spirit::qi::long_long          >> separator >> // time
						*~boost::spirit::qi::char_(separator) >> separator >> // is_buyer_maker
						*~boost::spirit::qi::char_('\n');                     // is_best_match

				}

				~Trade_Parser() noexcept = default;

			private:

				static inline const auto separator = ',';

			private:

				rule_t start;
			};

		} // namespace detail

	} // namespace system

} // namespace solution

BOOST_FUSION_ADAPT_STRUCT
(
	solution::system::detail::Trade,
		(std::size_t, id)
		(double,      price)
		(double,      volume_base)
		(double,      volume_quote)
		(std::time_t, time)
		(std::string, is_buyer_maker)
		(std::string, is_best_match)
)

#endif // #ifndef SOLUTION_SYSTEM_DETAIL_TRADES_HPP