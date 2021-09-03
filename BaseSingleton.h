//=============================================================================
//
// シングルトン用デザインパターン [BaseSingleton.h]
// Author : 
//
//=============================================================================
#ifndef _BASESINGLETON_H_
#define _BASESINGLETON_H_

//*****************************************************************************
// クラス定義
//*****************************************************************************
template<class T>
class BaseSingleton
{
public:
	static T* Instance()
	{
		static T mInstance;

		return &mInstance;
	}

protected:
	BaseSingleton() {};
	virtual ~BaseSingleton() {};

	BaseSingleton(const BaseSingleton &) = delete;
	BaseSingleton(BaseSingleton &&) = delete;

	BaseSingleton& operator=(const BaseSingleton &) = delete;
	BaseSingleton& operator=(BaseSingleton &&) = delete;
};

#endif
