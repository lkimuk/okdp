//!!!This file is not finished.
#ifndef OKDP_COMPOSITE_HPP
#define OKDP_COMPOSITE_HPP

#include <iostream>
#include <algorithm> // std::find
#include <string>
#include <vector>


namespace okdp
{	
	template<typename T>
	class component
	{
		using pointer = component<T>*;
	public:
		component(const char* name) : name_{name} {}
		
		virtual void add(pointer comp) = 0;
		virtual void remove(pointer comp) = 0;
		virtual void display(int depth) = 0;
	
	protected:
		const char* name_;
	};
	
	template<typename BaseType, typename ChildType>
	class leaf : public BaseType
	{
		using pointer = BaseType*;
	public:
		leaf(const char* name) : BaseType{name} {}
		
		void add(pointer child) {}
		void remove(pointer child) {}
		
		void display(int depth) override
		{
			std::cout << std::string(depth, '-') << name_ << std::endl;
		}
	};
	
	template<typename BaseType, typename ChildType>
	class composite : public BaseType
	{
		using pointer = BaseType*;
	public:
		composite(const char* name) : BaseType{name} {}
	
		void add(pointer child) override
		{
			children_.push_back(child);
		}
		
		void remove(pointer child) override
		{
			auto it = std::find(std::begin(children_), std::end(children_), child);
			if(it != std::end(children_))
				children_.erase(it);
			else
				throw std::invalid_argument("error: removed object does not a child of the parent component!");
		}
		
		void display(int depth) override
		{
			std::cout << std::string(depth, '-') << name_ << std::endl;
			
			for(auto&& child : children_)
				child->display(depth + 2);
		}
		
	private:
		std::vector<BaseType*> children_;
	};
	
	
} // namespace okdp

#endif // OKDP_COMPOSITE_HPP
