//Contact if any query Tarun Chatterjee (taruncha@gmail.com)
Used VS 2022 Comunity Edition for code
C++ code - Win32 App - Each MCU is a seperate app
Each MCUs can be developed for diff devices like in Android, Rasp Pi, Linux anything as connected through ETH to CC
HMI can be an Android & can feed data through Serial / ETH / logs / any communication medium or spec

SDV_Generic_Lib - Lib defines common macros & reusable func
SDV_MCU_CetralCompute - CC - Server socket & the main controller of this system
SDV_MCU_CloudInfra - Scans the OpwnWeather API
SDV_MCU_Connectivity - Scans the WLAN available
SDV_MCU_SystemControl - Scans the battery power
SDV_MOCK_HMI - Mock HMI which reads the Central Compute processed logs & displays status