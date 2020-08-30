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

#ifndef OKDP_UTILS_LOCK_HPP
#define OKDP_UTILS_LOCK_HPP

#include <atomic>


namespace okdp
{
namespace utils
{


class spin_lock
{
private:
	std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
public:
	void lock() { while(flag_.test_and_set(std::memory_order_acquire)); }

	void unlock() { flag_.clear(std::memory_order_release); }
};


// spin_lock wrapper
class lock_guard
{
private:
	spin_lock lock_;

public:
	lock_guard() { lock_.lock(); }
	~lock_guard() { lock_.unlock(); }
};


} // namespace utils
} // namespace okdp


#endif // OKDP_UTILS_LOCK_HPP
