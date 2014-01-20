#ifdef __LOCALINFO_INTERNAL__
	#define LOCALINFO_EXTERN 
#else
	#define LOCALINFO_EXTERN extern
#endif

#include <pthread.h>   /* needs to be before stdio.h */
#include "intdefs.h"
#include <sys/time.h>

/*
*   global info  
*/

LOCALINFO_EXTERN pthread_mutex_t *GlobalMutPtr;      // pointer to the global mutex

typedef struct LocalInfo_s {
  int4 istatus;
  char *status;
  int4 icomstatus;
  char *comstatus;
  int4 debug;
  char *tablename;                      // the table name
//  int4 portno;                         // the local server port number//this goes to global
  time_t startime;                     // time when the thread started or when calbox was switched on
  time_t writetime;                    // time when last report was sent
//  int4 statedt;                  // status thresholds: 3 OK, 9 unavailable; It is also used in CalibInfo for laser: 1 firing, 2 error, 0 off
//  int4 statetd;                  // status delays: 3 OK, IPRC on: 4, 9 unavailable
//  int4 statetw;                  // status width: 3 OK, 9 unavailable
//  int4 fullreport;               // yes or no, needed for SA communication
//  int2 monitor;                  // 1: monitor, 0: do not monitor
  int2 varlocked;            // lock access to variables
//  bool cardlock;         // locker for relay card  
//  bool wheellock;         // locker for both filter wheels  
//  bool PIDlock;         // locker for PID controller
//  bool laserlock;         // locker for laser controller  
//  bool sensorlock;         // locker for temp./ hum. sensor  
  pthread_mutex_t cardMutex;  //mutex for relay card
  pthread_mutex_t wheelMutex;  //mutex for both filter wheels
  pthread_mutex_t PIDMutex;  //mutex PID controller
  pthread_mutex_t laserMutex;  //mutex for laser controller
  pthread_mutex_t sensorMutex;  //mutex for temp./ hum. sensor
} LocalInfo_t;

//  create structures to store different status
//LOCALINFO_EXTERN  LocalInfo_t CalibInfo;   // calibration unit
LOCALINFO_EXTERN  LocalInfo_t DummyInfo;   // trigger L3 unit

LOCALINFO_EXTERN enum {
  DUMMY_STATUS_ERROR=0,
  DUMMY_STATUS_READY,
  DUMMY_STATUS_LOADING,  
  DUMMY_STATUS_UNKNOWN};

LOCALINFO_EXTERN enum {
  DUMMY_COMSTATUS_NOTREADY=0,
  DUMMY_COMSTATUS_READY,
  DUMMY_COMSTATUS_NOTRECVD};

