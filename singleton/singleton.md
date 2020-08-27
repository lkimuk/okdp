# 7 C++ DP.07 Singleton
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
	Singleton() = delete;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	~Singleton() = delete;

	static Singleton* Instance() {
		if(!pInstance_) {
			pInstance_ = new Singleton;
		}
		return pInstance_;
	}
private:
	static Singleton* pInstance_;
};

Singleton* Singleton::pInstance_ = 0;
```

这种方式的确满足了唯一性，用户除了从Instance()获取对象之外，别无他法。且用户无法意外删除对象，因为析构函数也被delete了（不过依然可以由返回的指针删除之，所以最好以引用返回）。

最大的问题在于此法只满足三大关键之一：创建，对于删除和多线程都不满足。

所以，在满足唯一性的前提下，Singleton的实现还应设法管理产生对象的生命期和多线程环境下的访问安全。

## 7.2 适用于多数情况的Singleton
C++实现Singleton最简单而有效的方法就是Meyers singleton，实现如下：
```cpp
class singleton
{
public:
	singleton() = delete;
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
	~singleton() = delete;

	static singleton& instance() {
		static singleton obj;
		return obj;	
	}
};
```

此法并未使用动态分配和静态指针，而是借用了一个局部静态变量。

和上一个方法一样，该法也是到第一次执行时都会初始化对象（网上有些地方称之为懒汉式，然而在正统C++设计模式相关书籍中，皆未出现过此叫法），它返回的是引用，所以用户无法对返回的对象进行delete操作（暴力转换法不算：））。此外，在C++11之后，这种方法也是线程安全的，所以对于大多情况下，这是最简单也是最有效的实作法，仅有两行代码。

编译器会负责管理局部静态变量，当程序结束时进行析构，看样子一切安好。然而，当生成的多个Singleton对象具有依赖关系时，Meyers singleton就无能为力了。

假设我们有四个Singleton，Director（导演）、Scene（场景）、Layer（图层）、Log（日志）。我们处于一个简单的游戏中，该游戏只有一个导演，一个场景，一个图层，图层上会有若干精灵。我们依次创建，到Layer时初始化失败了，此时创建一个Log来记录崩溃原因。之后程序关闭，执行期的相关机制会来摧毁静态对象，摧毁的顺序是LIFO，所以会先摧毁Log，然后是Layer。但若是Scene关闭失败，此时再想向Log记录崩溃原因，由于Log已被摧毁，所以返回的用只是一个“空壳”，之后的操作便会发生不确定性行为，这种问题称为dead-reference。

dead-reference问题的原因在于C++并不保证静态对象析构的顺序，因此具有依赖关系的多个Singleton无法正确删除。

使用局部静态变量的另一个缺点是难以通过派生子类来扩展Singleton，因为instance创建的始终都是singleton类型的对象。

## 7.3 隐式析构的Singleton

## 7.4 不死的Singleton

## 7.5 带顺序的Singleton

## 7.6 okdp选择的Singleton

## 7.7 使用okdp为类型添加Singleton
