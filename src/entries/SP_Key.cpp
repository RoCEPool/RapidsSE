/*============================================================================*/
/* SP_Key.cpp - Key sprint helper                                             */
/*============================================================================*/

#include "Entries.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Key::SP_Key(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);
  
  //! FIX RAY
  //_checkInitialised_(CN_SPSMALL);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Key::SP_Key(Key* aKey, IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);
    
  //! FIX RAY
  //_checkInitialised_(CN_SPSMALL);
  
  vSprintKey(aKey,pIndexDef);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Key::~SP_Key(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Key::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Key::sz(Key* aKey, IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vSprintKey(aKey,pIndexDef);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint the key                                                             */
/*----------------------------------------------------------------------------*/
void SP_Key::vSprintKey(Key* aKey, IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nElmBytes;

  char caLiteral[128];
  char caElm[128];

  vReset();

  if (KE_INT == pIndexDef->eKeyKind()) {
    
    Column* pCol = pIndexDef->pGetColumn(0);
    
    StrLib::vSprint((Stz*)m_caResult,"IntElm0 %lls %lld"_Stz,
                    aDM[pCol->eDatum()],
                    *(i8*)aKey);
    return;
  }
      
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  byte* ayCursor = aKey->ayElmSeq();
  

  StrLib::vSprint((Stz*)m_caResult,"Key[%lldB]; "_Stz,aKey->nElmSeqSize());

  i8 nIdxCols = pIndexDef->nColumns();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {
    
    Column* pCol = pIndexDef->pGetColumn(i_IdxCol);
    
    Dtm* ayElm = (Dtm*)EntWrk::ayEachElm(pCol,nElmBytes,&ayCursor);
    
    eDM eDatum = pCol->eDatum();
    
    if (eDatum == DM_VCH)
      nElmBytes = StrLib::iWordFlipBack(caLiteral, (char*)ayElm);
    else
      Literal::vSprint((Stz*)caLiteral,ayElm,pCol);
 
    if (i_IdxCol)
      StrLib::vConcat(m_caResult,", ");

    StrLib::vSprint((Stz*)caElm,"Elm[%lld %s %lldB] %s"_Stz,
                    i_IdxCol,
                    aDM[pCol->eDatum()],
                    nElmBytes,
                    caLiteral);
    
    StrLib::vConcat(m_caResult,caElm);
  }

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Key)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
