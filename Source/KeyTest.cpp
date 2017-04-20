/*============================================================================*/
/* KeyTest.cpp -- key library / test                                          */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* key testing                                                                */
/*----------------------------------------------------------------------------*/
void KeyTest::vTest(void)
{
  vTrace("\n\nKeyTest:*************************************************\n"_Str);

  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  KeyVal* pKeyVal;
  
  Column* pCol = new Column("Col0"_Str);
  ColumnVec* pColVec = new ColumnVec;
  pColVec->vAdd(pCol);
  
  RelOpAry* pOpAryFind = new RelOpAry;
  
  KeyVec* pKeyVec = new KeyVec;
  
  // create a vector of key values (ascending order 0,1,2, ... 9)
  for (i8 i = 0; i < 10; i++) {
  
    xValWsp = i;
    
    xKeyWsp.vReset();
    xKeyWsp.vAddElement(xValWsp);
    
    pKeyVal = new KeyVal(xKeyWsp);
    
    pKeyVec->vAdd(pKeyVal);
  }
  
  i8 v;
  i8 nBytes;
  char* aCursor;
  
  // dump the key vector
  for (i8 i = 0; i < 10; i++) {
    
    pKeyVal = pKeyVec->pGet(i);
    
    aCursor = pKeyVal->m_xKey.aWspArea();
    v = *(i8*)pKeyVal->aElement(pCol,nBytes,&aCursor);
    
    vTrace("%lld: %lld\n"_Str,i,v);
  }

  // asserts that keyval(n) < keyval(n+1)
  KeyVal* pKeyValA = pKeyVec->pGet(0);
  for (i8 i = 1; i < 10; i++) {
    
    KeyVal* pKeyValB = pKeyVec->pGet(i);
    ASSERT(pKeyValA->bLessThan(pKeyValB,pColVec));
    
    ASSERT(pKeyValA->iCmp(pKeyValB,pColVec) < 0);
    
    pKeyValA = pKeyValB;
  }
  
  // scan the key vector for key value: >= 3
  xValWsp = 3;
  xKeyWsp.vReset();
  xKeyWsp.vAddElement(xValWsp);
  KeyVal* pKeyValFind = new KeyVal(xKeyWsp);
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  pKeyVal = nullptr;
  for (i8 i = 0; i < 10; i++) {

    pKeyVal = pKeyVec->pGet(i);
    
    if (pKeyVal->bMatches(pKeyValFind,pColVec,pOpAryFind))
      break;
    
    pKeyVal = nullptr;
  }
  
  if (pKeyVal) {
    aCursor = pKeyVal->m_xKey.aWspArea();
    v = *(i8*)pKeyVal->aElement(pCol,nBytes,&aCursor);
    
    vTrace("%p: %lld\n"_Str,pKeyVal,v);
  }
  
  delete pKeyVec;
  delete pColVec;
  delete pOpAryFind;
  
  vTrace("\n\nEnd KeyTest**********************************************\n"_Str);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
