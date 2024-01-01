Software Defined Vechicle (SDV) Model Project
  It has a Central Compute (CC) & couple of Micro Kernel Unit (MCU) connected with Ethernet as model representation
  Gamification done on X-Mas Santa theme to bring interest into learning 

Features
SANTA Car Mechanics:
    One Central Compute		- Connects / Controls all MCU in Farm
    One Connectivity MCU		- Can scan Hotspots (WLANS) 
    One System MCU 			- Can scan device’s remain battery power %
    One Cloud Connect MCU		- Can scan Weather data from Web
    One Sensor MCU			- Under development
    One HMI					- Shows Central Compute processed data
    Communication				- Communicates with Ethernet TCP/IP![image](https://github.com/taruncha2701/SDV_Model_Project/assets/155322025/595f155c-9986-4961-89eb-01e3d409bfb8)


//Contact if any query Tarun Chatterjee (taruncha@gmail.com)
Used VS 2022 Comunity Edition for code
C++ code - Win32 App - Each MCU is a seperate app
Each MCUs can be developed for diff devices like in Android, Rasp Pi, Linux anything as connected through ETH to CC
HMI can be an Android & can feed data through Serial / ETH / logs / any communication medium or spec

Project module list
SDV_Generic_Lib - Lib defines common macros & reusable func
SDV_MCU_CetralCompute - CC - Server socket & the main controller of this system
SDV_MCU_CloudInfra - Scans the OpwnWeather API
SDV_MCU_Connectivity - Scans the WLAN available
SDV_MCU_SystemControl - Scans the battery power
SDV_MOCK_HMI - Mock HMI which reads the Central Compute processed logs & displays status
