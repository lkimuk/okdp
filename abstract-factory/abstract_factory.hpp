/*
 *
MIT License
Copyright (c) 2020 lkimuk(Gaoxing Li)
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

#ifndef OKDP_ABSTRACT_FACTORY_HPP
#define OKDP_ABSTRACT_FACTORY_HPP

#include "../utils/type_list.hpp"
#include <memory>


using namespace okdp::utils; // for tl::type_list


namespace okdp
{


template<typename T>
struct type2type
{
	typedef T OriginalType;	
};


/*!
@brief a class template to create interface with type T.

@tparam T type is the type which will be created.
*/
template<typename T>
class abstract_factory_meta_fun
{
public:
	virtual T* do_create(type2type<T>) = 0; 
	virtual ~abstract_factory_meta_fun() {}
};


/*!
@brief a class template to implement abstract factory pattern.

@tparam Ts variadic parameter type is the elements of the typelist.
@tparam MetaFun type for creating abstract functions for each type in List.

@since version 1.1.0
*/
template<typename List, template<typename> class MetaFun = abstract_factory_meta_fun>
class abstract_factoryT : public tl::gen_scatter_hierarchy<List, MetaFun>
{
public:
	using ProductList = List;
	/// return product T by invoking new operator
	/// !!!note!!! if use this method to create an object, user have the responsibility to delete it.
	template<class T> T* create()
	{
		MetaFun<T>& meta = *this;
		return meta.do_create(type2type<T>());
	}

	/// return product T by invoking shared_ptr
	template<class T> std::shared_ptr<T> create_shared()
	{
		return std::shared_ptr<T>(create<T>());
	}

	/// return product T by invoking unique_ptr
	template<class T> std::unique_ptr<T> create_unique()
	{
		return std::unique_ptr<T>(create<T>());	
	}


};

template<typename... Ts>
using abstract_factory = abstract_factoryT<tl::type_list<Ts...>>;


/*!
@brief class template creation_meta_fun 

Creates an object by invoking the new operator or smart pointers.

@tparam ConcreteProduct type for creating products.
@tparam Base type for recursive inheriting others products(via gen_linear_hierarchy).
*/
template<typename ConcreteProduct, typename Base>
class creation_meta_fun : public Base
{
	using BaseProductList = typename Base::ProductList;
protected:
	using ProductList = tl::tail_type<BaseProductList>;
public:
	/// return concrete product by invoking new operator
	/// !!!note!!! if use this method to create an object, user have the responsibility to delete it.
	using AbstractProduct = tl::head_type<BaseProductList>;
	ConcreteProduct* do_create(type2type<AbstractProduct>)
	{
		return new ConcreteProduct;
	}
};


/*!
@brief recursively generates concrete productes via gen_linear_hierarchy.

@tparam AbstractFactory type for abstract factory.
@tparam Creator type for specifying the way of creating products.
@tparam List type for the type of all concrete products.

@since version 1.0.0
*/
template
<
	typename AbstractFact,
	template<typename, typename> class Creator = creation_meta_fun,
	typename List = typename AbstractFact::ProductList
>
class concrete_factory : 
	public tl::gen_linear_hierarchy<tl::reverse<List>, Creator, AbstractFact>
{
public:
	using ProductList = typename AbstractFact::ProductList;
	using ConcreteProductList = List;
};


} // namespace okdp


#endif // OKDP_ABSTRACT_FACTORY_HPP
