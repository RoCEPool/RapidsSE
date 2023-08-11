/*============================================================================*/
/* Card_TableSize.cpp -- Size (of table)                                           */
/*============================================================================*/

#include "Card_TableSize.hpp"

#include "Card_Table.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_TableSize, CK_TABLESIZE)
__aNewFunction(Card_TableSize,CN_CARD)
__vDeleteFunction(Card_TableSize,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TableSize::Card_TableSize(void)
: Card_symbol(CK_TABLESIZE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TableSize::~Card_TableSize(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* produce <size> of table                                                    */
/*----------------------------------------------------------------------------*/
Card_TableSize* Card_TableSize::pDerive(Table* pTable)
{
  _static_

  if (! pTable) {
    vTrace("A: m_pTable not set\n"_Stz);
    return nullptr;
  }

//  i8 jTableSize = pTable->pStreamDef()->jTableSize();

  i8 jTableSize = 0;
  i8 jRowSize   = 0;
  i8 nRowCount  = 0;

  Card_Table::vGetSizes(pTable,jTableSize,jRowSize,nRowCount);

  Card_TableSize* p_size = new Card_TableSize;
  p_size->SYM(SF_INT_LIT,jTableSize);

  return p_size;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
