/*============================================================================*/
/* KeyBuilder.cpp -- Key Builder                                              */
/*============================================================================*/

#include "Indexes.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void KeyBuilder::vAddElement(EntWrk* pEntWrk, Column* pCol,
                               Stz* szSrc, Format* pFmt)
{
  i8 nBytes;

  char* aSrc = (char*)szSrc;
  char* aDst = (char*)pEntWrk->ayElementCrs();

  if (DM_VCH == pCol->eDatum()) {
    nBytes = StrLib::iWordFlip(aDst,aSrc);
    pEntWrk->vIncKeyBytes(nBytes);
    return;
  }

  Datum oDatum;

  //bool b = KeyBuilder::bExtractDtm(&oDatum,pCol,&aSrc,pFmt);
  bool b = CsvValue::bToDtm(&oDatum,pCol,&aSrc,pFmt);
  ASSERT(b);

  if (oDatum.bIsNull()) {
    nBytes = Null::nCopy((byte*)aDst);
    pEntWrk->vIncKeyBytes(nBytes);
    return;
  }

  nBytes = Dtm::nCopy((byte*)aDst,oDatum.iInt());
  pEntWrk->vIncKeyBytes(nBytes);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
