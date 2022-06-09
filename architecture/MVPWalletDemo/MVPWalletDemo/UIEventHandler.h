#pragma once
#include "Common.h"


template
<
	typename IdentifierType,
	typename CallbackType
>
class Delegate
{
public:
	using AssocMap = std::map<IdentifierType, CallbackType>;
public:

	template<typename T>
	bool Insert(T&& id, CallbackType callback)
	{
		return associations_.emplace(AssocMap::value_type(
			std::forward<T>(id), callback)).second;
	}

	template<typename T>
	bool Remove(T&& id)
	{
		return associations_.erase(std::forward<T>(id)) == 1;
	}

	void Clear()
	{
		associations_.clear();
	}

	typename AssocMap::const_iterator Begin()
	{
		return associations_.cbegin();
	}

	typename AssocMap::const_iterator End()
	{
		return associations_.cend();
	}

	template<typename T>
	bool Find(T&& id, typename AssocMap::const_iterator& iter)
	{
		iter = associations_.find(std::forward<T>(id));
		if (iter != associations_.end())
			return true;

		return false;
	}

private:
	AssocMap associations_;
};


class UIEventHandler
{
	using delegate_type = Delegate<CControlUI*, std::function<void(TNotifyUI*)>>;
public:

	~UIEventHandler()
	{
		for (auto i = delegate_.Begin(); i != delegate_.End(); )
		{
			delegate_.Remove(i->first);
			i = delegate_.Begin();
		}
	}

	void AddHandler(CControlUI* pSender, std::function<void(TNotifyUI*)> callback)
	{
		delegate_.Insert(pSender, callback);
	}

	void Dispatch(TNotifyUI* pObj)
	{
		delegate_type::AssocMap::const_iterator iter;
		bool ret = delegate_.Find(pObj->pSender, iter);
		if (ret)
		{
			iter->second(pObj);
		}
	}

private:
	delegate_type delegate_;
};
