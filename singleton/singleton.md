# C++ DP.07 Singleton
Singleton模式从概念上看非常简单，[DP]上所述意图如下：
> 保证一个类仅有一个实例，并提供一个访问它的全局访问点。

然而，实现起来却并非如此简单，甚至有些棘手。

棘手之处在于删除问题，如何删除Singleton实例？谁来删除？何时删除？

在[DP]中，并未探讨有关析构和删除的问题，这是GoF的疏忽。此模式虽小，内涵却多，随着观察的深入，问题便突显出来。之后，John Vlissides（GoF之一）在*Pattern hatching*（1998）一书中探讨了这个问题。

和工厂模式等不同，Singleton对象需要自己对「所有权」进行管理，用户无权删除实例。

Singleton对象从创建到删除之间便是其生命期，然而，我们只知道创建时间，而不知其删除时间，也就无法管理所有权。

事实上就算我们不进行释放操作，程序在结束之时，操作系统也会将进程所用的内存完全释放，并不会产生内存泄漏。然而，若Singleton产生的实例在构造时申请过广泛的资源（通常是内核资源，如套接字，信号量，互斥体，事件），便会产生资源泄漏。解决办法就是在程序关闭时正确地删除Singleton对象，此时，删除时机便至关重要。

Singleton模式还有一个问题是多线程支持，不过C++11之后并非主要问题。

总结一下，Singleton模式虽然易于表达和理解，但却难以实现。关键在于三点：创建，删除，多线程。其中删除是最棘手的问题，能同时满足这三点的实现，便能适应几乎所有情况。

## 7.1 Singleton的唯一性
Singleton模式是一种经过改进的全局变量，重点集中于“产生和管理一个独立对象，并且不允许产生另外一个这样的对象”。

也就是说具有唯一性，因此一切能够再次产生同样对象的方式都应该被杜绝，比如：构造函数，拷贝构造，移动构造，赋值操作符。

[DP]中的定义如下：
```cpp
class Singleton
{
public:

	static Singleton* Instance() {
		if(!pInstance_) {
			pInstance_ = new Singleton;
		}
		return pInstance_;
	}

private:
	Singleton();
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	~Singleton();

	static Singleton* pInstance_;
};

Singleton* Singleton::pInstance_ = 0;
```

这种方式的确满足了唯一性，用户除了从Instance()获取对象之外，别无他法。且用户无法意外删除对象，因为析构函数也被delete了（不过依然可以由返回的指针删除之，所以最好以引用返回）。

最大的问题在于此法只满足三大关键之一：创建，对于删除和多线程都不满足。

所以，在满足唯一性的前提下，Singleton的实现还应设法管理产生对象的生命期和多线程环境下的访问安全。

## 7.2 隐式析构的Singleton
7.1节的Singleton只有创建，没有删除，这可能会导致资源泄漏，一种解决方法是使用隐式析构。

C++的static成员变量的生命期伴随着整个程序，在程序关闭时由编辑器负责销毁。于是我们可以创建一个static嵌套类，在其析构函数中释放singleton对象。

具体的实现如下：
```cpp
class singleton
{
public:
	static singleton& instance()
	{
		if(!pInstance_)
		{
			destroy_.create();
			std::cout << "create\n";
			pInstance_ = new singleton;
		}
		return *pInstance_;
	}

private:
	static singleton* pInstance_;

	// embedded class
	// implicit deconstructor
	struct singleton_destroyer {
		~singleton_destroyer() {
			if(pInstance_) {
				std::cout << "singleton destroyed!\n";
				delete pInstance_;
			}
		}
		void create() {}
	};
	static singleton_destroyer destroy_;

protected:
	singleton() = default;
	virtual ~singleton() {}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};

singleton* singleton::pInstance_ = nullptr;
singleton::singleton_destroyer singleton::destroy_;
```

此时singleton将能够具有摧毁功能，此时再加入线程处理也并非难事，完整例子见7.5节。

## 7.3 Meyers singleton
隐式析构的Singleton的确是一种不错的实现方式，然而C++实现Singleton最简单而有效的方法就是Meyers singleton，实现如下：
```cpp
class singleton
{
public:
	static singleton& instance() {
		static singleton obj;
		return obj;	
	}

private:
	Singleton();
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	~Singleton();
};
```

此法并未使用动态分配和静态指针，而是借用了一个局部静态变量。

和上一个方法一样，该法也是到第一次执行时都会初始化对象（网上有些地方称之为懒汉式，然而在正统C++设计模式相关书籍中，皆未出现过此种叫法），它返回的是引用，所以用户无法对返回的对象进行delete操作（暴力转换法不算：））。此外，在C++11之后，这种方法也是线程安全的，所以对于大多情况下，这是最简单也是最有效的实作法，仅有两行代码。

编译器会负责管理局部静态变量，当程序结束时进行析构，看样子一切安好。然而，当生成的多个Singleton对象具有依赖关系时，Meyers singleton就无能为力了（隐式析构Singleton亦是如此）。

假设我们有四个Singleton，Director（导演）、Scene（场景）、Layer（图层）、Log（日志）。我们处于一个简单的游戏中，该游戏只有一个导演，一个场景，一个图层，图层上会有若干精灵。我们依次创建，到Layer时初始化失败了，此时创建一个Log来记录崩溃原因。之后程序关闭，执行期的相关机制会来摧毁静态对象，摧毁的顺序是LIFO，所以会先摧毁Log，然后是Layer。但若是Scene关闭失败，此时再想向Log记录崩溃原因，由于Log已被摧毁，所以返回的用只是一个“空壳”，之后的操作便会发生不确定性行为，这种问题称为dead-reference。

dead-reference问题的原因在于C++并不保证静态对象析构的顺序，因此具有依赖关系的多个Singleton无法正确删除。

使用局部静态变量的另一个缺点是难以通过派生子类来扩展Singleton，因为instance创建的始终都是singleton类型的对象。不过可以通过泛型编程来解决，见7.5节。


## 7.4 可以复活的Singleton
要满足具有依赖关系的Singleton，其中一个思路是当需要再次用到已经被执行期处理机制删除的Singleton对象时，使其死灰复燃，复活的Singleton需要自己负责删除。

所以首先需要增加一个标志来记录singleton是否已被摧毁，若已摧毁，则使其复生；若未摧毁，则创建Singleton对象。

大体上的代码如下：
```cpp
class singleton
{
public:
	static singleton& instance()
	{
		if(!pInstance_)
		{
			if(destroyed_)
				on_dead_reference(); // 出现dead-reference，重生singleton
			else
				create(); // 创建singleton
		}
		return *pInstance_;
	}
private:
	static singleton *pInstance_;
	static bool destroyed_; // 记录是否已被摧毁
};
```

那么具体就是要看on_dead_reference和create这两个函数是如何处理的，create和之前一样，可以直接使用Meyers singleton：
```cpp
static void create()
{
	static singleton obj;
	pInstance = &obj;
}
```

当singleton对象析构时，也就是摧毁实例时，改变destroyed_标记（默认为false），将其改为摧毁状态：
```cpp
virtual ~singleton()
{
	pInstance = nullptr;
	destroyed_ = true;
}
```

而如何让对象死灰复燃呢？我们可以借助placement new，在对象死去的“空壳”上重新创建对象。

```cpp
void on_dead_reference()
{
	create();
	new(pInstance_) singleton;
	std::atexit(kill_singleton);
	destroyed_ = false;
}
```

这里的关键在于atexit函数，该函数可以注册一些函数，注册的函数将会在程序结束之时被调用。事实上static成员变量的默认释放也是通过此函数完成的，编译器会自动生成释放函数向atexit注册，释放机制是LIFO。

此时我们是通过placement new重新构造了singleton对象，所以需要自己释放，因此定义一个kill_singleton函数：
```cpp
static void kill_singleton()
{
	pInstance_->~singleton();
}
```

只需调用析构函数释放便可。

## 7.5 okdp选择的Singleton
okdp是一个modern c++ design pattern 库，伴随本教程而写，github地址为：https://github.com/gxkey/okdp。

使用该库，你无需再为自己的类定义设计模式，只要直接引用该库，简单的两行代码，就能使你的类天生具有对应设计模式的结构和属性，具体用法见7.6节。

我是在okdp的singleton模块实现完成之后才写的本教程，而本篇介绍了诸多方法，封装到库中应该选择哪一种呢？

对于大多数情况下来说，Singleton类并不存在依赖关系，所以不应该直接使用最后重生的方式，因为效率并没有前面的高。而有时又有此种需求，所以可以提供两种方式，一种方式是普通的方式，Singleton的生命期短，摧毁了就无法再用；另一种是重生的方式，Singleton的生命期长，可以处理依赖类Singleton。

至此，倒应该对以上方式进行抽象化，因为此时这些方式只是针对具体类型而言，而用户使用时的类型是未知而多变的，我们使用抽象化工具泛型编程来完成此需求。

okdp选择了Meyers singleton作为普通方式，因为其简单而高效，且天生线程安全。此处便以其为例：
```cpp
template<typename T, bool LongLifeTime = false> class singleton;
```

T是对Singleton类型的抽象，LongLifeTime作为生命期选项，默认为false，即普通版本。

接着来具体实现：
```cpp
template<typename T>
class singleton<T, false>
{
public:

	static T& instance() {
		static T obj;
		return obj;	
	}

protected:
	singleton() = default;
	virtual ~singleton() {}

private:
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
};
```

这里的类型抽象为了T，所以instance函数返回也应该是T类型的对象。

而要想使用支持依赖的长生命期Singleton，便要自己指定true打开，这里只带大家看instance()的实现，因为这里要处理多线程问题：

```cpp
static T& instance() 
{
	if(!pInstance_)
	{
		// DCL
		utils::lock_guard lock;
		if(!pInstance_)
		{
			destroyed_ ? on_dead_reference() : create();
		}
	}

	return *pInstance_;
}
```
多线程问题已经是老生常谈了，我们之前在并发编程中介绍的那些方法都能使用，但更加轻便高效的还是之前给大家讲过的Memory Model，这里用的lock_guard只是包覆了一个spin_lock，而spin_lock是用Memory model实现的，具体可参考okdp的utils/lock.hpp。

这是还使用了DCL（Double-Checked Locking）双检测技术，这也是为了效率。


## 7.6 使用okdp为类型添加Singleton
要使用okdp，首先要从https://github.com/gxkey/okdp clone到本地。

不过需要注意：由于我最近较忙，这个库还未进行充分测试，比如singleton，如果更改记录不是singleton last commit，那么就表达可能还有issues，我会抽空测试调整，大家如果遇到任何问题，也欢迎提issue。

回归正题，clone到本地后，可以直接使用okdp，若你想让你的类天生具有singleton属性，无需自己编写singleotn，只需简单几行代码：
```cpp
#include "singleton.hpp"
#include <iostream>


class Log : public okdp::singleton<Log>
{
public:
	void print() { std::cout << "log print" << std::endl; }
};


int main()
{
	Log::instance().print();
	Log::instance().print();
	Log::instance().print();

	return 0;
}

```

只要让你的类继承自okdp::singleton，并传入类名，之后这个类便天生是个Singleton类。

示例程序将会如下输出：
```
log print
log print
log print
```

这里是默认的普通方式，若你的类具有依赖性质，那么只需加个true，okdp::singleton<Log, true>，之后类便具有重生Singleton的属性。

限于篇幅，此篇至此便要结束，如还有任何问题，可在本文下方留言，或到github：https://github.com/gxkey/okdp提问。
