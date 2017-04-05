/*============================================================================*/
/* Datums.cpp -- datums (single units of data)                                */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Datums::vCheckAssumptions(void)
{
}

/*----------------------------------------------------------------------------*/
/* extract datum                                                              */
/*----------------------------------------------------------------------------*/
bool Datums::bExtractDatum(Str* aDst, Str* aSrc, eDM eDatum, i8& nBytesTaken)
{
  bool b = false;
  
  switch (eDatum) {
    case DM_BCD: break;
    case DM_BOL: break;
    case DM_DTE: break;
    case DM_FLT: break;
    case DM_INT: b = Int::bExtract(aDst,aSrc,nBytesTaken); break;
    case DM_IVL: break;
    case DM_TME: break;
    case DM_TSP: break;
    default:
      vFatal("Datums::bExtractDatum: Invalid aDatum argument: %lld\n"_Str,
             eDatum);
      
  }
  return b;
}

/*----------------------------------------------------------------------------*/
/* sprint datum                                                               */
/*----------------------------------------------------------------------------*/
void Datums::vSprint(Str* aDst, Str* aSrc, eDM eDatum, i8& nBytesTaken)
{
  switch (eDatum) {
    case DM_BCD: break;
    case DM_BOL: break;
    case DM_DTE: break;
    case DM_FLT: break;
    case DM_INT: Int::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_IVL: break;
    case DM_TME: break;
    case DM_TSP: break;
    default:
      vFatal("Datums::bExtractDatum: Invalid aDatum argument: %lld\n"_Str,
             eDatum);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
