#include "VEDirect.h"
#include "config.h"

char newlineChars[buffsize];
char recv_label[num_keywords][label_bytes]  = {0};  // {0} tells the compiler to initalize it with 0. 
char recv_value[num_keywords][value_bytes]  = {0};  // That does not mean it is filled with 0's
char value[num_keywords][value_bytes]       = {0};  // The array that holds the verified data
static byte currentIndex = 0;
static byte futureIndex = 0;

VEDirect::VEDirect(Stream& port):
	VESerial(port)
	// Initialise the serial port that the
	// VE.Direct device is connected to and
	// store it for later use.
{
}

VEDirect::~VEDirect() {
	// virtual destructor
}


bool VEDirect::HandleLine() {
    if (VESerial.available()<1)
        return 0;
    VESerial.setTimeout(200);
    String serialline = VESerial.readStringUntil(13);
    serialline.trim();
    if (serialline.length()>buffsize){
        Serial.println("Serial line too long for buffer! Check cables and baud rate!");
        return 0;
    }
    serialline.toCharArray(newlineChars, buffsize);
    char* linekey = strtok(newlineChars,"\t");
    if (linekey==NULL)
        return 0;
    char* linevalue = strtok(NULL,"\r"); // is trimmed so will not find carrige return anyway
    if (linevalue==NULL)
        return 0;
    for (int i=currentIndex; i<num_keywords; i++){
        if (strcmp(keywords[i], linekey)==0) // search for index
        {
            strcpy(recv_value[i], linevalue);
            strcpy(recv_label[i], linekey);
            futureIndex = i+1; // suggest to start search at next keyword
            break;
        }
    }
    if (futureIndex == currentIndex || futureIndex == num_keywords){
        futureIndex = 0; // start from beginning because search was not succesful or because end of block
    }   
    currentIndex = futureIndex;
    if (currentIndex==0)
    {
        byte checksum = 0;
        for (int x = 0; x < num_keywords; x++) {
            if(strlen(recv_value[x]) == 0 || strlen(recv_label[x]) == 0) {
                continue;
            }
            char *v = recv_value[x];
            char *l = recv_label[x];
            while (*v) {
                checksum = (checksum + *v) & 255;
                v++;
            }
            while (*l) {
                checksum = (checksum + *l) & 255;
                l++;
            }
            // Because we strip the new line(10), the carriage return(13) and 
            // the horizontal tab(9) we add them here again.  
            checksum += 32;
        }
        if (checksum == 0) {
            for (int i = 0; i < num_keywords; i++) 
            {
                strcpy(value[i], recv_value[i]);
            }
            return 1;
        }
    }
    return 0;
}


void VEDirect::PrintEverySecond() {
	static unsigned long prev_millis;
    if (millis() - prev_millis > 1000) {
        PrintValues();
        prev_millis = millis();
    }
}

void VEDirect::PrintValues() {
	for (int i = 0; i < num_keywords; i++){
        Serial.print(keywords[i]);
        Serial.print(",");
        Serial.println(value[i]);
    }
}

char * VEDirect::GetValue(int i){
    return &value[i][0];
}

bool VEDirect::Collect(){
    // Listen for data - Timeout after 2.1 seconds of no valid checksum
    unsigned long prev_millis = millis();
    while (millis() - prev_millis < 2100) {
        if (HandleLine()==1){
            return 1;
        }
    }
    return 0;
}
