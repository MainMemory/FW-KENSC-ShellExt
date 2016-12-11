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
	L".enim",
	L".nem",
	L".nemm",
	L".sax",
	L".sax",
	L".saxm",
	L".comp",
	L".compm",
	L".rock",
	L".rockm",
	L".kosp",
	L".kospm",
	L".twiz",
	L".twim"
};

struct iteminfo { int id; wchar_t *text; iteminfo *subitems; };

int curid = 0;

#define defaultmenu(name) iteminfo name##menu[] = { \
{ curid++, L"&Decompress" }, \
{ curid++, L"&Compress" }, \
{ curid++, L"Decompress (&Moduled)" }, \
{ curid++, L"Compress (M&oduled)" }, \
{ -1 } \
}

defaultmenu(kos);
defaultmenu(eni);
defaultmenu(nem);
iteminfo saxmenu[] = {
	{ curid++, L"&Decompress" },
	{ curid++, L"&Compress" },
	{ curid++, L"Decompress (No Size)" },
	{ curid++, L"Compress (No Size)" },
	{ curid++, L"Decompress (&Moduled)" },
	{ curid++, L"Compress (M&oduled)" },
	{ -1 }
};
defaultmenu(comp);
defaultmenu(rock);
defaultmenu(kosp);

const int END_OF_FW_KENSC = curid;

defaultmenu(twiz);

int maxid = curid;

iteminfo rootmenu[] = {
	{ curid++, L"&Kosinski", kosmenu },
	{ curid++, L"&Enigma", enimenu },
	{ curid++, L"&Nemesis", nemmenu },
	{ curid++, L"&Saxman", saxmenu },
	{ curid++, L"&Comper", compmenu },
	{ curid++, L"&Rocket", rockmenu },
	{ curid++, L"Kosinski+", kospmenu },
	{ curid++, L"Twizzler", twizmenu },
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
		case 6:
		{
			enigma::moduled_decode(instr, outstr);
			break;
		}
		case 7:
		{
			enigma::moduled_encode(instr, outstr);
			break;
		}
		// Nemesis
		case 8:
		{
			nemesis::decode(instr, outstr);
			break;
		}
		case 9:
		{
			nemesis::encode(instr, outstr);
			break;
		}
		case 10:
		{
			nemesis::moduled_decode(instr, outstr);
			break;
		}
		case 11:
		{
			nemesis::moduled_encode(instr, outstr);
			break;
		}
		// Saxman
		case 12:
		{
			saxman::decode(instr, outstr);
			break;
		}
		case 13:
		{
			saxman::encode(instr, outstr, true);
			break;
		}
		// Saxman (no size)
		case 14:
		{
			instr.seekg(0, ifstream::end);
			auto size = instr.tellg();
			instr.seekg(0);
			saxman::decode(instr, outstr, size);
			break;
		}
		case 15:
		{
			saxman::encode(instr, outstr, false);
			break;
		}
		case 16:
		{
			saxman::moduled_decode(instr, outstr);
			break;
		}
		case 17:
		{
			saxman::moduled_encode(instr, outstr);
			break;
		}
		// Comper
		case 18:
		{
			comper::decode(instr, outstr);
			break;
		}
		case 19:
		{
			comper::encode(instr, outstr);
			break;
		}
		case 20:
		{
			comper::moduled_decode(instr, outstr);
			break;
		}
		case 21:
		{
			comper::moduled_encode(instr, outstr);
			break;
		}
		// Rocket
		case 22:
		{
			rocket::decode(instr, outstr);
			break;
		}
		case 23:
		{
			rocket::encode(instr, outstr);
			break;
		}
		case 24:
		{
			rocket::moduled_decode(instr, outstr);
			break;
		}
		case 25:
		{
			rocket::moduled_encode(instr, outstr);
			break;
		}
		// Kosinski Plus
		case 26:
		{
			kosplus::decode(instr, outstr);
			break;
		}
		case 27:
		{
			kosplus::encode(instr, outstr);
			break;
		}
		case 28:
		{
			kosplus::moduled_decode(instr, outstr);
			break;
		}
		case 29:
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

		for (int i = 0; i < filesize; ++i)
		{
			buffer[i] = fgetc(infile);
		}
		fclose(infile);

		switch (mode)
		{
		case 30:
		case 32:
		{
			TwizDec(buffer, filesize);
			break;
		}
		case 31:
		{
			TwizComp(buffer, filesize);
			break;
		}
		case 33:
		{
			TwizComp(buffer, filesize, true, 0x1000);
			break;
		}
		}

		FILE *outfile = _wfopen(out, L"wb");
		delete[] out;
		for (int i = 0; i < filesize; ++i)
		{
			fputc(buffer[i], outfile);
		}
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
		mii.dwTypeData = info[i].text;
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
	mii.dwTypeData = L"FW-KENSC";
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