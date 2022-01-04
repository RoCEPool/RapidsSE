/*============================================================================*/
/* UteBuffer_c.inc -- Utility Buffer                                          */
/*============================================================================*/

#include "UteBuffer_h.inc"

/*----------------------------------------------------------------------------*/
/*                                buffer                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* how much free space is available?                                          */
/*----------------------------------------------------------------------------*/
inline i8 UTE_nAvailSpace(struct UteBuffer* aUteBuffer)
{
  char* aRegularsStart = aUteBuffer->aUpperBreak
                           - (aUteBuffer->nRegular * aUteBuffer->nRegularSize);
  
  i8 nSpace = aUteBuffer->aUpperBreak - aRegularsStart;
  
  return nSpace;
}

/*----------------------------------------------------------------------------*/
/* initialise the utility structure                                           */
/*----------------------------------------------------------------------------*/
inline void UTE_vInit(struct UteBuffer* aUteBuffer)
{
  memset(aUteBuffer,0,sizeof(struct UteBuffer));
  
  aUteBuffer->aLengthyDeposit = aUteBuffer->caWorkSpace;
  
  aUteBuffer->aLowerBreak = & aUteBuffer->caWorkSpace[-1];
  aUteBuffer->aUpperBreak = & aUteBuffer->caWorkSpace[UTE_SIZE];
}

/*----------------------------------------------------------------------------*/
/* get unit of space                                                          */
/*----------------------------------------------------------------------------*/
inline i8 UTE_nUnitOfSpace(struct UteBuffer* aUteBuffer)
{
  return 16;
}

/*----------------------------------------------------------------------------*/
/*                                lengthy                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a lengthy                                                            */
/*----------------------------------------------------------------------------*/
inline char* UTE_aLengthyAllot(struct UteBuffer* aUteBuffer, i8 nLengthyBytes)
{
  if (nLengthyBytes > UTE_nAvailSpace(aUteBuffer))
    return nullptr;
  
  aUteBuffer->aLengthyLast = aUteBuffer->aLengthyDeposit;
  aUteBuffer->aLengthyDeposit += nLengthyBytes;
  
  return aUteBuffer->aLengthyLast;
}

/*----------------------------------------------------------------------------*/
/* get first lengthy                                                          */
/*----------------------------------------------------------------------------*/
inline char* UTE_aLengthyFirst(struct UteBuffer* aUteBuffer)
{
  if (! aUteBuffer->nLengthy)
    return nullptr;
  
  return aUteBuffer->caWorkSpace;
}

/*----------------------------------------------------------------------------*/
/*                                regular                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a regular                                                            */
/*----------------------------------------------------------------------------*/
inline char* UTE_aRegularAllot(struct UteBuffer* aUteBuffer)
{
  if (! UTE_bRegularAvail(aUteBuffer))
    return nullptr;
  
  aUteBuffer->aRegularLast = UTE_aRegularStart(aUteBuffer)
                               - aUteBuffer->nRegularSize;
  
  return aUteBuffer->aRegularLast;
}

/*----------------------------------------------------------------------------*/
/* is there space for a regular?                                              */
/*----------------------------------------------------------------------------*/
inline bool UTE_bRegularAvail(struct UteBuffer* aUteBuffer)
{
  if (UTE_nAvailSpace(aUteBuffer) < aUteBuffer->nRegularSize)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get first regular                                                          */
/*----------------------------------------------------------------------------*/
inline char* UTE_aRegularFirst(struct UteBuffer* aUteBuffer)
{
  if (! aUteBuffer->nRegular)
    return nullptr;
  
  char* aFirst = aUteBuffer->aUpperBreak - aUteBuffer->nRegular;
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* get next regular                                                           */
/*----------------------------------------------------------------------------*/
inline char* UTE_aRegularNext(struct UteBuffer* aUteBuffer, char* aPrevRegular)
{
  char* aNext = aPrevRegular - aUteBuffer->nRegular;
  
  if (aNext < aUteBuffer->aRegularLast)
    return nullptr;
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* how much free space is available?                                          */
/*----------------------------------------------------------------------------*/
inline char* UTE_aRegularStart(struct UteBuffer* aUteBuffer)
{
  char* aRegularsStart = aUteBuffer->aUpperBreak
                           - (aUteBuffer->nRegular * aUteBuffer->nRegularSize);
    
  return aRegularsStart;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
