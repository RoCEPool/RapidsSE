/*============================================================================*/
/* Datum.cpp -- datum                                                         */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

static const i8 C_CACHELINES = C_64KB;

CacheLine xaCacheLine[C_CACHELINES];

i8 i_CurrentCacheLine = -1;

/*----------------------------------------------------------------------------*/
/* begin encoding into new cache line                                         */
/*----------------------------------------------------------------------------*/
void Datum::vBegin(void)
{
  i_CurrentCacheLine++;
  
  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  aCacheLine->vSetAll(0xFF);
}

/*----------------------------------------------------------------------------*/
/* encode given datum into cache line                                         */
/*----------------------------------------------------------------------------*/
void Datum::vEncode(i8 i_Column, eDM eDatum, byte* aDatum)
{
  // don't encode columns 16+
  if (i_Column >= 16)
    return;

  w8 wVal;

  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  
  if (DM_INT == eDatum) {
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    aCacheLine->v1N(i_Column,8);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));

    wVal = *(i8*)aDatum;
    aCacheLine->v8B(8 + i_Column * 8,wVal);
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    int iDebug = 0;
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
