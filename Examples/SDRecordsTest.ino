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
    if (SDRecordFile1.errCode == NoError) {

        int n = 0;
        while ((!records.errorOccurred) && (n < 20)) {
            for (int i = 0; i < recordDataSize; i++) {
                //Fill an array[i] with data
                dataArray[i] = n;
            };

            //Write the data from dataArray into SDRecordFile1, at record Location n (record locations start at 0)
            Serial.print("Writing data to record array, record "); Serial.println(n);
            if (!records.WrRecord(SDRecordFile1, n, dataArray)) {
                Serial.print("Unable to write record "); Serial.print(n); Serial.println(" to RecTest file.");
            }
            n++;
        }

        if (!records.errorOccurred) {
            Serial.println();
            Serial.println("20 records stored in File RecTest on SD Card in ascending order.");
            Serial.println();
            Serial.println("Now read them back and put into File RecTest2 on SD Card");
            Serial.println("in inverse order thereby proving read works as well.");
            Serial.println();

            SDRecordFile2 = records.BeginRecords("RecTest2", recordDataSize);
            if (SDRecordFile2.errCode == NoError) {
                int o = 0;   //o for out
                int i = 19;  //i for in
                while ((!records.errorOccurred) && (o < 20)) {

                    //Read from SDRecordFile1
                    Serial.print("Reading from RecTest file, Record "); Serial.print(i);

                    if (records.RdRecord(SDRecordFile1, i, dataArray)) {

                        Serial.print(", writing to RecTest2 File, Record "); Serial.print(o);

                        //..and write into SDRecordFile2

                        if (!records.WrRecord(SDRecordFile2, o, dataArray)) {
                            Serial.println();  Serial.print("----- Unable to write record "); Serial.print(o); Serial.print(" to RecTest2 file.");
                        }
                    } else {
                         Serial.print("----- Unable to read record "); Serial.print(i); Serial.print(" from RecTest file.");
                         errRpt.DecodeError(SDRecordFile2);
                         errRpt.PrintRecordInfo(SDRecordFile2);
                    }
                    i = i - 1;
                    o = o + 1;
                    Serial.println();
                }
            } else {

                Serial.println("Unable to open file RecTest2");
                errRpt.DecodeError(SDRecordFile1);
            }
        } else {
            errRpt.DecodeError(SDRecordFile2);
        }

    } else {
        Serial.println("Unable to open file RecTest");
        errRpt.DecodeError(SDRecordFile1);
    }

    Serial.print("Number of Records in RecTest  "); Serial.println( records.numberOfRecords( SDRecordFile1 ));
    Serial.print("Number of Records in RecTest2 "); Serial.println( records.numberOfRecords( SDRecordFile1 ));

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
