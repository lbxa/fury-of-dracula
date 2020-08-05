////////////////////////////////////////////////////////////////////////
// Runs a player's game turn ...
//
// Can  produce  either a hunter player or a Dracula player depending on
// the setting of the I_AM_DRACULA #define
//
// This  is  a  dummy  version of the real player.c used when you submit
// your AIs. It is provided so that you can test whether  your  code  is
// likely to compile ...
//
// Note that this is used to drive both hunter and Dracula AIs. It first
// creates an appropriate view, and then invokes the relevant decideMove
// function,  which  should  use the registerBestPlay() function to send
// the move back.
//
// The real player.c applies a timeout, and will halt your  AI  after  a
// fixed  amount of time if it doesn 't finish first. The last move that
// your AI registers (using the registerBestPlay() function) will be the
// one used by the game engine. This version of player.c won't stop your
// decideMove function if it goes into an infinite loop. Sort  that  out
// before you submit.
//
// Based on the program by David Collien, written in 2012
//
// 2017-12-04	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.3	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"

//#define I_AM_DRACULA

#ifdef I_AM_DRACULA
# include "dracula.h"
# include "DraculaView.h"
#else
# include "hunter.h"
# include "HunterView.h"
#endif

// Moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

// A pseudo-generic interface, which defines
// - a type `View',
// - functions `ViewNew', `decideMove', `ViewFree',
// - a trail `xtrail', and a message buffer `xmsgs'.
#ifdef I_AM_DRACULA

typedef DraculaView View;

# define ViewNew DvNew
# define decideMove decideDraculaMove
# define ViewFree DvFree

# define xPastPlays "GSZ.... SLO.... HSR.... MZU.... DCO.V.. GBE.... SLV.... HMA.... MST.... DHIT... GBD.... SMN.... HLS.... MNU.... DSTT... GBE.... SED.... HAO.... MSTTD.. DMUT... GSZ.... SNS.... HNS.... MCOTV.. DVET... GZA.... SHA.... HHA.... MST.... DGOT... GMUT... SVI.... HCO.... MMU.... DMRT... GZU.... SMU.... HCO.... MMI.... DMS.... GGE.... SVET... HPA.... MST.... DAO.... GGE.... SFL.... HFR.... MBU.... DGWT... GGOT... SGO.... HFR.... MST.... DHIT... GST.... SMI.... HFR.... MNU.... DDUT... GNU.... SMRT... HBU.... MFR.... DIR.... GST.... SBU.... HSR.... MFR.... DSW.V.. GNU.... SSR.... HSN.... MFR.... DLOT... GFR.... SSN.... HSN.... MBU.... DPLT.M. GFR.... SSN.... HLS.... MSR.... DHIT.M. GFR.... SLS.... HSN.... MSN.... DEC..M. GBU.... SSN.... HSN.... MSN.... DAO.... GCO.... SSN.... HSN.... MBB.... DMS..V. GCO.... SSN.... HBB.... MBB.... DTS..M. GCO.... SBB.... HBB.... MBB.... DIO..M. GFR.... SBB.... HBB.... MBB.... DBS..M. GMI.... SBB.... HBB.... MBB.... DCNT... GGO.... SBB.... HBB.... MBB.... DGAT... GMR.... SAO.... HAO.... MAO.... DKLT... GGE.... SCA.... HCA.... MCA.... DSZ.V.. GFL.... SGR.... HGR.... MGR.... DBDT... GRO.... SCA.... HCA.... MCA.... DD3T... GBI.... SGR.... HGR.... MGR.... DCDT.M. GAS.... SCA.... HAL.... MMA.... DHIT.M. GIO.... SAO.... HMS.... MSR.... DKLT.M. GBS.... SNS.... HTS.... MMR.... DSZT.V. GIO.... SEC.... HIO.... MGE.... DBDT.M. GBS.... SNS.... HBS.... MPA.... DD3T.M. GIO.... SEC.... HIO.... MPA.... DGAT.M. GBS.... SNS.... HBS.... MBO.... DCDT.M. GCN.... SHA.... HCN.... MNA.... DHIT.M. GGAT... SVI.... HGA.... MPA.... DKLT.M. GCDTT.. SBDT... HCD.... MST.... DSZ.V.. GGA.... SBD.... HGA.... MLI.... DZAT.M. GGA.... SBC.... HBE.... MNU.... DMUT... GBC.... SSZV... HSZ.... MMUTD.. DD1T... GVI.... SZAT... HZA.... MNU.... DZUT... GMUT... SMU.... HMU.... MMU.... DMRT.M. GVE.... SST.... HST.... MMU.... DMS.... GGO.... SGO.... HCO.... MMU.... DAO.... GVE.... SMRT... HBU.... MMI.... DGWT... GGO.... SGE.... HST.... MFL.... DHIT... GMR.... SMR.... HGO.... MFL.... DDUT.M. GBO.... SSR.... HMR.... MVE.... DIR.... GBO.... SAL.... HGE.... MGO.... DSWT... GNA.... SSR.... HMR.... MVE.... DLO.V.. GCF.... SPA.... HSR.... MGO.... DPLT.M. GPA.... SLE.... HSN.... MTS.... DHIT.M. GLE.... SLE.... HSN.... MNP.... DEC..M. GLE.... SLE.... HSR.... MBI.... DNS.... GLE.... SMR.... HBO.... MRO.... DHAT.M. GMR.... SLE.... HTO.... MRO.... DLIT.V. GLE.... SLE.... HMR.... MFL.... DNUT.M. GLE.... SLE.... HLE.... MFL.... DPRT.M. GNA.... SBU.... HBO.... MRO.... DVIT... GCF.... SCO.... HNA.... MTS.... DBDT... GNA.... SHAT... HCF.... MMS.... DKLT... GCF.... SLIT... HNA.... MCG.... DGAT... GNA.... SMU.... HCF.... MTS.... DCN.VM. GCF.... SZA.... HTO.... MRO.... DBCT.M. GTO.... SVIT... HBO.... MMI.... DBET... GBO.... SZA.... HBA.... MFL.... DD5T.M. GSR.... SVI.... HBA.... MFL.... DCDT.M. GBO.... SBCT... HMS.... MVE.... DKLT.M. GMR.... SGA.... HTS.... MBD.... DSZT.V. GGO.... SCDT... HIO.... MGA.... DZAT... GMI.... SJM.... HTS.... MGA.... DHIT.M. GST.... SJM.... HNP.... MGA.... DVIT.M. GNU.... SJM.... HTS.... MBC.... DMUT... GST.... SJM.... HNP.... MBD.... DZUT.M. GGE.... SBE.... HFL.... MKL.... DSTT.M. GPA.... SSZ.... HVE.... MBD.... DGE.VM. GNA.... SKL.... HMI.... MBE.... DHIT.M. GNA.... SBE.... HRO.... MBD.... DPAT.M. GNA.... SSA.... HBI.... MZA.... DLET.M. GNA.... SVA.... HRO.... MBD.... DBUT.M. GNA.... SVA.... HNP.... MVE.... DSTT.M. GNA.... SVA.... HRO.... MVI.... DCOT.V. GCF.... SSA.... HGET... MVI.... DHIT... GBO.... SVA.... HMR.... MZA.... DHAT.M. GLET... SSA.... HGE.... MBD.... DNS.... GLE.... SIO.... HZU.... MSZ.... DEDT.M. GPA.... SSA.... HMR.... MBE.... DMNT.M. GNA.... SBE.... HCF.... MJM.... DLOT.M. GBO.... SSO.... HNA.... MZA.... DHI.VM. GCF.... SBE.... HBO.... MJM.... DSWT.M. GNA.... SSZ.... HBU.... MZA.... DD1T... GBO.... SSA.... HAM.... MJM.... DLVT.M. GBU.... SIO.... HNS.... MSJ.... DMNT.M. GST.... SAT.... HHA.... MZA.... DLOT.M. GCO.... SIO.... HNS.... MMU.... DPLT.V. GPA.... SAT.... HHA.... MVE.... DEC..M. GCO.... SIO.... HLI.... MGO.... DNS..M. GBU.... SAT.... HPR.... MGE.... DAO..M. GLE.... SIO.... HBR.... MMR.... DGWT.M. GNA.... SAS.... HLI.... MTO.... DDUT.M. GCF.... SIO.... HHA.... MBO.... DIR..M. GBO.... STS.... HLI.... MSR.... DSW.V.. GPA.... SNP.... HZU.... MSN.... DLOT... GLE.... STS.... HGE.... MSR.... DPLT... GNA.... SMS.... HST.... MTO.... DEC..M. GBO.... SCG.... HFR.... MBA.... DLET.M. GCF.... STS.... HFR.... MMS.... DPAT... GBO.... SCG.... HLI.... MAL.... DSTT.V. GTO.... STS.... HNU.... MLS.... DMUT.M. GSR.... SGO.... HSTT... MBO.... DZAT.M. GBO.... SMI.... HCO.... MNA.... DVIT... GMA.... SGE.... HBU.... MLET... DBDT... GGR.... SST.... HCO.... MPAT... DKLT... GGR.... SFR.... HCO.... MMA.... DGAT... GGR.... SFR.... HHA.... MCA.... DCD.VM. GMA.... SMU.... HBR.... MAO.... DHIT.M. GPA.... SVIT... HPR.... MNS.... DD1T... GST.... SBDT... HBD.... MHA.... DTP.... GMU.... SBC.... HBD.... MCO.... DKLT.M. GVI.... SBE.... HSZ.... MBU.... DGAT.M. GHA.... SKLT... HKL.... MST.... DCDT.V. GBR.... SCDTT.. HCDTD.. MMU.... DHIT... GPR.... SBE.... HKL.... MVI.... DD1T... GBD.... SGAT... HCDTT.. MBD.... DTPT... GBD.... SKL.... HZA.... MZA.... DKLT... GKLTD.. SKLD... HSZ.... MBD.... DGAT... GBC.... SBC.... HKL.... MKL.... DCDT... GKL.... SGAT... HCDTTD. MCDD... DHI.V.. GCDVD.. SCDD... HCDD... MCDD... DD1T... GBE.... SBE.... HBE.... MBE.... DTPT... GKL.... SJM.... HVI.... MKL.... DKLT... GKLTD.. SBE.... HBD.... MKLD... DGAT... GGATD.. SKL.... HSZ.... MGAD... DCDT... GBE.... SCDTTTD HGA.... MCDD... DHIT... GKL.... SBE.... HCDTD.. MBE.... DD1T... GCDTD.. SGA.... HGA.... MKL.... DTPT... GGA.... SCDTD.. HCN.... MBC.... DKLT... GCD.... SGA.... HSZ.... MGA.... DBCT... GGA.... SSZ.... HVE.... MCD.... DCNT... GSZ.... SVE.... HMU.... MGA...."
# define xMsgs { "", "", "", "", "", "", "", "", "" }

#else

typedef HunterView View;

# define ViewNew HvNew
# define decideMove decideHunterMove
# define ViewFree HvFree

# define xPastPlays "GZA.... SED.... HZU...."
# define xMsgs { "", "", "" }

#endif

int main(void)
{
	char *pastPlays = xPastPlays;
	Message msgs[] = xMsgs;

	View state = ViewNew(pastPlays, msgs);
	decideMove(state);
	ViewFree(state);

        printf("%s\n", xPastPlays);

	printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
	return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay(char *play, Message message)
{
	strncpy(latestPlay, play, MOVE_SIZE - 1);
	latestPlay[MOVE_SIZE - 1] = '\0';

	strncpy(latestMessage, message, MESSAGE_SIZE - 1);
	latestMessage[MESSAGE_SIZE - 1] = '\0';
}
