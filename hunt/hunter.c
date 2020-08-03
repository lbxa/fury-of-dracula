////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "hunter.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"

//Function to find if dracula is near from hunter's current location
static PlaceId draculaLocation (PlaceId *nearby, int numLoc, PlaceId drac)
static PlaceId draculaLocation (PlaceId nearby[], int numLoc, PlaceId drac) {

  int x = 0;
  while (x < numLoc) {
    if (nearby[x] == drac) {
      return 0;
    }
    x++;
  }
  return 1;
}


void decideHunterMove(void* hv) {
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
//  srand ( GvGetPlayer(1) );
  HunterView view = (HunterView)hv;

  FILE *turnLog = fopen("turns.log", "a");
  fprintf(turnLog, "\nHunter Move (%d)\n", HvGetRound(view));
  fclose(turnLog);

  int numLoc = 0;
  char *forward = "";
  char *backward = "abcde";

  //Player moves
  if (HvGetRound(view) == 0) {

    if (HvGetPlayer(view) == PLAYER_LORD_GODALMING) {

      registerBestPlay("CD", forward);  

    } else if (HvGetPlayer(view) == PLAYER_DR_SEWARD) {

      registerBestPlay("AT", forward); 

    } else if (HvGetPlayer(view) == PLAYER_VAN_HELSING) {

      registerBestPlay("VR", forward);

    } else {

      registerBestPlay("CN", forward);

    }

  //In this case if dracula is in same location as Hunter, Hunter will stay in that location.
  } else if (HvGetPlayerLocation(view, PLAYER_DRACULA) == HvGetPlayerLocation(view, HvGetPlayer(view))) {

      registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, PLAYER_DRACULA)), (char *)array);
  
  //In this case, if dracula is lost, Hunter goes back to his/her path
  } else if (strcmp((char *)array, forward) != 0 && strcmp((char *)array, backward) != 0) {

      if (draculaLocation(HvWhereCanIGoByType(view, true, true, false, &numLoc), numLoc, abbrevToID((char *)array)) == 1) {        
          registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, HvGetPlayer(view))), (char *)array);
      } else {
          registerBestPlay((char *)array, forward);
      }

  // All players case
  } else if (HvGetPlayer(view) == PLAYER_LORD_GODALMING) {

      //get message for Lord Godalming - implement

      //player taken to hospital
      if (HvGetPlayerLocation(view, PLAYER_LORD_GODALMING) == ST_JOSEPH_AND_ST_MARYS){ 
          registerBestPlay("SZ", forward);
      //player's way returning from hospital
      } else if (HvGetPlayerLocation(view, PLAYER_LORD_GODALMING) == SZEGED) {
          registerBestPlay("KL",forward);
      } else if(HvGetPlayerLocation(view, PLAYER_DRACULA) == CASTLE_DRACULA) {
          registerBestPlay("CD", forward);
      } else if(HvGetHealth(view, PLAYER_LORD_GODALMING) < GAME_START_HUNTER_LIFE_POINTS && HvGetPlayerLocation(view, PLAYER_DRACULA) != CASTLE_DRACULA) {
          registerBestPlay("GA", forward);  
      } else {
          registerBestPlay("CD", forward);
      }

  } else if (HvGetPlayer(view) == PLAYER_DR_SEWARD) {

      //get message for Dr Seward - implement

      if (HvGetPlayerLocation(view, PLAYER_DR_SEWARD) == ST_JOSEPH_AND_ST_MARYS){

          if (strcmp((char *)array, backward) == 0) {
              registerBestPlay("SJ", backward);
          } else {
              registerBestPlay("SJ", forward);
          }

      } else if (HvGetHealth(view, PLAYER_DR_SEWARD) < 3) {

          if (strcmp((char *)array, backward) == 0) {
              registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, PLAYER_DR_SEWARD)), backward);
          } else {
              registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, PLAYER_DR_SEWARD)), forward);
          }

      } else {
          switch (HvGetPlayerLocation(view, PLAYER_DR_SEWARD)) {

              case ATHENS:

                  registerBestPlay("VA", forward); 
                  break;

              case SALONICA:

                  registerBestPlay("IO", forward); 
                  break;

              case VALONA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("AT", forward); 
                      break;
                  } else {
                      registerBestPlay("SA", forward); 
                      break;
                  }

              case IONIAN_SEA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("VA", backward); 
                      break;
                  } else {
                      registerBestPlay("AS", forward); 
                      break;
                  }

              case ADRIATIC_SEA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("IO", backward); 
                      break;
                  } else {
                      registerBestPlay("BI", forward); 
                      break;
                  }

              case BARI:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("AS", backward); 
                      break;
                  } else {
                      registerBestPlay("NP", forward); 
                      break;
                  }

              case NAPLES:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("BI", backward);  
                      break;
                  } else {
                      registerBestPlay("RO", forward); 
                      break;
                  }

              case ROME:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("NP", backward); 
                      break;
                  } else {
                      registerBestPlay("FL", forward); 
                      break;
                  }

              case FLORENCE:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("RO", backward); 
                      break;
                  } else {
                      registerBestPlay("VE", forward); 
                      break;
                  }

              case VENICE:

                  registerBestPlay("MI", forward); 
                  break;
              case MILAN:

                  registerBestPlay("GO", forward); 
                  break;

              case GENOA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("FL", backward); 
                      break;
                  } else {
                      registerBestPlay("TS", forward); 
                      break;
                  }

              case TYRRHENIAN_SEA:

                  registerBestPlay("CG", forward); 
                  break;

              case CAGLIARI:

                  registerBestPlay("MS", forward); 
                  break;

              case MEDITERRANEAN_SEA:

                  registerBestPlay("MR", forward); 
                  break;

              case MARSEILLES:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("GO", backward); 
                      break;
                  } else {
                      registerBestPlay("TO", forward); 
                      break;
                  }

              case TOULOUSE:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("MR", backward); 
                      break;
                  } else {
                      registerBestPlay("BA", forward); 
                      break;
                  }

              case BARCELONA:

                  registerBestPlay("SR", forward); 
                  break;

              case SARAGOSSA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("TO", backward); 
                      break;
                  } else {
                      registerBestPlay("AL", forward); 
                      break;
                  }

              case ALICANTE:

                  registerBestPlay("GR", forward); 
                  break;

              case GRANADA:

                  registerBestPlay("CA", forward); 
                  break;

              case CADIZ:

                  registerBestPlay("LS", forward); 
                  break;

              case LISBON:

                  registerBestPlay("SN", forward); 
                  break;

              case SANTANDER:

                  registerBestPlay("MA", forward); 
                  break;

              case MADRID:

                  registerBestPlay("SR", backward); 
                  break;

              case SARAJEVO:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("VA", backward); 
                      break;
                  } else {
                      registerBestPlay("VA", forward); 
                      break;
                  }

              default:

                  registerBestPlay(idToAbbrev( HvWhereCanIGoByType(view, true, true, false, &numLoc)[rand() % numLoc]), (char *)array); break;
          }
      }

  } else if (HvGetPlayer(view) == PLAYER_VAN_HELSING) {

      //get message for Van Helsing - implement

      if (HvGetPlayerLocation(view, PLAYER_VAN_HELSING) == ST_JOSEPH_AND_ST_MARYS){

          if (strcmp((char *)array, backward) == 0) {
              registerBestPlay("SJ", backward);
          } else {
              registerBestPlay("ZA", forward);
          }

      } else if (HvGetHealth(view, PLAYER_VAN_HELSING) < 3) {

          if (strcmp((char *)array, backward) == 0) {
              registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, PLAYER_VAN_HELSING)), backward);
          } else {
              registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, PLAYER_VAN_HELSING)), forward);
          }

      } else {
          switch (HvGetPlayerLocation(view, PLAYER_VAN_HELSING)) {

              case VARNA:

                  registerBestPlay("SO", forward); 
                  break;

              case SOFIA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("VR", backward); 
                      break;    
                  } else {
                      registerBestPlay("BC", forward); 
                      break;
                  }

              case BUCHAREST:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("SO", backward); 
                      break;
                  } else {
                      registerBestPlay("BE", forward); 
                      break;
                  }

              case BELGRADE:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("BC", backward); 
                      break;
                  } else {
                      registerBestPlay("SJ", forward); 
                      break;
                  }

              case SARAJEVO:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("BE", backward); 
                      break;
                  } else {
                      registerBestPlay("ZA", forward); 
                      break;
                  }

              case ZAGREB:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("SJ", backward); 
                      break;
                  } else {
                      registerBestPlay("MU", forward); 
                      break;
                  }

              case MUNICH:

                  if (strcmp((char *)array, backward) == 0) { 
                      registerBestPlay("ZA", backward); 
                      break;
                  } else {
                      registerBestPlay("NU", forward); 
                      break;
                  }
                  
              case NUREMBURG:

                  registerBestPlay("LI", forward); 
                  break;

              case LEIPZIG:
              
                  registerBestPlay("FR", forward); 
                  break;

              case FRANKFURT:

                  registerBestPlay("ST", forward); 
                  break;

              case STRASBOURG:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("ZU", backward); 
                      break;
                  } else {
                      registerBestPlay("BU", forward); 
                      break;
                  }

              case BRUSSELS:

                  registerBestPlay("LE", forward); 
                  break;

              case LE_HAVRE:

                  registerBestPlay("NA", forward);
                  break;

              case NANTES:

                  registerBestPlay("BO", forward); 
                  break;

              case BORDEAUX:

                  registerBestPlay("CF", forward); 
                  break;

              case CLERMONT_FERRAND:

                  registerBestPlay("PA", forward); 
                  break;

              case PARIS:

                  registerBestPlay("GE", forward); 
                  break;

              case GENEVA:

                  registerBestPlay("ST", backward); 
                  break;

              case ZURICH:

                  registerBestPlay("MI", backward); 
                  break;

              case MILAN:

                  registerBestPlay("VE", backward); 
                  break;

              case VENICE:

                  registerBestPlay("MU", backward); 
                  break;

              default:

                  registerBestPlay(idToAbbrev(HvWhereCanIGoByType(view, true, true, false, &numLoc)[rand() % numLoc]), (char *)array); break;
          }
      }

  } else if (HvGetPlayer(view) == PLAYER_MINA_HARKER) {

      //get message for Mina Harker - implement

      if (HvGetPlayerLocation(view, PLAYER_MINA_HARKER) == ST_JOSEPH_AND_ST_MARYS){

          if (strcmp((char *)array, backward) == 0) {
              registerBestPlay("SZ", backward);
          } else {
              registerBestPlay("SZ", forward);
          }

      } else if (HvGetHealth(view, PLAYER_MINA_HARKER) < 3) {

          if (strcmp((char *)array, backward) == 0) {
              registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, PLAYER_MINA_HARKER)), backward);
          } else {
              registerBestPlay(idToAbbrev(HvGetPlayerLocation(view, PLAYER_MINA_HARKER)), forward);
          }

      } else {
          switch (HvGetPlayerLocation(view, PLAYER_MINA_HARKER)) {

              case CONSTANTA:

                  registerBestPlay("GA", forward); 
                  break;

              case GALATZ:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("CN", backward); 
                      break;
                  } else {
                      registerBestPlay("KL", forward); 
                      break;
                  }

              case KLAUSENBURG:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("GA", backward); 
                      break;
                  } else {
                      registerBestPlay("SZ", forward); 
                      break;
                  }

              case SZEGED:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("KL", backward); 
                      break;
                  } else {
                      registerBestPlay("BD", forward); 
                      break;
                  }

              case BUDAPEST:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("SZ", backward); 
                      break;
                  } else {
                      registerBestPlay("VI", forward); 
                      break;
                  }

              case VIENNA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("BD", backward); 
                      break;
                  } else {
                      registerBestPlay("PR", forward); 
                      break;
                  }
                  
              case PRAGUE:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("VI", backward); 
                      break;
                  } else {
                      registerBestPlay("BR", forward); 
                      break;
                  }

              case BERLIN:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("PR", backward); 
                      break;
                  } else {
                      registerBestPlay("HA", forward); 
                      break;
                  }

              case HAMBURG:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("BR", backward); 
                      break;
                  } else {
                      registerBestPlay("CO", forward); 
                      break;
                  }

              case COLOGNE:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("HA", backward); 
                      break;
                  } else {
                      registerBestPlay("AM", forward); 
                      break;
                  }

              case AMSTERDAM:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("CO", backward); 
                      break;
                  } else {
                      registerBestPlay("NS", forward); 
                      break;
                  }

              case NORTH_SEA:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("AM", backward); 
                      break;
                  } else {
                      registerBestPlay("ED", forward); 
                      break;
                  }

              case EDINBURGH:

                  registerBestPlay("MN", forward); 
                  break;

              case MANCHESTER:

                  registerBestPlay("LV", forward); 
                  break;

              case LIVERPOOL:

                  registerBestPlay("SW", forward); 
                  break;

              case SWANSEA:

                  registerBestPlay("IR", forward); 
                  break;

              case IRISH_SEA:

                  registerBestPlay("DU", forward); 
                  break;

              case DUBLIN:
                  registerBestPlay("GW", forward); 
                  break;

              case GALWAY:
                  registerBestPlay("AO", forward); 
                  break;

              case ATLANTIC_OCEAN:
                  registerBestPlay("EC", forward); 
                  break;

              case ENGLISH_CHANNEL:

                  if (strcmp((char *)array, backward) == 0) {
                      registerBestPlay("NS", backward); 
                      break;
                  } else {
                      registerBestPlay("PL", forward); 
                      break;
                  }

              case PLYMOUTH:

                  registerBestPlay("LO", forward); 
                  break;

              case LONDON:

                  registerBestPlay("EC", backward); 
                  break;

              default:

                  registerBestPlay(idToAbbrev(HvWhereCanIGoByType(view, true, true, false, &numLoc)[rand() % numLoc]), (char *)array); break;
          }
      }
  }

}

}
