// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SDRecords.cpp
    Created:	01/07/2021 20:00:16
    Author:     Robert E Bridges

    vs 0.1  01/07/2021 - Initial Trial Release 
    vs 0.2  02/07/2021 - void PrintRecordInfo(SDRecordType rec) moved to SDRecordsErrRpt
    vs 0.3  03/07/2021 - Added Global bool errorOccured. This will be set when an error occurs and will NOT be reset by SDRecords.
                         It is the responsibility of the user code to examine and reset or ignore this variable.
                       - Improved BeginRecords Error reporting.
                       - Added error reporting for NumberOfRecords if the fileSeze does not match the use of Records
*/
#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include "SDRecords.h"

    bool SDRecords::init(uint8_t chipSelect)
    {
        // see if the card is present and can be initialized:
        errorOccurred = SD.begin(chipSelect);
        return errorOccurred;
    }

    SDRecordType SDRecords::BeginRecords( const char* filepath, uint16_t recordSize ) {
        RecordType rec;
        ulong   fSize;

        rec.errCode          = NoError;
        rec.recordFile       = SD.open(filepath, FILE_WRITE);
        rec.errCodeExpansion = 0;
        if (!rec.recordFile) {
            rec.errCode = Cannot_Open_Record_File;
            errorOccurred = true;
        }
        else {
            rec.recordSize = recordSize;
            fSize = rec.recordFile.size();
            if ((fSize % rec.recordSize) != 0) {
                rec.errCode = File_Size_Does_Not_Match_Records;
                errorOccurred = true;
            }
        }
        return rec;
    }

    int SDRecords::numberOfRecords(SDRecordType rec) {
        ulong   fSize;

        if ((fSize % rec.recordSize) != 0) {
            rec.errCode = File_Size_Does_Not_Match_Records;
            errorOccurred = true;
        }
        return ( rec.recordFile.size() / rec.recordSize );
    }

    bool SDRecords::WrRecord(SDRecordType rec, uint16_t recNumber, const void* buf) {

        bool    okState = true;
        size_t  bytesWritten;

        rec.errCodeExpansion = 0;
        if ( rec.recordFile.seek( recNumber * rec.recordSize )) {

            bytesWritten = rec.recordFile.write(buf, rec.recordSize);
            if ( bytesWritten != rec.recordSize) {
                rec.errCodeExpansion    = bytesWritten;
                rec.errCode             = Cannot_Write_Record_to_File;
                okState = false; 
            }
        }
        else {
            rec.errCode = Cannot_Seek_to_file_position;
            okState = false; 
        }
        errorOccurred = ( errorOccurred && okState ); 
        return okState;
    }

    bool SDRecords::RdRecord(SDRecordType rec, uint16_t recNumber, void* buf) {

        bool okState = true;
        size_t  bytesRead;

        rec.errCodeExpansion = 0;
        if ( rec.recordFile.seek( recNumber * rec.recordSize )) {

            bytesRead = rec.recordFile.read(buf, rec.recordSize);
            if ( bytesRead != rec.recordSize ) {
                rec.errCodeExpansion = bytesRead;
                rec.errCode = Cannot_Read_Record_from_File;
                okState = false;
            }
        }
        else {
            rec.errCode = Cannot_Seek_to_file_position;
            okState     = false;
        }
        errorOccurred = ( errorOccurred && okState );
        return okState;
    }

    void SDRecords::CloseRecord( SDRecordType rec) {
        rec.recordFile.close();
        rec.errCodeExpansion = 0;
        rec.errCode          = NoError;
    }


