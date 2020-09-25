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

#ifndef OKDP_COMPOSITE_HPP
#define OKDP_COMPOSITE_HPP

#include <algorithm> // std::find
#include <stdexcept> // std::invalid_argument
#include <vector>
#include <memory> // for smart pointer


namespace okdp
{
	

/*!
@brief a class template to implement composite pattern.
@tparam BaseType for abstract component.

@since version 1.0.0
*/
template<typename BaseType>
class composite : public BaseType
{
	using pointer = BaseType*;
	using spointer = std::unique_ptr<BaseType>;
public:
	template<typename... Args>
	composite(Args&&... args) : BaseType(std::forward<Args>(args)...) {}

	// add child to composite
	void add(pointer child)
	{
		children_.push_back(spointer(child));
	}
	
	// remove child from composite
	void remove(pointer child)
	{
		auto it = std::find(std::begin(children_), std::end(children_), child);
		if(it == std::end(children_))
			throw std::invalid_argument("error: removed object does not a child of the parent component!");
		
		children_.erase(it);
	}
	
protected:
	std::vector<spointer> children_;
};
	
	
} // namespace okdp

#endif // OKDP_COMPOSITE_HPP
