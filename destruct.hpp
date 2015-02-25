#ifndef __DESTRUCT_EXPORT_HPP__
#define __DESTRUCT_EXPORT_HPP__

#ifndef WIN32

#ifndef IMPORT
#define IMPORT	extern
#endif

#define EXPORT
#else

#ifndef EXPORT
#define EXPORT	 __declspec( dllexport )
#endif
#ifndef IMPORT
#define IMPORT 	__declspec(dllimport)
#endif
//#include "windows.h"
#endif

#endif
