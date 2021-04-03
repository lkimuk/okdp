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
#ifndef OKDP_OBSERVER_HPP
#define OKDP_OBSERVER_HPP

#include <vector>
#include <memory>
#include <algorithm>


namespace okdp {

template<typename ConcreteSubject>
class subject : public ConcreteSubject {
	using Token = std::shared_ptr<void>;
	using Target = typename ConcreteSubject::ObserverType;
	using WeakTarget = std::weak_ptr<Target>;
	using SharedTarget = std::shared_ptr<Target>;
public:
	
	Token attach(Target target) {
		//auto token = std::make_shared<Target>(std::move(callback));
		std::shared_ptr<Target> token(new Target(std::move(target)), 
			[&](Target* obj) { delete obj; this->cleanup(); }
		);
		observers_.push_back(token);
		return token;
	}

	void detach(Token& t) {
		t.reset();
	}

	// Push Model
	template<typename... Args>
	void notify_push(Args&&... args) const {
		for (auto wp : observers_) {
			if (auto sp = wp.lock())
				(*sp)(std::forward<Args>(args)...);
		}
	}

	// Pull Model
	void notify_pull() {
		for (auto wp : observers_) {
			if (auto sp = wp.lock())
				(*sp)(this);
		}
	}

private:
	void cleanup()
	{
		observers_.erase(
			std::remove_if(observers_.begin(), observers_.end(),
				[&](WeakTarget t) { return t.expired(); }
			),
			observers_.end()
		);
	}

private:
	std::vector<WeakTarget> observers_;
};

} // namespace okdp


#endif // OKDP_OBSERVER_HPP