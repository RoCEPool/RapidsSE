/*============================================================================*/
/* Partitions.cpp - table partitions                                          */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Partitions::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Partitions class cannot be new'd");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Partitions::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: Partitions class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Partitions::Partitions(void)
: Object(CN_PARTITIONS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONS,_FOOTER_);
  
  _checkInitialised_(CN_PARTITIONS);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partitions::~Partitions(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
Partition* Partitions::pPartition(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONS,_FOOTER_);
  
  //vTrace("Partition %lld\n",iPartition);
  
  return &m_xPartition[iPartition];
}

/*----------------------------------------------------------------------------*/
/* tuncate all partitions                                                     */
/*----------------------------------------------------------------------------*/
void Partitions::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONS,_FOOTER_);
  i8 iPartn;
  for (iPartn = 0; iPartn < gxArguments.m_nPartitions; iPartn++)
    m_xPartition[iPartn].vTruncate();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Partitions::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITIONS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/