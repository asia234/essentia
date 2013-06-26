/* 
 * Copyright (C) 2006-2013  Music Technology Group - Universitat Pompeu Fabra
 *
 * This file is part of Essentia
 * 
 * Essentia is free software: you can redistribute it and/or modify it under 
 * the terms of the GNU Affero General Public License as published by the Free 
 * Software Foundation (FSF), either version 3 of the License, or (at your 
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
 * details.
 * 
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 */

#ifndef ESSENTIA_ATOMIC_H
#define ESSENTIA_ATOMIC_H



#ifdef OS_WIN32

#include <windows.h>

//
// Atomic for OS_WIN32
//

namespace essentia {

class Atomic
{
private:
	LONG volatile i_;
public:
	inline Atomic(const int &i = 0) : i_(i)
	{
	}

	inline operator int () const
	{
		return i_;
	}

	inline void operator -=(const int &i)
	{
		InterlockedExchangeAdd(&i_, -i);
	}

	inline void operator +=(const int &i)
	{
		InterlockedExchangeAdd(&i_, i);
	}

	inline void operator ++()
	{
		InterlockedIncrement(&i_);
	}

	inline void operator --()
	{
		InterlockedDecrement(&i_);
	}
};

} // namespace essentia

#else // OS_WIN32

//
// Atomic for OS_MAC and OS_LINUX:
//

#if GCC_VERSION >= 40200
   // atomicity.h has moved to ext/ since g++ 4.2
  #if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ <=1050) // not on osx 10.5
  #  include <bits/atomicity.h>
  #else
  #  include <ext/atomicity.h>
  #endif
#else
#  include <bits/atomicity.h>
#endif

namespace essentia {

class Atomic
{
public:
	_Atomic_word _a;

	inline Atomic(const int &i = 0)
	:_a(i)
	{
	}

	inline operator int () const
	{
		return _a;
	}

	inline void add(const int& i)
	{
// not sure 4.0 is the correct version, it happened somewhere between 3.3 and 4.1
#if GCC_VERSION >= 40000
		__gnu_cxx::__atomic_add(&_a,i);
#else
                __atomic_add(&_a, i);
#endif
	}

	inline void operator -=(const int &i)
	{
		add(-i);
	}

	inline void operator +=(const int &i)
	{
		add(i);
	}

	inline void operator ++()
	{
		add(1);
	}

	inline void operator --()
	{
		add(-1);
	}
};

} // namespace essentia

#endif // OS_WIN32

#endif // ESSENTIA_ATOMIC_H