/*============================================================================*/
/* UteBuffer_h.inc -- Double-Ended Buffer                                     */
/*============================================================================*/
#pragma once

#define UTE_SIZE 10000000

struct UteBuffer
{
  i8    nLengthy;
  char* aLengthyDeposit;
  char* aLengthyLast;

  i8    nRegular;
  char* aRegularLast;
  i8    nRegularSize;
  char* aRegularAllot;
  
  char* aLowerBreak;
  char* aUpperBreak;
  
  char  caWorkSpace[UTE_SIZE];
};

inline i8    UTE_nAvailSpace(struct UteBuffer* aUteBuffer);
inline void  UTE_vInit(struct UteBuffer* aUteBuffer);
inline i8    UTE_nUnitOfSpace(struct UteBuffer* aUteBuffer);

inline char* UTE_aLengthyAllot(struct UteBuffer* aUteBuffer, i8 nLengthyBytes);
inline char* UTE_aLengthyFirst(struct UteBuffer* aUteBuffer);

inline char* UTE_aRegularAllot(struct UteBuffer* aUteBuffer);
inline bool  UTE_bRegularAvail(struct UteBuffer* aUteBuffer);
inline char* UTE_aRegularFirst(struct UteBuffer* aUteBuffer);
inline char* UTE_aRegularNext(struct UteBuffer* aUteBuffer, char* aPrevRegular);
inline char* UTE_aRegularStart(struct UteBuffer* aUteBuffer);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
