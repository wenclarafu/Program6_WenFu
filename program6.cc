/*
 * Name: Wen Fu
 * Course: CS3377
 * Email: wxf170430@utdallas.edu
 */

#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <string>
#include <sstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 18
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;



class BinaryFileHeader
{
public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};



const int maxRecordStringLength = 25;
class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];

};



int
main ()
{

  WINDOW *window;
  CDKSCREEN *cdkscreen;
  CDKMATRIX *myMatrix;		// CDK Screen Matrix

  // matrix starts out at 1,1

  const char *rowTitles[] = { "", "a", "b", "c", "d", "e" };
  const char *columnTitles[] = { "", "a", "b", "c" };
  int boxWidths[] =
    { BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH };
  int boxTypes[] = { vMIXED, vMIXED, vMIXED, vMIXED, vMIXED, vMIXED };


  string magic, version, numRec;	// to hold the string value of header

  //create a new BinaryFileHeader object 
  BinaryFileHeader *myHeader = new BinaryFileHeader ();

  //declare a new input file
  ifstream infileHeader ("cs3377.bin", ios::in | ios::binary);

  infileHeader.read ((char *) myHeader, sizeof (BinaryFileHeader));

  //stream from binary file is sent to input
  stringstream input;

  //to fetch the value of version number, send to stringstream and convert to string
  input << "Version: " << myHeader->versionNumber;
  version = input.str ();
  input.str ("");//clear content of input

  //get the value magic number, manually convert to uppercase and add 0x in front
  input << "Magic: " << "0x" << uppercase << hex << myHeader->magicNumber;
  magic = input.str ();
  input.str ("");

  //get the value of number of records
  input << "numRecords: " << myHeader->numRecords;
  numRec = input.str ();
  input.str ("");
  infileHeader.close ();


  //done with the header 
  //start to read from binary file record
  BinaryFileRecord *myRecord = new BinaryFileRecord ();

  /*
   * Initialize the Cdk screen.
   *
   */
  window = initscr ();
  cdkscreen = initCDKScreen (window);

  /* Start CDK Colors */
  initCDKColor ();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
   */
  myMatrix =
    newCDKMatrix (cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH,
		  MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_NAME_STRING,
		  (char **) rowTitles, (char **) columnTitles, boxWidths,
		  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix == NULL)
    {
      printf ("Error creating Matrix\n");
      _exit (1);
    }

  /* Display the Matrix */
  drawCDKMatrix (myMatrix, true);

  //display header content
  setCDKMatrixCell (myMatrix, 1, 1, magic.c_str ());
  setCDKMatrixCell (myMatrix, 1, 2, version.c_str ());
  setCDKMatrixCell (myMatrix, 1, 3, numRec.c_str ());

  //to interate at most 4 times, counter statr from 0
  int i = 0;
  
  //declare another infile from reading header 
  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);

  //move pointer to current position after reading header
  binInfile.seekg (sizeof (BinaryFileHeader), ios::beg);

  //while keep reading and record number less than 4
  while (binInfile.read ((char *) myRecord, sizeof (BinaryFileRecord))
	 && i < 4)
    {
      //record holds string length stream and strRecord hold string stream
      stringstream record, strRecord;

      //convert sstream to string
      string str, strLen;
     
       
      record << myRecord->stringBuffer;
      str += record.str ();
      //write string into the matrix
      setCDKMatrixCell (myMatrix, 2 + i, 2, str.c_str ());
  
      //cast string length to an integer with uint16_t
      strRecord << "strlen: " << static_cast < uint16_t >
	(myRecord->strLength);
      strLen = strRecord.str ();

      setCDKMatrixCell (myMatrix, 2 + i, 1, strLen.c_str ());
      i++;
    }

  binInfile.close();

  //draw matrix with entered values
  drawCDKMatrix (myMatrix, true);	/* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK ();
}
