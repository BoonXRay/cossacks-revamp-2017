//Masking 2 or 3 textures in triangle
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
byte ResultMask[MaskLx*256];
extern byte trans4[65536];
extern byte trans8[65536];
void CopyMaskedBitmap64(byte* Bits,int x,int y,void* Mask){
	// BoonXRay 12.08.2017
	//__asm 
	{
		//push	esi
		//push	edi
		//pushf
		//mov		esi, Mask
		//mov		edi, Bits
		//xor		ebx, ebx
		//mov		bl, byte ptr x
		//mov		bh, byte ptr y
		//mov		cl, [esi + 2]    //Number of strings
		//add		esi, 4
		//xor edx, edx     //ResulMask offset
		unsigned int TmpESI = reinterpret_cast<unsigned int>(Mask);
		unsigned int TmpEDI = reinterpret_cast<unsigned int>(Bits);
		unsigned int TmpEAX = 0, TmpEBX = 0, TmpEDX = 0;
		unsigned char & TmpAL = *reinterpret_cast<unsigned char *>(&TmpEAX);
		unsigned char & TmpAH = *(reinterpret_cast<unsigned char *>(&TmpEAX) + 1);
		unsigned short & TmpAX = *reinterpret_cast<unsigned short *>(&TmpEAX);
		unsigned char & TmpBL = *reinterpret_cast<unsigned char *>(&TmpEBX);
		unsigned char & TmpBH = *(reinterpret_cast<unsigned char *>(&TmpEBX) + 1);
		unsigned short & TmpBX = *reinterpret_cast<unsigned short *>(&TmpEBX);
		unsigned char & TmpDL = *reinterpret_cast<unsigned char *>(&TmpEDX);
		unsigned char & TmpDH = *(reinterpret_cast<unsigned char *>(&TmpEDX) + 1);
		TmpBL = x;
		TmpBH = y;
		unsigned char TmpCL = *reinterpret_cast<unsigned char *>(TmpESI + 2), TmpCH = 0;	//Number of strings
		TmpESI += 4;
	rlclop1 :
		//mov		ch, [esi]    //number of RLE segments
		//inc		esi
		//or ch, ch
		//jz		endrlclop
		TmpCH = *reinterpret_cast<unsigned char *>(TmpESI);		//number of RLE segments
		TmpESI++;
		if (TmpCH == 0) goto endrlclop;
	rllpstrt :
		//mov		ax, [esi]
		//add		bl,al
		//add		dl,al
		TmpAX = *reinterpret_cast<unsigned short *>(TmpESI);
		TmpBL += TmpAL;
		TmpDL += TmpAL;
	lpp1:	
		//and		bx,0011111100111111b
		//mov		al,[edi+ebx]
		//inc		ebx
		TmpBX &= 0x3F3F;
		TmpAL = *reinterpret_cast<unsigned char *>(TmpEDI + TmpEBX);
		TmpEBX++;
		//mov		[ResultMask+edx],al
		//inc		edx
		//dec		ah
		//jnz		lpp1
		*reinterpret_cast<unsigned char *>(ResultMask + TmpEDX) = TmpAL;
		TmpEDX++;
		TmpAH--;
		if (TmpAH != 0) goto lpp1;
		//add		esi,2
		//dec		ch
		//jnz		rllpstrt
		TmpESI += 2;
		TmpCH--;
		if (TmpCH != 0) goto rllpstrt;
	endrlclop :
		//xor		dl, dl
		//inc		bh
		//inc		dh
		//mov		bl, byte ptr x
		//dec		cl
		//jnz		rlclop1
		//popf
		//pop		edi
		//pop		esi
		TmpDL = 0;
		TmpBH++;
		TmpDH++;
		TmpBL = x;
		TmpCL--;
		if (TmpCL != 0) goto rlclop1;
	};
};
void CopyMaskedTransparentBitmap_8(byte* Bits,int x,int y,void* Mask){
	byte nstr;
	// BoonXRay 12.08.2017
	//__asm 
	{
		//push	esi
		//push	edi
		//pushf
		//mov		esi, Mask
		//mov		edi, Bits
		//xor		ebx, ebx
		//mov		bl, byte ptr x
		//mov		bh, byte ptr y
		//mov		cl, [esi + 2]    //Number of strings
		//mov		nstr, cl
		//add		esi, 4
		//xor eax, eax
		//xor		edx, edx     //ResulMask offset
		unsigned int TmpESI = reinterpret_cast<unsigned int>(Mask);
		unsigned int TmpEDI = reinterpret_cast<unsigned int>(Bits);
		unsigned int TmpEAX = 0, TmpEBX = 0, TmpEDX = 0;
		unsigned char & TmpAL = *reinterpret_cast<unsigned char *>(&TmpEAX);
		unsigned char & TmpAH = *(reinterpret_cast<unsigned char *>(&TmpEAX) + 1);
		unsigned short & TmpAX = *reinterpret_cast<unsigned short *>(&TmpEAX);
		unsigned char & TmpBL = *reinterpret_cast<unsigned char *>(&TmpEBX);
		unsigned char & TmpBH = *(reinterpret_cast<unsigned char *>(&TmpEBX) + 1);
		unsigned short & TmpBX = *reinterpret_cast<unsigned short *>(&TmpEBX);
		unsigned char & TmpDL = *reinterpret_cast<unsigned char *>(&TmpEDX);
		unsigned char & TmpDH = *(reinterpret_cast<unsigned char *>(&TmpEDX) + 1);
		TmpBL = x;
		TmpBH = y;
		unsigned char TmpCL = *reinterpret_cast<unsigned char *>(TmpESI + 2), TmpCH = 0;	//Number of strings
		nstr = TmpCL;
		TmpESI += 4;
	rlclop1 :
		//mov		ch, [esi]    //number of RLE segments
		//inc		esi
		//or ch, ch
		//jz		endrlclop
		TmpCH = *reinterpret_cast<unsigned char *>(TmpESI);		//number of RLE segments
		TmpESI++;
		if (TmpCH == 0) goto endrlclop;
	rllpstrt :
		//mov		ax, [esi]
		//add		bl, al
		//add		dl, al
		//mov		cl, ah
		TmpAX = *reinterpret_cast<unsigned short *>(TmpESI);
		TmpBL += TmpAL;
		TmpDL += TmpAL;
		TmpCL = TmpAH;
	lpp1 : 
		//and		bx, 0011111100111111b
		//mov		ah, [edi + ebx]
		//mov		al, [ResultMask + edx]
		//mov		al, [trans8 + eax]
		//inc		ebx
		TmpBX &= 0x3F3F;
		TmpAH = *reinterpret_cast<unsigned char *>(TmpEDI + TmpEBX);
		TmpAL = *reinterpret_cast<unsigned char *>(ResultMask + TmpEDX);
		TmpAL = *reinterpret_cast<unsigned char *>(trans8 + TmpEAX);
		TmpEBX++;
		//mov[ResultMask + edx], al
		//inc		edx
		//dec		cl
		//jnz		lpp1
		//add		esi, 2
		//dec		ch
		//jnz		rllpstrt
		*reinterpret_cast<unsigned char *>(ResultMask + TmpEDX) = TmpAL;
		TmpEDX++;
		TmpCL--;
		if (TmpCL != 0) goto lpp1;
		TmpESI += 2;
		TmpCH--;
		if (TmpCH != 0) goto rllpstrt;
	endrlclop :
		//xor		dl, dl
		//inc		bh
		//inc		dh
		//mov		bl, byte ptr x
		//dec		nstr
		//jnz		rlclop1
		//popf
		//pop		edi
		//pop		esi
		TmpDL = 0;
		TmpBH++;
		TmpDH++;
		TmpBL = x;
		nstr--;
		if (nstr != 0) goto rlclop1;
	};
};
void CopyMaskedTransparentBitmap_4(byte* Bits,int x,int y,void* Mask){
	byte nstr;
	// BoonXRay 12.08.2017
	//__asm 
	{
		//push	esi
		//push	edi
		//pushf
		//mov		esi, Mask
		//mov		edi, Bits
		//xor		ebx, ebx
		//mov		bl, byte ptr x
		//mov		bh, byte ptr y
		//mov		cl, [esi + 2]    //Number of strings
		//mov		nstr, cl
		//add		esi, 4
		//xor eax, eax
		//xor		edx, edx     //ResulMask offset
		unsigned int TmpESI = reinterpret_cast<unsigned int>(Mask);
		unsigned int TmpEDI = reinterpret_cast<unsigned int>(Bits);
		unsigned int TmpEAX = 0, TmpEBX = 0, TmpEDX = 0;
		unsigned char & TmpAL = *reinterpret_cast<unsigned char *>(&TmpEAX);
		unsigned char & TmpAH = *(reinterpret_cast<unsigned char *>(&TmpEAX) + 1);
		unsigned short & TmpAX = *reinterpret_cast<unsigned short *>(&TmpEAX);
		unsigned char & TmpBL = *reinterpret_cast<unsigned char *>(&TmpEBX);
		unsigned char & TmpBH = *(reinterpret_cast<unsigned char *>(&TmpEBX) + 1);
		unsigned short & TmpBX = *reinterpret_cast<unsigned short *>(&TmpEBX);
		unsigned char & TmpDL = *reinterpret_cast<unsigned char *>(&TmpEDX);
		unsigned char & TmpDH = *(reinterpret_cast<unsigned char *>(&TmpEDX) + 1);
		TmpBL = x;
		TmpBH = y;
		unsigned char TmpCL = *reinterpret_cast<unsigned char *>(TmpESI + 2), TmpCH = 0;	//Number of strings
		nstr = TmpCL;
		TmpESI += 4;
	rlclop1 :
		//mov		ch, [esi]    //number of RLE segments
		//inc		esi
		//or ch, ch
		//jz		endrlclop
		TmpCH = *reinterpret_cast<unsigned char *>(TmpESI);		//number of RLE segments
		TmpESI++;
		if (TmpCH == 0) goto endrlclop;
	rllpstrt :
		//mov		ax, [esi]
		//add		bl, al
		//add		dl, al
		//mov		cl, ah
		TmpAX = *reinterpret_cast<unsigned short *>(TmpESI);
		TmpBL += TmpAL;
		TmpDL += TmpAL;
		TmpCL = TmpAH;
	lpp1 : 
		//and		bx, 0011111100111111b
		//mov		ah, [edi + ebx]
		//mov		al, [ResultMask + edx]
		//mov		al, [trans4 + eax]
		//inc		ebx
		TmpBX &= 0x3F3F;
		TmpAH = *reinterpret_cast<unsigned char *>(TmpEDI + TmpEBX);
		TmpAL = *reinterpret_cast<unsigned char *>(ResultMask + TmpEDX);
		TmpAL = *reinterpret_cast<unsigned char *>(trans4 + TmpEAX);
		TmpEBX++;
		//mov[ResultMask + edx], al
		//inc		edx
		//dec		cl
		//jnz		lpp1
		//add		esi, 2
		//dec		ch
		//jnz		rllpstrt
		*reinterpret_cast<unsigned char *>(ResultMask + TmpEDX) = TmpAL;
		TmpEDX++;
		TmpCL--;
		if (TmpCL != 0) goto lpp1;
		TmpESI += 2;
		TmpCH--;
		if (TmpCH != 0) goto rllpstrt;
	endrlclop :
		//xor		dl, dl
		//inc		bh
		//inc		dh
		//mov		bl, byte ptr x
		//dec		nstr
		//jnz		rlclop1
		//popf
		//pop		edi
		//pop		esi
		TmpDL = 0;
		TmpBH++;
		TmpDH++;
		TmpBL = x;
		nstr--;
		if (nstr != 0) goto rlclop1;
	};
};
void CopyMaskedTransparentBitmap_12(byte* Bits,int x,int y,void* Mask){
	byte nstr;
	// BoonXRay 12.08.2017
	//__asm
	{
		//push	esi
		//push	edi
		//pushf
		//mov		esi, Mask
		//mov		edi, Bits
		//xor		ebx, ebx
		//mov		bl, byte ptr x
		//mov		bh, byte ptr y
		//mov		cl, [esi + 2]    //Number of strings
		//mov		nstr, cl
		//add		esi, 4
		//xor eax, eax
		//xor		edx, edx     //ResulMask offset
		unsigned int TmpESI = reinterpret_cast<unsigned int>(Mask);
		unsigned int TmpEDI = reinterpret_cast<unsigned int>(Bits);
		unsigned int TmpEAX = 0, TmpEBX = 0, TmpEDX = 0;
		unsigned char & TmpAL = *reinterpret_cast<unsigned char *>(&TmpEAX);
		unsigned char & TmpAH = *(reinterpret_cast<unsigned char *>(&TmpEAX) + 1);
		unsigned short & TmpAX = *reinterpret_cast<unsigned short *>(&TmpEAX);
		unsigned char & TmpBL = *reinterpret_cast<unsigned char *>(&TmpEBX);
		unsigned char & TmpBH = *(reinterpret_cast<unsigned char *>(&TmpEBX) + 1);
		unsigned short & TmpBX = *reinterpret_cast<unsigned short *>(&TmpEBX);
		unsigned char & TmpDL = *reinterpret_cast<unsigned char *>(&TmpEDX);
		unsigned char & TmpDH = *(reinterpret_cast<unsigned char *>(&TmpEDX) + 1);
		TmpBL = x;
		TmpBH = y;
		unsigned char TmpCL = *reinterpret_cast<unsigned char *>(TmpESI + 2), TmpCH = 0;	//Number of strings
		nstr = TmpCL;
		TmpESI += 4;
	rlclop1 :
		//mov		ch, [esi]    //number of RLE segments
		//inc		esi
		//or ch, ch
		//jz		endrlclop
		TmpCH = *reinterpret_cast<unsigned char *>(TmpESI);		//number of RLE segments
		TmpESI++;
		if (TmpCH == 0) goto endrlclop;
	rllpstrt :
		//mov		ax, [esi]
		//add		bl, al
		//add		dl, al
		//mov		cl, ah
		TmpAX = *reinterpret_cast<unsigned short *>(TmpESI);
		TmpBL += TmpAL;
		TmpDL += TmpAL;
		TmpCL = TmpAH;
	lpp1 : 
		//and		bx, 0011111100111111b
		//mov		al, [edi + ebx]
		//mov		ah, [ResultMask + edx]
		//mov		al, [trans4 + eax]
		//inc		ebx
		TmpBX &= 0x3F3F;
		TmpAL = *reinterpret_cast<unsigned char *>(TmpEDI + TmpEBX);
		TmpAH = *reinterpret_cast<unsigned char *>(ResultMask + TmpEDX);
		TmpAL = *reinterpret_cast<unsigned char *>(trans4 + TmpEAX);
		TmpEBX++;
		//mov[ResultMask + edx], al
		//inc		edx
		//dec		cl
		//jnz		lpp1
		//add		esi, 2
		//dec		ch
		//jnz		rllpstrt
		*reinterpret_cast<unsigned char *>(ResultMask + TmpEDX) = TmpAL;
		TmpEDX++;
		TmpCL--;
		if (TmpCL != 0) goto lpp1;
		TmpESI += 2;
		TmpCH--;
		if (TmpCH != 0) goto rllpstrt;
	endrlclop :
		//xor		dl, dl
		//inc		bh
		//inc		dh
		//mov		bl, byte ptr x
		//dec		nstr
		//jnz		rlclop1
		//popf
		//pop		edi
		//pop		esi
		TmpDL = 0;
		TmpBH++;
		TmpDH++;
		TmpBL = x;
		nstr--;
		if (nstr != 0) goto rlclop1;
	};
};
extern RLCTable SimpleMaskA;
extern RLCTable SimpleMaskB;
extern RLCTable SimpleMaskC;
extern RLCTable SimpleMaskD;
void CopyMaskedBitmap(byte* Bits,int x,int y,int MaskID)
{
	CopyMaskedBitmap64(Bits,x,y,   (void*)(SimpleMaskA->OfsTable[MaskID]));
	CopyMaskedTransparentBitmap_4(Bits,x,y,(void*)(SimpleMaskB->OfsTable[MaskID]));
	CopyMaskedTransparentBitmap_8(Bits,x,y, (void*)(SimpleMaskC->OfsTable[MaskID]));
	CopyMaskedTransparentBitmap_12(Bits,x,y, (void*)(SimpleMaskD->OfsTable[MaskID]));
};
//  0
//	|\
//	|  \
//	|    \
//	|    /
//	|  /
//	|/ 
//Creates triangle (Type1) with bitmap
void FastCreateMaskedBitmap64_1(byte* Bits,int x,int y){
	int tmedi;
	// BoonXRay 12.08.2017
	//__asm 
	{
		//push	esi
		//push	edi
		//pushf
		//mov		al, 64
		//xor ebx, ebx
		//mov		bl, byte ptr x
		//mov		bh, byte ptr y
		//and		ebx, 0011111100111111b
		//sub		al, bl       //Max x in trapezoid
		//xor		ecx, ecx
		//mov		ah, 16//32
		//mov		edi, offset ResultMask
		//mov		dl, 2
		//cld
		//mov		tmedi, edi
		unsigned int TmpEAX = 0, TmpEBX = 0, TmpECX = 0, TmpEDX = 0;
		unsigned char & TmpAL = *reinterpret_cast<unsigned char *>(&TmpEAX);
		unsigned char & TmpAH = *(reinterpret_cast<unsigned char *>(&TmpEAX) + 1);
		unsigned char & TmpBL = *reinterpret_cast<unsigned char *>(&TmpEBX);
		unsigned char & TmpBH = *(reinterpret_cast<unsigned char *>(&TmpEBX) + 1);
		unsigned char & TmpCL = *reinterpret_cast<unsigned char *>(&TmpECX);
		unsigned char & TmpDL = *reinterpret_cast<unsigned char *>(&TmpEDX);
		unsigned char & TmpDH = *(reinterpret_cast<unsigned char *>(&TmpEDX) + 1);
		TmpBL = x;
		TmpBH = y;
		TmpEBX &= 0x3F3F;
		TmpAL = 64 - TmpBL;		//Max x in trapezoid
		TmpAH = 16;
		unsigned int TmpEDI = reinterpret_cast<unsigned int>(ResultMask), TmpESI;
		TmpDL = 2;
		tmedi = TmpEDI;
	StartTrapezoid1 :
		//and     ebx, 0011111100111111b
		//mov		esi, Bits
		//add		esi, ebx
		//cmp		dl, al
		//ja		RightPart1
		TmpEBX &= 0x3F3F;
		TmpESI = reinterpret_cast<unsigned int>(Bits) + TmpEBX;
		if (TmpDL > TmpAL) goto RightPart1;
		//mov		cl, dl
		//shr		cl, 2
		//rep		movsd
		//mov		cl, dl
		//and		cl, 3
		//rep     movsb
		//add		tmedi, 256
		//mov		edi, tmedi
		//add		dl, 2
		//inc		bh
		//dec		ah
		//jnz		StartTrapezoid1
		//jmp		Trap2
		memcpy(reinterpret_cast<void *>(TmpEDI), reinterpret_cast<void *>(TmpESI), TmpDL);
		TmpEDI += TmpDL;
		TmpESI += TmpDL;
		tmedi += 256;
		TmpEDI = tmedi;
		TmpDL += 2;
		TmpBH++;
		TmpAH--;
		if (TmpAH != 0) goto StartTrapezoid1;
		goto Trap2;
	RightPart1 :
		//mov		cl, al
		//shr		cl, 2
		//rep		movsd
		//mov		cl, al
		//and		cl, 3
		//rep		movsb
		memcpy(reinterpret_cast<void *>(TmpEDI), reinterpret_cast<void *>(TmpESI), TmpAL);
		TmpEDI += TmpAL;
		TmpESI += TmpAL;
		//sub		esi, 64
		//mov		cl, dl
		//sub		cl, al
		//mov		dh, cl
		//shr		cl, 2
		//rep		movsd
		//mov		cl, dh
		//and		cl, 3
		//rep		movsb
		TmpESI -= 64;
		TmpDH = TmpDL - TmpAL;
		memcpy(reinterpret_cast<void *>(TmpEDI), reinterpret_cast<void *>(TmpESI), TmpDH);
		TmpEDI += TmpDH;
		TmpESI += TmpDH;
		//add		dl, 2
		//add		tmedi, 256
		//mov		edi, tmedi
		//inc		bh
		//dec		ah
		//jnz		StartTrapezoid1
		TmpDL += 2;
		tmedi += 256;
		TmpEDI = tmedi;
		TmpBH++;
		TmpAH--;
		if (TmpAH != 0) goto StartTrapezoid1;
	Trap2 : 
		//sub		dl, 2
		//mov		ah, 16
		TmpDL -= 2;
		TmpAH = 16;
	DoTrapezoid2 :
		//and     ebx, 0011111100111111b
		//mov		esi, Bits
		//add		esi, ebx
		//cmp		dl, al
		//ja		RightPart2
		TmpEBX &= 0x3F3F;
		TmpESI = reinterpret_cast<unsigned int>(Bits) + TmpEBX;
		if (TmpDL > TmpAL) goto RightPart2;
		//mov		cl, dl
		//shr		cl, 2
		//rep		movsd
		//mov		cl, dl
		//and		cl, 3
		//rep     movsb
		memcpy(reinterpret_cast<void *>(TmpEDI), reinterpret_cast<void *>(TmpESI), TmpDL);
		TmpEDI += TmpDL;
		TmpESI += TmpDL;
		//add		tmedi, 256
		//mov		edi, tmedi
		//sub		dl, 2
		//inc		bh
		//dec		ah
		//jnz		DoTrapezoid2
		//jmp		EndTrapezoid
		tmedi += 256;
		TmpEDI = tmedi;
		TmpDL -= 2;
		TmpBH++;
		TmpAH--;
		if (TmpAH != 0) goto DoTrapezoid2;
		goto EndTrapezoid;
	RightPart2 :
		//mov		cl, al
		//shr		cl, 2
		//rep		movsd
		//mov		cl, al
		//and		cl, 3
		//rep		movsb
		memcpy(reinterpret_cast<void *>(TmpEDI), reinterpret_cast<void *>(TmpESI), TmpAL);
		TmpEDI += TmpAL;
		TmpESI += TmpAL;
		//sub		esi, 64
		//mov		cl, dl
		//sub		cl, al
		//mov		dh, cl
		//shr		cl, 2
		//rep		movsd
		//mov		cl, dh
		//and		cl, 3
		//rep		movsb		
		TmpESI -= 64;
		TmpDH = TmpDL - TmpAL;
		memcpy(reinterpret_cast<void *>(TmpEDI), reinterpret_cast<void *>(TmpESI), TmpDH);
		TmpEDI += TmpDH;
		TmpESI += TmpDH;
		//sub		dl, 2
		//add		tmedi, 256
		//mov		edi, tmedi
		//inc		bh
		//dec		ah
		//jnz		DoTrapezoid2
		TmpDL -= 2;
		tmedi += 256;
		TmpEDI = tmedi;
		TmpBH++;
		TmpAH--;
		if (TmpAH != 0) goto DoTrapezoid2;
	EndTrapezoid :
		//popf
		//pop		edi
		//pop		esi
		;
	};
};
//      /|
//    /  |
// 0/    |
//  \    |
//    \  |
//      \|
void FastCreateMaskedBitmap64_2(byte* Bits,int x,int y){
	int tmedi;
	// BoonXRay 12.08.2017
	//__asm 
	{
		//push	esi
		//push	edi
		//pushf
		//xor		ebx, ebx
		//mov		bl, byte ptr x
		//mov		bh, byte ptr y
		//add		bl, 32//64
		//sub		bh, 16//31
		//and ebx, 0011111100111111b
		//mov		al, bl       //Max x in trapezoid
		//xor		ecx, ecx
		//mov		ah, 16//32
		//mov		edi, offset ResultMask
		//add		edi, 32//64
		//mov		dl, 2
		//std
		//mov		tmedi, edi
		unsigned int TmpEAX = 0, TmpEBX = 0, TmpECX = 0, TmpEDX = 0;
		unsigned char & TmpAL = *reinterpret_cast<unsigned char *>(&TmpEAX);
		unsigned char & TmpAH = *(reinterpret_cast<unsigned char *>(&TmpEAX) + 1);
		unsigned char & TmpBL = *reinterpret_cast<unsigned char *>(&TmpEBX);
		unsigned char & TmpBH = *(reinterpret_cast<unsigned char *>(&TmpEBX) + 1);
		unsigned char & TmpCL = *reinterpret_cast<unsigned char *>(&TmpECX);
		unsigned char & TmpDL = *reinterpret_cast<unsigned char *>(&TmpEDX);
		unsigned char & TmpDH = *(reinterpret_cast<unsigned char *>(&TmpEDX) + 1);
		TmpBL = x;
		TmpBH = y;
		TmpBL += 32;
		TmpBH -= 16;
		TmpEBX &= 0x3F3F;
		TmpAL = TmpBL;		//Max x in trapezoid
		TmpAH = 16;
		unsigned int TmpEDI = reinterpret_cast<unsigned int>(ResultMask) + 32, TmpESI;
		TmpDL = 2;
		tmedi = TmpEDI;
	StartTrapezoid1 :
		//and     ebx, 0011111100111111b
		//mov		esi, Bits
		//add		esi, ebx
		//cmp		dl, al
		//ja		RightPart1
		TmpEBX &= 0x3F3F;
		TmpESI = reinterpret_cast<unsigned int>(Bits) + TmpEBX;
		if (TmpDL > TmpAL) goto RightPart1;
		//mov		cl, dl
		//dec		esi
		//dec		edi
		//cmp		cl, 4
		//jb		uue1
		//shr		cl, 2
		//sub		esi, 3
		//sub		edi, 3
		//rep		movsd
		//add		esi, 3
		//add		edi, 3
		TmpCL = TmpDL;
		TmpESI--;
		TmpEDI--;
		if (TmpCL < 4) goto uue1;
		TmpCL >>= 2;
		TmpESI -= 3;
		TmpEDI -= 3;
		for (; TmpECX != 0; TmpECX--, TmpESI -= 4 /*sizeof(int)*/, TmpEDI -= 4 /*sizeof(int)*/)
			*reinterpret_cast<unsigned int *>(TmpEDI) = *reinterpret_cast<unsigned int *>(TmpESI);
		TmpESI += 3;
		TmpEDI += 3;
	uue1:
		//mov		cl, dl
		//and		cl, 3
		//rep     movsb
		//add		tmedi, 256
		//mov		edi, tmedi
		//inc		bh
		//add		dl, 2
		//dec		ah
		//jnz		StartTrapezoid1
		//jmp		Trap2
		TmpCL = TmpDL & 3;
		for (; TmpECX != 0; TmpECX--, TmpESI--, TmpEDI--)
			*reinterpret_cast<unsigned char *>(TmpEDI) = *reinterpret_cast<unsigned char *>(TmpESI);
		tmedi += 256;
		TmpEDI = tmedi;
		TmpBH++;
		TmpDL += 2;
		TmpAH--;
		if (TmpAH != 0) goto StartTrapezoid1;
		goto Trap2;
	RightPart1 :
		//mov		cl, al
		//dec		esi
		//dec		edi
		//cmp		cl, 4
		//jb		uu3
		//sub		esi, 3
		//sub		edi, 3
		//shr		cl, 2
		//rep		movsd
		//add		esi, 3
		//add		edi, 3
		TmpCL = TmpAL;
		TmpESI--;
		TmpEDI--;
		if (TmpCL < 4) goto uu3;
		TmpCL >>= 2;
		TmpESI -= 3;
		TmpEDI -= 3;
		for (; TmpECX != 0; TmpECX--, TmpESI -= 4 /*sizeof(int)*/, TmpEDI -= 4 /*sizeof(int)*/)
			*reinterpret_cast<unsigned int *>(TmpEDI) = *reinterpret_cast<unsigned int *>(TmpESI);
		TmpESI += 3;
		TmpEDI += 3;
	uu3:
		//mov		cl, al
		//and		cl, 3
		//rep		movsb
		TmpCL = TmpAL & 3;
		for (; TmpECX != 0; TmpECX--, TmpESI--, TmpEDI--)
			*reinterpret_cast<unsigned char *>(TmpEDI) = *reinterpret_cast<unsigned char *>(TmpESI);
		//add		esi, 64
		//mov		cl, dl
		//sub		cl, al
		//mov		dh, cl
		//cmp		cl, 4
		//jb		uu31
		//shr		cl, 2
		//sub		esi, 3
		//sub		edi, 3
		//rep		movsd
		//add		esi, 3
		//add		edi, 3
		TmpESI += 64;
		TmpCL = TmpDL - TmpAL;
		TmpDH = TmpCL;
		if (TmpCL < 4) goto uu31;
		TmpESI -= 3;
		TmpEDI -= 3;
		for (; TmpECX != 0; TmpECX--, TmpESI -= 4 /*sizeof(int)*/, TmpEDI -= 4 /*sizeof(int)*/)
			*reinterpret_cast<unsigned int *>(TmpEDI) = *reinterpret_cast<unsigned int *>(TmpESI);
		TmpESI += 3;
		TmpEDI += 3;
	uu31:
		//mov		cl, dh
		//and		cl, 3
		//rep		movsb
		TmpCL = TmpDH & 3;
		for (; TmpECX != 0; TmpECX--, TmpESI--, TmpEDI--)
			*reinterpret_cast<unsigned char *>(TmpEDI) = *reinterpret_cast<unsigned char *>(TmpESI);
		//add		dl, 2
		//add		tmedi, 256
		//mov		edi, tmedi
		//inc		bh
		//dec		ah
		//jnz		StartTrapezoid1
		TmpDL += 2;
		tmedi += 256;
		TmpEDI = tmedi;
		TmpBH++;
		TmpAH--;
		if (TmpAH != 0) goto StartTrapezoid1;
	Trap2 : 
		//sub		dl, 2
		//mov		ah, 16
		TmpDL -= 2;
		TmpAH = 16;
	DoTrapezoid2 :
		//and     ebx, 0011111100111111b
		//mov		esi, Bits
		//add		esi, ebx
		//cmp		dl, al
		//ja		RightPart2
		TmpEBX &= 0x3F3F;
		TmpESI = reinterpret_cast<unsigned int>(Bits) + TmpEBX;
		if (TmpDL > TmpAL) goto RightPart2;
		//mov		cl, dl
		//dec		esi
		//dec		edi
		//cmp		cl, 4
		//jb		uue1x
		//shr		cl, 2
		//sub		esi, 3
		//sub		edi, 3
		//rep		movsd
		//add		esi, 3
		//add		edi, 3
		TmpCL = TmpDL;
		TmpESI--;
		TmpEDI--;
		if (TmpCL < 4) goto uue1x;
		TmpCL >>= 2;
		TmpESI -= 3;
		TmpEDI -= 3;
		for (; TmpECX != 0; TmpECX--, TmpESI -= 4 /*sizeof(int)*/, TmpEDI -= 4 /*sizeof(int)*/)
			*reinterpret_cast<unsigned int *>(TmpEDI) = *reinterpret_cast<unsigned int *>(TmpESI);
		TmpESI += 3;
		TmpEDI += 3;
	uue1x:
		//mov		cl, dl
		//and		cl, 3
		//rep     movsb
		//add		tmedi, 256
		//mov		edi, tmedi
		//inc		bh
		//sub		dl, 2
		//dec		ah
		//jnz		DoTrapezoid2
		//jmp		EndTrapezoid
		TmpCL = TmpDL & 3;
		for (; TmpECX != 0; TmpECX--, TmpESI--, TmpEDI--)
			*reinterpret_cast<unsigned char *>(TmpEDI) = *reinterpret_cast<unsigned char *>(TmpESI);
		tmedi += 256;
		TmpEDI = tmedi;
		TmpBH++;
		TmpDL -= 2;
		TmpAH--;
		if (TmpAH != 0) goto DoTrapezoid2;
		goto EndTrapezoid;
	RightPart2 :
		//mov		cl, al
		//dec		esi
		//dec		edi
		//cmp		cl, 4
		//jb		uu3x
		//sub		esi, 3
		//sub		edi, 3
		//shr		cl, 2
		//rep		movsd
		//add		esi, 3
		//add		edi, 3
		TmpCL = TmpAL;
		TmpESI--;
		TmpEDI--;
		if (TmpCL < 4) goto uu3x;
		TmpCL >>= 2;
		TmpESI -= 3;
		TmpEDI -= 3;
		for (; TmpECX != 0; TmpECX--, TmpESI -= 4 /*sizeof(int)*/, TmpEDI -= 4 /*sizeof(int)*/)
			*reinterpret_cast<unsigned int *>(TmpEDI) = *reinterpret_cast<unsigned int *>(TmpESI);
		TmpESI += 3;
		TmpEDI += 3;
	uu3x:
		//mov		cl, al
		//and		cl, 3
		//rep		movsb
		TmpCL = TmpAL & 3;
		for (; TmpECX != 0; TmpECX--, TmpESI--, TmpEDI--)
			*reinterpret_cast<unsigned char *>(TmpEDI) = *reinterpret_cast<unsigned char *>(TmpESI);
		//add		esi, 64
		//mov		cl, dl
		//sub		cl, al
		//mov		dh, cl
		//cmp		cl, 4
		//jb		uu31x
		//shr		cl, 2
		//sub		esi, 3
		//sub		edi, 3
		//rep		movsd
		//add		esi, 3
		//add		edi, 3
		TmpESI += 64;
		TmpCL = TmpDL - TmpAL;
		TmpDH = TmpCL;
		if (TmpCL < 4) goto uu31x;
		TmpCL >>= 2;
		TmpESI -= 3;
		TmpEDI -= 3;
		for (; TmpECX != 0; TmpECX--, TmpESI -= 4 /*sizeof(int)*/, TmpEDI -= 4 /*sizeof(int)*/)
			*reinterpret_cast<unsigned int *>(TmpEDI) = *reinterpret_cast<unsigned int *>(TmpESI);
		TmpESI += 3;
		TmpEDI += 3;
	uu31x:
		//mov		cl, dh
		//and		cl, 3
		//rep		movsb
		//sub		dl, 2
		//add		tmedi, 256
		//mov		edi, tmedi
		//inc		bh
		//dec		ah
		//jnz		DoTrapezoid2
		TmpCL = TmpDH & 3;
		for (; TmpECX != 0; TmpECX--, TmpESI--, TmpEDI--)
			*reinterpret_cast<unsigned char *>(TmpEDI) = *reinterpret_cast<unsigned char *>(TmpESI);
		TmpDL -= 2;
		tmedi += 256;
		TmpEDI = tmedi;
		TmpBH++;
		TmpAH--;
		if (TmpAH != 0) goto DoTrapezoid2;
	EndTrapezoid :
		//popf
		//pop		edi
		//pop		esi
		;
	};
};
int GetBmOfst(int i){
	return ((i&3)<<6)+((i>>2)<<(8+6));
};

// 1(x0,y0)
//  |\
//  |  \ s1
//s3|    \ 2
//  |    /
//  |  / s2
//  |/
// 3
//
// bm1,bm2,bm3-numbers of bitmaps in BitmapArray
// if bm1<bm2 then bm2 is over bm1
// s1 (0..2) - section of 1-2 
// s2 (0..2) - section of 2-3 
// s3 (0..2) - section of 3-1 
void PrepareIntersection1(int bm1,int bm2,int bm3,
						  int x0,int y0,
						  int s1,int s2,int s3,
						  RLCTable Masks,byte* BitmapsArray){
	if(bm1==bm2){
		if(bm3<bm2){
			//1,2 over 3 - inverse mask
			FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm1),x0,y0);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,27+(2-s2)*3+s3);
			return;
		}else{
			//3 over 1,2 - normal mask
			FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm1),x0,y0);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,18+(2-s2)*3+s3);
			return;
		};
	};
	if(bm2==bm3){
		if(bm1<bm3){
			//2,3 over 1 - inverse mask
			FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm3),x0,y0);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,45+(2-s3)*3+s1);
			return;
		}else{
			//1 over 2,3 - normal mask
			FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm3),x0,y0);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,36+(2-s3)*3+s1);
			return;
		};
	};
	if(bm1==bm3){
		if(bm2<bm3){
			//1,3 over 2 - inverse mask
			FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm3),x0,y0);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm2),x0,y0,9+(2-s1)*3+s2);
			return;
		}else{
			//2 over 1,3 - normal mask
			FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm3),x0,y0);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm2),x0,y0,(2-s1)*3+s2);
			return;
		};
	};
	//Triple intersections
	if(bm1<bm2&&bm1<bm3){
		FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm1),x0,y0);
		CopyMaskedBitmap64(BitmapsArray+GetBmOfst(bm2),x0,y0,
				(void*)(Masks->OfsTable[(2-s1)*3+s2]));
		CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,18+(2-s2)*3+s3);
		return;
	};
	if(bm2<bm1&&bm2<bm3){
		FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm2),x0,y0);
		CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,36+(2-s3)*3+s1);
		CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,18+(2-s2)*3+s3);
		return;
	};
	FastCreateMaskedBitmap64_1(BitmapsArray+GetBmOfst(bm3),x0,y0);
	CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,36+(2-s3)*3+s1);
	CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm2),x0,y0,(2-s1)*3+s2);
};
//
//                1
//         s1   /|
//            /  |
//        2 /    | s3
// (x01,y0) \    |
//            \  |
//         s2   \|
//                3
void PrepareIntersection2(int bm1,int bm2,int bm3,
						  int x0,int y01,
						  int s1,int s2,int s3,
						  RLCTable Masks,byte* BitmapsArray){
	int y0=y01-31;
	if(bm1==bm2){
		if(bm3<bm2){
			//1,2 over 3 - inverse mask
			FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm1),x0,y01);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,54+27+(2-s2)*3+s3);
			return;
		}else{
			//3 over 1,2 - normal mask
			FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm1),x0,y01);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,54+18+(2-s2)*3+s3);
			return;
		};
	};
	if(bm2==bm3){
		if(bm1<bm3){
			//2,3 over 1 - inverse mask
			FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm3),x0,y01);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,54+45+(2-s3)*3+s1);
			return;
		}else{
			//1 over 2,3 - normal mask
			FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm3),x0,y01);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,54+36+(2-s3)*3+s1);
			return;
		};
	};
	if(bm1==bm3){
		if(bm2<bm3){
			//1,3 over 2 - inverse mask
			FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm3),x0,y01);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm2),x0,y0,54+9+(2-s1)*3+s2);
			return;
		}else{
			//2 over 1,3 - normal mask
			FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm3),x0,y01);
			CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm2),x0,y0,54+(2-s1)*3+s2);
			return;
		};
	};
	//Triple intersections
	if(bm1<bm2&&bm1<bm3){
		FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm1),x0,y01);
		CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm2),x0,y0,54+(2-s1)*3+s2);
		CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,54+18+(2-s2)*3+s3);
		return;
	};
	if(bm2<bm1&&bm2<bm3){
		FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm2),x0,y01);
		CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,54+36+(2-s3)*3+s1);
		CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm3),x0,y0,54+18+(2-s2)*3+s3);
		return;
	};
	FastCreateMaskedBitmap64_2(BitmapsArray+GetBmOfst(bm3),x0,y01);
	CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm1),x0,y0,54+36+(2-s3)*3+s1);
	CopyMaskedBitmap(BitmapsArray+GetBmOfst(bm2),x0,y0,54+(2-s1)*3+s2);	
};

