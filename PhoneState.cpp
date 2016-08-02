// PhoneState.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "PhoneState.h"
#include <windows.h>
#include <commctrl.h>
#include "WirelessDevices.h"


_GetWirelessDevices		pGetWirelessDevices = NULL;
_ChangeRadioState		pChangeRadioState = NULL;
_FreeDeviceList			pFreeDeviceList = NULL;

HINSTANCE	g_DllWrlspwr;
BOOL InitDLL()
{
	g_DllWrlspwr = LoadLibrary(TEXT("ossvcs.dll"));
	if (g_DllWrlspwr == NULL)
		return FALSE;
	pGetWirelessDevices   = (_GetWirelessDevices)GetProcAddress(g_DllWrlspwr,MAKEINTRESOURCE(GetWirelessDevice_ORDINAL));
	if (pGetWirelessDevices == NULL)
		return FALSE;
	
	pChangeRadioState   = (_ChangeRadioState)GetProcAddress(g_DllWrlspwr,MAKEINTRESOURCE(ChangeRadioState_ORDINAL));
	if (pChangeRadioState == NULL)
		return FALSE;
	
	pFreeDeviceList	   = (_FreeDeviceList)GetProcAddress(g_DllWrlspwr,MAKEINTRESOURCE(FreeDeviceList_ORDINAL));
	if (pFreeDeviceList == NULL)
		return FALSE;
	return TRUE;
}

BOOL DeinitDLL()
{
	return FreeLibrary(g_DllWrlspwr);
}
//set the status of the desired wireless device
DWORD SetWDevState(DWORD dwDevice, DWORD dwState)
{
	RDD * pDevice = NULL;
    RDD * pTD;
    HRESULT hr;
	DWORD retval = 0;

//	InitDLL();
    hr = pGetWirelessDevices(&pDevice, 0);
	if(hr != S_OK) return -1;
    
    if (pDevice)
    {
        pTD = pDevice;

        // loop through the linked list of devices
        while (pTD)
        {
          if  (pTD->DeviceType == dwDevice)
          {
              hr = pChangeRadioState(pTD, dwState, RADIODEVICES_PRE_SAVE);
			  retval = 0;
          }
          
            pTD = pTD->pNext;
            
        }
        // Free the list of devices retrieved with    
        // GetWirelessDevices()
		pFreeDeviceList(pDevice);
    }

	if(hr == S_OK)return retval;
	
	return -2;
}

//get status of all wireless devices at once
DWORD GetWDevState(DWORD* bWifi, DWORD* bPhone, DWORD* bBT)
{
	RDD * pDevice = NULL;
    RDD * pTD;

    HRESULT hr;
	DWORD retval = 0;
	
    hr = pGetWirelessDevices(&pDevice, 0);

	if(hr != S_OK) return -1;
	
    if (pDevice)
    {
	    pTD = pDevice;

        // loop through the linked list of devices
		while (pTD)
		{
			switch (pTD->DeviceType)
			{
				case RADIODEVICES_MANAGED:
				*bWifi = pTD->dwState;
				break;
				case RADIODEVICES_PHONE:
				*bPhone = pTD->dwState;
				break;
				case RADIODEVICES_BLUETOOTH:
				*bBT = pTD->dwState;
				break;
				default:
				break;
			}
			pTD = pTD->pNext; 
	    }
        // Free the list of devices retrieved with    
        // GetWirelessDevices()
        pFreeDeviceList(pDevice);
    }

	if(hr == S_OK)return retval;
	
	return -2;
}



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitDLL();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DeinitDLL();
		break;
	}
    return TRUE;
}
PHONESTATE_API void setWiFiState(int state)
{
	//start WIFI
	SetWDevState( RADIODEVICES_MANAGED, state);
}
PHONESTATE_API void setBluetoothState(int state)
{
	SetWDevState( RADIODEVICES_BLUETOOTH, state);
}
PHONESTATE_API void setPhoneState(int state)
{
	//start phone
	SetWDevState( RADIODEVICES_PHONE, state);
}

PHONESTATE_API int getWiFiState()
{
	DWORD	dwWifi, dwPhone, dwBT;
	GetWDevState(&dwWifi, &dwPhone, &dwBT);
	return (dwWifi == 1) ? 1: 0;
}
PHONESTATE_API int getBluetoothState()
{
	DWORD	dwWifi, dwPhone, dwBT;
	GetWDevState(&dwWifi, &dwPhone, &dwBT);
	return (dwBT == 1) ? 1: 0;
}
PHONESTATE_API int getPhoneState()
{
	DWORD	dwWifi, dwPhone, dwBT;
	GetWDevState(&dwWifi, &dwPhone, &dwBT);
	return (dwPhone == 1) ? 1: 0;
}
PHONESTATE_API void Switch(int wifistate,int bluetoothstate,int phonestate)
{
	SetWDevState( RADIODEVICES_MANAGED, wifistate);
	SetWDevState( RADIODEVICES_BLUETOOTH, bluetoothstate);
	SetWDevState( RADIODEVICES_PHONE, phonestate);
}
