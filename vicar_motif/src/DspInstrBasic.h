///////////////////////////////////////////////////////////////////
//  DspInstrBasic.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRBASIC_H
#define DSPINSTRBASIC_H
#include "DspDefs.h"
#include "DspUIComponent.h"
#include "DspImageBasic.h"
#include "DspImageEmpty.h"
#include "DspHistBasic.h"
#include "DspInfoView.h"
#include "DspDisplayPolicy.h"
#include "DspCursCmd.h"
#include "DspCursDumpCmd.h"
// #include "DspStretchCmd.h"
//  #include "DspStretchCmdIF.h"
// #include "DspStretchCmdDialog.h"
//#include "DspPostDialogCmd.h"
#include "DspUtilMisc.h"
#include "Lut.h"
#include "Histogram.h"
#include "ImageData.h"
#include "ImageDisplayView.h"
#include "MenuCmdList.h"
#include "PrintWidgetTreeCmd.h"
#include "Histogram.h"
#include "PopupMenu.h"
#include "CollectHist.h"
#include <Xm/Xm.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
//  #include "PostStretchCmd.h"

class DspFileNameCmd;
class PostSingleFileDialogCmd;
class DspWindowBasic;

class DspInstrBasic : public DspUIComponent {
 
   protected:
   
   	/*static XtResource 	_resources[];  */	
	static String 		_defaults[]; // resource defaults.
	DspWindowBasic *	_window; 
   	ImageData **		_dataModel;
   	DspImageBasic *  	_imageDisplayer;
   	DspHistBasic *		_histDisplayer1;
   	DspHistBasic *		_histDisplayer2;
   	Histogram *		_rawHistModel;
   	Histogram *		_stretchHistModel;
   	DspInfoView *		_titleBar;
	DspInfoView *		_imageText;
	DspInfoView *		_stretchText;
	DspInfoView *		_picnoErt;
	char 			_filename[MAX_NAMESTRINGLENGTH];
	Widget			_parent;
	Widget			_imageArea;
   	Widget			_histArea;	
  	DspDisplayPolicy *	_displayPolicy;
  	int			_numbModels;
  	MenuCmdList *		_cmdList;
//	DspStretchCmd *		_stretchCmd;
//	CmdInterface * 		_stretchCmdInterface;
//	DspPostDialogCmd *	_postStretchCmd;
	PopupMenu *		_popup;
	Histogram *		_emptyHistogram; // blank unmanaged histogram used to inherit 
 						 // stretchcmd, etc from gui code. 
//	DspStretchCmdDialog *	_stretchDialog;
	ImageDisplayView *	_imageView;
	
	MenuCmdList *		_dumpMenuCmdList;
	DspCursCmd *		_cursorDnCmd;
	DspCursCmd *		_cursorMatrixCmd; 	
	DspCursDumpCmd *	_cursDumpCmd;
	DspFileNameCmd *	_fileNameCmd;
	Cmd *			_printWidgetTreeCmd;
	PostSingleFileDialogCmd * _postFileDialogCmd;	
	int			_pixelSize; // used in popup
   				
	virtual void		createDisplay();
   	virtual void		createImageModel()=0;
   	virtual void 		createImageDisplay(Widget)=0;
   	virtual void		createHistDisplay(Widget)=0;
   	virtual void		createTextDisplay()=0;
   	virtual void		setAttachments();
   	virtual DspDisplayPolicy * createDisplayPolicy();
   	virtual void		updateImageModel();
   	virtual void		createPopup()=0; // calls addpopup
   	virtual void		addPopup( // called by createPopup in subclass
   					ImageDisplayView * imageView,
   					ImageData * dataModel,
   					DspImageBasic * imageDisplayer,
   					int	i,
   					Histogram * rawHistModel,
   					Histogram * stretchHistModel,
					Boolean posSen=TRUE, 
					Boolean dnSen=TRUE,
					Boolean dumpSen=TRUE,
					Boolean strSen=TRUE, 
					Boolean treeSen=TRUE);
	virtual void		createPulloutDumpMenu(MenuCmdList * menuCmdList, 
								Boolean dumpSen,
								ImageData * dataModel,
								ImageDisplayView * imageView);
   public:  
   				DspInstrBasic(Widget, char *, DspWindowBasic * ,int);
				~DspInstrBasic();
	virtual void		manage();
	virtual void 		unmanage();  	  	  	
   	virtual void		update();
	virtual const char *const className() { return "DspInstrBasic"; }
	
};
#endif
