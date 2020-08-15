/*============================================================================*/
/* EntBuffer_position.cpp -- position related functions for EntBuffer         */
/*============================================================================*/

#include "Indexing.h"

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given height starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindH0Buffer(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pEntBuffer;

  pEntBuffer = pFindH0Buffer_gen(aFindKey);
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aPosPriorToEntry(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aEnt;
  
  aEnt = aPosPriorToEntry_gen(aFindKey);

  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorToEntry(Key* aFindKey,
                                  Ent* aPriorVec[],
                                  Ent* aFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vPosPriorToEntry_gen(aFindKey,aPriorVec,aFindEntry);
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorToBuffer(Key* aFindKey,
                                   Ent* aPriorVec[],
                                   EntBuffer* pRefdChildBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  vPosPriorToBuffer_gen(aFindKey,aPriorVec,pRefdChildBuffer);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
