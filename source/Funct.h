/*
 * Funct.h
 *
 *  Created on: 2010/01/27
 *      Author: Chase
 */

#ifndef FUNCT_H_
#define FUNCT_H_

class Funct {
public:
	virtual void execute() = 0;
	virtual ~Funct() = 0;
};

// defined even though it's pure virtual; it's faster this way; trust me
inline Funct::~Funct() { }

typedef Funct* FunctPtr;

class FunctKey {
public:
	virtual void execute(const char*) = 0;
	virtual ~FunctKey() = 0;
};

// defined even though it's pure virtual; it's faster this way; trust me
inline FunctKey::~FunctKey() { }

typedef FunctKey* FunctKeyPtr;

class FunctNum {
public:
	virtual void execute(int32) = 0;
	virtual ~FunctNum() = 0;
};

// defined even though it's pure virtual; it's faster this way; trust me
inline FunctNum::~FunctNum() { }

typedef FunctNum* FunctNumPtr;

#endif /* FUNCT_H_ */
