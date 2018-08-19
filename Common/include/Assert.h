/*
 * Assert.h
 *
 *  Created on: Apr 14, 2018
 *      Author: famez
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#define ASSERT(condition)		if(!(condition)) { fprintf(stderr, "Assertion failed in condition " #condition ", file %s, line %d\n", __FILE__, __LINE__); exit(-1); }

#endif /* ASSERT_H_ */
