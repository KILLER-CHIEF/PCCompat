// === RenameOrdinals ===
// Given the location of a particular PE and pairs of original name & alternative names for export ordinal names,
//  this program will rename each matching ordinal name in the PE to the second name in each pair.

//http://win32assembly.programminghorizon.com/pe-tut7.html
// Tutorial used for locating the Export Ordinal Names in PE's.

#include "stdafx.h"

DWORD RVAToFileMap(LPVOID pMapping, DWORD ddva) {
	IMAGE_DOS_HEADER* idh = (IMAGE_DOS_HEADER*)pMapping;
	IMAGE_NT_HEADERS* inh = (IMAGE_NT_HEADERS*)((BYTE*)pMapping + idh->e_lfanew);

	WORD nos = inh->FileHeader.NumberOfSections;

	IMAGE_SECTION_HEADER* ish = (IMAGE_SECTION_HEADER*)((BYTE*)inh + sizeof(IMAGE_NT_HEADERS));

	DWORD result = ddva;

	while (nos-- > 0) {
		DWORD va = ish->VirtualAddress;
		if (result >= va) {//check all sections
			va += ish->SizeOfRawData;
			if (result < va) {//The address is in this section
				result -= ish->VirtualAddress;
				result = ish->PointerToRawData + result + (DWORD)pMapping;
				break;
			}
		}
		ish = (IMAGE_SECTION_HEADER*)((DWORD)ish + (DWORD)sizeof(IMAGE_SECTION_HEADER));
	}

	return result;
}

void free_str_array(char** buffer, int len) {
	if (buffer == 0)
		return;
	for (int i = 0; i < len; i++) {
		if (buffer[i] != 0)
			free(buffer[i]);
	}
	free(buffer);
}

int main()
{
	int ArgCnt;
	LPWSTR* ArgList = CommandLineToArgvW(GetCommandLineW(), &ArgCnt);

	if (ArgCnt == 1) {
		printf("<PE File> <Original Ordinal Name> <Alternative Ordinal Name> ... [<OON> <AON>].\n");
		LocalFree(ArgList);
		return 0;
	}
	else if (ArgCnt == 2) {
		printf("ERROR: No ordinal name pairs given in input.\n");
		LocalFree(ArgList);
		return 1;
	}
	else if (ArgCnt % 2 == 1) {
		LocalFree(ArgList);
		printf("ERROR: Odd number of ordinal names in input.\n");
		return 2;
	}

	int num_ordinal_renames = (ArgCnt - 2) / 2;
	char** ordinal_name_pairs = (char**)malloc(sizeof(char*) * (num_ordinal_renames * 2));

	for (int i = 0; i < num_ordinal_renames * 2; i++) {
		ordinal_name_pairs[i] = 0;
	}

	int error = 0;

	for (int i = 2; i < ArgCnt; i += 2) {
		int orig_buflen = wcslen(ArgList[i]) + 1;
		int alt_buflen = wcslen(ArgList[i + 1]) + 1;
		if (alt_buflen > orig_buflen) {
			printf("ERROR: Input ordinal alternative name is longer than the original.\n");
			error = 3;
			break;
		}
		else if (orig_buflen <= 1) {
			printf("ERROR: Input ordinal original name is empty.\n");
			error = 4;
			break;
		}
		else if (alt_buflen <= 1) {
			printf("ERROR: Input ordinal alternative name is empty.\n");
			error = 5;
			break;
		}

		ordinal_name_pairs[i - 2] = (char*)malloc(sizeof(char) * orig_buflen);
		snprintf(ordinal_name_pairs[i - 2], orig_buflen, "%ls", ArgList[i]);

		ordinal_name_pairs[i + 1 - 2] = (char*)malloc(sizeof(char) * orig_buflen);
		memset(ordinal_name_pairs[i + 1 - 2], 0, orig_buflen);
		snprintf(ordinal_name_pairs[i + 1 - 2], orig_buflen, "%ls", ArgList[i + 1]);
	}

	if (error) {
		LocalFree(ArgList);
		free_str_array(ordinal_name_pairs, num_ordinal_renames * 2);
		return error;
	}

	FILE* fp;
	_wfopen_s(&fp, ArgList[1], L"rb+");

	if (!fp) {
		printf("ERROR: Unable to open file: \"%ls\".\n", ArgList[1]);
		LocalFree(ArgList);
		free_str_array(ordinal_name_pairs, num_ordinal_renames * 2);
		return 6;
	}

	LocalFree(ArgList);

	fseek(fp, 0L, SEEK_END);
	long fp_buflen = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	BYTE* fp_buffer = (BYTE*)malloc(sizeof(BYTE) * fp_buflen);

	if (!fp_buffer) {
		printf("ERROR: Unable to allocate enough memory to load file of size: %lx.\n", fp_buflen);
		fclose(fp);
		free_str_array(ordinal_name_pairs, num_ordinal_renames * 2);
		return 7;
	}

	fread(fp_buffer, sizeof(BYTE), fp_buflen, fp);

	IMAGE_DOS_HEADER* idh = (IMAGE_DOS_HEADER*)fp_buffer;

	if (idh->e_magic != IMAGE_DOS_SIGNATURE) {
		printf("ERROR: Not DOS - This file is not a DOS application.\n");
		free(fp_buffer);
		fclose(fp);
		free_str_array(ordinal_name_pairs, num_ordinal_renames * 2);
		return 2;
	}

	IMAGE_NT_HEADERS* inh = (IMAGE_NT_HEADERS*)((BYTE*)fp_buffer + idh->e_lfanew);

	if (inh->Signature != IMAGE_NT_SIGNATURE) {
		printf("ERROR: Not Valid PE - This file is not a valid NT Portable Executable.\n");
		free(fp_buffer);
		fclose(fp);
		free_str_array(ordinal_name_pairs, num_ordinal_renames * 2);
		return 3;
	}

	DWORD ddva = inh->OptionalHeader.DataDirectory->VirtualAddress;

	if (ddva == 0) {
		printf("WARNING: No Export Table - No export information in this PE.\n");
		free(fp_buffer);
		fclose(fp);
		free_str_array(ordinal_name_pairs, num_ordinal_renames * 2);
		return 4;
	}

	IMAGE_EXPORT_DIRECTORY* ied = (IMAGE_EXPORT_DIRECTORY*)RVAToFileMap(fp_buffer, ddva);

	DWORD nof = ied->NumberOfFunctions;

	DWORD aon = RVAToFileMap(fp_buffer, ied->AddressOfNames);

	for (int i = 0; i < (int)nof; i++) {
		char* ordinal_name = (char*)RVAToFileMap(fp_buffer, *(DWORD*)(aon + i * 4));
		for (int j = 0; j < num_ordinal_renames; j++) {
			if (strcmp(ordinal_name, ordinal_name_pairs[j * 2]) == 0) {
				fseek(fp, (DWORD)(ordinal_name)-(DWORD)fp_buffer, SEEK_SET);
				fwrite(ordinal_name_pairs[j * 2 + 1], sizeof(char), strlen(ordinal_name_pairs[j * 2]) + 1, fp);
				ordinal_name_pairs[j * 2][0] = 0;
			}
		}
	}

	for (int j = 0; j < num_ordinal_renames; j++) {
		if (ordinal_name_pairs[j * 2][0] != 0) {
			printf("WARNING: Failed to find match for renaming the Export Ordinal Name: \"%s\".\n", ordinal_name_pairs[j * 2]);
		}
	}

	free(fp_buffer);
	fclose(fp);
	free_str_array(ordinal_name_pairs, num_ordinal_renames * 2);
	return 0;
}
