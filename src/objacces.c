/*!
** @file   objacces.c
** @author Edouard TISSERANT and Francis DUPIN
** @date   Tue Jun  5 08:55:23 2007
**
** @brief
**
**
*/




/* #define DEBUG_WAR_CONSOLE_ON */
/* #define DEBUG_ERR_CONSOLE_ON */

#include "objacces.h"
#include "data.h"
#include <string.h>


void *memcpy_flash(void * dest, const CONSTSTORE void * source, size_t length) {
    char* dstPointer = dest;
    const CONSTSTORE char* srcPointer = source;
    size_t i;
    for(i = 0; i < length; i++) {
        dstPointer[i] = srcPointer[i];
    }
    return dstPointer;
}

//We need the function implementation for linking
//Only a placeholder with a define isnt enough!
UNS8 accessDictionaryError(UNS16 index, UNS8 subIndex,
                           UNS32 sizeDataDict, UNS32 sizeDataGiven, UNS32 code)
{
#ifdef DEBUG_WAR_CONSOLE_ON
  MSG_WAR(0x2B09,"Dictionary index : ", index);
  MSG_WAR(0X2B10,"           subindex : ", subIndex);
  switch (code) {
  case  OD_NO_SUCH_OBJECT:
    MSG_WAR(0x2B11,"Index not found ", index);
    break;
  case OD_NO_SUCH_SUBINDEX :
    MSG_WAR(0x2B12,"SubIndex not found ", subIndex);
    break;
  case OD_WRITE_NOT_ALLOWED :
    MSG_WAR(0x2B13,"Write not allowed, data is read only ", index);
    break;
  case OD_LENGTH_DATA_INVALID :
    MSG_WAR(0x2B14,"Conflict size data. Should be (bytes)  : ", sizeDataDict);
    MSG_WAR(0x2B15,"But you have given the size  : ", sizeDataGiven);
    break;
  case OD_NOT_MAPPABLE :
    MSG_WAR(0x2B16,"Not mappable data in a PDO at index    : ", index);
    break;
  case OD_VALUE_TOO_LOW :
    MSG_WAR(0x2B17,"Value range error : value too low. SDOabort : ", code);
    break;
  case OD_VALUE_TOO_HIGH :
    MSG_WAR(0x2B18,"Value range error : value too high. SDOabort : ", code);
    break;
  default :
    MSG_WAR(0x2B20, "Unknown error code : ", code);
  }
  #endif

  return 0;
}

UNS32 _getODentry( CO_Data* d,
                   UNS16 wIndex,
                   UNS8 bSubindex,
                   void * pDestData,
                   UNS32 * pExpectedSize,
                   UNS8 * pDataType,
                   UNS8 checkAccess,
                   UNS8 endianize)
{ /* DO NOT USE MSG_ERR because the macro may send a PDO -> infinite
    loop if it fails. */
  (void)endianize;
  UNS32 errorCode;
  UNS32 szData;
  const CONSTSTORE indextable *ptrTable;
  ODCallback_t *Callback;

  ptrTable = (*d->scanIndexOD)(wIndex, &errorCode, &Callback);

  if (errorCode != OD_SUCCESSFUL)
    return errorCode;
  if( ptrTable->bSubCount <= bSubindex ) {
    /* Subindex not found */
    accessDictionaryError(wIndex, bSubindex, 0, 0, OD_NO_SUCH_SUBINDEX);
    return OD_NO_SUCH_SUBINDEX;
  }

  if (checkAccess && (ptrTable->pSubindex[bSubindex].bAccessType == WO)) {
    MSG_WAR(0x2B30, "Access Type : ", ptrTable->pSubindex[bSubindex].bAccessType);
    accessDictionaryError(wIndex, bSubindex, 0, 0, OD_READ_NOT_ALLOWED);
    return OD_READ_NOT_ALLOWED;
  }

  if (pDestData == 0) {
    return SDOABT_GENERAL_ERROR;
  }

  if (ptrTable->pSubindex[bSubindex].size > *pExpectedSize) {
    /* Requested variable is too large to fit into a transfer line, inform    *
     * the caller about the real size of the requested variable.              */
    *pExpectedSize = ptrTable->pSubindex[bSubindex].size;
    return SDOABT_OUT_OF_MEMORY;
  }

  *pDataType = ptrTable->pSubindex[bSubindex].bDataType;
  szData = ptrTable->pSubindex[bSubindex].size;

#  ifdef CANOPEN_BIG_ENDIAN
  if(endianize && *pDataType > boolean && !(
         *pDataType >= visible_string &&
         *pDataType <= domain)) {
    /* data must be transmited with low byte first */
    UNS8 i, j = 0;
    MSG_WAR(boolean, "data type ", *pDataType);
    MSG_WAR(visible_string, "data type ", *pDataType);
    for ( i = szData ; i > 0 ; i--) {
      MSG_WAR(i," ", j);
      ((UNS8*)pDestData)[j++] = ((UNS8*)ptrTable->pSubindex[bSubindex].bAccessType == CONST ? ptrTable->pSubindex[bSubindex].pObjectConst)[i-1] :
        ptrTable->pSubindex[bSubindex].pObject)[i-1];
    }
    *pExpectedSize = szData;
  }
  else /* no endianisation change */
#  endif
  if(ptrTable->pSubindex[bSubindex].bAccessType == CONST) {
      
      if(ptrTable->pSubindex[bSubindex].bDataType == visible_string && bSubindex != 0) {
          const CONSTSTORE char* dp = *(const CONSTSTORE char* const CONSTSTORE *)ptrTable->pSubindex[bSubindex].pObjectConst;
          memcpy_flash(pDestData, dp,szData);
      } else {
          memcpy_flash(pDestData, ptrTable->pSubindex[bSubindex].pObjectConst,szData);
      }
  } else {
      memcpy(pDestData, ptrTable->pSubindex[bSubindex].pObject,szData);
  }

  if(*pDataType != visible_string)
      *pExpectedSize = szData;
  else {
    /* VISIBLE_STRING objects are returned with \0 termination, if the user   *
     * provided enough space.                                                 *
     * Note:  If the parameter "Default String Size" of the Object Dictionary *
     *        Editor is larger than the string, then the \0 byte will be      *
     *        appended anyways!                                               */
    if(*pExpectedSize > ptrTable->pSubindex[bSubindex].size) {
      *((UNS8*)pDestData + szData) = '\0';
      *pExpectedSize = szData + 1;
    }
    else
       *pExpectedSize = szData;
  }
  return OD_SUCCESSFUL;
}

UNS32 _setODentry( CO_Data* d,
                   UNS16 wIndex,
                   UNS8 bSubindex,
                   void * pSourceData,
                   UNS32 * pExpectedSize,
                   UNS8 checkAccess,
                   UNS8 endianize)
{
  (void)endianize;
  UNS32 szData;
  UNS8 dataType;
  UNS32 errorCode;
  const CONSTSTORE indextable *ptrTable;
  ODCallback_t *Callback;

  ptrTable =(*d->scanIndexOD)(wIndex, &errorCode, &Callback);
  if (errorCode != OD_SUCCESSFUL)
    return errorCode;

  if( ptrTable->bSubCount <= bSubindex ) {
    /* Subindex not found */
    accessDictionaryError(wIndex, bSubindex, 0, *pExpectedSize, OD_NO_SUCH_SUBINDEX);
    return OD_NO_SUCH_SUBINDEX;
  }
  if (checkAccess && (ptrTable->pSubindex[bSubindex].bAccessType == RO || ptrTable->pSubindex[bSubindex].bAccessType == CONST)) {
    MSG_WAR(0x2B25, "Access Type : ", ptrTable->pSubindex[bSubindex].bAccessType);
    accessDictionaryError(wIndex, bSubindex, 0, *pExpectedSize, OD_WRITE_NOT_ALLOWED);
    return OD_WRITE_NOT_ALLOWED;
  }


  dataType = ptrTable->pSubindex[bSubindex].bDataType;
  szData = ptrTable->pSubindex[bSubindex].size;

  /* check the size, we must allow to store less bytes than data size, even for intergers
	 (e.g. UNS40 : objdictedit will store it in a uint64_t, setting the size to 8 but PDO comes
	 with 5 bytes so ExpectedSize is 5 */
  if( *pExpectedSize == 0 || *pExpectedSize <= szData )
    {
#ifdef CANOPEN_BIG_ENDIAN
      /* re-endianize do not occur for bool, strings time and domains */
      if(endianize && dataType > boolean && !(
            dataType >= visible_string && 
            dataType <= domain))
        {
          /* we invert the data source directly. This let us do range
            testing without */
          /* additional temp variable */
          UNS8 i;
          for ( i = 0 ; i < ( ptrTable->pSubindex[bSubindex].size >> 1)  ; i++)
            {
              UNS8 tmp =((UNS8 *)pSourceData) [(ptrTable->pSubindex[bSubindex].size - 1) - i];
              ((UNS8 *)pSourceData) [(ptrTable->pSubindex[bSubindex].size - 1) - i] = ((UNS8 *)pSourceData)[i];
              ((UNS8 *)pSourceData)[i] = tmp;
            }
        }
#endif
      errorCode = (*d->valueRangeTest)(dataType, pSourceData);
      if (errorCode) {
        accessDictionaryError(wIndex, bSubindex, szData, *pExpectedSize, errorCode);
        return errorCode;
      }
      memcpy(ptrTable->pSubindex[bSubindex].pObject,pSourceData, *pExpectedSize);
     /* TODO : CONFORM TO DS-301 : 
      *  - stop using NULL terminated strings
      *  - store string size in td_subindex 
      * */
      /* terminate visible_string with '\0' */
      if(dataType == visible_string && *pExpectedSize < szData) {
          ((UNS8*)ptrTable->pSubindex[bSubindex].pObject)[*pExpectedSize] = 0;
      }

      *pExpectedSize = szData;

      /* Callbacks */
      if(Callback && Callback[bSubindex]){
        errorCode = (Callback[bSubindex])(d, wIndex, bSubindex);
        if(errorCode != OD_SUCCESSFUL)
        {
            return errorCode;
        }
       }

      /* Store value if requested with user defined function
	     Function should return OD_ACCES_FAILED in case of store error */
      if (ptrTable->pSubindex[bSubindex].bAccessType & TO_BE_SAVE){
        return (*d->storeODSubIndex)(d, wIndex, bSubindex);
      }
      return OD_SUCCESSFUL;
    }else{
      *pExpectedSize = szData;
      accessDictionaryError(wIndex, bSubindex, szData, *pExpectedSize, OD_LENGTH_DATA_INVALID);
      return OD_LENGTH_DATA_INVALID;
    }
}

const CONSTSTORE indextable * scanIndexOD (CO_Data* d, UNS16 wIndex, UNS32 *errorCode, ODCallback_t **Callback)
{
  return (*d->scanIndexOD)(wIndex, errorCode, Callback);
}

UNS32 RegisterSetODentryCallBack(CO_Data* d, UNS16 wIndex, UNS8 bSubindex, ODCallback_t Callback)
{
UNS32 errorCode;
ODCallback_t *CallbackList;
const CONSTSTORE indextable *odentry;

  odentry = scanIndexOD (d, wIndex, &errorCode, &CallbackList);
  if(errorCode == OD_SUCCESSFUL  &&  CallbackList  &&  bSubindex < odentry->bSubCount) 
    CallbackList[bSubindex] = Callback;
  return errorCode;
}

UNS32 _storeODSubIndex (CO_Data* d, UNS16 wIndex, UNS8 bSubindex)
{
  (void)d;
  (void)wIndex;
  (void)bSubindex;
  return OD_SUCCESSFUL;
}
