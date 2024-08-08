#include "StdAfx.h"
#include <string>
#include <fstream>
#include <Shlwapi.h>
#include <Shellapi.h>
#include "ContextMenu.h"
#include "FW-KENSC/kosinski.h"
#include "FW-KENSC/enigma.h"
#include "FW-KENSC/nemesis.h"
#include "FW-KENSC/saxman.h"
#include "FW-KENSC/comper.h"
#include "FW-KENSC/rocket.h"
#include "FW-KENSC/kosplus.h"
#include "Twizzler/Twizzler.h"

using std::ifstream;
using std::fstream;

extern HINSTANCE g_hInst;
extern long g_cDllRef;

const wchar_t* fileextentions[] = {
	L".kos",
	L".kosm",
	L".eni",
	L".nem",
	L".sax",
	L".sax",
	L".comp",
	L".compm",
	L".rock",
	L".rockm",
	L".kosp",
	L".kospm",
	L".twiz",
	L".twim"
};

struct iteminfo { int id; const wchar_t *text; iteminfo *subitems; };

int curid = 0;

// For some fucking reason options can't share a name or Windows
// will call InvokeCommand with the ID of the *first* one
// ...but only when you select a lot of files.
// I have absolutely no clue why this happens.
#define defaultmenu(name, format) iteminfo name##menu[] = { \
{ curid++, format L" - &Decompress" }, \
{ curid++, format L" - &Compress" }, \
{ -1 } \
}

#define moduledmenu(name, format) iteminfo name##menu[] = { \
{ curid++, format L" - &Decompress" }, \
{ curid++, format L" - &Compress" }, \
{ curid++, L"&Moduled " format L" - Decompress" }, \
{ curid++, L"M&oduled " format L" - Compress" }, \
{ -1 } \
}

moduledmenu(kos, L"Kosinski");
defaultmenu(eni, L"Enigma");
defaultmenu(nem, L"Nemesis");
iteminfo saxmenu[] = {
	{ curid++, L"Saxman - &Decompress" },
	{ curid++, L"Saxman - &Compress"},
	{ curid++, L"Saxman - Decompress (&No Size)" },
	{ curid++, L"Saxman - Compress (N&o Size)" },
	{ -1 }
};
moduledmenu(comp, L"Comper");
moduledmenu(rock, L"Rocket");
moduledmenu(kosp, L"Kosinski+");

const int END_OF_FW_KENSC = curid;

moduledmenu(twiz, L"Twizzler");

int maxid = curid;

iteminfo rootmenu[] = {
	{ curid++, L"&Kosinski", kosmenu },
	{ curid++, L"&Enigma", enimenu },
	{ curid++, L"&Nemesis", nemmenu },
	{ curid++, L"&Saxman", saxmenu },
	{ curid++, L"&Comper", compmenu },
	{ curid++, L"&Rocket", rockmenu },
	{ curid++, L"Kosinski+", kospmenu },
	{ curid++, L"&Twizzler", twizmenu },
	{ -1 }
};

wchar_t *chgext(const wchar_t *name, const wchar_t *ext)
{
	wchar_t *result = new wchar_t[MAX_PATH];
	wcscpy(result, name);
	PathRenameExtension(result, ext);
	return result;
}

void do_compression_decompression(const int mode, const wchar_t *in)
{
	const wchar_t* const out = chgext(in, (mode & 1) ? fileextentions[mode / 2] : L".unc");

	// FW-KENSC and Twizzler have different ways of being called
	if (mode < END_OF_FW_KENSC)
	{
		ifstream instr(in, std::ios::in | std::ios::binary);
		fstream outstr(out, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		delete[] out;
		switch (mode)
		{
			// Kosinski
		case 0:
		{
			kosinski::decode(instr, outstr);
			break;
		}
		case 1:
		{
			kosinski::encode(instr, outstr);
			break;
		}
		case 2:
		{
			kosinski::moduled_decode(instr, outstr);
			break;
		}
		case 3:
		{
			kosinski::moduled_encode(instr, outstr);
			break;
		}
		// Enigma
		case 4:
		{
			enigma::decode(instr, outstr);
			break;
		}
		case 5:
		{
			enigma::encode(instr, outstr);
			break;
		}
		// Nemesis
		case 6:
		{
			nemesis::decode(instr, outstr);
			break;
		}
		case 7:
		{
			nemesis::encode(instr, outstr);
			break;
		}
		// Saxman
		case 8:
		{
			saxman::decode(instr, outstr);
			break;
		}
		case 9:
		{
			saxman::encode(instr, outstr, true);
			break;
		}
		// Saxman (no size)
		case 10:
		{
			instr.seekg(0, ifstream::end);
			auto size = instr.tellg();
			instr.seekg(0);
			saxman::decode(instr, outstr, size);
			break;
		}
		case 11:
		{
			saxman::encode(instr, outstr, false);
			break;
		}
		// Comper
		case 12:
		{
			comper::decode(instr, outstr);
			break;
		}
		case 13:
		{
			comper::encode(instr, outstr);
			break;
		}
		case 14:
		{
			comper::moduled_decode(instr, outstr);
			break;
		}
		case 15:
		{
			comper::moduled_encode(instr, outstr);
			break;
		}
		// Rocket
		case 16:
		{
			rocket::decode(instr, outstr);
			break;
		}
		case 17:
		{
			rocket::encode(instr, outstr);
			break;
		}
		case 18:
		{
			rocket::moduled_decode(instr, outstr);
			break;
		}
		case 19:
		{
			rocket::moduled_encode(instr, outstr);
			break;
		}
		// Kosinski+
		case 20:
		{
			kosplus::decode(instr, outstr);
			break;
		}
		case 21:
		{
			kosplus::encode(instr, outstr);
			break;
		}
		case 22:
		{
			kosplus::moduled_decode(instr, outstr);
			break;
		}
		case 23:
		{
			kosplus::moduled_encode(instr, outstr);
			break;
		}
		}
	}
	else
	{
		FILE *infile = _wfopen(in, L"rb");
		fseek(infile, 0, SEEK_END);
		int filesize = ftell(infile);
		rewind(infile);
		char *buffer = (char*)malloc(filesize);
		fread(buffer, 1, filesize, infile);
		fclose(infile);

		switch (mode)
		{
		case 24:
		case 26:
		{
			TwizDec(buffer, filesize);
			break;
		}
		case 25:
		{
			TwizComp(buffer, filesize);
			break;
		}
		case 27:
		{
			TwizComp(buffer, filesize, true, 0x1000);
			break;
		}
		}

		FILE *outfile = _wfopen(out, L"wb");
		delete[] out;
		fwrite(buffer, 1, filesize, outfile);
		free(buffer);
		fclose(outfile);
	}
}

CContextMenu::CContextMenu(void) : m_cRef(1)
{
	InterlockedIncrement(&g_cDllRef);
}

CContextMenu::~CContextMenu(void)
{
	InterlockedDecrement(&g_cDllRef);
}

// IUnknown

IFACEMETHODIMP CContextMenu::QueryInterface(REFIID riid, void **ppv)
{
	static const QITAB qit[] = 
	{
		QITABENT(CContextMenu, IContextMenu),
		QITABENT(CContextMenu, IShellExtInit), 
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) CContextMenu::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) CContextMenu::Release()
{
	ULONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
		delete this;

	return cRef;
}


// IShellExtInit

IFACEMETHODIMP CContextMenu::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
{
	if (pDataObj == NULL)
		return E_INVALIDARG;

	HRESULT hr = E_FAIL;

	FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stm;

	// The pDataObj pointer contains the objects being acted upon. In this 
	// example, we get an HDROP handle for enumerating the selected files and 
	// folders.
	if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
	{
		// Get an HDROP handle.
		HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
		if (hDrop != NULL)
		{
			// Determine how many files are involved in this operation. This 
			// code sample displays the custom context menu item when only 
			// one file is selected. 
			UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			wchar_t buf[MAX_PATH];
			for (unsigned int i = 0; i < nFiles; i++)
				// Get the path of the file.
				if (DragQueryFile(hDrop, i, buf, MAX_PATH) != 0)
					selectedFiles.push_back(buf);
				else
					return hr;
			hr = S_OK;

			GlobalUnlock(stm.hGlobal);
		}

		ReleaseStgMedium(&stm);
	}

	// If any value other than S_OK is returned from the method, the context 
	// menu item is not displayed.
	return hr;
}

// IContextMenu

HMENU ProcessSubMenu(iteminfo *info, UINT idCmdFirst)
{
	HMENU hSubmenu = CreatePopupMenu();

	int i = 0;
	while (info[i].id != -1)
	{
		MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
		mii.fMask = MIIM_STRING | MIIM_ID;
		mii.wID = info[i].id + idCmdFirst;
		mii.dwTypeData = const_cast<LPWSTR>(info[i].text);
		if (info[i].subitems != nullptr)
		{
			mii.fMask |= MIIM_SUBMENU;
			mii.hSubMenu = ProcessSubMenu(info[i].subitems, idCmdFirst);
		}
		InsertMenuItem(hSubmenu, i++, TRUE, &mii);
	}
	return hSubmenu;
}

IFACEMETHODIMP CContextMenu::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	// If uFlags include CMF_DEFAULTONLY then we should not do anything.
	if (CMF_DEFAULTONLY & uFlags)
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));

	MENUITEMINFO mii = { sizeof(mii) };
	mii.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
	mii.wID = idCmdFirst + curid;
	mii.dwTypeData = const_cast<LPWSTR>(L"FW-KENSC");
	mii.hSubMenu = ProcessSubMenu(rootmenu, idCmdFirst);
	if (!InsertMenuItem(hMenu, indexMenu, TRUE, &mii))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// Return an HRESULT value with the severity set to SEVERITY_SUCCESS. 
	// Set the code value to the offset of the largest command identifier 
	// that was assigned, plus one (1).
	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(curid + 1));
}

IFACEMETHODIMP CContextMenu::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
	bool fUnicode = pici->cbSize == sizeof(CMINVOKECOMMANDINFOEX) && pici->fMask & CMIC_MASK_UNICODE;

	void *ptr;

	if (fUnicode)
		ptr = (void *)((CMINVOKECOMMANDINFOEX *)pici)->lpVerbW;
	else
		ptr = (void *)pici->lpVerb;

	if (HIWORD(ptr) != 0)
		return E_INVALIDARG;

	// Is the command identifier offset supported by this context menu 
	// extension?
	if (LOWORD(pici->lpVerb) < maxid)
		for (auto i = selectedFiles.begin(); i != selectedFiles.end(); i++)
			do_compression_decompression(LOWORD(pici->lpVerb), (*i).c_str());
	else
		return E_FAIL;
	return S_OK;
}

IFACEMETHODIMP CContextMenu::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	return E_INVALIDARG;
}