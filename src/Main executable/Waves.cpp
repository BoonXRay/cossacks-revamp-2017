#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "multipl.h"
#include "fog.h"
#include "walls.h"
#include "Nature.h"
#include <time.h>
#include "Nucl.h"

#include "Megapolis.h"
#include "dialogs.h"
#include <assert.h>
#include <math.h>
#include "Masks.h"
#include "fonts.h"
#include "3DGraph.h"
#include "MapSprites.h"
void ErrM(char* s);
class WaveFrame {
public:
	bool Enabled;
	int x;
	int y;
	NewAnimation* WAnm;
	byte CurSprite;
	char vx;
	char vy;
	char CheckShiftX;
	char CheckShiftY;
	byte Direction;
};
#define MaxWFrames  512
WaveFrame WAVES[MaxWFrames];
int NWaves;
NewAnimation* WaveAnm[32];
#define LoCRange 0xB0
#define HiCRange  0xBB
int MinX;
int MinY;
int MaxX;
int MaxY;
byte WSET[256];
NewAnimation* GetNewAnimationByName(char* Name);
void InitWaves() {
	for (int i = 0; i < MaxWFrames; i++)WAVES[i].Enabled = false;
};
void LoadWaveAnimations() {
	ResFile ff = RReset("water.set");
	RBlockRead(ff, WSET, 256);
	RClose(ff);
	char gx[128];
	char ann[64];
	GFILE* f1 = Gopen("WaveList.lst", "r");
	
	if (f1)
	{
		Gscanf(f1, "%d", &NWaves);
		for (int j = 0; j < NWaves; j++)
		{
			Gscanf(f1, "%s", ann);
			WaveAnm[j] = GetNewAnimationByName(ann);
			if (!WaveAnm[j])
			{
				sprintf(gx, " WaveList.lst : Unknown animation (%d) : %s", j, ann);
				ErrM(gx);
			}
		}
		Gclose(f1);
	}
	else
	{
		ErrM("Could not load WaveList.lst");
	}
}

int mul3(int);
