
#ifndef OKDP_UTILS_DYNAMIC_TYPE_HPP
#define OKDP_UTILS_DYNAMIC_TYPE_HPP

#include <string_view>
#include <memory>
#include <functional>
#include <any>


namespace okdp::utils
{


struct type_fields {
	std::string_view name;
	int size;
	std::function<std::any()> pfn_create_object;

	static type_fields* load(const char* name) 
	{
		for(auto p = first; p != nullptr; p = p->next)
		{
			if(p->name.compare(name) == 0)
				return p;
		}
		return nullptr; // not found
	}

	static type_fields* first; // start of field list
	type_fields* next;
};

type_fields* type_fields::first = nullptr;


struct field_list_init {
	field_list_init(type_fields* tf_new) {
		tf_new->next = type_fields::first;
		type_fields::first = tf_new;
	}
};

#define DYNAMIC_OBJECT(class_name) \
public: \
	static okdp::utils::type_fields class##class_name; \
	static std::any create_object(); \
	virtual okdp::utils::type_fields* get_type_fields() const;

#define __IMPLEMENT_DYNAMIC_OBJECT(class_name, pfn_new) \
	static char _lpsz##class_name[] = #class_name; \
	okdp::utils::type_fields class_name::class##class_name = { \
		_lpsz##class_name, sizeof(class_name), pfn_new, nullptr }; \
	okdp::utils::type_fields* class_name::get_type_fields() const \
		{ return &class_name::class##class_name; } \
	static okdp::utils::field_list_init _init_##class_name(&class_name::class##class_name);

#define IMPLEMENT_DYNAMIC_OBJECT(class_name) \
	std::any class_name::create_object() \
		{ return std::make_shared<class_name>(); } \
	__IMPLEMENT_DYNAMIC_OBJECT(class_name, class_name::create_object)


} // namespace okdp::utils


#endif // OKDP_UTILS_DYNAMIC_TYPE_HPP  
