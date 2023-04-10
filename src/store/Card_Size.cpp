/*============================================================================*/
/* Card_Size.cpp -- Size (of table)                                           */
/*============================================================================*/

#include "Card_Size.hpp"

#include "Card_Table.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Size,CK_SIZE)
__aNewFunction(Card_Size,CN_CARD)
__vDeleteFunction(Card_Size,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Size::Card_Size(void)
: Card(CK_SIZE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Size::~Card_Size(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* produce <size> of table                                                    */
/*----------------------------------------------------------------------------*/
Card_Size* Card_Size::pDerive(Table* pTable)
{
  _static_

  if (! pTable) {
    vTrace("A: m_pTable not set\n"_Stz);
    return nullptr;
  }

//  i8 jTableSize = pTable->pStreamDef()->jTableSize();

  i8 jTableSize = 0;
  i8 jRowSize   = 0;

  Card_Table::vGetSizes(pTable,jTableSize,jRowSize);

  Card_Size* p_size = new Card_Size;
  s32 sSize(jTableSize);
  p_size->vValue(sSize);

  return p_size;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
