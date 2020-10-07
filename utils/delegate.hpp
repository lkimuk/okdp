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

#ifndef OKDP_UTIL_DELEGATE_HPP
#define OKDP_UTIL_DELEGATE_HPP

#include <map>


namespace okdp
{
namespace utils
{
	
template<typename IdentifierType, typename CallbackType>
class delegate
{
public:
	using value_type = std::map<IdentifierType, CallbackType>;
	
public:
	template<typename T>
	void insert(T&& id, CallbackType callback)
	{
		map_.emplace(std::forward<T>(id), callback);
	}
	
	template<typename T>
	bool remove(T&& id)
	{
		return map_.erase(std::forward<T>(id)) == 1;
	}
	
	void clear()
	{
		map_.clear();
	}
	
	template<typename T>
	bool find(T&& id, typename value_type::const_iterator& it)
	{
		it = map_.find(std::forward<T>(id));
		if(it != map_.end())
			return true;
		
		return false;
	}
	
private:
	value_type map_;
};
	
} // namespace utils
} // namespace okdp

#endif // OKDP_UTIL_DELEGATE_HPP