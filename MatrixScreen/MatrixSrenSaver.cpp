#ifdef _UNICODE   
#if defined _M_IX86   
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")   
#elif defined _M_IA64   
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")   
#elif defined _M_X64   
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")   
#else   
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")   
#endif   
#endif  
#pragma comment(lib, "comctl32.lib")
#ifdef UNICODE
#pragma comment(lib, "scrnsavw.lib")
#else
#pragma comment(lib, "scrnsave.lib")
#endif
#include<stdio.h>
#include<Windows.h>
#include<ScrnSave.h>
#include<commctrl.h>
#include"resource.h"
#include<process.h>
#include<time.h>
#define ID_TIME		101
#define WM_CHECK   1001
#define THREAD_OUT 1002
TCHAR szAppName[]=TEXT("Matrix Screen Saver");
#pragma data_seg("GLOBAL")
static int THREADCONSIST=0;
static int MAXTHREAD; //50
#pragma data_seg()
HWND WinHandle;
int WofW=12;//Text  Width
int	HofW=18;//Text Height
int Type;
int SCRNW,SCRNH;  //SCRN->SCREEN W/H->Width/Height->X/Y
HFONT Font;
TCHAR Table[127];
LOGFONT logFONT;
BOOL TIMER=TRUE;
int R=137,G=228,B=157;
short SCRNL;
int Thread;
short Length;
BOOL Erase;
typedef struct infoWords
{
	int x,x1;
	int y,y1;
	int C,C1; 
	int TM;//rand 8-43  rand%35+8
};
typedef struct TypeThread
{
	int Type;
	int Thread;
	BOOL Erase;
};
void SetText(HDC hdc,short opt)
{
	static COLORREF color[]={ RGB(30,80,35),
		                      RGB(255,255,150),
							  RGB(212,231,106),
							  RGB(192,230,101),

						      RGB(140,180,80),

						      RGB(R-80,G-80,B-80),
							  RGB(R-90,G-90,B-90),							  
							  RGB(R-90,G-90,B-90),       //6
							  RGB(100,165,80),       //7
							  RGB(R-100,G-100,B-100),    //8
							  RGB(120,155,70),    //9
 							  RGB(R-110,G-110,B-110),    //10
	                          RGB(0,0,0),
							  RGB(20,55,20),
	                         };
	static short BKMODE[]={2,3,3,2,2,2,2,2,2,2,2,2,2,2};
	SetBkMode(hdc,BKMODE[opt]);
	SetBkColor(hdc,RGB(0,0,0));
	SetTextColor(hdc,color[opt]);
}
typedef enum Multi
{
	Single=0,
	Multi=1
};
void EmptySpace(HDC hdc,char*CH)
{
	SetText(hdc,12);
	CH[0]=32;CH[1]=0;
}
void RandZ(char*CH)
{CH[0]=(CH[0]=rand()%65+33)>0x3F?CH[0]+28:CH[0];CH[1]=0;}
void RandO(char*CH)
{CH[0]=rand()%94+33;CH[1]=0;}
void RandS(char*CH)
{CH[0]=rand()%10+0x30;CH[1]=0;}
void RandT(char*CH)
{CH[0]=rand()%2+'0';CH[1]=0;}
void (*RandFunc[4])(char*CH)
={&RandZ,&RandO,&RandS,&RandT};
 void  MatrixString(void*params)
{
	infoWords*Pls=(infoWords*)params;
	HDC hdc;
	short Nail=(Pls->y/HofW-Pls->C)<(Pls->y1/HofW-Pls->C1)?
		       (Pls->y/HofW-Pls->C):(Pls->y1/HofW-Pls->C1);
	short CL,CL1;
	char CH[3];	
	short C=0;
	hdc=GetDC(WinHandle);
	SelectObject(hdc,Font);
	SetBkMode(hdc,2);
	SetBkColor(hdc,RGB(0,0,0));
	Nail-=5;
	while(Nail<SCRNL)
	{		
		 CL=(Pls->y/HofW-Pls->C),CL1=(Pls->y1/HofW-Pls->C1);
	    for(int i=1;i<Pls->C+2&&CL-2<SCRNL;i++)
	    {
			
			(RandFunc[Type])(CH);
			if(i<4)
				 SetText(hdc,i);
			else if(i==Pls->C-1||i==Pls->C-2)
	             i==Pls->C-1?SetText(hdc,9):SetText(hdc,7);
			else if(i==Pls->C)
                 SetText(hdc,0);//:EmptySpace(hdc,CH);
			else if(i==Pls->C+1)
				 Erase==0?SetText(hdc,13):EmptySpace(hdc,CH);
      		else
			     SetText(hdc,4);
			C=Type==0?i*6:0;
		TextOut(hdc,Pls->x,Pls->y-i*HofW+C,CH,Length);		
	    }
		for(int i=1;i<Pls->C1+2&&CL1-2<SCRNL;i++)
	    {
			(RandFunc[Type])(CH);
			if(i<4)
				 SetText(hdc,i);
			else if(i==Pls->C1-1||i==Pls->C1-2)
	             i==Pls->C1-1?SetText(hdc,9):SetText(hdc,7);
			else if(i==Pls->C1)
                 SetText(hdc,0);
			else if(i==Pls->C1+1)
				 Erase==0?SetText(hdc,13):EmptySpace(hdc,CH);
      		else
			     SetText(hdc,4);
			C=Type==0?i*6:0;
		TextOut(hdc,Pls->x1,Pls->y1-i*HofW+C,CH,Length);		
	    }
		Pls->y1+=HofW;
		Pls->y+=HofW;
		Sleep(Pls->TM);
		Nail+=1;
		
	}
	ReleaseDC(WinHandle,hdc);
	THREADCONSIST-=1;
	free(Pls);
	SendMessage(WinHandle,WM_CHECK,0,0);
	_endthread();
}
 void TimerThread(void *NUL)
 {
	 while(1&&TIMER!=0)
	 {	 
		 if(THREADCONSIST<MAXTHREAD)
			 SendMessage(WinHandle,WM_CHECK,0,0),
			 ++THREADCONSIST,Sleep(100);
		 
	 }
 }
LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static PAINTSTRUCT ps;
	static HDC hdc;
	static TypeThread Info;
	static infoWords*ptrLink;
	static FILE*fs=NULL;
	switch(message)
	{	
	case WM_CREATE:
		fs=fopen("MatrixScrnSaver.ini","r");
		if(fs!=NULL)
		{
		  fread(&Info,sizeof(TypeThread),1,fs);
		  fclose(fs);
		}
		else
		{
			Info.Erase=TRUE;
			Info.Thread=25;
			Info.Type=1;
		}
		
		MAXTHREAD=Info.Thread;
		Type=Info.Type;
		Length=1;
		SCRNW=GetSystemMetrics(SM_CXVIRTUALSCREEN);
		SCRNH=GetSystemMetrics(SM_CYVIRTUALSCREEN);
		SCRNL=SCRNH/HofW;
		hMainWindow=hwnd;
		memset(&logFONT,0,sizeof(LOGFONT));
			logFONT.lfCharSet=0;
			if(Type==0)
			{WofW=11;
			 HofW=24;}
		    logFONT.lfHeight=HofW;
		    Type==0?wsprintf(logFONT.lfFaceName,"Matrix Code NFI"):wsprintf(logFONT.lfFaceName,"OCR A Std");
		    logFONT.lfQuality=0;
		    logFONT.lfPitchAndFamily=0x30|0;
		    logFONT.lfWidth=WofW;
		    logFONT.lfWeight=900;		
		srand(time(0));
		Font=CreateFontIndirect(&logFONT);
		WinHandle=hwnd;
		Erase=Info.Erase;
		for(int i=0;i<MAXTHREAD;i++)
		{
		ptrLink=(infoWords*)malloc(sizeof(infoWords));
		ptrLink->C=rand()%9+12;ptrLink->C1=rand()%9+12;
		ptrLink->x=WofW*(rand()%(SCRNW/(WofW)));
		ptrLink->y=HofW*(rand()%((SCRNH)/HofW))-SCRNH;
        ptrLink->x1=WofW*(rand()%(SCRNW/(WofW)));
		ptrLink->y1=HofW*(rand()%(SCRNH/HofW))-SCRNH;
		ptrLink->TM=rand()%(85-27)+27;
		_beginthread(MatrixString,0,ptrLink);
		Sleep(30);
		}
		break;
	case WM_CHECK:	
		    ptrLink=(infoWords*)malloc(sizeof(infoWords));
			ptrLink->C=rand()%9+12;ptrLink->C1=rand()%9+12;
			ptrLink->x=WofW*(rand()%(SCRNW/(WofW)));
			ptrLink->y=HofW*(rand()%((SCRNH)/HofW))-SCRNH;

			ptrLink->x1=WofW*(rand()%(SCRNW/(WofW)));
			ptrLink->y1=HofW*(rand()%(SCRNH/HofW))-SCRNH;
			ptrLink->TM=rand()%(85-27)+27;
			_beginthread(MatrixString,0,ptrLink);
		break;
	case WM_DESTROY:

		PostQuitMessage(0);	
		TIMER=0;
		break;
	default:
		break;
	}
	return DefScreenSaverProc(hwnd, message, wParam, lParam);
}
BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	static FILE*fs;
	static TypeThread Info={1,24,FALSE};
	static TCHAR TdCod[4];
	static HWND Slider=GetDlgItem(hDlg,IDC_SLIDER1);
  switch(message)
  {
  case WM_INITDIALOG:
	  fs=fopen("MatrixScrnSaver.ini","w+");
	  SendMessage(Slider,TBM_SETRANGE, (WPARAM)1,(LPARAM)MAKELONG(0,199));
	  SendMessage(GetDlgItem(hDlg,IDC_RADIO2),BM_SETCHECK,1,0);	  
	  SendMessage(GetDlgItem(hDlg,IDC_RADIO5),BM_SETCHECK,1,0);
	  SendMessage(GetDlgItem(hDlg,IDC_SLIDER1),TBM_SETPOS,1,4);
	  return false;
   case WM_COMMAND:
	   if(LOWORD(wParam)>1004&&LOWORD(wParam)<1009)EnableWindow(GetDlgItem(hDlg,IDC_RADIO5),true);
     switch(LOWORD(wParam))
     {
       case IDC_RADIO1:
	  	 Info.Type=0;
		 Info.Erase=1;
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO2),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO4),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO1),BM_SETCHECK,1,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO5),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO6),BM_SETCHECK,1,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO7),BM_SETCHECK,0,0);
		 EnableWindow(GetDlgItem(hDlg,IDC_RADIO5),false);
	  	 return false;
       case IDC_RADIO2:
	  	 Info.Type=1;
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO1),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO4),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO2),BM_SETCHECK,1,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO7),BM_SETCHECK,0,0);
	  	 return false;
	   case IDC_RADIO4:
		 Info.Type=2;
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO1),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO2),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO4),BM_SETCHECK,1,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO7),BM_SETCHECK,0,0);
		 return false;
	   case IDC_RADIO7:
		 Info.Type=3;
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO1),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO2),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO4),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO7),BM_SETCHECK,1,0);
		 return false;
	   case IDC_RADIO5:
		 Info.Erase=FALSE;
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO6),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO5),BM_SETCHECK,1,0);
		 return false;
	   case IDC_RADIO6:
		 Info.Erase=TRUE;
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO5),BM_SETCHECK,0,0);
		 SendMessage(GetDlgItem(hDlg,IDC_RADIO6),BM_SETCHECK,1,0);
		 return false;
       case IDOK:
		Info.Thread = (unsigned char)SendMessage(GetDlgItem(hDlg,IDC_SLIDER1),TBM_GETPOS,0,0);
        Info.Thread++;
		fwrite(&Info,sizeof(TypeThread),1,fs);
		fclose(fs);
		EndDialog(hDlg,1);
        return true;
       case IDCANCEL:
		   Info.Thread++;
	    fwrite(&Info,sizeof(TypeThread),1,fs);
		fclose(fs);
        EndDialog(hDlg,0);
        return true;
     }
        break;
   case WM_DESTROY:
	   Info.Thread++;
	    fwrite(&Info,sizeof(TypeThread),1,fs);
	   fclose(fs);
       EndDialog(hDlg,0);	   
	   return true;
    }
   return false;
}
BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}


