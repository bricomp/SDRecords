// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SDRecordsTest.ino
    Created:	01/07/2021 20:00:16
    Author:     Robert E Bridges

    vs 0.1  01/07/2021 - Initial Trial Release
    vs 0.2  02/07/2021 - Incorporated SDRecordsErrRpt
*/
#include "Arduino.h"
#include "SDRecords.h"
#include "SDRecordsErrRpt.h"

const int  ledPin = 13;

SDRecords       records;
SDRecordsErrRpt errRpt;

SDRecordType SDRecordFile1, SDRecordFile2;  //Create SDRecordFile variables

uint8_t    dataArray[16];
uint8_t    emptyArray[16]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
uint16_t   recordDataSize = sizeof(dataArray);

// The setup() function runs once each time the micro-controller starts
void setup()
{
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect.
    }

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    Serial.print("Initializing SDFileRecords card...");
    
// init() defaults to chipSelect = BUILTIN_SDCARD. Do init(anotherChipSelect) if required
// see if the card is present and can be initialized:

    if (!records.init()) {                                   
        Serial.println("Card failed, or not present");                   
        // don't do anything more:
        return;
    }else{
        Serial.println();
    }

    Serial.println("Creating Record File");
    Serial.println();

//Create a recordFile, name it and set the Record Size (in Bytes. Max Size = sizeof(uin16_t)

    SDRecordFile1  = records.BeginRecords("RecTest", recordDataSize);    
    if (SDRecordFile1.errCode != NoError) {
        errRpt.DecodeError(SDRecordFile1);
    }
    
    for (int n = 0; n < 20; n++) {
        for (int i = 0; i < recordDataSize; i++) {                       

            //Fill an array[i] with data

            dataArray[i] = n;
        };

//Write the data from dataArray into SDRecordFile1, at record Location n (record locations start at 0)
        
        if (records.WrRecord(SDRecordFile1, n, dataArray)) {}           
    }

    Serial.println("20 records stored in File RecTest on SD Card in ascending order.");
    Serial.println();
    Serial.println("Now read them back and put into File RecTest2 on SD Card");
    Serial.println("in inverse order thereby proving read works as well.");
    Serial.println();

    SDRecordFile2 = records.BeginRecords("RecTest2", recordDataSize);
    if (SDRecordFile2.errCode != NoError) {
        errRpt.DecodeError(SDRecordFile1);
    }
    int r = 0;
    for (int n = 19; n >= 0; n--) {

//Read from SDRecordFile1

        if (records.RdRecord(SDRecordFile1, n, dataArray)) {

            Serial.print("Writing to Record ");
            Serial.println(r);

            //..and write into SDRecordFile2

            if (records.WrRecord(SDRecordFile2, r, dataArray)) {
                r = r + 1;
            }
            else {
                errRpt.DecodeError(SDRecordFile2);
                errRpt.PrintRecordInfo(SDRecordFile2);
            }
        } 
        else {
            errRpt.DecodeError(SDRecordFile1);
            errRpt.PrintRecordInfo(SDRecordFile1);
        }
    }
    Serial.print("Number of Records in RecTest  "); Serial.println(records.numberOfRecords(SDRecordFile1));
    Serial.print("Number of Records in RecTest2 "); Serial.println(records.numberOfRecords(SDRecordFile1));


//Close the SDRecordFiles

    records.CloseRecord(SDRecordFile1);                                 
    records.CloseRecord(SDRecordFile2);

    digitalWrite(ledPin, LOW );

    Serial.println(recordDataSize);
    Serial.println("Data saved to file");
    Serial.println();
    Serial.println("Data Info for SDRecordFile1 (RecTest1)");
    errRpt.DecodeError(SDRecordFile1);
    errRpt.PrintRecordInfo(SDRecordFile1);
    Serial.println();
    Serial.println("Data Info for SDRecordFile1 (RecTest2)");
    errRpt.DecodeError(SDRecordFile1);
    errRpt.PrintRecordInfo(SDRecordFile2);
}

// Add the main program code into the continuous loop() function
void loop()
{


}
