// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PHONESTATE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PHONESTATE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PHONESTATE_EXPORTS
#define PHONESTATE_API __declspec(dllexport)
#else
#define PHONESTATE_API __declspec(dllimport)
#endif

extern "C"
{
	PHONESTATE_API void Switch(int wifistate,int bluetoothstate,int phonestate);	PHONESTATE_API void setWiFiState(int state);
	PHONESTATE_API void setBluetoothState(int state);
	PHONESTATE_API void setPhoneState(int state);
	PHONESTATE_API int getWiFiState();
	PHONESTATE_API int getBluetoothState();
	PHONESTATE_API int getPhoneState();
}

