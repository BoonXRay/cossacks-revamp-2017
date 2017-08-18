#pragma warning (disable : 4035)

#include <windows.h>

void isiDecryptMem(LPBYTE lpbBuffer, DWORD dwSize, BYTE dbKey)
{
	// BoonXRay 18.08.2017
//_asm
//	{
//		mov	ecx,dwSize
//		mov	ebx,lpbBuffer
//		mov	ah,dbKey
//
//next_byte:
//
//		mov	al,[ebx]
//		not	al
//		xor	al,ah
//		mov	[ebx],al
//		inc	ebx
//
//		loop next_byte
//	}	
	while (dwSize != 0)
	{
		unsigned char Tmp = ~(*lpbBuffer);
		*lpbBuffer = Tmp ^ dbKey;
		lpbBuffer++;
		dwSize--;
	}
}

DWORD isiCalcHash(LPSTR lpszFileName)
{
	char	szFileName[64];

	ZeroMemory(szFileName,64);
	strcpy(szFileName,_strupr(lpszFileName));
	// BoonXRay 19.08.2017
//_asm
//	{
//		mov	edx,0
//		mov	ecx,16
//		lea ebx,szFileName
//
//new_dword:
//
//		mov	eax,[ebx]
//		xchg ah,al
//		rol	eax,16
//		xchg ah,al
//
//		add	edx,eax
//
//		add	ebx,4
//
//		loop new_dword
//
//		mov	eax,edx
//	}
	unsigned int Result = 0, *Ptr = reinterpret_cast<unsigned int *>(szFileName);
	for (int i = 0; i < 16; i++)
	{
		unsigned int TmpEAX = *Ptr;
		unsigned char & TmpAL = *reinterpret_cast<unsigned char *>(&TmpEAX);
		unsigned char & TmpAH = *(reinterpret_cast<unsigned char *>(&TmpEAX) + 1);
		unsigned char TmpUChar = TmpAL;
		TmpAL = TmpAH;
		TmpAH = TmpUChar;
		TmpEAX = (TmpEAX << 16) | (TmpEAX >> 16);
		TmpUChar = TmpAL;
		TmpAL = TmpAH;
		TmpAH = TmpUChar;

		Result += TmpEAX;
		Ptr++;
	}
	return Result;
}


BOOL isiMatchesMask(LPSTR lpszFile, LPSTR lpszMask)
{
	char	szFile[255];
	char	szMask[255];

	strcpy(szFile,_strupr(lpszFile));
	strcpy(szMask,_strupr(lpszMask));
	// BoonXRay 19.08.2017
//_asm
//	{
//		lea esi,szMask
//		lea edi,szFile
//
//	next_char:
//
//		mov ah,byte ptr [esi]	// Mask
//		mov al,byte ptr [edi]	// File
//
//		cmp ax,0
//		jnz short cont_compare
//		mov eax,1
//		jmp short exit_comp
//
//	cont_compare:
//
//		cmp ah,'?'				// Mask
//		jz	skip_one_char
//
//		cmp ah,al				// Mask & File
//		jz	skip_one_char
//
//		cmp ah,'*'				// Mask
//		jz	skip_multiple
//
//		mov eax,0
//		jmp short exit_comp
//
//	skip_multiple:
//
//		inc esi
//
//		mov ah,byte ptr [esi]	// Mask
//
//	next:
//
//		mov al,byte ptr [edi]	// File
//
//		cmp al,ah
//		jz	next_char
//		inc edi
//		jmp short next
//
//	skip_one_char:
//
//		inc esi
//		inc edi
//		jmp short next_char
//
//	exit_comp:
//
//	}
	char * MaskPtr = szMask, *FilePtr = szFile;
	while (true)
	{
		char MaskChar = *MaskPtr;	// Mask
		char FileChar = *FilePtr;	// File

		if ((MaskChar == 0) && (FileChar == 0)) return 1;
		if ((MaskChar == '?') || (MaskChar == FileChar))	// Mask or match
		{
			MaskPtr++;
			FilePtr++;
			continue;
		}
		else if (MaskChar == '*')	// Mask
		{
			MaskPtr++;
			MaskChar = *MaskPtr;	// Mask
			bool Flag = false;
			while (true)
			{
				FileChar = *FilePtr;	// File
				if (FileChar == MaskChar)
				{
					Flag = true;
					break;
				}
				FilePtr++;
			}
			if (Flag) continue;
		}
		return 0;
	}
}

BOOL isiFileExists(LPSTR lpszFileName)
{
	WIN32_FIND_DATA	FindData;
	HANDLE			hFindFile;
	
	if((hFindFile=FindFirstFile(lpszFileName,&FindData))!=INVALID_HANDLE_VALUE)
		{
			FindClose(hFindFile);
			return TRUE;
		}
	else
		return FALSE;
}

#pragma warning (default : 4035)