#ifndef __COMMON__PLATFORM__H__
#define __COMMON__PLATFORM__H__


/**
 *
 *  作者: hm
 *
 *  说明: 平台信息
 *
 */


#define PLATFORM_LINUX		1
#define PLATFORM_APPLE		2
#define PLATFORM_WINDOWS	3


#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32 ) || defined( _WIN32_ ) || defined( __WIN32__ ) || \
	defined( WIN64 ) || defined( _WIN64 ) || defined( __WIN64 ) || defined( _WIN64_ ) || defined( __WIN64__ )
#
#  define PLATFORM_TYPE PLATFORM_WINDOWS
#
#elif defined( APPLE ) || defined( __APPLE ) || defined( __APPLE__ )
#
#  define PLATFORM_TYPE PLATFORM_APPLE
#
#elif defined( __linux__ )
#
#  define PLATFORM_TYPE PLATFORM_LINUX
#
#else
#
#  error "platform not supported"
#
#endif


#endif // __COMMON__PLATFORM__H__
