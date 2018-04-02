#ifndef PERFSEC_H
#define PERFSEC_H

#if defined(WALL_TIMING)
#include "WallTime.h"
#elif defined(BUFFER_PROFILING)
#include "BOM_Profiler.h"
#elif defined(PAPI)
	#include <cc.h>
//	#include <cc.cxx>
#endif

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#if defined __linux__

  #include <sched.h>

  /* mask - Bit mask. 

     mask = 2^0 = 1 core 1 
     mask = 2^1 = 2 core 2 
     ...
     mask = 2^7 = 128 core 8

     mask = (2^0 | 2^1) = 3 core 1 or core 2 
  */ 

  static inline int set_processor_affinity(unsigned long mask) { 


    unsigned int len = sizeof(mask);

    if (sched_setaffinity(0, len, (const cpu_set_t *)&mask) < 0) {
      printf("sched_setaffinity call failed.\n");
      return -1;
    }

    //printf("Set CPU affinity mask: %08lx\n", mask);

    return 0;
  }

#elif defined _MSC_VER

  #include <process.h>
  #include <Windows.h>
  #define getpid _getpid

  static inline int set_processor_affinity(DWORD_PTR mask) {

    HANDLE hProcess  = OpenProcess(PROCESS_ALL_ACCESS, 0, getpid());

	if(0 == SetProcessAffinityMask(hProcess, mask)) {
              printf("SetProcessAffinityMask call failed.\n");
	      return -1;
	}

    	//printf("CPU affinity mask: %08lx\n", mask);

	return 0;
  }

#else

  inline int set_processor_affinity(unsigned long mask) {
	printf("set_processor_affinity is not implemented on the current OS.\n");
	return -1;
  }

#endif

#define PERF_SEC_BIND(mask) \
	set_processor_affinity(mask);

#if defined(WALL_TIMING)

	#define PERF_SEC_INIT(timer) \
		timer = init_Wall_timer()
	
	#define PERF_SEC_ADD_PARAM(timer, name, value) 

	#define PERF_SEC_START(timer) \
		start_Wall_interval(timer) 
	
	#define PERF_SEC_END(timer,elems) \
		end_Wall_interval(timer,elems) 
	
	#define PERF_SEC_DUMP(timer) \
		dump_Timer_Table(timer)

	#define PERF_SEC_WRITE(timer) 
	
	#define PERF_SEC_DESTROY(timer) \
		destroy_Wall_timer(timer)	
	
#elif defined(BUFFER_PROFILING)

	#define PERF_SEC_INIT(timer) \
		timer = init_BOM_timer()

	#define PERF_SEC_ADD_PARAM(timer, name, value) 
	
	#define PERF_SEC_START(timer) \
		start_BOM_interval(timer) 
	
	#define PERF_SEC_END(timer,elems) \
		end_BOM_interval(timer,elems) 
	
	#define PERF_SEC_DUMP(timer) \
		dump_BOM_table(timer)
	
	#define PERF_SEC_WRITE(timer) 

	#define PERF_SEC_DESTROY(timer) \
		destroy_BOM_timer(timer)	
	
#elif defined(PAPI)
/*  Quick and dirty hack to gather multiple events. - Oct 17. 2012.

	//#PAPI_L1_DCM  
	#if (0x80000000 == CC_EVENT) 
				int Events[1] = {PAPI_L1_DCM};	
	//#PAPI_L1_ICM  
	#elif (0x80000001 == CC_EVENT)
				int Events[1] = {PAPI_L1_ICM};
	//#PAPI_L2_DCM  
	#elif (0x80000002 == CC_EVENT)
				int Events[1] = {PAPI_L2_DCM};
	//#PAPI_L2_ICM  
	#elif (0x80000003 == CC_EVENT)
				int Events[1] = {PAPI_L2_ICM};
	//#PAPI_L3_DCM  
	#elif (0x80000004 == CC_EVENT)
				int Events[1] = {PAPI_L3_DCM};
	//#PAPI_L3_ICM  
	#elif (0x80000005 == CC_EVENT)
				int Events[1] = {PAPI_L3_ICM};
	//#PAPI_BR_CN   
	#elif (0x8000002b == CC_EVENT)
				int Events[1] = {PAPI_BR_CN};
	//#PAPI_BR_MSP  
	#elif (0x8000002e == CC_EVENT)
				int Events[1] = {PAPI_BR_MSP};
	//#PAPI_TOT_INS  
	#elif (0x80000032 == CC_EVENT)
				int Events[1] = {PAPI_TOT_INS};
	//#PAPI_TOT_CYC   
	#elif (0x8000003b == CC_EVENT)
				int Events[1] = {PAPI_TOT_CYC };
	#else
				#warning CC_EVENT not defined in Makefile. Default to PAPI_TOT_CYC.
				int Events[1] = {PAPI_TOT_CYC };
	#endif
*/
	#define PERF_SEC_INIT(timer) \
		timer = new CC(PAPI_EVENTS, PAPI_EVENTS_COUNT, 0)
		// timer = new CC(Events,1,0,false) \

	#define PERF_SEC_ADD_PARAM(timer, name, value) \
		timer->set_param(name, value) \

	#define PERF_SEC_START(timer) \
		timer->start_interval() \

	#define PERF_SEC_END(timer,elems) \
		timer->end_interval(elems)
	
	#define PERF_SEC_DUMP(timer) \
		timer->display_system_info();\
		timer->display_raw_event_data();\
		timer->dump_avg_kelem() \

	#define PERF_SEC_WRITE(timer) \
		timer->write_csv_file()
		// timer->write_xml_file() 
		
	#define PERF_SEC_DESTROY(timer) \
		if(timer) delete timer
		
#else

	#define PERF_SEC_INIT(timer)
	#define PERF_SEC_START(timer)
	#define PERF_SEC_ADD_PARAM(timer, name, value) 
	#define PERF_SEC_END(timer,elems)
	#define PERF_SEC_DUMP(timer)
	#define PERF_SEC_DESTROY(timer)
	#define PERF_SEC_WRITE(timer) 

#endif
#endif

