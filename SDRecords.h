// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SDRecords.h
    Created:	01/07/2021 20:00:16
    Author:     Robert E Bridges

    vs 0.1  01/07/2021 - Initial Trial Release
    vs 0.2  02/07/2021 - void PrintRecordInfo(SDRecordType rec) moved to SDRecordsErrRpt
    vs 0.3  03/07/2021 - Added Global bool SDRecordsErrorOccurred. This will be set when an error occurs and will NOT be reset by SDRecords.
                         It is the responsibility of the user code to examine and reset or ignore this variable.
                       - Improved BeginRecords Error reporting.
                       - Added error reporting for NumberOfRecords if the fileSeze does not match the use of Records
*/

#ifndef SDRecords_h
#define SDRecords_h

#include "Arduino.h"
#include <SD.h>

const char SDRecords_vs[4] = { "0.3" };

typedef struct SDRecordType {
    File        recordFile;
    uint16_t    recordSize;
    uint8_t     errCode;
    size_t      errCodeExpansion;
} RecordType;

enum errorCode {
    NoError = 1,
    Cannot_Open_Record_File,
    File_Size_Does_Not_Match_Records,
    Cannot_Seek_to_file_position,
    Cannot_Write_Record_to_File,
    Cannot_Read_Record_from_File
};


class SDRecords {

public:

    bool errorOccurred;

    bool init(uint8_t chipSelect = BUILTIN_SDCARD);

    SDRecordType BeginRecords(const char* filepath, uint16_t recordSize);

    int numberOfRecords(SDRecordType rec);

    bool WrRecord(SDRecordType rec, uint16_t recNumber, const void* buf);

    bool RdRecord(SDRecordType rec, uint16_t recNumber, void* buf);

    void CloseRecord(SDRecordType rec);

};

#endif