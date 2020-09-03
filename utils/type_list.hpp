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

#ifndef OKDP_UTILS_TYPE_LIST_HPP
#define OKDP_UTILS_TYPE_LIST_HPP


namespace okdp::utils
{


// declaration of the typelist
template<typename... Ts> class type_list {};


// head type of the type_list
template<typename List> class head_typeT;

template<typename Head, typename... Tail>
class head_typeT<type_list<Head, Tail...>>
{
public:
	using Type = Head;
};

template<typename List>
using head_type = typename head_typeT<List>::Type;


// tail type of the type_list
template<typename List> class tail_typeT;

template<typename Head, typename... Tail>
class tail_typeT<type_list<Head, Tail...>>
{
public:
	using Type = type_list<Tail...>;
};

template<typename List>
using tail_type = typename tail_typeT<List>::Type;


// insert elements onto the front of the type_list
template<typename List, typename NewElement> class push_frontT;

template<typename... Ts, typename NewElement>
class push_frontT<type_list<Ts...>, NewElement>
{
public:
	using Type = type_list<NewElement, Ts...>;
};

template<typename List, typename NewElement>
using push_front = typename push_frontT<List, NewElement>::Type;


// Appending to a type_list
template<typename List, typename NewElement> struct push_backT;

template<typename... Ts, typename NewElement>
struct push_backT<type_list<Ts...>, NewElement>
{
	using Type = type_list<Ts..., NewElement>;
};

template<typename List, typename NewElement>
using push_back = typename push_backT<List, NewElement>::Type;


// Indexing of the type_list
template<typename List, unsigned IDX>
class type_index : public type_index<tail_type<List>, N - 1> {};

template<typename List>
class type_index<List, 0> : public head_type<List> {};

template<typename List, unsigned IDX>
using type_at = typename type_index<List, IDX>::Type;


// Finding the best match type within the type_list
//template<typename List> class search_type;
//
//template<typename List>
//class search_type
//{
//private:
//	using First = head<List>;
//	using Rest = typename search_type<tail<List>>::Type;
//public:
//	using Type = sizeof(First) >= sizeof(Rest) ? First : Rest;
//};
//
//template<>
//class search_type<type_list<>>
//{
//public:
//	using Type = char;
//};
//
//template<typename List>
//using find_type = typename


// Determines whether the given type_list has no elements.
template<typename List>
struct is_empty
{
	static constexpr bool value = false;
};

template<>
struct is_empty<type_list<>>
{
	static constexpr bool value = true;
};


// Reversing a type_list
template<typename List, bool Empty = is_empty<List>::value> class reverse_typelist;

template<typename List>
using reverse_tl = typename reverse_typelist<List>::Type;

// recursive case:
template<typename List>
class reverse_typelist<List, false>
	: public push_back<reverse_tl<tail_type<List>>, head_type<List>> {};

// basis case:
template<typename List>
class rever_typelist<List, true>
{
public:
	using Type = List;
};


// Remove the last element within the type_list.
template<typename List>
struct pop_backT
{
	using Type = reverse_tl<tail_type<reverse_tl<List>>>;
};

template<typename List>
using pop_back = typename pop_backT<List>::Type;


// Add const to a type
template<typename T>
struct add_constT
{
	using Type = T const;
};

template<typename T>
using add_const = typename add_constT<T>::Type;


// Transforming a type_list
template<typename List, template<typename T> class MetaFun, 
	bool Empty = is_empty<List>::value>
class transformT;

// recursive case:
template<typename List, template<typename T> class MetaFun>
class transformT<List, MetaFun, false>
	: public push_front<typename transformT<tail_type<List>, MetaFun>::Type, 
		typename MetaFun<head_type<List>>::Type>
{};

// basis case:
template<typename List, tempalte<typename T> class MetaFun>
class transformT<List, MetaFun, true>
{
public:
	using Type = List;
};


// ......

	
} // namespace okdp::utils


#endif // OKDP_UTILS_TYPE_LIST_HPP
