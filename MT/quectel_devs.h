#include <WinSock2.h>
#include <Windows.h>
#include <devguid.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <string>
#include <vector>
#include <map>
typedef struct tag_dev_info{	
#if UNICODE	
	std::wstring InstanceID;
	std::wstring PortName;

	std::wstring DeviceDesc;
	std::wstring HardwareID;
	std::wstring CompatibleIDs;
	std::wstring Service;
	std::wstring Class;
	std::wstring ClassGUID;
	std::wstring Driver;
	std::wstring FriendlyName;
	std::wstring Device_Type;
#else
	std::string InstanceID;
	std::string PortName;

	std::string DeviceDesc;
	std::string HardwareID;
	std::string CompatibleIDs;
	std::string Service;
	std::string Class;
	std::string ClassGUID;
	std::string Driver;
	std::string FriendlyName;
#endif
	DEVINST	ParentDevInst;
}dev_info;

#if UNICODE
void dump(dev_info &dev_);
#else
void dump(dev_info &dev_);
#endif
/*
 0 --> success
*/
#if UNICODE
typedef bool (*isinteresting_dev)(std::wstring instanceid);
#else
typedef bool (*isinteresting_dev)(std::string instanceid);
#endif


int enum_system_dev_infos(std::vector<dev_info>& vec, const GUID& dev_guid);
int enum_all_modems(std::vector<dev_info>& vec);
int enum_all_ports(std::vector<dev_info>& vec);

void set_filter_fun(isinteresting_dev filter_fun_ptr);



#if UNICODE
bool is_quectel_dev(std::wstring instanceid);
#else
bool is_quectel_dev(std::string instanceid);
#endif



typedef std::vector<dev_info> quectel_usb_interfaces;

typedef std::map<DEVINST, quectel_usb_interfaces>	quectel_devices;

void dump(const quectel_devices&);

/*
key : 系统复合设备的DEVINST
value : 这个复合设备的所有接口列表（at,modem,nmea,dm)
*/
bool build_quectel_devices(quectel_devices &devices);
typedef std::vector<dev_info>::iterator	dev_info_interator;



//quectel network adapter
typedef struct tag_quectel_network_adapter{
	std::wstring adapter_name;
	std::wstring description;
	std::wstring firendname;
}quectel_network_adapter;
typedef std::vector<quectel_network_adapter> quectel_network_adapters;
bool build_quectel_network_adapter(quectel_network_adapters & adapters);

bool is_quectel_network_dapter(const TCHAR *adapter_name);

/*
get modem port or at port from underlying net adatper
adapter = {0x11212,0x1212,0x1212,0x1212}
*/
bool get_ports_underlying_net_adpter(std::wstring adapter, std::wstring &at, std::wstring &modem);
/////////////////////////////////////////help///////////////////////////////

enum os_info{
	os_unknown = 0,
	os_winxp,
	os_vista,
	os_windows7,
	os_windows8,	
	os_windows10
};

enum os_info get_os_info();

//
#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif

DLL_IMP bool get_ports_underlying_net_adpter_Ext(std::wstring strDM_Port, std::wstring &at);
bool build_quectel_devices_Ext(quectel_devices &devices);
int enum_all_modems_Ext(std::vector<dev_info>& vec);
int enum_all_ports_Ext(std::vector<dev_info>& vec);
bool is_quectel_dev_Ext(std::wstring instanceid);
int enum_system_dev_infos_Ext(std::vector<dev_info>& vec, const GUID& dev_guid);