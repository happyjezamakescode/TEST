/*******************************************************************
 * Sum-Trigger Info 
 * date: september 2013
 * by: jrg
 ********************************************************************/

    #define CB_N 18               // Number of Clip boards
    #define DM_N 32               // Number of Delay Modules
    #define MC_N 55               // Number of Macrocels
    #define FN_N 3                // Number of FANOUT
    #define PX_N 558              // Number of PIXELS
/*
 * Clip Board Status
*/
    typedef struct Clip_struct 
    {
        float clip[DM_N]; // clipping channels (mv)
        float delay[DM_N]; // delay channels (V)
        float gain[DM_N]; // gain channels (dB)
        unsigned int fixd[DM_N];// fix delay on of
        unsigned int onoff[DM_N]; // switch on of
        unsigned long SerialN; // Board SN
        int TCPLD; // CPLD temperature C
        int TAMP; // Amplifiers Temperature C
    } Clip_t;

        Clip_t ClipData[CB_N];

/*
 * Sum Boards and Astro Board Status
*/
    typedef struct Astro_Sum_struct 
    {
        unsigned int ddelay[MC_N];    
        unsigned int dref[MC_N];    
        float rate[MC_N];
        float timming[MC_N];
        float amplitude[MC_N];
        float threshodl[MC_N];
        float edel1;
        float edel2;
        float edel3;
        int TAst;   
        unsigned int Tmax;
        unsigned int Cmax;
        unsigned short Astatus; //errors?

    }AS_t;

         AS_t ASData;

/*
 * Mapping
*/
    typedef struct mapping_struct 
    {
       //CAMERA
        unsigned int PIXEL; //CAMERA (Counting on hardware)
       
       //RECEIVER
        unsigned int RECEIVER_CONNECTOR; //RECEIVER BOARD: Connector
        unsigned int RECEIVER_CHANNEL; //RECEIVER BOARD: Channel # in connector
       
       //CLIP-BOARD 
        unsigned int CLIP_SLOT;  //CLIP-BOARD: Board position in crate (L to R)
        unsigned int CLIP_CONNECTOR;    //CLIP-BOARD: Front connector positio (U to D)
        char  CLIP_POSITIVE; //CLIP-BOARD: Pinout, Positive Signal
        char  CLIP_NEGATIVE; //CLIP-BOARD: Pinout, Negative Signal
        unsigned int CLIP_ERNI_REAR;  //CLIP-BOARD: Back view, rear connector position
       //SUM-BACKPLANE - (FANOUT 1)

        unsigned int S_CLIP_ERNI_FN1;//Connector # (U to D & L to R) ERNII
        unsigned int S_CLIP_CHANNELN_FN1; // Channel # in connector. ERNI 
        unsigned int S_CLIP_CONNECTORN_FN1;//Connector # in board (U to D) ERNI
        char  S_CLIP_POSITIVEN_FN1[3]; //CLIP-BOARD: Pinout, Positive Signal
        char  S_CLIP_NEGATIVEN_FN1[3]; //CLIP-BOARD: Pinout, Negative Signal
        unsigned int S_SUM_ERNI_FN1;//Connector # (U to D & L to R) ERNII
        char  S_SUM_POSITIVE_FN1[3]; // Pinout, Positive Signal
        char  S_SUM_NEGATIVE_FN1[3]; //Pinout, Negative Signal
        unsigned int S_CLIP_CONNECTOR_FN1;//Connector # in board (U to D) ERNI
        unsigned int S_SUM_CHANNEL_FN1; // Channel # iN macrocell (UL to DR) 
        
       //SUM-BACKPLANE - (FANOUT 2)
        unsigned int S_CLIP_ERNI_FN2;//Connector # (U to D & L to R) ERNII
        unsigned int S_CLIP_CHANNELN_FN2; // Channel # in connector. ERNI 
        unsigned int S_CLIP_CONNECTORN_FN2;//Connector # in board (U to D) ERNI
        char  S_CLIP_POSITIVEN_FN2[3]; //CLIP-BOARD: Pinout, Positive Signal
        char  S_CLIP_NEGATIVEN_FN2[3]; //CLIP-BOARD: Pinout, Negative Signal
        unsigned int S_SUM_ERNI_FN2;//Connector # (U to D & L to R) ERNII
        char  S_SUM_POSITIVE_FN2[3]; // Pinout, Positive Signal
        char  S_SUM_NEGATIVE_FN2[3]; //Pinout, Negative Signal
        unsigned int S_CLIP_CONNECTOR_FN2;//Connector # in board (U to D) ERNI
        unsigned int S_SUM_CHANNEL_FN2; // Channel # iN macrocell (UL to DR) 
        
       //SUM-BACKPLANE - (FANOUT 3)
        unsigned int S_CLIP_ERNI_FN3;//Connector # (U to D & L to R) ERNII
        unsigned int S_CLIP_CHANNELN_FN3; // Channel # in connector. ERNI 
        unsigned int S_CLIP_CONNECTORN_FN3;//Connector # in board (U to D) ERNI
        char  S_CLIP_POSITIVEN_FN3[3]; //CLIP-BOARD: Pinout, Positive Signal
        char  S_CLIP_NEGATIVEN_FN3[3]; //CLIP-BOARD: Pinout, Negative Signal
        unsigned int S_SUM_ERNI_FN3;//Connector # (U to D & L to R) ERNII
        char  S_SUM_POSITIVE_FN3[3]; // Pinout, Positive Signal
        char  S_SUM_NEGATIVE_FN3[3]; //Pinout, Negative Signal
        unsigned int S_CLIP_CONNECTOR_FN3;//Connector # in board (U to D) ERNI
        unsigned int S_SUM_CHANNEL_FN3; // Channel # iN macrocell (UL to DR) 
        
    } Mapping_t;

    Mapping_t MAP[PX_N];

/*
 *   Dummy info  
 */


    typedef struct Dummy_struct_s 
    {
      short channelvalue[100];
      short value;
    } Dummy_struct_t;

    Dummy_struct_t DummyData;

int dummyvalor;
int dummyvalor2;

/*
 * Define here the status structure to be used for communicating with CC
 */ 
    enum
    {
      NOERR=0, ERINFO, ERWARN, EREROR, ERSEVRi
    };  //was in vme.h

    typedef struct Global_s 
    {
      int4 debug;
      int4 istatus;
      char *status;
      int4 icomstatus;
      char *comstatus;
      int4 uselog;                         // the log flag
      char *hostname[2];                      // the host name
      int4 hostno[2];                         // the host port number
      int client_conn[2];                  //se usa en el report para los sokets?????
      char *logfile;                       // the log file name
      char *logpath;                       // the log file path
      char *tablepath;                     // the tables directory
      int4 portno;                         // the local server port number
      time_t startime;                     // time when the program started
      time_t lastcmdtime;                  // time when last command was received
      int lock_mode;
      int server_conn;
      int4 ncrates;                 // the number of connected crates through VME
      int4 nreccrates;              // the number of connected receiver crates
      int4 npulcrates;              // the number of connected pulsar crates
      pthread_mutex_t *mutptr;      // pointer to the global mutex
      int4 iprc;                    // IPR control ON and OFF 
      int2 cmdbusy;			// busy processing a long command
      bool exit;			  // exit thread
      bool abortdominocalib;			 // abort domino calibration
    } Global_t;

    Global_t GlobalInfo;   

/*
 * Define the status strings
 */
    enum 
    {
      Global_STATUS_ERROR=0,
      Global_STATUS_IDLE,
      Global_STATUS_LOADING,
      Global_STATUS_READY,
      Global_STATUS_ACTIVE,
      Global_STATUS_STOPPED,
      Global_STATUS_UNKNOWN
    };

    char *Global_status[]=
    {
      "error",
      "idle",
      "loading",
      "ready",
      "active",
      "stopped",
      "unknown"
    };
/*
* communication status
*/ global:
    char *Global_comstatus[]=
    {
      "notReady",
      "cmdReady",
      "notReceived",
      "cmdReceived",
      "cmdIgnored",
      "unknown"
    };

enum {
  Global_COMSTATUS_NOTREADY=0,
  Global_COMSTATUS_READY,
  Global_COMSTATUS_NOTRECVD,
  Global_COMSTATUS_RECVD,
  Global_COMSTATUS_IGNORED,
  Global_COMSTATUS_UNKNOWN};


//global
enum {
  Global_STANDALONE=0,
  Global_LOCK};


//definition of sockets
//2 sockets to try twice to comunicate
enum {
  Socket_1=0,
  Socket_2};

