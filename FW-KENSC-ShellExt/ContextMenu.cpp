#include "StdAfx.h"
#include <string>
#include <fstream>
#include <Shlwapi.h>
#include <Shellapi.h>
#include <tchar.h>
#include "ContextMenu.h"
#include "mdcomp/kosinski.hh"
#include "mdcomp/enigma.hh"
#include "mdcomp/nemesis.hh"
#include "mdcomp/saxman.hh"
#include "mdcomp/comper.hh"
#include "mdcomp/rocket.hh"
#include "mdcomp/kosplus.hh"
#include "Twizzler/Twizzler.h"

using std::ifstream;
using std::fstream;

extern HINSTANCE g_hInst;
extern long g_cDllRef;

LPCTSTR fileextentions[] = {
	TEXT(".kos"),
	TEXT(".kosm"),
	TEXT(".eni"),
	TEXT(".nem"),
	TEXT(".sax"),
	TEXT(".sax"),
	TEXT(".comp"),
	TEXT(".compm"),
	TEXT(".rock"),
	TEXT(".rockm"),
	TEXT(".kosp"),
	TEXT(".kospm"),
	TEXT(".twiz"),
	TEXT(".twim")
};

struct iteminfo { int id; LPCTSTR text; iteminfo *subitems; };

int curid = 0;

// For some fucking reason options can't share a name or Windows
// will call InvokeCommand with the ID of the *first* one
// ...but only when you select a lot of files.
// I have absolutely no clue why this happens.
#define defaultmenu(name, format) iteminfo name##menu[] = { \
{ curid++, format TEXT(" - &Decompress") }, \
{ curid++, format TEXT(" - &Compress") }, \
{ -1 } \
}

#define moduledmenu(name, format) iteminfo name##menu[] = { \
{ curid++, format TEXT(" - &Decompress") }, \
{ curid++, format TEXT(" - &Compress") }, \
{ curid++, TEXT("&Moduled ") format TEXT(" - Decompress") }, \
{ curid++, TEXT("M&oduled ") format TEXT(" - Compress") }, \
{ -1 } \
}

moduledmenu(kos, TEXT("Kosinski"));
defaultmenu(eni, TEXT("Enigma"));
defaultmenu(nem, TEXT("Nemesis"));
iteminfo saxmenu[] = {
	{ curid++, TEXT("Saxman - &Decompress") },
	{ curid++, TEXT("Saxman - &Compress") },
	{ curid++, TEXT("Saxman - Decompress (&No Size)") },
	{ curid++, TEXT("Saxman - Compress (N&o Size)") },
	{ -1 }
};
moduledmenu(comp, TEXT("Comper"));
moduledmenu(rock, TEXT("Rocket"));
moduledmenu(kosp, TEXT("Kosinski+"));

const int END_OF_FW_KENSC = curid;

moduledmenu(twiz, TEXT("Twizzler"));

int maxid = curid;

iteminfo rootmenu[] = {
	{ curid++, TEXT("&Kosinski"), kosmenu },
	{ curid++, TEXT("&Enigma"), enimenu },
	{ curid++, TEXT("&Nemesis"), nemmenu },
	{ curid++, TEXT("&Saxman"), saxmenu },
	{ curid++, TEXT("&Comper"), compmenu },
	{ curid++, TEXT("&Rocket"), rockmenu },
	{ curid++, TEXT("Kosinski+"), kospmenu },
	{ curid++, TEXT("&Twizzler"), twizmenu },
	{ -1 }
};

LPTSTR chgext(LPCTSTR name, LPCTSTR ext)
{
	LPTSTR result = new TCHAR[MAX_PATH];
	_tcscpy(result, name);
	PathRenameExtension(result, ext);
	return result;
}

void do_compression_decompression(const int mode, LPCTSTR in)
{
	const LPCTSTR out = chgext(in, (mode & 1) ? fileextentions[mode / 2] : TEXT(".unc"));

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
		FILE *infile = _tfopen(in, TEXT("rb"));
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

		FILE *outfile = _tfopen(out, TEXT("wb"));
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
			TCHAR buf[MAX_PATH];
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
		mii.dwTypeData = const_cast<LPTSTR>(info[i].text);
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
	mii.dwTypeData = const_cast<LPTSTR>(TEXT("FW-KENSC"));
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