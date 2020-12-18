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
                             Stz* szElementSrc, Format* pFmt)
{
  if (eKeyKind == KE_INT)
    vAddElement_int(pEntWrk, pCol, szElementSrc, pFmt);
  else
    vAddElement_gen(pEntWrk, pCol, szElementSrc, pFmt);
}

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void KeyBuilder::vAddElement_gen(EntWrk* pEntWrk, Column* pCol,
                                 Stz* szElementSrc, Format* pFmt)
{
  i8 nElmSize;
  
  char* azSrc = (char*)szElementSrc;
  char* azDst = (char*)pEntWrk->ayElmNext();
  
  if (DM_VCH == pCol->eDatum()) {
    nElmSize = StrLib::iWordFlip(azDst,azSrc);
    pEntWrk->vIncElmSeqSize(nElmSize);
    return;
  }
  
  Dtm* aCell = (Dtm*)azDst;
  
  bool b = CsvValue::bToCell(aCell,pCol,&azSrc,pFmt);
  nElmSize = sizeof(Dtm);
  pEntWrk->vIncElmSeqSize(nElmSize);
}
/*----------------------------------------------------------------------------*/
//void KeyBuilder::vAddElement_gen(EntWrk* pEntWrk, Column* pCol,
//                                 Stz* szElementSrc, Format* pFmt)
//{
//  i8 nElmSize;
//
//  char* azSrc = (char*)szElementSrc;
//  char* azDst = (char*)pEntWrk->ayElmNext();
//
//  if (DM_VCH == pCol->eDatum()) {
//    nElmSize = StrLib::iWordFlip(azDst,azSrc);
//    pEntWrk->vIncElmSeqSize(nElmSize);
//    return;
//  }
//
//  Datum oDatum;
//
//  bool b = CsvValue::bToDatum(&oDatum,pCol,&azSrc,pFmt);
//  ASSERT(b);
//
//  if (oDatum.bIsNull()) {
//    nElmSize = Null::nCopy((byte*)azDst);
//    pEntWrk->vIncElmSeqSize(nElmSize);
//    return;
//  }
//
//  nElmSize = Dtm::nCopy((byte*)azDst,oDatum.iInt());
//  pEntWrk->vIncElmSeqSize(nElmSize);
//}

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void KeyBuilder::vAddElement_int(EntWrk* pEntWrk, Column* pCol,
                                 Stz* szElementSrc, Format* pFmt)
{
  char* azSrc = (char*)szElementSrc;
  
  //ASSERT (DM_INT == pCol->eDatum());
  
  Datum oDatum;
  
  bool b = CsvValue::bToDatum(&oDatum,pCol,&azSrc,pFmt);
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
