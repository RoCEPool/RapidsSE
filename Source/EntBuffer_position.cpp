/*============================================================================*/
/* EntBuffer_position.cpp -- position related functions for EntBuffer         */
/*============================================================================*/

#include "Indexing.h"

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given height starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pPosH0Buffer(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pEntBuffer;

  pEntBuffer = pPosH0Buffer_gen(aFindKey);
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aPosPriorEntry(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aEnt;
  
  aEnt = aPosPriorEntry_gen(aFindKey);

  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorVecEntry(Key* aFindKey,
                                  Ent* aPriorVec[],
                                  Ent* aFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  vPosPriorVecEntry_gen(aFindKey,aPriorVec,aFindEntry);
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorVecBuffer(Key* aFindKey,
                                   Ent* aPriorVec[],
                                   EntBuffer* pRefdChildBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  vPosPriorVecBuffer_gen(aFindKey,aPriorVec,pRefdChildBuffer);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
