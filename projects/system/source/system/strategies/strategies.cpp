#include "strategies.hpp"

namespace solution
{
	namespace system
	{
		namespace strategies
		{
			Strategy::State Strategy::handle(const inputs_container_t & inputs,
				double transaction) const
			{
				return State();
			}

			Strategy::State Strategy::handle(const inputs_container_t & inputs,
				double transaction, const State & state) const
			{
				return State();
			}

		} // namespace strategies

	} // namespace system

} // namespace solution