/*============================================================================*/
/* Tools.cpp -- Drafting Tools                                                */
/*============================================================================*/

#include "Tools.hpp"
#include "Bench.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Tools,CN_TOOLS)
__aNewFunction(Tools,CN_TOOLS)
__vDeleteFunction(Tools,CN_TOOLS)
__aFooterFunction(Tools)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tools::Tools(void)
: Object(CN_TOOLS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
    
  _checkInitialised_(CN_TOOLS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tools::~Tools(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* order stream def by given byList                                           */
/*----------------------------------------------------------------------------*/
void Tools::vOrderStreamDef(StreamDef* pStreamDef,
                            Card_GbyList* pCard_ByList)
{
  _static_

  ListExoIter2_t<Card>* pByList_I = pCard_ByList->pNewIter();

  while (pByList_I->bHasNext()) {
    Card* pPart = pByList_I->pNext();
    Card_By* pCard_By = _pCard_By(pPart);

    Card_ByPred* pCard_ByPred
      = _pCard_ByPred(pCard_By->pFindSubCard(CK_BYPRED));
    if (! pCard_ByPred)
      return;
    
    Card_Term* pCard_Term = _pCard_Term(pCard_ByPred->pGet(0));
    Stz* szByColumnNm =pCard_Term->szGetProperty("szTerm"_Stz);
    
    Column* pStreamColumn = pStreamDef->pFindColumn(szByColumnNm);
    i8 iColNo = pStreamColumn->iColumnNo();
    pStreamDef->vAddOrder(iColNo);
  }
  
  delete pByList_I;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
