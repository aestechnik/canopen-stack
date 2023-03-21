/******************************************************************************
   Copyright 2023 AES Autonome Energiesysteme GmbH

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/
#ifndef CO_REAL32_H_
#define SRC_OBJECT_BASIC_CO_REAL32_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"
#include "co_obj.h"

/******************************************************************************
* DEFINES
******************************************************************************/

#define CO_TREAL32 ((const CO_OBJ_TYPE *)&COTReal32)

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE REAL32
*
*    This type is a basic type for 32bit float values
*/
extern const CO_OBJ_TYPE COTReal32;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* CO_REAL32_H_ */
