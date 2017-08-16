#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "fog.h"
#include "Megapolis.h"

#include <assert.h>
#include "walls.h"
#include "mode.h"
#include "GSound.h"
#include "MapSprites.h"
#include "NewMon.h"

int prevrtime = 0;
FILE* FX = nullptr;
extern bool RecordMode;
extern byte PlayGameMode;
char CURLOG[32];

extern int tmtmt;
extern DWORD RealTime;

void Syncro::Copy( Syncro* Syn )
{
	memcpy( Syn->RSL, RSL, NSlots * sizeof RandSlot );
	Syn->NSlots = NSlots;
	NSlots = 0;
}

Syncro SYN;
Syncro SYN1;
extern short randoma[8192];
extern char LASTFILE[128];
extern int LastLine;
char* LastFile;

int RandNew( char* File, int Line )
{
	LastFile = File;
	LastLine = Line;
	int r = randoma[rpos];
	rpos = ( rpos + 1 ) & 8191;
	if ( SYN.NSlots < maxUline )
	{
		RandSlot* RS = SYN.RSL + SYN.NSlots;
		RS->FileName = File;
		RS->Line = Line;
		RS->Param = 0;
		RS->rpos = rpos;
		RS->Type = -1;
		SYN.NSlots++;
	}

	return r;
}
