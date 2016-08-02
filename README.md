PhoneState is a simple utility class for enabling and disabling the wireless communications radios in your Windows Mobile device.

It provides the following functionality:
        1. setWiFiState
        2. setBluetoothState
        3. setPhoneState
        4. getWiFiState
        5. getBluetoothState
        6. getPhoneState

To use inside a cs file:
        [DllImport("PhoneState.dll")]
        private static extern void setPhoneState(int state);
        [DllImport("PhoneState.dll")]
        private static extern int getPhoneState();
        [DllImport("PhoneState.dll")]
        private static extern void switch(int wifistate,int bluetoothstate,int phonestate);
        
For example:

To switch on the phone and switch off bluetooth and wifi:

       switch(false,false,true);

