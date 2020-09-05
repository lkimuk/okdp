/// This file is not finished!!!
///


#ifndef OKDP_ABSTRACT_FACTORY_HPP
#define OKDP_ABSTRACT_FACTORY_HPP

#include "../utils/type_list.hpp"
#include <typeinfo>
#include <iostream>

using namespace okdp::utils;

namespace okdp
{


template<typename T>
struct type2type
{
	typedef T OriginalType;	
};


// Generate concrete factory.
template<typename T>
class AbstractFactoryMetaFun
{
public:
	virtual T* DoCreate(type2type<T>)  {
		std::cout << "do create" << typeid(T).name() << std::endl;
	}
	virtual ~AbstractFactoryMetaFun() {}
};


/*!
@brief a class template to implement abstract factory pattern
@tparam List type for typelists
@tparam MetaFun type for 

@since version 1.0.0
*/
template<typename List, template<typename> class MetaFun = AbstractFactoryMetaFun>
class abstract_factory : public tl::gen_scatter_hierarchy<List, MetaFun>
{
public:
	using ProductList = List;
	template<class T> T* Create()
	{
		MetaFun<T>& meta = *this;
		return this->DoCreate(type2type<T>());
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
@brief a class template to implement concreate factory
@tparam AbstractFactory type for ...
@tparam Creator type for specifying the way of creating products.
@tparam List type for typelists

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

//class PrototypeFactoryMetaFun


} // namespace okdp


#endif // OKDP_ABSTRACT_FACTORY_HPP
