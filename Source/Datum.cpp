/*============================================================================*/
/* Datum.cpp -- datum                                                         */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

static const i8 C_CACHELINES = C_64KB;

CacheLine xaCacheLine[C_CACHELINES];

i8 i_CurrentCacheLine = -1;
i8 i_CacheOffset = -1;

#ifdef WITH_ROWBUFFER
//ALKA's versions

/*----------------------------------------------------------------------------*/
/* set given datum in cache line                                              */
/*----------------------------------------------------------------------------*/
void Datum::vSet(i8 i_Column, eDM eDatum, byte* aDatum, CacheLine* aCacheLine, i8 &i_CacheOffset)
{
  // don't encode columns 16+
  if (i_Column >= 16)
    return;
  
  w8 wVal;
  i8 iEncoding;
  
  if (DM_INT == eDatum) {
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    iEncoding = RowEncoding::iEncode(pColumn,aDatum);
    
    aCacheLine->v1N(i_Column,iEncoding);
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    wVal = *(i8*)aDatum;
    //aCacheLine->v8B(8 + i_Column * 8,wVal);
    aCacheLine->vPut(i_CacheOffset,aDatum,iEncoding, aCacheLine);
    
    MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    
    i_CacheOffset += iEncoding;
  }
  
}

/*----------------------------------------------------------------------------*/
/* get encoded datum from cache line                                          */
/*----------------------------------------------------------------------------*/
w8 Datum::wGet(i8 i_Column, eDM eDatum, CacheLine* aCacheLine)
{
  i8 i_Offset = 8;
  for (i8 i_Nibble = 0; i_Nibble < i_Column; i_Nibble++) {
    i8 nDataBytes = aCacheLine->w8_1N(i_Nibble);
    i_Offset += nDataBytes;
  }
  
  i8 iEncoding = aCacheLine->w8_1N(i_Column);
  
  w8 wVal = C_UNKNOWN;
  
  switch (iEncoding) {
      
    case 0: wVal = C_DATUM_NULL;                break;
    case 1: wVal = aCacheLine->w8_1B(i_Offset); break;
    case 2: wVal = aCacheLine->w8_2B(i_Offset); break;
    case 3: wVal = aCacheLine->w8_3B(i_Offset); break;
    case 4: wVal = aCacheLine->w8_4B(i_Offset); break;
    case 5: wVal = aCacheLine->w8_5B(i_Offset); break;
    case 6: wVal = aCacheLine->w8_6B(i_Offset); break;
    case 7: wVal = aCacheLine->w8_7B(i_Offset); break;
    case 8: wVal = aCacheLine->w8_8B(i_Offset); break;
      
    default:
      NEVER_REACH;
  }
  
  
  return wVal;
}

void Datum::vPut(i8 i_Offset, byte* aDatum, i8 iEncoding, CacheLine* aCacheLine)
{
  w8 wVal = *(i8*)aDatum;
  //CacheLine* aCacheLine = &xaCacheLine[i_CurrentCacheLine];
  
  switch (iEncoding) {
      
    case 0: return;
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
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
