/*
 *
MIT License

Copyright (c) 2020 gxkey(Gaoxing Li)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/

#ifndef OKDP_COMMAND_HPP
#define OKDP_COMMAND_HPP

#include <functional>
#include <vector>


namespace okdp
{
	
template<typename ReturnType>
class command
{
	using fn_type = std::function<ReturnType()>;
	using value_type = std::vector<fn_type>;
public:
	// insert order
	void push_order(fn_type callback) {
		cmd_.push_back(callback);
	}
	
	// remove the last order.
	void pop_order() {
		cmd_.pop_back();
	}
	
	// execute all commands
	void execute() {
		for(auto&& order : cmd_)
			order();
	}
private:
	value_type cmd_; // command list
};
	
} // namespace okdp

#endif // OKDP_COMMAND_HPP