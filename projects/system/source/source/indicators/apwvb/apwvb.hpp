#ifndef SOLUTION_SYSTEM_SOURCE_INDICATORS_APWVB_HPP
#define SOLUTION_SYSTEM_SOURCE_INDICATORS_APWVB_HPP

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
				class APWVB final : public Indicator
				{
				public:

					explicit APWVB(const Config & config) noexcept : Indicator(config)
					{}

					virtual ~APWVB() noexcept = default;

				public:

					virtual void run(inputs_container_t & inputs) const override;

				private:

					void run_implementation(inputs_container_t & inputs, 
						std::size_t timesteps_pwvb, std::size_t timesteps_sma, const std::string & key) const;

				public:

					static double price_weighted_volume_bias(const Input & input) noexcept;

				public:

					static inline const std::string name = "APWVB";
				};

			} // namespace indicators

		} // namespace source

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SOURCE_INDICATORS_APWVB_HPP