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
  char* azDst = (char*)pEntWrk->ayElmNext_dtm();
  
  if (DM_VCH == pCol->eDatum()) {
    nElmSize = StrLib::iWordFlip(azDst,azSrc);
    pEntWrk->vIncElmSeqSize(nElmSize);
    return;
  }
  
  Dtm* aCell = (Dtm*)azDst;
  
  bool b = Literal::bToCell(aCell,pCol,&azSrc,pFmt);
  nElmSize = sizeof(Dtm);
  pEntWrk->vIncElmSeqSize(nElmSize);
}

/*----------------------------------------------------------------------------*/
/* add element to entwrk key                                                  */
/*----------------------------------------------------------------------------*/
void KeyBuilder::vAddElement_int(EntWrk* pEntWrk, Column* pCol,
                                 Stz* szElementSrc, Format* pFmt)
{
  char* azSrc = (char*)szElementSrc;
    
  pEntWrk->aEnt()->vKind(KE_INT);

  Dtm* aCell = pEntWrk->aEnt()->aIntElm0();
  bool b = Literal::bToCell(aCell,pCol,&azSrc,pFmt);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
