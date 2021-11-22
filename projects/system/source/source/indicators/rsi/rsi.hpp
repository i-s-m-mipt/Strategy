#ifndef SOLUTION_SYSTEM_SOURCE_INDICATORS_RSI_HPP
#define SOLUTION_SYSTEM_SOURCE_INDICATORS_RSI_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include "../indicator.hpp"

namespace solution
{
	namespace system
	{
		namespace source
		{
			namespace indicators
			{
				class RSI final : public Indicator
				{
				public:

					explicit RSI(const Config & config) noexcept : Indicator(config)
					{}

					virtual ~RSI() noexcept = default;

				public:

					virtual void run(inputs_container_t & inputs) const override;

				public:

					static inline const std::string name = "RSI";
				};

			} // namespace indicators

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_INDICATORS_RSI_HPP