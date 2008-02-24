// Clavier+
// Keyboard shortcuts manager
//
// Copyright (C) 2000-2008 Guillaume Ryder
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Internationalization support. Relies on Windows resource localization system for storing
// translations of strings, icons, dialog boxes, etc. but does not use SetThreadLocale() to set the
// current language. Indeed, this function has a strange behavior with some locales (e.g. US Windows
// versions with French locale) so we prefer using our own resource loading functions so that we can
// chose exactly the locale we want for a resources.

#pragma once

namespace i18n {

enum {
	langFR,
	langEN,
	langDE,
	langPT,
	langCount
};


int getLanguage();
void setLanguage(int lang);
int getDefaultLanguage();


// Memory representation of a string resource. Should only be used for low-level string resource
// loading, when callers need to know the string length.
#pragma warning(disable: 4200)
struct STRING_RESOURCE {
	WORD length;
	WCHAR strbuf[];
	
	void copy(LPTSTR dest_strbuf, int buf_length) const {
#ifdef UNICODE
		lstrcpyn(dest_strbuf, strbuf, buf_length);
#else
		WideCharToMultiByte(CP_ACP, 0, strbuf, length, dest_strbuf, buf_length, NULL, NULL);
		dest_strbuf[(length < buf_length) ? length : buf_length - 1] = _T('\0');
#endif
	}
};
#pragma warning(default: 4200)

// Loads a string for getLanguage() locale.
const STRING_RESOURCE* loadStringResource(UINT id);

// Loads a string for getLanguage() locale. Copies it to the supplied string buffer.
void loadString(UINT id, LPTSTR strbuf, int buf_length);

// Loads a string for getLanguage() locale. Copies it to the supplied statically allocated string
// buffer. Should not be used on dynamically allocated string buffers.
#define loadStringAuto(id, strbuf) \
	loadString(id, strbuf, arrayLength(strbuf))

// Displays a modal dialog box loaded from a resource, for getLanguage() locale. Sets e_hdlgModal
// before displaying the dialog box, restores it to its previous value when the dialog is closed.
INT_PTR dialogBox(UINT id, HWND hwnd_parent, DLGPROC window_proc, LPARAM lInitParam = 0);

// Loads a menu from a resource, for getLanguage() locale.
HMENU loadMenu(UINT id);

// Loads a bitmap from a resource, for getLanguage() locale.
HBITMAP loadBitmap(UINT id);

}  // i18n namespace
