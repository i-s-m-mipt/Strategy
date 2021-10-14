#ifndef SOLUTION_SYSTEM_INDICATORS_AWVB_HPP
#define SOLUTION_SYSTEM_INDICATORS_AWVB_HPP

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
			class AWVB
			{
			public:

				explicit AWVB(const std::string & name, std::size_t timesteps_wvb, std::size_t timesteps_sma) :
					m_name(name), m_timesteps_wvb(timesteps_wvb), m_timesteps_sma(timesteps_sma)
				{
					initialize();
				}

			private:

				void initialize() const;

			public:

				void operator()(inputs_container_t & inputs) const;

			public:

				static inline const std::string type = "AWVB";

			private:

				const std::string m_name;

			private:

				std::size_t m_timesteps_wvb;
				std::size_t m_timesteps_sma;
			};

		} // namespace indicators

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_INDICATORS_AWVB_HPP