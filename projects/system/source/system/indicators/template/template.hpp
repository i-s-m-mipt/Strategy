#ifndef SOLUTION_SYSTEM_INDICATORS_TEMPLATE_HPP
#define SOLUTION_SYSTEM_INDICATORS_TEMPLATE_HPP

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
			class TEMPLATE
			{
			public:

				explicit TEMPLATE(const std::string & name, std::size_t timesteps) : 
					m_name(name), m_timesteps(timesteps)
				{
					initialize();
				}

			private:

				void initialize() const;

			public:

				void operator()(inputs_container_t & inputs) const;

			public:

				static inline const std::string type = "TEMPLATE";

			private:

				const std::string m_name;

			private:

				std::size_t m_timesteps;
			};

		} // namespace indicators

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_INDICATORS_TEMPLATE_HPP