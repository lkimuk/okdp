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

#ifndef OKDP_CYCLIC_VISITOR_HPP
#define OKDP_CYCLIC_VISITOR_HPP


namespace okdp
{

template<typename... Ts> struct visitor {};
template<typename T> struct visitor<T> {
	virtual void visit(const T&) = 0;
};

template<typename T, typename... Ts>
struct visitor<T, Ts...> : visitor<Ts...> {
	using visitor<Ts...>::visit;
	virtual void visit(const T&) = 0;
};


template<typename... VisitorTypes>
struct visitable_base {
	using visitor_type = visitor<VisitorTypes...>;
	virtual void accept(visitor_type&) = 0;
};


#define VISITABLE_CYCLIC_OBJECT() \
	void accept(visitable_base::visitor_type& v) override \
	{ v.visit(*this); }

} // namespace okdp


#endif // OKDP_CYCLIC_VISITOR_HPP
