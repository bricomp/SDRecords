// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SDRecordsErrInfo.h
    Created:	01/07/2021 20:00:16
    Author:     Robert E Bridges

    vs 0.1  02/07/2021 - Initial Trial Release
    vs 0.2  03/07/2021 - Added Cannot_Open_Record_File and File_Size_Does_Not_Match_Records reporting
    vs 0.3  03/07.2021 - Just upped to 0.3 to keep in step with SDRecords

*/

#ifndef SDRecordsErrRpt_h
#define SDRecordsErrRpt_h

#include "Arduino.h"
#include <SD.h>
#include "SDRecords.h"

/*
const char SDRecords_vs[4] = { '0','.','1'};

typedef struct SDRecordType {
    File        recordFile;
    uint16_t    recordSize;
    uint8_t     errCode;
    size_t      errCodeExpansion;
} RecordType;

enum errorCode {
    NoError,
    Cannot_Seek_to_file_position,
    Cannot_Write_Record_to_File,
    Cannot_Read_Record_from_File
};

*/

class SDRecordsErrRpt {

public:

    void DecodeError(SDRecordType rec);

    void PrintRecordInfo(SDRecordType rec);

};

#endif