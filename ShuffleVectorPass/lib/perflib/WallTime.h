/*
    WallTime.h - Binary Order of Magnitude Execution Time Profiler 
    Copyright (C) 2010  Robert D. Cameron and Dan Lin
    Version 0.4
    Licensed to the general public under Academic Free License version 3.0

Wall_Profiler provides a lightweight multiplatform execution time profiling
utility based on processor cycle counters.  It uses a binary logarithmic
histogram technique that is useful in both highlighting patterns of 
cycle time distributions as well as identifing outliers in timing events
due to interruptions for operating system services.

In essence, Wall profiler is designed to collect statistics over a number
of repetitions of particular timed events.  Statistics are gathered in the
form of a logarithmic histogram of cycle times for processing a fixed number 
of elements between calls to start_Wall_interval and end_Wall_interval.  
For example, an interval may correspond to processing 1024 single-byte
elements by a particular routine.

A timer t is created and initialized by a call to t = init_Wall_timer().  
Given a timer t, start_Wall_interval(t) initiates an interval measurement 
which completes with end_Wall_interval(t, n), where n is the number of 
elements processed.  

dump_Timer_Table(t) prints out a rudimentary histogram of the recorded
intervals for a particular timer.  

Although the basic concept of Wall_Profiler is architecture independent,
processor-dependent routines for accessing time-stamp counters and
determining the binary order of magnitude are included through external
files.


*/

#ifndef WallTimer_H
#define WallTimer_H

#include <sys/time.h>
#include "i386_timer.h"

#define BIT_COUNT 64
#define MAX_TIMER_ENTRIES (1<<18)
#define TIMER_SCALE_FACTOR 1000

#define timestamp_t uint64_t

struct Timer_Table {
  // current timing interval information
  int timer_on;
  int full;
  struct timeval interval_start[MAX_TIMER_ENTRIES];
  struct timeval interval_end[MAX_TIMER_ENTRIES];
  unsigned int interval_elems[MAX_TIMER_ENTRIES];  
  unsigned int current_entry;
};

typedef struct Timer_Table Timer_Table;

static inline Timer_Table * init_Wall_timer () {
  Timer_Table * timer_table = (Timer_Table *) malloc(sizeof(Timer_Table));
  if (!timer_table) {
    printf("Couldn't initialize Wall timer.\n");
    exit(-1);
  }
  timer_table -> current_entry = 0;
  timer_table -> full = 0;
  timer_table -> timer_on = 0;
  return timer_table;
}


static inline void start_Wall_interval(Timer_Table * timer_table) {
  timer_table->timer_on = 1;
  gettimeofday(&timer_table->interval_start[timer_table->current_entry], NULL);
}

static inline void end_Wall_interval(Timer_Table * timer_table, unsigned int elems) {
  if (timer_table->timer_on) {
    gettimeofday(&timer_table->interval_end[timer_table->current_entry], NULL);
    timer_table->interval_elems[timer_table->current_entry] = elems;
    timer_table->current_entry++;
    if(timer_table->current_entry >= MAX_TIMER_ENTRIES) {
      timer_table->full=1;
      timer_table->current_entry=0;
    }
    timer_table->timer_on = 0;
  }
  else
  	fprintf(stderr,"Time interval end without a start!\n"); 
}



static void dump_Timer_Table(Timer_Table * timer_table) {
  // an array of counts and timings per binary order of magnitude
  int Wall_count[BIT_COUNT];
  unsigned int Wall_elems[BIT_COUNT];  
  timestamp_t Wall_total_time[BIT_COUNT];
  int i, BOM, b;
  int this_count;
  int cum_count = 0;
  unsigned int entry = 0;
  unsigned int total_entries;
  unsigned int this_elems = 0;
  unsigned int cum_elems = 0;
  timestamp_t this_time;
  timestamp_t cum_time = 0ULL;
  timestamp_t this_avg;
  timestamp_t cum_avg;
  timestamp_t accumulated_usec;  


  for (b = 0; b < BIT_COUNT; b++) {
	Wall_count[b] = 0;
	Wall_elems[b] = 0;
	Wall_total_time[b] = 0;
  }


  total_entries = timer_table->full ? MAX_TIMER_ENTRIES : timer_table->current_entry;
  for(entry = 0; entry < total_entries; entry++){
  	accumulated_usec = 1000000 * (timer_table->interval_end[entry].tv_sec - 
                                        (timer_table->interval_start[entry]).tv_sec) +
                             (timer_table->interval_end[entry].tv_usec - timer_table->interval_start[entry].tv_usec);
  	if (accumulated_usec > 0ULL) {
//	  cout << "accumulated_usec =" << accumulated_usec << "; elems = " << timer_table->interval_elems[entry] << endl;
	    BOM = binary_order_of_magnitude(accumulated_usec*TIMER_SCALE_FACTOR/timer_table->interval_elems[entry]);
	    Wall_count[BOM]++;
	    Wall_elems[BOM] += timer_table->interval_elems[entry];
	    Wall_total_time[BOM] += accumulated_usec;
	}
  }
  
  printf("Binary Order of Magnitude Profile\n");
  for (b = 0; b < BIT_COUNT; b++) {
    this_count = Wall_count[b];
    cum_count += this_count;
    this_time = Wall_total_time[b];
    cum_time += this_time;
    this_elems = Wall_elems[b];
    cum_elems += this_elems;
    if (this_count == 0) this_avg = 0ULL;
    else this_avg = (TIMER_SCALE_FACTOR*this_time)/(this_elems);
    if (cum_count == 0) cum_avg = 0ULL;
    else cum_avg = (TIMER_SCALE_FACTOR*cum_time)/(cum_elems);
    if (this_count > 0) {  // Only report intervals with nonzero counts.
      printf("BOM %i: %i ", b, this_count);
      printf("(avg time: %i microsec/kElem) ", (int) this_avg);
      printf("Cumulative: %i ", cum_count);
      printf("(avg: %i microsec/kElem)\n", (int) cum_avg);
    }
  }           
}

static inline void destroy_Wall_timer(Timer_Table * timer_table) {
	if(timer_table) {
		free(timer_table);
	}
	
}

#endif
