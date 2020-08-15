/*============================================================================*/
/* EntBuffer_position.cpp -- position related functions for EntBuffer         */
/*============================================================================*/

#include "Indexing.h"

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given height starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindH0Buffer(Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pEntBuffer;

  if (KE_INT == m_eEntKind)
    pEntBuffer = pFindH0Buffer_int(aWithKey);
  else
    pEntBuffer = pFindH0Buffer_gen(aWithKey);
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aPosPriorToEntry(Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aEnt;
  
  if (KE_INT == m_eEntKind)
    aEnt = aPosPriorToEntry_int(aWithKey);
  else
    aEnt = aPosPriorToEntry_gen(aWithKey);

  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorToEntry(Key* aWithKey,
                                  Ent* aPriorVec[],
                                  Ent* aToEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (KE_INT == m_eEntKind)
    vPosPriorToEntry_int(aWithKey,aPriorVec,aToEntry);
  else
    vPosPriorToEntry_gen(aWithKey,aPriorVec,aToEntry);
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorToBuffer(Key* aWithKey,
                                   Ent* aPriorVec[],
                                   EntBuffer* pRefdChildBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  if (KE_INT == m_eEntKind)
    vPosPriorToBuffer_int(aWithKey,aPriorVec,pRefdChildBuffer);
  else
    vPosPriorToBuffer_gen(aWithKey,aPriorVec,pRefdChildBuffer);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
