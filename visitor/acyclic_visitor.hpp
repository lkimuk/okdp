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

#ifndef OKDP_ACYCLIC_VISITOR_HPP
#define OKDP_ACYCLIC_VISITOR_HPP


namespace okdp {

// define strawnman base class
struct visitor_base {
	virtual ~visitor_base() = default;
};

// define class template of visitor
template<typename Visitable, typename R = void>
struct visitor {
	using ReturnType = R;
	virtual ReturnType visit(Visitable&) = 0;
};

// define visited hierarchy base class
template<typename R = void>
class visitable_base {
public:
	using ReturnType = R;
	virtual ~visitable_base() = default;
	virtual ReturnType accept(visitor_base&) = 0;
protected:
	template<typename T>
	static ReturnType accept_impl(T& visited, visitor_base& vb) {
		if(visitor<T>* p = dynamic_cast<visitor<T>*>(&vb)) {
			return p->visit(visited);
		}
		return ReturnType();
	}
};



#define VISITABLE_OBJECT() \
	ReturnType accept(okdp::visitor_base& vb) override \
	{ return accept_impl(*this, vb); }


} // namespace okdp



#endif // OKDP_ACYCLIC_VISITOR_HPP
