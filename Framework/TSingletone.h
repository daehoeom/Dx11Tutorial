#pragma once

#include "AlignedAllocationPolicy.h"
template < typename classname >
class TemplateSingleton : public AlignedAllocationPolicy<16>
{
protected:
	TemplateSingleton() { }
	virtual ~TemplateSingleton() { }

public:
	static classname* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new classname;

		return m_pInstance;
	};

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	};

private:
	static classname* m_pInstance;
};

template <typename classname> classname* TemplateSingleton<classname>::m_pInstance = nullptr;