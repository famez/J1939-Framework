/*
 * Singleton.h
 *
 *  Created on: Mar 15, 2016
 *      Author: famez
 *      Interfaz para crear una clase singleton.
 *      Incluir siempre al inicio de la definición de la clase la macro SINGLETON_ACCESS
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <stdio.h>
#include <exception>
#include <typeinfo>


//#define SINGLETON_DEBUG

#define SINGLETON_ACCESS		friend class ISingleton;


template<class T>
class ISingleton {

protected:
	ISingleton() {}
	static T* mInstance;

public:
	virtual ~ISingleton(){}
	inline static T& getInstance()
	{
		if(!mInstance)
		{
			try{
				mInstance = new T();

			}catch (std::exception& e) {
#ifdef SINGLETON_DEBUG
				printf("[ISingleton] Could not allocate instance for class %s. Constructor not defined\n", typeid(T).name());
#endif
			}
#ifdef SINGLETON_DEBUG
			if(mInstance)	printf("[ISingleton] Instance created for class %s\n", typeid(T).name());
#endif
		}
		return *mInstance;
	}

	inline static T* getInstancePtr()
	{
		if(!mInstance)
		{
			try{
				mInstance = new T();

			}catch (std::exception& e) {
#ifdef SINGLETON_DEBUG
				printf("[ISingleton] Could not allocate instance for class %s. Constructor not defined\n", typeid(T).name());
#endif

			}
#ifdef SINGLETON_DEBUG
			if(mInstance)	printf("[ISingleton] Instance created for class %s\n", typeid(T).name());
#endif
		}
		return mInstance;
	}


	inline static void releaseInstance()
	{
		delete mInstance;
		mInstance = NULL;
#ifdef SINGLETON_DEBUG
			printf("[ISingleton] Instance released for class %s\n", typeid(T).name());
#endif
	}
};


template<class T>
T* ISingleton<T>::mInstance = NULL;


#endif /* SINGLETON_H_ */
