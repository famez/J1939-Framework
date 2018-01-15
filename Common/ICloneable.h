/*
 * ICloneable.h
 *
 *  Created on: Oct 10, 2017
 *      Author: famez
 */

#ifndef ICLONEABLE_H_
#define ICLONEABLE_H_


#define IMPLEMENT_CLONEABLE(CLASS, SUBCLASS)		\
	virtual CLASS* clone() const {								\
		return new SUBCLASS(*this);				\
	}

template<class T>
class ICloneable {
public:
	ICloneable() {}
	virtual ~ICloneable() {}

	virtual T* clone() const = 0;

};

#endif /* ICLONEABLE_H_ */
