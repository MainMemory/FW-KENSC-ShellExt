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

wchar_t *chgext(const wchar_t *name, const wchar_t *ext)
{
	wchar_t *result = new wchar_t[wcslen(name) + 1];
	PathRenameExtension(result, ext);
	return result;
}

void decomp_kos(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"unc");
	ifstream instr(in);
	fstream outstr(out);
	kosinski::decode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void comp_kos(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"kos");
	ifstream instr(in);
	fstream outstr(out);
	kosinski::encode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void decomp_eni(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"unc");
	ifstream instr(in);
	fstream outstr(out);
	enigma::decode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void comp_eni(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"eni");
	ifstream instr(in);
	fstream outstr(out);
	enigma::encode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void decomp_nem(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"unc");
	ifstream instr(in);
	fstream outstr(out);
	nemesis::decode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void comp_nem(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"nem");
	ifstream instr(in);
	fstream outstr(out);
	nemesis::encode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void decomp_sax(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"unc");
	ifstream instr(in);
	fstream outstr(out);
	saxman::decode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void comp_sax(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"sax");
	ifstream instr(in);
	fstream outstr(out);
	saxman::encode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void decomp_sax_nosize(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"unc");
	ifstream instr(in);
	instr.seekg(0, ifstream::end);
	auto size = instr.tellg();
	instr.seekg(0, ifstream::beg);
	fstream outstr(out);
	saxman::decode(instr, outstr, 0, size);
	instr.close();
	outstr.close();
	delete[] out;
}

void comp_sax_nosize(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"sax");
	ifstream instr(in);
	fstream outstr(out);
	saxman::encode(instr, outstr, false);
	instr.close();
	outstr.close();
	delete[] out;
}

void decomp_kosm(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"unc");
	ifstream instr(in);
	fstream outstr(out);
	kosinski::decode(instr, outstr, 0, true);
	instr.close();
	outstr.close();
	delete[] out;
}

void comp_kosm(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"kosm");
	ifstream instr(in);
	fstream outstr(out);
	kosinski::encode(instr, outstr, true);
	instr.close();
	outstr.close();
	delete[] out;
}

void decomp_cmp(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"unc");
	ifstream instr(in);
	fstream outstr(out);
	comper::decode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void comp_cmp(const wchar_t *in)
{
	wchar_t *out = chgext(in, L"cmp");
	ifstream instr(in);
	fstream outstr(out);
	comper::encode(instr, outstr);
	instr.close();
	outstr.close();
	delete[] out;
}

void(*itemargs[])(const wchar_t *) = {
	decomp_kos,
	comp_kos,
	decomp_eni,
	comp_eni,
	decomp_nem,
	comp_nem,
	decomp_sax,
	comp_sax,
	decomp_sax_nosize,
	comp_sax_nosize,
	decomp_kosm,
	comp_kosm,
	decomp_cmp,
	comp_cmp
};

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
			itemargs[LOWORD(pici->lpVerb)]((*i).c_str());
	else
		return E_FAIL;
	return S_OK;
}

IFACEMETHODIMP CContextMenu::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	return E_INVALIDARG;
}