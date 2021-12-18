#ifndef SOLUTION_SYSTEM_SOURCE_INDICATORS_EMA_HPP
#define SOLUTION_SYSTEM_SOURCE_INDICATORS_EMA_HPP

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
				class EMA final : public Indicator
				{
				public:

					explicit EMA(const Config & config) noexcept : Indicator(config)
					{}

					virtual ~EMA() noexcept = default;

				public:

					virtual void run(inputs_container_t & inputs) const override;

				private:

					void run_implementation(inputs_container_t & inputs,
						std::size_t timesteps, const std::string & key) const;

				public:

					static inline const std::string name = "EMA";
				};

			} // namespace indicators

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_INDICATORS_EMA_HPP