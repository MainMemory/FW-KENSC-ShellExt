#include "StdAfx.h"
#include <string>
#include <fstream>
#include <Shlwapi.h>
#include <Shellapi.h>
#include "ContextMenu.h"
#include "kosinski.h"
#include "enigma.h"
#include "nemesis.h"
#include "saxman.h"
#include "comper.h"

using std::ifstream;
using std::fstream;

extern HINSTANCE g_hInst;
extern long g_cDllRef;

const wchar_t* fileextentions[] = {
	L".kos",
	L".eni",
	L".nem",
	L".sax",
	L".sax",
	L".kosm",
	L".comp"
};

wchar_t *chgext(const wchar_t *name, const wchar_t *ext)
{
	wchar_t *result = new wchar_t[MAX_PATH];
	wcscpy(result, name);
	result[wcslen(name)] = '\0';
	PathRenameExtension(result, ext);
	return result;
}

void do_compression_decompression(const int mode, const wchar_t *in)
{
	const wchar_t* const out = chgext(in, (mode&1) ? fileextentions[mode/2] : L".unc");
	ifstream instr(in, std::ios::in | std::ios::binary);
	fstream outstr(out, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
	delete[] out;

	instr.seekg(0, ifstream::end);
	auto size = instr.tellg();
	instr.seekg(0, ifstream::beg);

	switch(mode)
	{
	case 0:
		kosinski::decode(instr, outstr);
		break;
	case 1:
		kosinski::encode(instr, outstr);
		break;
	case 2:
		enigma::decode(instr, outstr);
		break;
	case 3:
		enigma::encode(instr, outstr);
		break;
	case 4:
		nemesis::decode(instr, outstr);
		break;
	case 5:
		nemesis::encode(instr, outstr);
		break;
	case 6:
		saxman::decode(instr, outstr);
		break;
	case 7:
		saxman::encode(instr, outstr);
		break;
	case 8:
		saxman::decode(instr, outstr, 0, size);
		break;
	case 9:
		saxman::encode(instr, outstr, false);
		break;
	case 10:
		kosinski::decode(instr, outstr, 0, true);
		break;
	case 11:
		kosinski::encode(instr, outstr, true);
		break;
	case 12:
		comper::decode(instr, outstr);
		break;
	case 13:
		comper::encode(instr, outstr);
		break;
	}
}

struct iteminfo { int id; wchar_t *text; iteminfo *subitems; };

int curid = 0;

#define defaultmenu(name) iteminfo name##menu[] = { \
{ curid++, L"&Decompress" }, \
{ curid++, L"&Compress" }, \
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
	{ -1 }
};
defaultmenu(kosm);
defaultmenu(cmp);

int maxid = curid;

iteminfo rootmenu[] = {
	{ curid++, L"&Kosinski", kosmenu },
	{ curid++, L"&Enigma", enimenu },
	{ curid++, L"&Nemesis", nemmenu },
	{ curid++, L"&Saxman", saxmenu },
	{ curid++, L"&Moduled Kosinski", kosmmenu },
	{ curid++, L"&Comper", cmpmenu },
	{ -1 }
};

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