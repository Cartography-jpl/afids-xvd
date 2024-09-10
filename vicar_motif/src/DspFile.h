////////////////////////////////////////////////////////////////
// DspFile.h
//	needs a lot more work to be usefull .. quick and dirty
///////////////////////////////////////////////////////////////
#ifndef DSPFILE_H
#define DSPFILE_H
#include "DspDefs.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <Xm/Xm.h>
#include <assert.h>

class DspFile {

    protected:
    
    	int		_bufferSize;
    	char *  	_buffer;
    	FILE *		_file;  
    	char   		_fileName[MAX_NAMESTRINGLENGTH];
    	DSPSTATUSTYPE	_error;  // local error/status 
    	int		_errno;
    	char 		_flags[5];
    	Boolean		_open;
    	Boolean		_eof;
    	int		_bytesReadInLastRead;
    	int		_bytesWrittenInLastWrite;
    	
    	virtual void		deleteBuffer();
        
     	
    public:
    
        DspFile();
        virtual ~DspFile() { deleteBuffer();}
    
        virtual DSPSTATUSTYPE	  createNewBuffer(int bufferSize);
    	virtual DSPSTATUSTYPE 	  readAllBytes(int nbytes, char * buffer);
        virtual DSPSTATUSTYPE	  readOnce(int nbytes, char * buffer) ;
	virtual DSPSTATUSTYPE 	  writeAllBytes(int nbytes, char * buffer);
	virtual DSPSTATUSTYPE	  writeOnce(int nbytes, char * buffer) ;
	virtual void 		  printErrorToScreen(char * msg) ;
	virtual void		  setFlags(Boolean read, Boolean write,
					 Boolean append, Boolean truncate);
	
	virtual DSPSTATUSTYPE	open( char * fileName, int bufferSize = 1024,
					 Boolean read = True, Boolean write = True,
					 Boolean append = True, Boolean truncate = True);
	virtual void		close();
	inline DSPSTATUSTYPE 	isError() 	{ return _error; }
	inline char *		getFileName() 	{ return _fileName;}
	inline  Boolean		isOpened()	{ return _open; }
	virtual Boolean		isEOF();	
	inline  char *		getBufferAddr()	{ return _buffer; }
	inline  int  		getBufferSize() { return _bufferSize;}
	inline  int		getNumbBytesRead() { return _bytesReadInLastRead; }
	inline  int		getNumbBytesWritten() {return _bytesWrittenInLastWrite; }
	//virtual void		  getBehavior() ;
	//virtual void		  setBehavior() ;
};
#endif
