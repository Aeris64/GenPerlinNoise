#pragma once

#include <mutex>

template<typename T>
class Singleton
{
public:
	virtual ~Singleton()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

	static T* GetInstance()
	{
		std::call_once(m_onceFlag, []()
		{
			m_pInstance = new T;
		});
		return m_pInstance;
	}

	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton) = delete;
	Singleton& operator=(Singleton&&) = delete;

protected:
	Singleton() = default;

private:
	static T* m_pInstance;
	static std::once_flag m_onceFlag;
};

template<typename T>
T* Singleton<T>::m_pInstance = nullptr;

template<typename T>
std::once_flag Singleton<T>::m_onceFlag;
