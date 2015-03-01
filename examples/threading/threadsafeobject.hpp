#ifndef _DESTRUCT_THREADSAFEOBJECT_HPP_
#define _DESTRUCT_THREADSAFEOBJECT_HPP_

#include "dobject.hpp"
#include "dsimpleobject.hpp"
#include "dfunction.hpp"

#ifdef WIN32
#include <Windows.h>
//#include <WinBase.h>
  #define mutex_lock			EnterCriticalSection
  #define mutex_unlock			LeaveCriticalSection
  #define mutex_def(var)		CRITICAL_SECTION var
  #define mutex_init(var)  		InitializeCriticalSection(var)
  #define mutex_destroy(var)	DeleteCriticalSection(var)	
  #define cond_def(var)			CONDITION_VARIABLE var
  #define cond_init(var)        InitializeConditionVariable(var)
  #define cond_destroy(var)		
  #define cond_signal(var)		WakeConditionVariable(var)
  //#define cond_wait(cond, mut)  pthread_testcancel(); SleepConditionVariableCS(cond, mut, INFINITE)
  #define cond_wait(cond, mut)  SleepConditionVariableCS(cond, mut, INFINITE)
  #define cond_broadcast(var)	WakeAllConditionVariable(var)
#else
  #include <pthread.h>
  #define mutex_lock 			pthread_mutex_lock
  #define mutex_unlock 			pthread_mutex_unlock
  #define mutex_def(var)		pthread_mutex_t	var	
  #define mutex_init(var)		pthread_mutex_init(var, NULL)
  #define mutex_destroy(var)	pthread_mutex_destroy(var)
  #define cond_def(var)			pthread_cond_t var
  #define cond_init(var)        pthread_cond_init(var, NUL)
  #define cond_destroy(var)		pthread_cond_destroy(var)
  #define cond_signal(var)		pthread_cond_signal(var)
  #define cond_wait(cond, mut)  pthread_cond_wait(cond, mut)
  #define cond_broadcast		pthread_cond_broadcast(var)
#endif

/*


static int pthread_cond_destroy(pthread_cond_t *c)
{
	(void) c;
	return 0;
}

static int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, struct timespec *t)
{
	unsigned long long tm = _pthread_rel_time_in_ms(t);
	
	pthread_testcancel();
	
	if (!SleepConditionVariableCS(c, m, tm)) return ETIMEDOUT;
	

	if (!_pthread_rel_time_in_ms(t)) return ETIMEDOUT;
	
	return 0;
}
*/
namespace Destruct
{

class BaseValue;

//XXX ThreadSafeFunctionObject

//XXX RecursiveThreadSafeObject
//XXX RecursiveThreadSafeFunctionObject

////thread safe functin object ?
//class DebugFunctionObject : public DFunctionObject
//{
//public:
//DebugFunctionObject(DFunctionObject* functionObject); 
//~DebugFunctionObject();
//
//virtual DValue call(void) const;
//virtual DValue call(DValue const& args) const;
//private:
//DFunctionObject*      __functionObject;
//};

class ThreadSafeObject : public DObject 
{
public:
  ThreadSafeObject(DStruct* dstruct, DValue const& args);
  ThreadSafeObject(ThreadSafeObject const&);
  //ThreadSafeObject(DObject*); //untested

  static DObject*   newObject(DStruct *, DValue const& args);

  DObject*          clone() const;
  virtual DValue    getValue(size_t index) const;
  void              setValue(size_t idx, DValue const &);
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(DUnicodeString const& name) const;
  void              setValue(DUnicodeString  const& name, DValue const &);

  virtual DValue    call(DUnicodeString const& name, DValue const &);
  virtual DValue    call(DUnicodeString const& name);
protected:
  ~ThreadSafeObject();

  BaseValue*        getBaseValue(size_t index);
  BaseValue const*  getBaseValue(size_t index) const;
  DObject*          object(void) const;

  //addRef // delRef a locker ... ? a part si ds des func deja locker ?
  
private:
  DObject*          __dobject; //RealValue (refcount)
  mutex_def(__mutex);
};

}

#endif
