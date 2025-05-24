#pragma once

template <Callable Func, typename... Args>
class Evaluator
{
	std::decay_t<Func> function; 
public:
	Evaluator(Func&& _function)
	{
		function = _function;
	}
	inline bool eval(Args&& args...)
	{
		return 
	}
};