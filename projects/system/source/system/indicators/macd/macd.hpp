#ifndef SOLUTION_SYSTEM_INDICATORS_MACD_HPP
#define SOLUTION_SYSTEM_INDICATORS_MACD_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../indicators.hpp"

namespace solution
{
	namespace system
	{
		namespace indicators
		{
			class MACD
			{
			public:

				explicit MACD(const std::string & name, 
					std::size_t timesteps_long, 
					std::size_t timesteps_short, 
					std::size_t timesteps_signal) :
						m_name(name), 
						m_timesteps_long(timesteps_long), 
						m_timesteps_short(timesteps_short), 
						m_timesteps_signal(timesteps_signal)
				{
					initialize();
				}

			private:

				void initialize() const;

			public:

				void operator()(inputs_container_t & inputs) const;

			private:

				const std::string m_name;

			private:

				std::size_t m_timesteps_long;
				std::size_t m_timesteps_short;
				std::size_t m_timesteps_signal;
			};

		} // namespace indicators

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_INDICATORS_MACD_HPP