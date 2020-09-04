/// This file is not finished!!!
///


#ifndef OKDP_ABSTRACT_FACTORY_HPP
#define OKDP_ABSTRACT_FACTORY_HPP

#include "../utils/type_list.hpp"


namespace okdp
{

template<typename T>
struct type2type
{
	using value_type = T;
};

template<typename T>
class AbstractFactoryMetaFun
{
public:
	virtual T* DoCreate(type2type<T>) = 0;
	virtual ~AbstractFactoryMetaFun() {}
};


/*!
@brief a class template to implement abstract factory pattern
@tparam List type for typelists
@tparam MetaFun type for 

@since version 1.0.0
*/
template<typename List, template<typename> class MetaFun = AbstractFactoryMetaFun>
class abstract_factory : public utils::gen_scatter_hierarchy<List, MetaFun>
{
public:
	using ProductList = List;
	template<class T> T* Create()
	{
		MetaFun<T>& meta = *this;
		return this.DoCreate(type2type<T>());
	}
};


template<typename ConcreateProduct, typename Base>
class OpNewFactoryMetaFun : public Base
{
	using BaseProductList = typename Base::ProductList;
protected:
	using ProductList = typename BaseProductList::Tail;
public:
	using AbstractProduct = typename BaseProductList::Head;
	ConcreateProduct* DoCreate(type2type<AbstractProduct>)
	{
		return new ConcreateProduct;
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
	typename AbstractFactory,
	template<typename, typename> class Creator = OpNewFactoryMetaFun
	typename List = typename AbstractFactory::ProductList
>
class concreate_factory : 
	public utils::gen_linear_hierarchy<reverse_tl<List>, Creator, AbstractFactory>
{
public:
	using ProductList = typename AbstractFactory::ProductList;
	using ConcreateProductList = List;
};

//class PrototypeFactoryMetaFun


} // namespace okdp

#endif // OKDP_ABSTRACT_FACTORY_HPP
