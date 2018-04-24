/*
 * Name: Wen Fu
 * Course: CS3377
 * Email: wxf170430@utdallas.edu
 */

#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <fstream>
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

/*
const int maxRecordStringLength = 25;

class BinaryFileRecord
{
public:
  unit8_t strLength;
  char stringBuffer[maxRecordStringLength];

};
*/


int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // matrix starts out at 1,1.
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"", "a", "b", "c"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH,BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED,vMIXED, vMIXED, vMIXED};

  string magic, version,numRec; // to hold the string value of header

  BinaryFileHeader *myHeader = new BinaryFileHeader();

  ifstream binInfile ("cs3377.bin", ios::in | ios::binary);

  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));

  stringstream input;

  //to fetch the value of version number, send to stringstream and convert to string
  input <<"Version: " << myHeader->versionNumber;
  version = input.str();
  input.str("");

  //get the value magic number, manually convert to uppercase and add 0x in front
  input << "Magic: " <<"0x" <<uppercase << hex << myHeader->magicNumber;
  magic = input.str();
  input.str("");
  
  //get the value of number of records
  input <<"numRecords: " << myHeader->numRecords;
  numRec = input.str();
  input.str("");

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */

  setCDKMatrixCell(myMatrix, 1, 1,  magic.c_str());
  setCDKMatrixCell(myMatrix, 1, 2,  version.c_str());
  setCDKMatrixCell(myMatrix, 1, 3,  numRec.c_str());
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
