#ifndef OKDP_DECORATOR_HPP
#define OKDP_DECORATOR_HPP

namespace okdp
{

/*!
@brief a class template to implement decorator pattern.

All concrete decorators inherits from this class and implements
its abstract interfaces to decorate concrete components.

@tparam Component for an abstract class including some interfaces.

@since version 1.0.0
*/
template<typename Component>
class decorator : public Component
{
	using pointer = Component*;
public:
	decorator(pointer comp) {
		comp_ = comp;
	}

private:
	pointer comp_;
};


} // namespace okdp

#endif // OKDP_DECORATOR_HPP