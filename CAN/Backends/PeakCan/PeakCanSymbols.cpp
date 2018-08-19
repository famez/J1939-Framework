/*
 * PeakCanSymbols.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: famez
 */

#include <dlfcn.h>


#include <Backends/PeakCan/PeakCanSymbols.h>

namespace Can {
namespace PeakCan {

bool PeakCanSymbols::tryLoadSymbols() {

	void* handle = dlopen(PEAKCAN_LIB, RTLD_LAZY);

	if (!handle) {
		mLoadingError = true;
		mSymbolsLoaded = false;
		return false;
	}

	//Reset errors
	dlerror();

	CAN_Initialize = (CAN_InitializePtr)(dlsym(handle, "CAN_Initialize"));
	if (dlerror()) goto dll_error;

	CAN_Uninitialize = (CAN_UninitializePtr)(dlsym(handle, "CAN_Uninitialize"));
	if (dlerror()) goto dll_error;

	CAN_Reset = (CAN_ResetPtr)(dlsym(handle, "CAN_Reset"));
	if (dlerror()) goto dll_error;

	CAN_Read = (CAN_ReadPtr)(dlsym(handle, "CAN_Read"));
	if (dlerror()) goto dll_error;

	CAN_Write = (CAN_WritePtr)(dlsym(handle, "CAN_Write"));
	if (dlerror()) goto dll_error;

	CAN_FilterMessages = (CAN_FilterMessagesPtr)(dlsym(handle, "CAN_FilterMessages"));
	if (dlerror()) goto dll_error;

	CAN_GetValue = (CAN_GetValuePtr)(dlsym(handle, "CAN_GetValue"));
	if (dlerror()) goto dll_error;

	CAN_SetValue = (CAN_SetValuePtr)(dlsym(handle, "CAN_SetValue"));
	if (dlerror()) goto dll_error;

	CAN_GetErrorText = (CAN_GetErrorTextPtr)(dlsym(handle, "CAN_GetErrorText"));
	if (dlerror()) goto dll_error;




	//Success reading all the symbols
	mSymbolsLoaded = true;
	mLoadingError = false;

	return true;


	dll_error:

	dlclose(handle);

	mSymbolsLoaded = false;
	mLoadingError = true;

	return false;

}


} /* namespace PeakCan */
} /* namespace Can */
