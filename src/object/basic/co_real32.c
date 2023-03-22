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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PRIVATE TYPEDEFS
******************************************************************************/

typedef union {
    uint32_t rawValue;
    float floatValue;
} floatToUInt;

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define COT_ENTRY_SIZE    (uint32_t)4

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static uint32_t COTReal32Size (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTReal32Read (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTReal32Write(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTReal32Init (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTReal32 = { COTReal32Size, COTReal32Init, COTReal32Read, COTReal32Write, 0 };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

static uint32_t COTReal32Size(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    uint32_t result = (uint32_t)0;

    CO_UNUSED(node);
    CO_UNUSED(width);

    if (CO_IS_DIRECT(obj->Key) != 0) {
        result = (uint32_t)COT_ENTRY_SIZE;
    } else {
        /* check for valid reference */
        if ((obj->Data) != (CO_DATA)0) {
            result = (uint32_t)COT_ENTRY_SIZE;
        }
    }
    return (result);
}

static CO_ERR COTReal32Read(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    CO_ERR result = CO_ERR_NONE;
    floatToUInt value;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);

    if (CO_IS_DIRECT(obj->Key) != 0) {
    	value.rawValue = (uint32_t)(obj->Data);
    } else {
        uint32_t* rawValuePtr = (uint32_t*)(obj->Data);
        value.rawValue = *rawValuePtr;
    }

    if (size == COT_ENTRY_SIZE) {
        *((float*)buffer) = value.floatValue;
    } else {
        result = CO_ERR_BAD_ARG;
    }

    return result;
}

static CO_ERR COTReal32Write(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    CO_ERR result = CO_ERR_NONE;
    floatToUInt value;
    uint32_t encodedValue;
    uint32_t oldValue;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);

    value.floatValue = *((float *)buffer);
    encodedValue = *((uint32_t *)(&value.rawValue)); // get IEEE754 encoding of float value

    if (size == COT_ENTRY_SIZE) {
    	//CO_IS_NODEID is not supported
        if (CO_IS_DIRECT(obj->Key) != 0) {
            oldValue = (uint32_t)obj->Data;
            obj->Data = (CO_DATA)(encodedValue);
        } else {
            oldValue = *((uint32_t *)(obj->Data));
            *((uint32_t *)(obj->Data)) = encodedValue;
        }
        if ((CO_IS_ASYNC(obj->Key)  != 0    ) &&
            (CO_IS_PDOMAP(obj->Key) != 0    ) &&
            (oldValue               != encodedValue)) {
            COTPdoTrigObj(node->TPdo, obj);
        }
    } else {
        result = CO_ERR_BAD_ARG;
    }

    return result;
}

static CO_ERR COTReal32Init(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
	CO_ERR result = CO_ERR_TYPE_INIT;

	CO_UNUSED(node);
	ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

	//check for disabled direct storage - during OD building CO_Data will cast a direct float value to a uint
    if (CO_IS_DIRECT(obj->Key) == 0) {
		result = CO_ERR_NONE;
	}
	return(result);
}
