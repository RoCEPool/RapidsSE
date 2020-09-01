/*============================================================================*/
/* KeyBuilder.cpp -- Key Builder                                              */
/*============================================================================*/

#include "Indexing.h"


/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void KeyBuilder::vAddElement(eKE eKeyKind, EntWrk* pEntWrk, Column* pCol,
                             Stz* szSrc, Format* pFmt)
{
  if (eKeyKind == KE_INT)
    vAddElement_int(pEntWrk, pCol, szSrc, pFmt);
  else
    vAddElement_gen(pEntWrk, pCol, szSrc, pFmt);
}

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void KeyBuilder::vAddElement_gen(EntWrk* pEntWrk, Column* pCol,
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

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void KeyBuilder::vAddElement_int(EntWrk* pEntWrk, Column* pCol, Stz* szSrc, Format* pFmt)
{
  char* aSrc = (char*)szSrc;
  
  ASSERT (DM_INT == pCol->eDatum());
  
  Datum oDatum;
  
  bool b = CsvValue::bToDtm(&oDatum,pCol,&aSrc,pFmt);
  ASSERT(b);
  
  pEntWrk->aEnt()->vKind(KE_INT);
  
  if (oDatum.bIsNull()) {
    pEntWrk->aEnt()->vIntElm0(C_DATUM_NULL);
    return;
  }
  
  pEntWrk->aEnt()->vIntElm0(oDatum.iInt());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
