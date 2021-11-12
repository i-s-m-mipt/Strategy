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
#include <string>

namespace solution
{
    namespace system
    {
        struct Config
        {
            bool test;

            bool required_inputs;

            std::time_t inputs_year_begin;
            std::time_t inputs_year_end;
            std::size_t inputs_timeframe;
            std::string inputs_timeframe_type;
            std::string inputs_asset;

            double price_aggregated_trade_step;

            std::size_t price_aggregated_trades_depth;

            std::size_t timesteps_prehistory;
            std::size_t timesteps_prediction;

            std::size_t skipped_timesteps;

            double min_movement;

            double transaction;
            double commission;
            double stop_loss;

            std::string test_hard_strategy;
            std::string test_soft_strategy;

            bool required_backtest;

            std::size_t awvb_timesteps_wvb;
            std::size_t awvb_timesteps_sma;

            bool required_research;

            double assimilator_min_deviation;

            bool required_backtest_fit;

            bool has_reinvestment;

            bool required_run;

            std::time_t interval;

            double max_drawdown;

            std::string benchmark;

            std::string main_strategy;

            std::time_t server_start_hour;
            std::time_t server_start_minute;

            std::size_t server_start_iteration;

            bool required_markup;

            bool required_correlation;
        };

    } // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_CONFIG_HPP