#ifndef SOLUTION_SYSTEM_CONFIG_HPP
#define SOLUTION_SYSTEM_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_IX86))
#  define _X86_
#  if (!defined(_CHPE_X86_ARM64_) && defined(_M_HYBRID))
#    define _CHPE_X86_ARM64_
#  endif // #  if (!defined(_CHPE_X86_ARM64_) && defined(_M_HYBRID))
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_AMD64))
#  define _AMD64_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_ARM))
#  define _ARM_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_ARM64))
#  define _ARM64_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_M68K))
#  define _68K_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_IA64_)  && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
      defined(_M_MPPC))
#  define _MPPC_
#endif // #if (!defined(_68K_)   && \

#if (!defined(_68K_)   && \
     !defined(_MPPC_)  && \
     !defined(_X86_)   && \
     !defined(_M_IX86) && \
     !defined(_AMD64_) && \
     !defined(_ARM_)   && \
     !defined(_ARM64_) && \
     !defined(_IA64_)  && \
      defined(_M_IA64))
#  define _IA64_
#endif // #if (!defined(_68K_)   && \

#if !defined(_MAC) && (defined(_68K_) || defined(_MPPC_)) 
#  define _MAC
#endif // #if !defined(_MAC) && (defined(_68K_) || defined(_MPPC_))

#include <ctime>
#include <cstdlib>
#include <string>

namespace solution
{
    namespace system
    {
        struct Config
        {
            double accelerator_flat_deviation;

            double accelerator_max_deviation_s;
            double accelerator_min_deviation_s;

            double accelerator_stop_loss;

            double apwvb_threshold_long;
            double apwvb_threshold_short;

            std::size_t apwvb_timesteps_pwvb_long;
            std::size_t apwvb_timesteps_pwvb_short;

            std::size_t apwvb_timesteps_sma_long;
            std::size_t apwvb_timesteps_sma_short;

            std::string asset;

            std::size_t combinator_neighbors;

            double combinator_threshold_long;
            double combinator_threshold_short;

            double commission;

            std::size_t ema_timesteps_fast;
            std::size_t ema_timesteps_slow;

            bool has_reinvestment;

            std::time_t inputs_first_year;
            std::time_t inputs_last_year;

            double investment;

            double level_deviation;

            std::size_t level_max_bias;
            std::size_t level_min_bias;

            std::string permitted_state;

            double profit_rollback_long;
            double profit_rollback_short;

            double pulse_threshold;

            bool required_backtest;
            bool required_backtest_fit;
            bool required_inputs;

            double rsi_threshold_long;
            double rsi_threshold_short;

            std::size_t rsi_timesteps_long;
            std::size_t rsi_timesteps_short;

            std::size_t skipped_timesteps;

            double stop_loss_long;
            double stop_loss_short;

            std::string strategy;

            double take_profit_long;
            double take_profit_short;
            
            std::string timeframe_backtest;
            std::string timeframe_strategy;
            
            std::size_t timesteps_prediction;
            std::size_t timesteps_prehistory;

            // ====================================

            std::time_t interval;

            bool required_correlation;
            bool required_run;

            double reserve;

            std::time_t server_start_hour;
            std::time_t server_start_minute;
        };

    } // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_CONFIG_HPP