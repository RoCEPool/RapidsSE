/*============================================================================*/
/* Card_TablePtr.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_TablePtr.hpp"

#include "Card_IndexPtr.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexList.hpp"

#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_TablePtr, CK_TABLEPTR)
__aNewFunction(Card_TablePtr, CN_CARD)
__vDeleteFunction(Card_TablePtr, CN_CARD)

__bIsCard(STORE, Card_TablePtr, CK_TABLEPTR)
__pGetSubFunction_Card(STORE,TablePtr, CK_TABLEPTR)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtr::Card_TablePtr(Table* pTable)
: Card(CK_TABLEPTR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vTable(pTable);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TablePtr::~Card_TablePtr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_IndexList* Card_TablePtr::pNew_indexList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Table* pTable = this->pTable();

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec)
    return nullptr;

  Card_IndexList* p_indexList = new Card_IndexList;

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);

    Card_IndexPtr* pCard_index = new Card_IndexPtr(pIndex);
    p_indexList->vAppend(pCard_index);

    Card_IdxColList* pCard_idxColList = new Card_IdxColList(pIndex);
    pCard_index->vAppend(pCard_idxColList);
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
StreamDef* Card_TablePtr::pStreamDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Table *pTable = this->pTable();
  if (! pTable)
    return nullptr;

  StreamDef* pStreamDef = pTable->pStreamDef();

  return pStreamDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
