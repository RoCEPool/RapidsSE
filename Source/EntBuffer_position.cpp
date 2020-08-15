/*============================================================================*/
/* EntBuffer_position.cpp -- position related functions for EntBuffer         */
/*============================================================================*/

#include "Indexing.h"

/*----------------------------------------------------------------------------*/
/* position at a leaf buffer starting from this buffer                        */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindLeafBuffer(Key* aForKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  EntBuffer* pEntBuffer;

  if (KE_INT == m_eEntKind)
    pEntBuffer = pFindLeafBuffer_int(aForKey);
  else
    pEntBuffer = pFindLeafBuffer_gen(aForKey);
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry before given key                      */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aFindEntryBefore(Key* aGivenKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  Ent* aEnt;
  
  if (KE_INT == m_eEntKind)
    aEnt = aFindEntryBefore_int(aGivenKey);
  else
    aEnt = aFindEntryBefore_gen(aGivenKey);

  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosVecPriorToEntry(Ent* aToEntry,
                                 Key* aWithKey,
                                 Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  if (KE_INT == m_eEntKind)
    vPosVecPriorToEntry_int(aToEntry,aWithKey,aPriorVec);
  else
    vPosVecPriorToEntry_gen(aToEntry,aWithKey,aPriorVec);
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosVecPriorToBuffer(EntBuffer* pToRefdBuffer,
                                  Key* aWithKey,
                                  Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  if (KE_INT == m_eEntKind)
    vPosVecPriorToBuffer_int(pToRefdBuffer,aWithKey,aPriorVec);
  else
    vPosVecPriorToBuffer_gen(pToRefdBuffer,aWithKey,aPriorVec);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
