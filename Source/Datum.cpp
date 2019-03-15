/*============================================================================*/
/* Datum.cpp -- datum                                                         */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

static const i8 C_CACHELINES = C_64KB;

CacheLine xaCacheLine[C_CACHELINES];

i8 i_CurrentCacheLine = -1;
i8 i_CacheOffset = -1;

/*----------------------------------------------------------------------------*/
/* begin encoding into new cache line                                         */
/*----------------------------------------------------------------------------*/
void Datum::vBegin(void)
{
  i_CurrentCacheLine++;
  i_CacheOffset = 8;

  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  aCacheLine->vSetAll(0xFF);
}

/*----------------------------------------------------------------------------*/
/* set given datum in cache line                                              */
/*----------------------------------------------------------------------------*/
void Datum::vSet(i8 i_Column, eDM eDatum, byte* aDatum)
{
  // don't encode columns 16+
  if (i_Column >= 16)
    return;

  w8 wVal;
  i8 iEncoding;

  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  
  if (DM_INT == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = iEncode(eDatum,aDatum);
    
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));

    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    vPut(i_CacheOffset,aDatum,iEncoding);

    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
    
    int iDebug = 0;
  }
  
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode given datum before setting in cache line                            */
/*----------------------------------------------------------------------------*/
i8 Datum::iEncode(eDM eDatum, byte* aDatum)
{
  w8 wVal = *(i8*)aDatum;

  switch (eDatum) {

    case DM_INT:
      if (wVal == (u8)C_DATUM_NULL) return 0;
      
      if (wVal < 256) return 1; // (2^8)^1
      if (wVal < 65536) return 2; // (2^8)^2
      if (wVal < 16777216) return 3; // (2^8)^3
      if (wVal < 4294967296) return 4; // (2^8)^4
      if (wVal < 1099511627776) return 5; // (2^8)^5
      if (wVal < 281474976710656) return 6; // (2^8)^6
      if (wVal < 72057594037927936) return 7; // (2^8)^7
      
      return 8; // (2^8)^8
      
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* put encoded datam into cache line                                          */
/*----------------------------------------------------------------------------*/
void Datum::vPut(i8 i_Offset, byte* aDatum, i8 iEncoding)
{
  w8 wVal = *(i8*)aDatum;
  CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];

  switch (iEncoding) {
      
    case 1: aCacheLine->v1B(i_Offset,wVal); return;
    case 2: aCacheLine->v2B(i_Offset,wVal); return;
    case 3: aCacheLine->v3B(i_Offset,wVal); return;
    case 4: aCacheLine->v4B(i_Offset,wVal); return;
    case 5: aCacheLine->v5B(i_Offset,wVal); return;
    case 6: aCacheLine->v6B(i_Offset,wVal); return;
    case 7: aCacheLine->v7B(i_Offset,wVal); return;
    case 8: aCacheLine->v8B(i_Offset,wVal); return;

    default:
      NEVER_REACH;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
