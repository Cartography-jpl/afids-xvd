////////////////////////////////////////////////////////////////
// DspFile.cc
//	Warning - only Open and Write are tested
///////////////////////////////////////////////////////////////
#include "DspFile.h"
#include <errno.h>

///////////////////////////////////////////////////////////////
//	CONSTRUCTOR
///////////////////////////////////////////////////////////////
DspFile::DspFile()
{
//	
	_bytesWrittenInLastWrite = 0;
	_bytesReadInLastRead = 0;
	_eof = False;
	_open = False;
	_buffer = 0;
	_bufferSize = 0; // for reading from file (not writing)
	strcpy(_fileName,"");
	_file = (FILE *) 0;
	strcpy(_flags,"a+");
}

///////////////////////////////////////////////////////////////
//	open
///////////////////////////////////////////////////////////////
DSPSTATUSTYPE DspFile::open( char * fileName, int bufferSize,
					 Boolean read, Boolean write,
					 Boolean append, Boolean truncate) 			
{
  assert ( strlen(fileName) <= (size_t) MAX_NAMESTRINGLENGTH );
	strcpy(_fileName, fileName);
	if ( bufferSize < 0) _bufferSize = 0;
	else _bufferSize = bufferSize;
	setFlags(read, write, append ,truncate); 
	if (createNewBuffer(_bufferSize)== __failure)
		return __failure;
	
	_file = fopen(_fileName, _flags );
	if (_file == 0) {
	 	_open = False;
	 	return __failure;
	}
	else {
		_open = True;
		return __success;
	}
}

///////////////////////////////////////////////////////////////
//	close
///////////////////////////////////////////////////////////////
void DspFile::close()
{
	deleteBuffer();
	fclose(_file);
	_open = False;
}


///////////////////////////////////////////////////////////////
//	readAllBytes
///////////////////////////////////////////////////////////////
DSPSTATUSTYPE DspFile::readAllBytes(int nbytes, char * buffer)
{
	int totalBytesRead = 0;
	_eof = False;
	if (nbytes > 0) {
	  while (totalBytesRead < nbytes) {
		int n = fread ( buffer, (size_t) 1,(size_t) nbytes, _file);
		if ( n == 0) {
			_bytesReadInLastRead = totalBytesRead;
			return __success;
		}
		if ( n < 0 ) {
			_error = __success;
			_bytesReadInLastRead = totalBytesRead;
			return __failure;
		}
		totalBytesRead += n;
		nbytes -=  n;
		buffer += n;
	  }
	}
	_bytesReadInLastRead = totalBytesRead;
	return __success;
}
///////////////////////////////////////////////////////////////
//	readOnce
///////////////////////////////////////////////////////////////
DSPSTATUSTYPE DspFile::readOnce(int nbytes, char * buffer)
{
	_bytesReadInLastRead = 0;
	_eof = False;
	if  (nbytes > 0) {
	  int n = fread ( buffer, (size_t) 1, (size_t) nbytes,_file);
	  if ( n == 0) {
			_bytesReadInLastRead = 0;
			return __success;
	  }
	  if ( n < 0 ) {
			_error = __failure;
			_bytesReadInLastRead = 0;
			return __failure;
	  }
	  if ( n > 0) 
			_bytesReadInLastRead = n;
	}
	return __success;
}
///////////////////////////////////////////////////////////////
//	writeAllBytes
//		{tested and works}
///////////////////////////////////////////////////////////////
DSPSTATUSTYPE DspFile::writeAllBytes(int nbytes, char * buffer)
{
	_bytesWrittenInLastWrite = 0;
	int totalBytesRead = 0;
	_eof = False;
	int remainingBytes = nbytes;
	if (nbytes > 0 && isOpened()) {
	    while (remainingBytes > 0 ) {
		int n = fwrite ( buffer,(size_t) 1, (size_t) remainingBytes,_file);
		if ( n < 0 ) {
			_error = __failure;
			_bytesWrittenInLastWrite = totalBytesRead;
			return __failure;
		}
		totalBytesRead += n;	
		buffer = ((char *)(buffer + n));
		remainingBytes = nbytes - totalBytesRead;
	   }
	   _bytesWrittenInLastWrite = totalBytesRead;
	}
	return __success;
}
///////////////////////////////////////////////////////////////
//	writeOnce  
///////////////////////////////////////////////////////////////
DSPSTATUSTYPE DspFile::writeOnce(int nbytes, char * buffer)
{
	int n=0;
	_bytesWrittenInLastWrite = 0;
	_eof = False;
	if (nbytes > 0) {
	  int n = fwrite ( buffer, (size_t) 1, (size_t) nbytes,_file);
	  if ( n < 0 ) {
			_error = __failure;
			_bytesWrittenInLastWrite = 0;
			return __failure;
	  }
	}
	_bytesWrittenInLastWrite = n;
	return __success;
}
///////////////////////////////////////////////////////////////
//	printErrorToScreen
///////////////////////////////////////////////////////////////
void DspFile::printErrorToScreen(char * msg) 
{
	if (errno < 0 )
		perror( msg );
	else
		printf( msg );
}
///////////////////////////////////////////////////////////////
//	isEOF
///////////////////////////////////////////////////////////////
Boolean	DspFile::isEOF()
{ 
	assert (isOpened());
	if (feof( _file ) ) return True;
	else return False;
}
///////////////////////////////////////////////////////////////
//	createBuffer (non-public)
///////////////////////////////////////////////////////////////
DSPSTATUSTYPE DspFile::createNewBuffer(int bufferSize)
{
	if (_buffer != 0 ) deleteBuffer();
	_buffer = new char[bufferSize];
	if ( _buffer == 0 ) {
		_error = __failure;
		return __failure;
	}
	return __success;
}
///////////////////////////////////////////////////////////////
//	deleteBuffer (non-public)
///////////////////////////////////////////////////////////////
void DspFile::deleteBuffer()
{
	if (_buffer != 0 ) {
		delete _buffer ;
		_buffer = 0;
	}
}
///////////////////////////////////////////////////////////////
//	setFlags (non-public)
//		if both append and truncate are set, append is used
//		
///////////////////////////////////////////////////////////////
void DspFile::setFlags(Boolean read, Boolean write,
				Boolean append,Boolean truncate)
{
	assert ( read | write );
	strcpy(_flags,"");
	if (read && !write) strcpy(_flags,"r");
	if (!read && write) strcpy(_flags,"w");
	if (read && write && !truncate) strcpy(_flags,"r+");
	if (read && write && truncate)strcpy(_flags, "w+");
	if (read && write && append ) strcpy(_flags, "a+");
	if (!read && write && truncate)strcpy(_flags, "w");
	if (!read && write && append) strcpy(_flags, "a");
		
}
