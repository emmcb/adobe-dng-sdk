#include "public/include/XMP_Environment.h"

#if XMP_MacBuild

	#include <Multiprocessing.h>
	#include <DriverServices.h>
	
	typedef MPTaskID XMP_ThreadID;
	typedef OSStatus XMP_ThreadResult;
	typedef XMP_ThreadResult (*XMP_ThreadProc) ( void * params );
	
	static XMP_ThreadID SpawnThread ( TaskProc proc, const void* param )
	{
		XMP_ThreadID taskID;
		OSStatus err = MPCreateTask ( proc, (void*)param, 32*1024, 0, 0, 0, 0, &taskID );
		if ( err != noErr ) throw std::logic_error ( "MPCreateTask failed" );
		return taskID;
	}
	
	static void KillThread ( XMP_ThreadID taskID )
	{
		OSStatus err = MPTerminateTask ( taskID, 0 );
		if ( err != noErr ) throw std::logic_error ( "MPTerminateTask failed" );
	}
	
	static void GoToSleep ( int millisecs )
	{
		AbsoluteTime wakeTime = AddDurationToAbsolute ( millisecs, UpTime() );
		OSStatus err = MPDelayUntil ( &wakeTime );
		if ( err != noErr ) throw std::logic_error ( "MPDelayUntil failed" );
	}

#elif XMP_WinBuild

	#include <Windows.h>
	
	typedef HANDLE XMP_ThreadID;
	typedef DWORD XMP_ThreadResult;
	typedef XMP_ThreadResult (*XMP_ThreadProc) ( LPVOID param );
	
	static XMP_ThreadID SpawnThread ( XMP_ThreadProc proc, const void* param )
	{
		return CreateThread ( 0, 32*1024, (LPTHREAD_START_ROUTINE)proc, (LPVOID)param, 0, 0 );
	}
	
	static void KillThread ( XMP_ThreadID taskID )
	{
		TerminateThread ( taskID, 0 );
	}
	
	static void GoToSleep ( int millisecs )
	{
		Sleep ( millisecs );
	}

#elif XMP_UNIXBuild | XMP_AndroidBuild

	#include <time.h>
	#include <string.h>
	#include <pthread.h>
	
	typedef pthread_t XMP_ThreadID;
	typedef void* XMP_ThreadResult;
	typedef XMP_ThreadResult (*XMP_ThreadProc) ( void * params );
	
	static XMP_ThreadID SpawnThread ( XMP_ThreadProc proc, const void* param )
	{
		XMP_ThreadID taskID;
		int err = pthread_create ( &taskID, 0, proc, (void*)param );
		if ( err != 0 ) throw std::logic_error ( "pthread_create failed" );
		return taskID;
	}
	
	static void KillThread ( XMP_ThreadID taskID )
	{
		int err = pthread_cancel ( taskID );
		if ( err != 0 ) throw std::logic_error ( "pthread_cancel failed" );
	}
	
	static void GoToSleep ( int millisecs )
	{
		struct timespec delay;
		memset ( &delay, 0, sizeof(delay) );
		delay.tv_sec = millisecs / 1000;
		delay.tv_nsec = (millisecs - delay.tv_sec*1000) * 1000*1000;
		int err = nanosleep ( &delay, 0 );
		if ( err != 0 ) throw std::logic_error ( "nanosleep failed" );
	}

#endif
