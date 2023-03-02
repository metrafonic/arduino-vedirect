#include <Arduino.h>

class VEDirect {
public:
	VEDirect(Stream& port);
	virtual ~VEDirect();
	bool HandleLine();
    void PrintEverySecond();
    void PrintValues();
    char* GetValue(int i);
    bool Collect();
private:
	Stream& VESerial;
};