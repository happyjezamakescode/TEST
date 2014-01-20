/******
 * Sum-Trigger Info                     *
 * date: september 2013                 *
 * by: jrg                              *
 *                              *********/

    #define CB_N 18               // Number of Clip boards
    #define DM_N 32               // Number of Delay Modules
    #define MC_N 55               // Number of Macrocels
    #define FN_N 3                // Number of FANOUT
    #define PX_N 559              // Number of PIXELS
    #define N_LUT 58                  // Number of LUT lines

void calib_menu();

/*
 * Clip Board Status
*/
    typedef struct Clip_struct 
    {
        float clip[DM_N]; // clipping channels (mv)
        float delay[DM_N]; // delay channels (V)
        float att[DM_N]; // gain channels (dB)
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
        float  ddelay[MC_N];    
        float dref[2];//channels 0 and 1    
        float rate[MC_N];
        int globalrate;
        int L3Rate;
        int rate_max[MC_N];
        int rate_min[MC_N];
        float tim[MC_N];
        float amp[MC_N];
        float threshold[MC_N];
        float cthreshold[2];//channels 0 and 1
        float edel[6];
        int TAst;   
        int Gate;   
        unsigned int Tmax;//=20;
        unsigned int Cmax;//=6500;
        unsigned short Astatus; //errors?
        char thr_outputname[1000];
        char rates_outputname[1000];

    }AS_t;

         AS_t ASData;

/*
 * Mapping
*/
    typedef struct mapping_struct 
    {
       //CAMERA
        short PIXEL; //CAMERA (Counting on hardware)
       
       //RECEIVER
        short RECEIVER_CONNECTOR; //RECEIVER BOARD: Connector
        short RECEIVER_CHANNEL; //RECEIVER BOARD: Channel # in connector
       
       //CLIP-BOARD 
        short CLIP_SLOT;  //CLIP-BOARD: Board position in crate (L to R)
        short CLIP_CONNECTOR;    //CLIP-BOARD: Front connector positio (U to D)
        char  *CLIP_POSITIVE; //CLIP-BOARD: Pinout, Positive Signal
        char  *CLIP_NEGATIVE; //CLIP-BOARD: Pinout, Negative Signal
        short CLIP_ERNI_REAR;  //CLIP-BOARD: Back view, rear connector position

       //SUM-BACKPLANE - ([0]=FANOUT1,[1]=FANOUT2,[2]=FANOUT3 )
        short S_CLIP_ERNI[FN_N];//Connector # (U to D & L to R) ERNII
        short S_CLIP_CHANNEL[FN_N]; // Channel # in connector. ERNI 
        short S_CLIP_CONNECTOR[FN_N];//Connector # in board (U to D) ERNI
        char  *S_CLIP_POSITIVE[FN_N]; //CLIP-BOARD: Pinout, Positive Signal
        char  *S_CLIP_NEGATIVE[FN_N]; //CLIP-BOARD: Pinout, Negative Signal
        short S_SUM_ERNI[FN_N];//Connector # (U to D & L to R) ERNII
        char  *S_SUM_POSITIVE[FN_N]; // Pinout, Positive Signal
        char  *S_SUM_NEGATIVE[FN_N]; //Pinout, Negative Signal
        short S_SUM_MACROCELL[FN_N];//Macrocell 
        short S_SUM_CHANNEL[FN_N]; // Channel # iN macrocell (UL to DR) 
        short S_SUM_LAYER[FN_N]; // Layer number
    } Mapping_t;

    Mapping_t MAP[PX_N];
/*
 * Bias Voltage -- Delay -- Optimum Attenuation mapping
 * Added by Daisuke 
*/
    typedef struct Delay_LUT
    {
        float Bias;
        short Fixed;
        float Delay;
        float Attenuation;
    } Delay_LUT_t;

Delay_LUT_t DelayLUT[N_LUT]; 

bool LUTLoaded;

/*
 *   Dummy info  
 */

    typedef struct Dummy_struct_s 
    {
      short channelvalue[100];
      short value;
    } Dummy_struct_t;

    Dummy_struct_t DummyData;


/*
 *   Calibration info  
 */
    typedef struct Cal_struct_s 
    {
      int Pixel[MC_N];
      int Lboard[MC_N];
      int Lchannel[MC_N];
      int Lmacro[MC_N];   
      char outputname[1000];
      char outputfolder[1000];
      int TempFlag;
      int Laser;
      int Time;
      int Rate;
    } Cal_struct_t;

    Cal_struct_t CalSet;

  char outputname2[50];

int dummyvalor;
int dummyvalor2;

/*
 * Define here the status structure, and communicating with CC
 */ 
    enum
    {
      NOERR=0, ERINFO, ERWARN, EREROR, ERSEVR
    }; 

    typedef struct Info_s 
    {
      int telescope;
      int  DBG;                               //flag: 0=0ff, 1=on
      bool  threshold_control_flag;                //flag: 0=0ff, 1=on
      int  Global_Target_rate;               
      float  Macrocell_Target_rate;             
      short  Monitoring_control;               //flag: 0=0ff, 1=on
      char delaytable[1000];
      char atttable[1000];
      char thretable[1000];
      int4 istatus;
      char *status;
      int4 icomstatus;
      char *comstatus;
      int4 uselog;                         // the log flag
      char *hostname[2];                   // the host name
      int4 hostno[2];                      // the host port number
      int client_conn[2];                  //se usa en el report para los sokets?????
      char *tablepath;                     // the tables directory
      int4 portno;                         // the local server port number
      time_t startime;                     // time when the program started
      time_t lastcmdtime;                  // time when last command was received
      time_t writetime;                  // time when we writte
      int lock_mode;
      int server_conn;
      int2 cmdbusy;			                // busy processing a long command
      bool exit;                             // exit thread
      bool monexit;                          // exit monitoring thread
      int temp_control;                     // change to monittoring the T of the crate              
      int total_time;                 
      int time_interval;                 
      float SumL3_Az;			//Deg                 
      float SumL3_Zd;                   //Deg
      float SumL3_delay;                //ns 
    } Info_t;

    Info_t STInfo;   

/*
 * Define the status strings
 */
    enum 
    {
      ST_STATUS_ERROR=0,           //Error on the program, or on the communication
      ST_STATUS_BUSY,             //Loading tables or doing something
      ST_STATUS_TRIGGERING,       //Ready to work 
   //   ST_STATUS_ACTIVE,
      ST_STATUS_STOPPED,       
      ST_STATUS_ZERORATE,
      ST_STATUS_DEADZONE,
    };

    char *ST_status[]=
    {
      "error",
      "busy",
      "triggering",
    //  "ready",
      "stopped",
      "zerorate",
      "deadzone"
    };
/*
* communication status
*/
    char *ST_comstatus[]=
    {
      "notReady",
      "cmdReady",
      "notReceived",
      "cmdReceived",
      "cmdIgnored",
      "unknown"
    };

    enum 
    {
      ST_COMSTATUS_NOTREADY=0,
      ST_COMSTATUS_READY,
      ST_COMSTATUS_NOTRECVD,
      ST_COMSTATUS_RECVD,
      ST_COMSTATUS_IGNORED,
      ST_COMSTATUS_UNKNOWN
    };

    enum 
    {
      ST_STANDALONE=0,
      ST_LOCK
    };


//definition of sockets
//2 sockets to try twice to comunicate
    enum 
    {
        Socket_1=0,
        Socket_2
    };

