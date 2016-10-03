#ifndef __DESTRUCT_THREADING__
#define __DESTRUCT_THREADING__
#ifdef WIN32
#include <Windows.h>

  typedef HANDLE                ThreadStruct;
  typedef DWORD          ThreadResult;
  typedef LPVOID                ThreadData;
  #define createThread(function, data, threadStruct)\
  threadStruct = CreateThread(NULL, 0, function, data, 0, NULL);\
  if (threadStruct == NULL)\
    throw DException("Error creating thread");
  #define destroyThread(var)    CloseHandle(var)    
 
  typedef CRITICAL_SECTION      mutex;
  #define mutex_init(var)  	InitializeCriticalSection(var)
  #define mutex_destroy(var)	DeleteCriticalSection(var)	
  #define mutex_lock	        EnterCriticalSection
  #define mutex_unlock		LeaveCriticalSection

  #define cond_def(var)		CONDITION_VARIABLE var
  #define cond_init(var)        InitializeConditionVariable(var)
  #define cond_destroy(var)		
  #define cond_signal(var)      WakeConditionVariable(var)
  //#define cond_wait(cond, mut)  pthread_testcancel(); SleepConditionVariableCS(cond, mut, INFINITE)
  #define cond_wait(cond, mut)  SleepConditionVariableCS(cond, mut, INFINITE)
  #define cond_broadcast(var)	WakeAllConditionVariable(var)
#else
  #include <pthread.h>

  typedef pthread_t             ThreadStruct;
  typedef void*                 ThreadResult;
  typedef void*                 ThreadData;
  #define createThread(function, data, threadStruct)\
  int result = pthread_create(&threadStruct, NULL, function, data);\
  if (result)\
    throw DException("Error creating thread");
  #define destroyThread(var)

  typedef pthread_mutex_t       mutex;
  #define mutex_init(var)	pthread_mutex_init(var, NULL)
  #define mutex_destroy(var)	pthread_mutex_destroy(var)
  #define mutex_lock 	        pthread_mutex_lock
  #define mutex_unlock 		pthread_mutex_unlock

  #define cond_def(var)		pthread_cond_t var
  #define cond_init(var)        pthread_cond_init(var, NULL)
  #define cond_destroy(var)	pthread_cond_destroy(var)
  #define cond_signal(var)	pthread_cond_signal(var)
  #define cond_wait(cond, mut)  pthread_cond_wait(cond, mut)
  #define cond_broadcast(var)	pthread_cond_broadcast(var)
#endif
#endif
