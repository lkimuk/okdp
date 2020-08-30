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

#ifndef OKDP_SINGLETON_HPP
#define OKDP_SINGLETON_HPP

#include "../utils/lock.hpp"
#include <cstdlib>
#include <new>


namespace okdp
{

/*!
@brief a class template to implement singleton pattern
@tparam T type for singleton class
@tparam LongLifeTime type is a bool value that false is not supported dead-reference and true is on the contrary.

@subclass Any subclass should use CRTP to become a singleton type. 
For example, class Log : public okdp::singleton<T> {}; // non-dead-reference version
	     class Log : public okdp::singleton<T, true> {}; // dead-reference version

@since version 1.0.0
*/
template<typename T, bool LongLifeTime = false> class singleton;

template<typename T>
class singleton<T, false>
{
public:

	static T& instance() {
		static T obj;
		return obj;	
	}

protected:
	singleton() = default;
	virtual ~singleton() {}

private:
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
};



template<typename T>
class singleton<T, true>
{
public:
	static T& instance() 
	{
		if(!pInstance_)
		{
			// DCL
			utils::lock_guard lock;
			if(!pInstance_)
			{
				destroyed_ ? on_dead_reference() : create();
			}
		}

		return *pInstance_;
	}

private:

	static void create() 
	{
		static T obj;
		pInstance_ = &obj;
	}

	static void on_dead_reference()
	{
		create();
		new(pInstance_) T;
		std::atexit(kill_singleton);
		destroyed_ = false;

	}
	
	static void kill_singleton()
	{
		pInstance_->~singleton();
	}


	singleton(const singleton&);
	singleton& operator=(const singleton&);
	singleton(singleton&&);
	singleton& operator=(singleton&&);

protected:
	singleton() = default;
	virtual ~singleton()
	{
		pInstance_ = nullptr;
		destroyed_ = true;
	}


	static T* pInstance_;
	static bool destroyed_;
};

template<typename T> T* singleton<T, true>::pInstance_ = nullptr;
template<typename T> bool singleton<T, true>::destroyed_ = false;


} // namespace okdp


#endif // OKDP_SINGLETON_HPP
