// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SDRecordsErrInfo.cpp
    Created:	01/07/2021 20:00:16
    Author:     Robert E Bridges

    vs 0.1  02/07/2021 - Initial Trial Release
    vs 0.2  03/07/2021 - Added Cannot_Open_Record_File and File_Size_Does_Not_Match_Records reporting
    vs 0.3  03/07.2021 - Just upped to 0.3 to keep in step with SDRecords
*/
#include "Arduino.h"
#include "SDRecords.h"
#include "SDRecordsErrRpt.h"

SDRecords sdRecs;

    void SDRecordsErrRpt::DecodeError(SDRecordType rec) {
        Serial.print("Err: ");
        switch (rec.errCode) {
            case NoError:
                Serial.println("No Error");
                break;
            case Cannot_Open_Record_File:
                Serial.println("Cannot Open Record File");
                break;
            case File_Size_Does_Not_Match_Records:
                Serial.println("File Size Does Not Match Records");
                break;
            case Cannot_Seek_to_file_position:
                Serial.println("Cannot Seek to file position");
                break;
            case Cannot_Write_Record_to_File:
                Serial.println("Cannot Write Record to File");                
                break;
            case Cannot_Read_Record_from_File:
                Serial.println("Cannot Read Record from File");            
                break;

        }
    };


    void SDRecordsErrRpt::PrintRecordInfo(SDRecordType rec){
        Serial.print("SDRecords Vs     "); Serial.println(SDRecords_vs);
        Serial.print("ErrorOccured     "); if (sdRecs.errorOccurred) { Serial.println("True"); } else { Serial.println("False"); }
        Serial.print("recordSize       "); Serial.println(rec.recordSize);
        Serial.print("errCode          "); Serial.println(rec.errCode);
        Serial.print("errCodeExpansion "); Serial.println(rec.errCodeExpansion);
    }

