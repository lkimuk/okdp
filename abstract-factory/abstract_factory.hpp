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

#ifndef OKDP_ABSTRACT_FACTORY_HPP
#define OKDP_ABSTRACT_FACTORY_HPP

#include "../utils/type_list.hpp"


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
class AbstractFactoryMetaFun
{
public:
	virtual T* DoCreate(type2type<T>) = 0; 
	virtual ~AbstractFactoryMetaFun() {}
};


/*!
@brief a class template to implement abstract factory pattern.
@tparam List type is a typelist.
@tparam MetaFun type for creating abstract functions for each type in List.

@since version 1.0.0
*/
template<typename List, template<typename> class MetaFun = AbstractFactoryMetaFun>
class abstract_factory : public tl::gen_scatter_hierarchy<List, MetaFun>
{
public:
	using ProductList = List;
	template<class T> T* create()
	{
		MetaFun<T>& meta = *this;
		return meta.DoCreate(type2type<T>());
	}
};


template<typename ConcreteProduct, typename Base>
class OpNewFactoryMetaFun : public Base
{
	using BaseProductList = typename Base::ProductList;
protected:
	using ProductList = tl::tail_type<BaseProductList>;
public:
	using AbstractProduct = tl::head_type<BaseProductList>;
	ConcreteProduct* DoCreate(type2type<AbstractProduct>)
	{
		return new ConcreteProduct;
	}
};


/*!
@brief a class template to implement concrete factory.
@tparam AbstractFactory type for abstract factory.
@tparam Creator type for specifying the way of creating products.
@tparam List type for the type of all concrete products.

@since version 1.0.0
*/
template
<
	typename AbstractFact,
	template<typename, typename> class Creator = OpNewFactoryMetaFun,
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
