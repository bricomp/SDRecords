## SDRecords

`SDRecords` is an easy way to access data that is stored in data Records on the SD Card.

The format of the data is whatever you decide but for an example let's use that shown below:-

```
struct DataType {`
  `unsigned long Count;`
  `int Bits;`
  `float Volts;`
  `float Amps;`
`};`

`DataType myData;`
`size_t sizeOfDataRecord = sizeof(DataType);`
```

In order to start saving `myData` structures to `SDRecords` we first have to create an exitance of `SDRecords` as below:-

```
SDRecords records;
```

We also need a way to address/recognize our `SDRecords` and they are created from `RecortTypes`.
The structure of the record type is shown below:-

```
typedef struct SDRecordType {
    File        recordFile;
    size_t      recordSize;
    uint8_t     errCode;
    size_t      errCodeExpansion;
} RecordType;
```

TODO INCLUDE 

When `we` open a `RecordFile` it creates an SD File. This is stored in `recordFile`. The Record Size is stored in `recordSize` and if an error occurs in any operation then an Error Code will be stored in `errCode`. There may be more information that could help in determining what caused the error and this is stored  in `errCodeExpansion`.

There is one other important way of determining if an error has occurred and that is stored in the bool variable `errorOccurred`. Any errors stored in `recordFile.errCode` is local to the function that has just taken place. 
The bool `errorOccurred` on the other hand is global to all `SDRecord` functions and once set is not reset. The only way to reset it is to address it in the main program and set it to false if, for example, the errors have been cleared by the sketch and it is safe to carry on.

Before we can start using `SDRecords` we have to initialize them. The command to do that is

   `bool init( uint8_t chipSelect );`      It can be used in two forms as either

   `if records.init( 10 ) {   // Will initialise SD with chipselect 10`
OR
   `if records.init() {		// where the default BUILTIN_SDCARD identifier is used.`

Besides initializing the SD card, `init` also sets the global `errorOccurred` to false.

Having initialized `SDREcords` we can now create/open a Record File. 

To do that the function `BeginRecords` is used as below:-

```
  RecordType myDataRecordFile;

  myDataRecordFile = records.BeginRecords( "DataFile1", sizeOfDataRecord );
```

`BeginRecords` will attempt to open/create a file called "DataFile1" in read/write mode. 

If it is not possible to open/create the file `myDataRecordFile.errCode` will be filled with the code for `Cannot_Open_Record_File`. 

After having created/opened a file `BeginRecords` will check that the file size is compatible with the `RecordSize`. The opened/created file size MUST be divisible by the `RecordSize` with no remainder.

ie n Records will fit exactly within the file size. If the records will not fit exactly 
into the file `myDataRecordFile.errCode` will be set to `File_Size_Does_Not_Match_Records`.

If `BeginRecords` has been successful `myDataRecordFile.errCode` will be set to `NoError`. So to determine if `BeginRecords` has been successful use something like:-

```
   myDataRecordFile = records.BeginRecords( "DataFile1", sizeOfDataRecord );
   if ( myDataRecordFile == NoError ) { //  carry on with the sketch.
```

You could have also checked `errorOccurred`,  just as well as long as you could guarantee that it was false before calling `BeginRecords`. Remember that it is global to all `SDRecords` functions and may already have been set to true, error occurred.

OK, we have opened/created a record file, to save data in the record file we use `WrRecord` as below:_

```
  if ( records.WrRecord( myDataRecordFile, 0, &myData )) {  // do something
```

  .`wrRecords`  returns true if the function was a success. The data supplied to the function is the `SDRecordsFile` type, the number of the record to be written and the data. 

If `.wrRecord` is unsuccessful one of two error codes will be returned in`myDataRecordFile.errcode`. 

They are `Cannot_Seek_to_file_position` OR `Cannot_Write_Record_to_File`.
`myDataRecordFile.errCodeExpansion` will be set to the number of bytes written.

Note that record numbering starts at 0 ( the first record is record 0 ) and that this function does NO checking that the record being written to is compatible with the file size. ie if you have a file of 20 records and you attempt to write to record position 30, no checking will be made. That is down to the person writing the sketch.

Reading from `SDRecords` is similar to writing with the format 

```
  if ( records.RdRecord( myDataRecordFile, 10, &myData )) {  // do something
```

In the instance above a record will be read from record position 10.
Potential error codes are again similar: `Cannot_Seek_to_file_position` OR `Cannot_Read_Record_From_File`.

The number of bytes read is stored in `myDataRecordFile.errCodeExpansion.`

After having used the Record Files they are closed with 

```
   records.close( myDataRecordFile );
```

​                             this will attempt to ensure that all the data for the file is flushed to the SD Card.  

Another function which may be useful  is `numberOfRecords` used as below:-

```
  size_t numRecords = records.numberOfRecords( myDataRecordFile );
```

If an error occurs the following `error code File_Size_Does_Not_Match_Records` will be returned in `myDataRecordFile.errCode`.

