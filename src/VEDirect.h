#include <Arduino.h>

class VEDirect {
public:
	VEDirect(HardwareSerial& port);
	virtual ~VEDirect();
	bool HandleLine();
    void PrintEverySecond();
    void PrintValues();
    char* GetValue(int i);
    bool Collect();
private:
	HardwareSerial& VESerial;
};