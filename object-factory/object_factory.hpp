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
#ifndef OKDP_OBJECT_FACTORY_HPP
#define OKDP_OBJECT_FACTORY_HPP

#include "../singleton/singleton.hpp"
#include <map>
#include <string>
#include <functional>
#include <stdexcept>
#include <memory>

#if defined(__cplusplus) && __cplusplus >= 201709L 
	#include <concepts>
#endif


namespace okdp
{


/*!
@brief Automatically register type into factory and yield them by identifier "key".

This class template is inherited from okdp::singleton and it can hence be used as a "singleton class".

@tparam AbstractProduct The abstract base class of the products, all registered types are its subclass.

@since version 1.0.0
*/
template<typename AbstractProduct>
class object_factory : public singleton<object_factory<AbstractProduct>>
{
	using this_type = object_factory<AbstractProduct>;
public:

	/*!
	@brief register type into map using functor.

	@tparam T the creating type.
	*/
	template<typename T>
#if defined(__cplusplus) && __cplusplus >= 201709L 
	requires std::derived_from<T, AbstractProduct>
#endif
	struct register_type
	{
		// registers T into object factory.
		register_type(const std::string& key)
		{
			this_type::instance().map_.emplace(key, []{ return new T(); });
		}

		// register T with variadic parameters into object factory 
		template<typename... Args>
		register_type(const std::string& key, Args... args)
		{
			this_type::instance().map_.emplace(key, [&]{ return new T(args...); });
		}
	};


	// removes object 
	static void unregister(const std::string& key)
	{
		this_type::instance().map_.erase(key);
	}


	/// return concrete object by key by invoking new operator
	/// !!!note!!! if use this method user should delete it to avoid memory leaks.
	static AbstractProduct* create(const std::string& key)
	{
		if(this_type::instance().map_.find(key) == this_type::instance().map_.end())
			throw std::invalid_argument("error: unknown object type passed to factory!");
		return this_type::instance().map_[key]();
	}

	
	/// return concrete object by key by invoking shared ptr
	static std::shared_ptr<AbstractProduct> create_shared(const std::string& key)
	{
		return std::shared_ptr<AbstractProduct>(create(key));
	}


	/// return concrete object by invoking unique ptr
	static std::unique_ptr<AbstractProduct> create_unique(const std::string& key)
	{
		return std::unique_ptr<AbstractProduct>(create(key));
	}


private:
	static std::map<std::string, std::function<AbstractProduct*()>> map_;
};

template<typename T> std::map<std::string, std::function<T*()>> object_factory<T>::map_;


} // namespace okdp

#endif // OKDP_OBJECT_FACTORY_HPP
