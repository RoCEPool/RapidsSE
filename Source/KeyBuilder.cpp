/*============================================================================*/
/* KeyBuilder.cpp -- Key Builder                                              */
/*============================================================================*/

#include "IndexTrees.h"
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
  i8 nElmSize;

  char* aSrc = (char*)szSrc;
  char* aDst = (char*)pEntWrk->ayElmNext();

  if (DM_VCH == pCol->eDatum()) {
    nElmSize = StrLib::iWordFlip(aDst,aSrc);
    pEntWrk->vIncElmSeqSize(nElmSize);
    return;
  }

  Datum oDatum;

  bool b = CsvValue::bToDtm(&oDatum,pCol,&aSrc,pFmt);
  ASSERT(b);

  if (oDatum.bIsNull()) {
    nElmSize = Null::nCopy((byte*)aDst);
    pEntWrk->vIncElmSeqSize(nElmSize);
    return;
  }

  nElmSize = Dtm::nCopy((byte*)aDst,oDatum.iInt());
  pEntWrk->vIncElmSeqSize(nElmSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
