/*
   Directory Opus 9 Viewer Plugins
   Header File

   (c) Copyright 2007 GP Software
   All Rights Reserved
*/

#ifndef DOPUS_VIEWERPLUGINS
#define DOPUS_VIEWERPLUGINS

// Current version - define VIEWERPLUGINVERSION when including this file to use an older version
#ifndef VIEWERPLUGINVERSION
#define VIEWERPLUGINVERSION		4
#endif

// Taken from winnt.h in case _WIN32_WINNT is not >= 0x0500
#ifndef RTL_SIZEOF_THROUGH_FIELD
#define RTL_FIELD_SIZE(type, field) (sizeof(((type *)0)->field))
#define RTL_SIZEOF_THROUGH_FIELD(type, field) \
    (FIELD_OFFSET(type, field) + RTL_FIELD_SIZE(type, field))
#endif

// VIEWERPLUGININFO flags
#define DVPFIF_CanHandleStreams				(1<<0)	// Plugin can handle IStreams to identify and read files
#define DVPFIF_CanHandleBytes				(1<<1)	// Plugin can identify files based on file header
#define DVPFIF_CatchAll						(1<<2)	// 'Catch All' plugin wants to be called last
#define DVPFIF_DefaultCatchAll				(1<<3)	// 'Default Catch All' plugin is last unless there's another 'catch all'
#define DVPFIF_ExtensionsOnly				(1<<4)	// Plugin only wants to identify files based on file extension
#define DVPFIF_ExtensionsOnlyIfSlow			(1<<5)	// Plugin looks only at file extensions if file is 'slow'
#define DVPFIF_ExtensionsOnlyIfNoRndSeek	(1<<6)	// Plugin looks only at file extensions if file doesn't support random seeking
#define DVPFIF_ExtensionsOnlyForThumbnails	(1<<7)	// Plugin looks only at file extensions if generating thumbnails
#define DVPFIF_NoSlowFiles					(1<<8)	// Plugin doesn't handle 'slow' files
#define DVPFIF_NeedRandomSeek				(1<<9)	// Plugin needs random seek capability
#define DVPFIF_CanConfigure					(1<<10)	// Plugin has a configuration interface
#define DVPFIF_CanShowAbout					(1<<11)	// Plugin has an About function
#define DVPFIF_NoThumbnails					(1<<12)	// Plugin doesn't want to be called to generate thumbnails
#define DVPFIF_NoProperties					(1<<13)	// Don't show 'Properties' item on default context menu
#define DVPFIF_ZeroBytesOk					(1<<14)	// Zero byte files are ok (eg text files)
#define DVPFIF_OverrideInternal				(1<<15)	// Plugin can override internal Opus image routines
#if ( VIEWERPLUGINVERSION >= 3 )
#define DVPFIF_InitialDisable				(1<<16)	// Plugin is disabled by default
#define DVPFIF_NoFileInformation			(1<<17)	// Is not to be called for file information (eg Description field)
#define DVPFIF_ProvideFileInfo				(1<<18)	// Provides information without viewing
#define DVPFIF_NoMultithreadThumbnails		(1<<19)	// Don't call simultaneously on multiple threads for thumbnails
#define DVPFIF_FolderThumbnails				(1<<20)	// Can provide folder thumbnails
#define DVPFIF_TrueThumbnailSize			(1<<21)	// Always provide true thumbnail size
#endif
#if ( VIEWERPLUGINVERSION >= 4 )
#define DVPFIF_UseVersionResource			(1<<22)	// Use the plugin's version resource for version information
#define DVPFIF_CanShowHex					(1<<23)	// The plugin can be used as a hex viewer for any type of file
#define DVPFIF_OnlyThumbnails				(1<<24)	// Plugin is only to be called for thumbnails
#endif

// Plugin major types
enum DOpusViewerPluginFileType
{
	DVPMajorType_Image,
	DVPMajorType_Sound,
	DVPMajorType_Text,
	DVPMajorType_Other,
	DVPMajorType_Movie,
};

typedef struct DOpusViewerPluginInfoW
{
	UINT		cbSize;						// Structure size
	DWORD		dwFlags;					// Flags
	DWORD		dwVersionHigh;				// Version (high)
	DWORD		dwVersionLow;				// Version (low)
	LPWSTR		lpszHandleExts;				// File extensions the plugin handles, eg ".xxx;.yyy;.zzz"
	LPWSTR		lpszName;					// Plugin primary file format name, eg "JPEG"
	LPWSTR		lpszDescription;			// Plugin description string, eg "Directory Opus JPEG Viewer"
	LPWSTR		lpszCopyright;				// Copyright string, eg "(c) 2001 GP Software"
	LPWSTR		lpszURL;					// Reference URL, eg "http://www.gpsoft.com.au"
	UINT		cchHandleExtsMax;			// Max. length of buffer
	UINT		cchNameMax;					// Max. length of buffer
	UINT		cchDescriptionMax;			// Max. length of buffer
	UINT		cchCopyrightMax;			// Max. length of buffer
	UINT		cchURLMax;					// Max. length of buffer
	DWORDLONG	dwlMinFileSize;				// Minimum size of file that we handle
	DWORDLONG	dwlMaxFileSize;				// Maximum size of file that we handle
	DWORDLONG	dwlMinPreviewFileSize;		// Minimum size of file that we handle in preview mode
	DWORDLONG	dwlMaxPreviewFileSize;		// Maximum size of file that we handle in preview mode
	UINT		uiMajorFileType;			// Primary type of file this plugin handles
	GUID		idPlugin;					// Unique identifier for this plugin
#if ( VIEWERPLUGINVERSION >= 4 )
	DWORD		dwOpusVerMajor;				// Opus major version
	DWORD		dwOpusVerMinor;				// Opus minor version
	DWORD		dwInitFlags;				// Initialisation flags
	HICON		hIconSmall;					// Small icon (Opus will call DestroyIcon on this)
	HICON		hIconLarge;					// Large icon (Opus will call DestroyIcon on this)
#endif
} VIEWERPLUGININFOW, * LPVIEWERPLUGININFOW;

#define VIEWERPLUGININFOW_V1_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGININFOW,idPlugin)
#define VIEWERPLUGININFOW_V4_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGININFOW,dwInitFlags)

typedef struct DOpusViewerPluginInfoA
{
	UINT		cbSize;
	DWORD		dwFlags;
	DWORD		dwVersionHigh;
	DWORD		dwVersionLow;
	LPSTR		lpszHandleExts;
	LPSTR		lpszName;
	LPSTR		lpszDescription;
	LPSTR		lpszCopyright;
	LPSTR		lpszURL;
	UINT		cchHandleExtsMax;
	UINT		cchNameMax;
	UINT		cchDescriptionMax;
	UINT		cchCopyrightMax;
	UINT		cchURLMax;
	DWORDLONG	dwlMinFileSize;
	DWORDLONG	dwlMaxFileSize;
	DWORDLONG	dwlMinPreviewFileSize;
	DWORDLONG	dwlMaxPreviewFileSize;
	UINT		uiMajorFileType;
	GUID		idPlugin;
#if ( VIEWERPLUGINVERSION >= 4 )
	DWORD		dwOpusVerMajor;
	DWORD		dwOpusVerMinor;
	DWORD		dwInitFlags;
	HICON		hIconSmall;
	HICON		hIconLarge;
#endif
} VIEWERPLUGININFOA, * LPVIEWERPLUGININFOA;

#define VIEWERPLUGININFOA_V1_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGININFOA,idPlugin)
#define VIEWERPLUGININFOA_V4_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGININFOA,dwInitFlags)

#ifdef UNICODE
#define VIEWERPLUGININFO			VIEWERPLUGININFOW
#define LPVIEWERPLUGININFO			LPVIEWERPLUGININFOW
#define VIEWERPLUGININFO_V1_SIZE	VIEWERPLUGININFOW_V1_SIZE
#define VIEWERPLUGININFO_V4_SIZE	VIEWERPLUGININFOW_V4_SIZE
#else
#define VIEWERPLUGININFO			VIEWERPLUGININFOA
#define LPVIEWERPLUGININFO			LPVIEWERPLUGININFOA
#define VIEWERPLUGININFO_V1_SIZE	VIEWERPLUGININFOA_V1_SIZE
#define VIEWERPLUGININFO_V4_SIZE	VIEWERPLUGININFOA_V4_SIZE
#endif

// VIEWERPLUGININFO initialisation flags
#if ( VIEWERPLUGINVERSION >= 4 )
#define VPINITF_FIRSTTIME				(1<<0)	// First time this plugin has been initialised
#define VPINITF_USB						(1<<1)	// Opus is running in USB mode
#endif


// VIEWERPLUGINFILEINFO flags
#define DVPFIF_CanReturnBitmap			(1<<0)	// We can return a bitmap for this file type
#define DVPFIF_CanReturnViewer			(1<<1)	// We can create a viewer for this file type
#define DVPFIF_CanReturnThumbnail		(1<<2)	// We can return a thumbnail for this filetype (in bitmap form)
#define DVPFIF_CanShowProperties		(1<<3)	// We can show a Properties dialog for this file
#if ( VIEWERPLUGINVERSION >= 3 )
#define DVPFIF_ResolutionInch			(1<<4)	// Resolution is given in DPI
#define DVPFIF_ResolutionCM				(1<<5)	// Resolution is given in CM
#define DVPFIF_WantFileInfo				(1<<6)	// Being called for file information
#define DVPFIF_ReturnsText				(1<<7)	// All we return is text
#define DVPFIF_HasAlphaChannel			(1<<8)	// Bitmap returned is 32 bits and has valid alpha channel
#define DVPFIF_HasTransparentColor		(1<<9)	// Use transparent color to generate mask
#define DVPFIF_HasTransparentPen		(1<<10)	// Use transparent pen to generate mask (upper 8 bits of crTransparentColor)
#define DVPFIF_CanReturnFileInfo		(1<<11)	// Provides file information
#define DVPFIF_NoThumbnailBorder		(1<<12) // Don't display a border for this thumbnail
#define DVPFIF_NoShowThumbnailIcon		(1<<13)	// Don't display filetype icon for this thumbnail
#define DVPFIF_ShowThumbnailIcon		(1<<14)	// Force display of filetype icon for this thumbnail
#define DVPFIF_FolderThumbnail			(1<<15)	// Being called for a folder thumbnail
#define DVPFIF_RegenerateOnResize		(1<<16)	// Regenerate instead of scale for dynamic resizing
#endif
#if ( VIEWERPLUGINVERSION >= 4 )
#define DVPFIF_JPEGStream				(1<<17)	// Plugin has returned a JPEG stream allocated with LocalAlloc(LMEM_FIXED)
#define DVPFIF_PNGStream				(1<<18)	// Plugin has returned a PNG stream allocated with LocalAlloc(LMEM_FIXED);
#define DVPFIF_InFolderThumbnail		(1<<19)	// Being called for a thumbnail for a folder (on a file within the folder)
#endif

enum
{
	DVPFITypeHint_None,
	DVPFITypeHint_PlainText,
	DVPFITypeHint_RichText,
	DVPFITypeHint_HTML,
};

#if ( VIEWERPLUGINVERSION >= 4 )
enum
{
	DVPColorSpace_Unknown,
	DVPColorSpace_Grayscale,
	DVPColorSpace_RGB,
	DVPColorSpace_YCBCR,
	DVPColorSpace_CMYK,
	DVPColorSpace_YCCK,
};
#endif

typedef struct DOpusViewerPluginFileInfoW
{
	UINT		cbSize;
	DWORD		dwFlags;
	WORD		wMajorType;
	WORD		wMinorType;
	SIZE		szImageSize;
	int			iNumBits;
	LPWSTR		lpszInfo;
	UINT		cchInfoMax;
	DWORD		dwPrivateData[8];
#if ( VIEWERPLUGINVERSION >= 2 )
	SIZE		szResolution;
	int			iTypeHint;
#endif
#if ( VIEWERPLUGINVERSION >= 3 )
	COLORREF	crTransparentColor;
	WORD		wThumbnailQuality;
	DWORDLONG	dwlFileSize;
#endif
#if ( VIEWERPLUGINVERSION >= 4 )
	int			iColorSpace;
#endif
} VIEWERPLUGINFILEINFOW, * LPVIEWERPLUGINFILEINFOW;

#define VIEWERPLUGINFILEINFOW_V1_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGINFILEINFOW,dwPrivateData[8])
#define VIEWERPLUGINFILEINFOW_V2_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGINFILEINFOW,iTypeHint)
#define VIEWERPLUGINFILEINFOW_V3_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGINFILEINFOW,dwlFileSize)
#define VIEWERPLUGINFILEINFOW_V4_SIZE	sizeof(VIEWERPLUGINFILEINFOW)

typedef struct DOpusViewerPluginFileInfoA
{
	UINT		cbSize;
	DWORD		dwFlags;
	WORD		wMajorType;
	WORD		wMinorType;
	SIZE		szImageSize;
	int			iNumBits;
	LPSTR		lpszInfo;
	UINT		cchInfoMax;
	DWORD		dwPrivateData[8];
#if ( VIEWERPLUGINVERSION >= 2 )
	SIZE		szResolution;
	int			iTypeHint;
#endif
#if ( VIEWERPLUGINVERSION >= 3 )
	COLORREF	crTransparentColor;
	WORD		wThumbnailQuality;
	DWORDLONG	dwlFileSize;
#endif
#if ( VIEWERPLUGINVERSION >= 4 )
	int			iColorSpace;
#endif
} VIEWERPLUGINFILEINFOA, * LPVIEWERPLUGINFILEINFOA;

#define VIEWERPLUGINFILEINFOA_V1_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGINFILEINFOA,dwPrivateData[8])
#define VIEWERPLUGINFILEINFOA_V2_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGINFILEINFOA,iTypeHint)
#define VIEWERPLUGINFILEINFOA_V3_SIZE	(UINT)RTL_SIZEOF_THROUGH_FIELD(VIEWERPLUGINFILEINFOA,dwlFileSize)
#define VIEWERPLUGINFILEINFOA_V4_SIZE	sizeof(VIEWERPLUGINFILEINFOA)

#ifdef UNICODE
#define VIEWERPLUGINFILEINFO			VIEWERPLUGINFILEINFOW
#define LPVIEWERPLUGINFILEINFO			LPVIEWERPLUGINFILEINFOW
#define VIEWERPLUGINFILEINFO_V1_SIZE	VIEWERPLUGINFILEINFOW_V1_SIZE
#define VIEWERPLUGINFILEINFO_V2_SIZE	VIEWERPLUGINFILEINFOW_V2_SIZE
#define VIEWERPLUGINFILEINFO_V3_SIZE	VIEWERPLUGINFILEINFOW_V3_SIZE
#define VIEWERPLUGINFILEINFO_V4_SIZE	VIEWERPLUGINFILEINFOW_V4_SIZE
#else
#define VIEWERPLUGINFILEINFO			VIEWERPLUGINFILEINFOA
#define LPVIEWERPLUGINFILEINFO			LPVIEWERPLUGINFILEINFOA
#define VIEWERPLUGINFILEINFO_V1_SIZE	VIEWERPLUGINFILEINFOA_V1_SIZE
#define VIEWERPLUGINFILEINFO_V2_SIZE	VIEWERPLUGINFILEINFOA_V2_SIZE
#define VIEWERPLUGINFILEINFO_V3_SIZE	VIEWERPLUGINFILEINFOA_V3_SIZE
#define VIEWERPLUGINFILEINFO_V4_SIZE	VIEWERPLUGINFILEINFOA_V4_SIZE
#endif


// Messages sent to a plugin viewer window
enum DOpusViewerPluginMsg
{
	DVPLUGINMSG_BASE = (WM_APP + 0xf00),

	DVPLUGINMSG_LOADA,					// Load picture (lParam = LPSTR lpszName)
	DVPLUGINMSG_LOADW,					// Load picture (lParam = LPWSTR lpszName)
	DVPLUGINMSG_LOADSTREAMA,			// Load picture (wParam = LPSTR lspzName, lParam = LPSTREAM lpStream)
	DVPLUGINMSG_LOADSTREAMW,			// Load picture (wParam = LPWSTR lspzName, lParam = LPSTREAM lpStream)
	DVPLUGINMSG_GETIMAGEINFOA,			// Get info for loaded picture (lParam = LPVIEWERPLUGINFILEINFOA)
	DVPLUGINMSG_GETIMAGEINFOW,			// Get info for loaded picture (lParam = LPVIEWERPLUGINFILEINFOW)
	DVPLUGINMSG_GETCAPABILITIES,		// Get viewer capabilities
	DVPLUGINMSG_RESIZE,					// Resize window (wParam = DWORD left,top lParam = DWORD width, height )
	DVPLUGINMSG_SETROTATION,			// Set initial rotation ( wParam = int rotate_angle )
	DVPLUGINMSG_ROTATE,					// Rotate image ( wParam = int rotate_amount, lParam = TRUE if window autosize is enabled ) 
	DVPLUGINMSG_SETZOOM,				// Set initial zoom ( wParam = int zoom_factor )
	DVPLUGINMSG_ZOOM,					// Zoom image (wParam = int zoom_factor )
	DVPLUGINMSG_GETZOOMFACTOR,			// Get current zoom factor
	DVPLUGINMSG_SELECTALL,				// Select contents
	DVPLUGINMSG_TESTSELECTION,			// Is there a selection?
	DVPLUGINMSG_COPYSELECTION,			// Copy selection to clipboard
	DVPLUGINMSG_PRINT,					// Print picture
	DVPLUGINMSG_PROPERTIES,				// Show properties dialog ( wParam = HWND hWndParent )
	DVPLUGINMSG_REDRAW,					// Redraw (if wParam = TRUE it means the background colour has changed and is supplied in lParam )
	DVPLUGINMSG_GETPICSIZE,				// Return size of current image (wParam = LPINT lpiNumBits (optional), lParam = LPSIZE)
	DVPLUGINMSG_GETAUTOBGCOL,			// Return 'automatic' background colour based on image
	DVPLUGINMSG_MOUSEWHEEL,				// Mouse wheel message
	DVPLUGINMSG_ADDCONTEXTMENUA,		// Add items to context menu ( wParam = LPDWORD numItems, result = LPDVPCONTEXTMENUITEMA array)
	DVPLUGINMSG_ADDCONTEXTMENUW,		// Add items to context menu ( wParam = LPDWORD numItems, result = LPDVPCONTEXTMENUITEMW array)
	DVPLUGINMSG_SETABORTEVENT,			// Provides an event that is set to abort the loading of a file ( lParam = HANDLE )
	DVPLUGINMSG_GETORIGINALPICSIZE,		// Return original size of current image (wParam = LPINT lpiNumBits (optional), lParam = LPSIZE)
	DVPLUGINMSG_CLEAR,					// Plugin should clear its display and free the current picture
	DVPLUGINMSG_NOTIFY_LOADPROGRESS,	// Posted from plugin back to owner to notify of load progress ( wParam = percent value complete )
	DVPLUGINMSG_ISDLGMESSAGE,			// Plugin should return true if ( lParam = LPMSG ) is a message for one of its dialogs and it has handled it
	DVPLUGINMSG_TRANSLATEACCEL,			// Plugin should return true if ( lParam = LPMSG ) was handled by its accelerators
	DVPLUGINMSG_REINITIALIZE,			// Plugin config has changed and viewer should reinitialize itself if necessary
	DVPLUGINMSG_SHOWHIDESCROLLBARS,		// wParam = BOOL indicating whether scrollbars should be shown or hidden
#if ( VIEWERPLUGINVERSION >= 3 )
	DVPLUGINMSG_INLOADLOOP,				// Viewer is currently loading a picture
	DVPLUGINMSG_SETIMAGEFRAME,			// wParam = BOOL indicating whether image should be framed
	DVPLUGINMSG_SETDESKWALLPAPERA,		// lParam = optional LPSTR parameters ("center"/"tile"/"stretch")
	DVPLUGINMSG_GETZOOMLIMITS,			// Return value indicates max/min zoom limits (HIWORD(max), LOWORD(min))
	DVPLUGINMSG_THUMBSCHANGED,			// Thumbnails have changed, wParam = DWORD flags
	DVPLUGINMSG_GETBITMAP,				// Return an HBITMAP representing your current image (will be freed by Directory Opus)
	DVPLUGINMSG_GAMMACHANGE,			// Gamma correction value has changed
	DVPLUGINMSG_APPCOMMAND,				// AppCommand message, return TRUE if handled
	DVPLUGINMSG_PREVENTFRAME,			// Return TRUE to prevent framing of image
	DVPLUGINMSG_FULLSCREEN,				// Full screen mode has been turned on or off (wParam indicates state, return TRUE to disallow change)
	DVPLUGINMSG_SHOWFILEINFO,			// Show file information (wParam = BOOL on or off)
	DVPLUGINMSG_ISFILEINFOSHOWN,		// Returns TRUE if file information is currently visible
	DVPLUGINMSG_SETDESKWALLPAPERW,		// lParam = optional LPWSTR parameters ("center"/"tile"/"stretch")
#endif
#if ( VIEWERPLUGINVERSION >= 4 )
	DVPLUGINMSG_PREVENTAUTOSIZE,		// Return TRUE to prevent auto-sizing of viewer window
	DVPLUGINMSG_SHOWHEX,				// if wParam = TRUE, lParam contains desired hex state. Return TRUE if in hex mode
	DVPLUGINMSG_ISALPHAHIDDEN,			// lParam = BOOL*, set to TRUE if alpha channel is currently hidden. Return FALSE if no alpha channel
	DVPLUGINMSG_HIDEALPHA,				// wParam = TRUE if alpha channel should be hidden
#endif
};

#ifdef UNICODE
#define DVPLUGINMSG_LOAD				DVPLUGINMSG_LOADW
#define DVPLUGINMSG_LOADSTREAM			DVPLUGINMSG_LOADSTREAMW
#define DVPLUGINMSG_GETIMAGEINFO		DVPLUGINMSG_GETIMAGEINFOW
#define DVPLUGINMSG_ADDCONTEXTMENU		DVPLUGINMSG_ADDCONTEXTMENUW
#define DVPLUGINMSG_SETDESKWALLPAPER	DVPLUGINMSG_SETDESKWALLPAPERW
#else
#define DVPLUGINMSG_LOAD				DVPLUGINMSG_LOADA
#define DVPLUGINMSG_LOADSTREAM			DVPLUGINMSG_LOADSTREAMA
#define DVPLUGINMSG_GETIMAGEINFO		DVPLUGINMSG_GETIMAGEINFOA
#define DVPLUGINMSG_ADDCONTEXTMENU		DVPLUGINMSG_ADDCONTEXTMENUA
#define DVPLUGINMSG_SETDESKWALLPAPER	DVPLUGINMSG_SETDESKWALLPAPERA
#endif


// Notification messages sent from a plugin to its parent window
#define DVPN_FIRST			(0U-2000U)
#define DVPN_LAST			(0U-2020U)

#define DVPN_GETBGCOL		(DVPN_FIRST-0)		// Get background color (return value is a COLORREF)
#define DVPN_SIZECHANGE		(DVPN_FIRST-1)		// Image size has changed (DVPNMSIZECHANGE structure)
#define DVPN_CLICK			(DVPN_FIRST-2)		// Mouse click (single left button click, DVPNMCLICK structure)
#define DVPN_RESETZOOM		(DVPN_FIRST-3)		// Reset zoom factor (zoom factor has had to be changed, DVPNMRESETZOOM structure)
#define DVPN_LBUTTONSCROLL	(DVPN_FIRST-4)		// Get 'left button scroll' flag (return value is BOOL indicating whether left button should be used for scrolling)
#define DVPN_CLEARED		(DVPN_FIRST-5)		// Picture has been cleared (sent by plugin to tell parent that picture has been cleared from display)
#define DVPN_FOCUSCHANGE	(DVPN_FIRST-6)		// Focus has changed (DVPNMFOCUSCHANGE structure)
#define DVPN_CAPABILITIES	(DVPN_FIRST-7)		// Capabilities flags have changed (DVPNMCAPABILITIES structure)
#define DVPN_STATUSTEXT		(DVPN_FIRST-8)		// Display status text (DVPNMSTATUSTEXT structure)
#if ( VIEWERPLUGINVERSION >= 3 )
#define DVPN_LOADNEWFILE	(DVPN_FIRST-9)		// Load a new file (DVPNMLOADNEWFILE structre)
#define DVPN_SETCURSOR		(DVPN_FIRST-10)		// Set cursor (DVPNMSETCURSOR structure)
#define DVPN_MCLICK			(DVPN_FIRST-11)		// Middle button click, same as DVPN_CLICK
#define DVPN_GETGAMMA		(DVPN_FIRST-12)		// Get gamma correction settings (DVPNMGAMMA structure)
#define DVPN_BUTTONOPTS		(DVPN_FIRST-13)		// Get button options (DVPNMBUTTONOPTS structure)
#define DVPN_GETCURSORS		(DVPN_FIRST-14)		// Get handles to standard Opus cursors (DVPNMGETCURSORS structure)
#define DVPN_MOUSEWHEEL		(DVPN_FIRST-15)		// Mouse wheel message (DVPNMMOUSEWHEEL structure)
#endif
#if ( VIEWERPLUGINVERSION >= 4 )
#define DVPN_HEXSTATE		(DVPN_FIRST-16)		// Hex state changed (DVPNMHEXSTATE structure)
#endif

// Notification structure for DVPN_SIZECHANGE
typedef struct tagDVPNMSIZECHANGE
{
	NMHDR	hdr;
	SIZE	szSize;
} DVPNMSIZECHANGE, * LPDVPNMSIZECHANGE;

// Notification structure for DVPN_CLICK
typedef struct tagDVPNMCLICK
{
	NMHDR	hdr;
	POINT	pt;
	BOOL	fMenu;
} DVPNMCLICK, * LPDVPNMCLICK;

// Notification structure for DVPN_RESETZOOM
typedef struct tagDVPNMRESETZOOM
{
	NMHDR	hdr;
	int		iZoom;
} DVPNMRESETZOOM, * LPDVPNMRESETZOOM;

// Notification structure for DVPN_FOCUSCHANGE
typedef struct tagDVPNMFOCUSCHANGE
{
	NMHDR	hdr;
	BOOL	fGotFocus;
} DVPNMFOCUSCHANGE, * LPDVPNMFOCUSCHANGE;

// Notification structure for DVPN_CAPABILITIES
typedef struct tagDVPNMCAPABILITIES
{
	NMHDR	hdr;
	DWORD	dwCapabilities;
} DVPNMCAPABILITIES, * LPDVPNMCAPABILITIES;

// Notification structure for DVPN_STATUSTEXT
typedef struct tagDVPNMSTATUSTEXT
{
	NMHDR	hdr;
	LPTSTR	lpszStatusText;
	BOOL	fUnicode;
} DVPNMSTATUSTEXT, * LPDVPNMSTATUSTEXT;

// Notification structure for DVPN_LOADNEWFILE
typedef struct tagDVPNMLOADNEWFILE
{
	NMHDR		hdr;
	LPTSTR		lpszFilename;
	BOOL		fUnicode;
	LPSTREAM	lpStream;
} DVPNMLOADNEWFILE, * LPDVPNMLOADNEWFILE;

// Notification structure for DVPN_SETCURSOR
typedef struct tagDVPNMSETCURSOR
{
	NMHDR		hdr;
	POINT		pt;
	BOOL		fMenu;
	BOOL		fCanScroll;
	int			iCursor;
} DVPNMSETCURSOR, * LPDVPNMSETCURSOR;

// Notification structure for DVPN_GETGAMMA
typedef struct tagDVPNMGAMMA
{
	NMHDR		hdr;
	BOOL		fEnable;
	double		dbGamma;
} DVPNMGAMMA, * LPDVPNMGAMMA;

// Notification structure for DVPN_BUTTONOPTS
typedef struct tagDVPNMBUTTONOPTS
{
	NMHDR		hdr;
	int			iLeft;
	int			iRight;
	int			iMiddle;
} DVPNMBUTTONOPTS, * LPDVPNMBUTTONOPTS;

// Button options
enum
{
	BUTTONOPT_NONE,
	BUTTONOPT_SELECT,
	BUTTONOPT_ADVANCE,
	BUTTONOPT_SCROLL,
	BUTTONOPT_FULLSCREEN,
	BUTTONOPT_CLOSE,
};

// Notification structure for DVPN_GETCURSORS
typedef struct tagDVPNMGETCURSORS
{
	NMHDR		hdr;
	HCURSOR		hCurHandOpen;
	HCURSOR		hCurHandClosed;
	HCURSOR		hCurCrosshair;
} DVPNMGETCURSORS, * LPDVPNMGETCURSORS;

// Zoom constants for DVPLUGINMSG_ZOOM
#define ZOOM_ORIGINAL	0
#define ZOOM_FITPAGE	-1
#define ZOOM_TILED		-2

// Cursor constants for DVPN_SETCURSOR
#define VPCURSOR_NONE	0
#define VPCURSOR_DRAG	1
#define VPCURSOR_SELECT	2

// Notification structure for DVPN_MOUSEWHEEL
typedef struct tagDVPNMMOUSEWHEEL
{
	NMHDR		hdr;
	WPARAM		wParam;
	LPARAM		lParam;
} DVPNMMOUSEWHEEL, * LPDVPNMMOUSEWHEEL;

// Notification structure for DVPN_HEXSTATE
#if ( VIEWERPLUGINVERSION >= 4 )

typedef struct tagDVPNMHEXSTATE
{
	NMHDR		hdr;
	BOOL		fState;
} DVNMHEXSTATE, * LPDVPNMHEXSTATE;

#endif


// Viewer capability flags returned by DVPLUGINMSG_GETCAPABILITIES
#define VPCAPABILITY_RESIZE_FIT			(1<<0)	// Can resize to fit page
#define VPCAPABILITY_RESIZE_ANY			(1<<1)	// Can resize to any percentage
#define VPCAPABILITY_ROTATE_RIGHTANGLE	(1<<2)	// Can rotate in steps of 90 degrees
#define VPCAPABILITY_ROTATE_ANY			(1<<3)	// Can rotate to any angle
#define VPCAPABILITY_SELECTALL			(1<<4)	// Can 'select all' for clipboard copy
#define VPCAPABILITY_COPYALL			(1<<5)	// Can 'copy all' to clipboard
#define VPCAPABILITY_COPYSELECTION		(1<<6)	// Can 'copy selection' to clipboard
#define VPCAPABILITY_PRINT				(1<<7)	// Can print
#define VPCAPABILITY_WANTFOCUS			(1<<8)	// Viewer wants input focus
#define VPCAPABILITY_SHOWPROPERTIES		(1<<9)	// Can show properties dialog
#define VPCAPABILITY_WANTMOUSEWHEEL		(1<<10)	// Viewer wants to handle mouse wheel itself
#define VPCAPABILITY_ADDCONTEXTMENU		(1<<11)	// Viewer can add context menu items
#define VPCAPABILITY_HASDIALOGS			(1<<12)	// Viewer may have dialogs and wants DVPLUGINMSG_ISDLGMESSAGE messages
#define VPCAPABILITY_HASACCELERATORS	(1<<13)	// Viewer may have accelerators and wants DVPLUGINMSG_TRANSLATEACCEL messages
#if ( VIEWERPLUGINVERSION >= 3 )
#define VPCAPABILITY_CANSETWALLPAPER	(1<<14)	// Viewer can set desktop wallpaper
#define VPCAPABILITY_CANTRACKFOCUS		(1<<15)	// Viewer can reliably track its focus state
#define VPCAPABILITY_SUPPLYBITMAP		(1<<16)	// Viewer can return a bitmap of its current image
#define VPCAPABILITY_GAMMA				(1<<17)	// Viewer supports gamma correction
#define VPCAPABILITY_FILEINFO			(1<<18)	// Viewer can display file information
#endif
#if ( VIEWERPLUGINVERSION >= 4 )
#define VPCAPABILITY_RESIZE_TILE		(1<<19)	// Can display a tiled preview
#define VPCAPABILITY_HIDEALPHA			(1<<20)	// Can hide (ignore) the alpha channel
#define VPCAPABILITY_NOFULLSCREEN		(1<<21)	// Viewer does not support full screen mode
#endif

// Flags for DVPLUGINMSG_THUMBSCHANGED
#define DVPTCF_REDRAW		(1<<0)
#define DVPTCF_FLUSHCACHE	(1<<1)

// Structure used to add items to the context menu
typedef struct DVPContextMenuItemA
{
	LPSTR	lpszLabel;
	DWORD	dwFlags;
	UINT	uID;
} DVPCONTEXTMENUITEMA, * LPDVPCONTEXTMENUITEMA;

typedef struct DVPContextMenuItemW
{
	LPWSTR	lpszLabel;
	DWORD	dwFlags;
	UINT	uID;
} DVPCONTEXTMENUITEMW, * LPDVPCONTEXTMENUITEMW;

#ifdef UNICODE
#define DVPCONTEXTMENUITEM		DVPCONTEXTMENUITEMW
#define LPDVPCONTEXTMENUITEM	LPDVPCONTEXTMENUITEMW
#else
#define DVPCONTEXTMENUITEM		DVPCONTEXTMENUITEMA
#define LPDVPCONTEXTMENUITEM	LPDVPCONTEXTMENUITEMA
#endif

// DVPCONTEXTMENUITEM flags
#define DVPCMF_CHECKED					(1<<0)	// Item appears checked
#define DVPCMF_RADIOCHECK				(1<<1)	// Checkmark uses a radio button
#define DVPCMF_DISABLED					(1<<2)	// Item is disabled
#define DVPCMF_SEPARATOR				(1<<3)	// Item is a separator
#define DVPCMF_BEGINSUBMENU				(1<<4)	// Item is a submenu
#define DVPCMF_ENDSUBMENU				(1<<5)	// Item is the last in a submenu


// Structure passed to DVP_USBSafe function
#ifndef DEF_OPUSUSBSAFEDATA
#define DEF_OPUSUSBSAFEDATA

typedef struct OpusUSBSafeData
{
	UINT		cbSize;
	LPWSTR		pszOtherExports;
	UINT		cchOtherExports;
} OPUSUSBSAFEDATA, * LPOPUSUSBSAFEDATA;

#endif


// Exported plugin DLL functions
#define DVPFUNCNAME_INIT					"DVP_Init"
#if ( VIEWERPLUGINVERSION >= 4 )
#define DVPFUNCNAME_INITEX					"DVP_InitEx"
#define DVPFUNCNAME_USBSAFE					"DVP_USBSafe"
#endif
#define DVPFUNCNAME_UNINIT					"DVP_Uninit"
#define DVPFUNCNAME_IDENTIFYA				"DVP_IdentifyA"
#define DVPFUNCNAME_IDENTIFYW				"DVP_IdentifyW"
#define DVPFUNCNAME_IDENTIFYFILEA			"DVP_IdentifyFileA"
#define DVPFUNCNAME_IDENTIFYFILEW			"DVP_IdentifyFileW"
#define DVPFUNCNAME_IDENTIFYFILESTREAMA		"DVP_IdentifyFileStreamA"
#define DVPFUNCNAME_IDENTIFYFILESTREAMW		"DVP_IdentifyFileStreamW"
#define DVPFUNCNAME_IDENTIFYFILEBYTESA		"DVP_IdentifyFileBytesA"
#define DVPFUNCNAME_IDENTIFYFILEBYTESW		"DVP_IdentifyFileBytesW"
#define DVPFUNCNAME_LOADBITMAPA				"DVP_LoadBitmapA"
#define DVPFUNCNAME_LOADBITMAPW				"DVP_LoadBitmapW"
#define DVPFUNCNAME_LOADBITMAPSTREAMA		"DVP_LoadBitmapStreamA"
#define DVPFUNCNAME_LOADBITMAPSTREAMW		"DVP_LoadBitmapStreamW"
#define DVPFUNCNAME_LOADTEXTA				"DVP_LoadTextA"
#define DVPFUNCNAME_LOADTEXTW				"DVP_LoadTextW"
#define DVPFUNCNAME_SHOWPROPERTIESA			"DVP_ShowPropertiesA"
#define DVPFUNCNAME_SHOWPROPERTIESW			"DVP_ShowPropertiesW"
#define DVPFUNCNAME_SHOWPROPERTIESSTREAMA	"DVP_ShowPropertiesStreamA"
#define DVPFUNCNAME_SHOWPROPERTIESSTREAMW	"DVP_ShowPropertiesStreamW"
#define DVPFUNCNAME_CREATEVIEWER			"DVP_CreateViewer"
#define DVPFUNCNAME_CONFIGURE				"DVP_Configure"
#define DVPFUNCNAME_ABOUT					"DVP_About"
#define DVPFUNCNAME_GETFILEINFOFILEA		"DVP_GetFileInfoFileA"
#define DVPFUNCNAME_GETFILEINFOFILEW		"DVP_GetFileInfoFileW"
#define DVPFUNCNAME_GETFILEINFOFILESTREAMA	"DVP_GetFileInfoFileStreamA"
#define DVPFUNCNAME_GETFILEINFOFILESTREAMW	"DVP_GetFileInfoFileStreamW"

#ifdef UNICODE
#define DVPFUNCNAME_IDENTIFY				DVPFUNCNAME_IDENTIFYW
#define DVPFUNCNAME_IDENTIFYFILE			DVPFUNCNAME_IDENTIFYFILEW
#define DVPFUNCNAME_IDENTIFYFILESTREAM		DVPFUNCNAME_IDENTIFYFILESTREAMW
#define DVPFUNCNAME_IDENTIFYFILEBYTES		DVPFUNCNAME_IDENTIFYFILEBYTESW
#define DVPFUNCNAME_LOADBITMAP				DVPFUNCNAME_LOADBITMAPW
#define DVPFUNCNAME_LOADBITMAPSTREAM		DVPFUNCNAME_LOADBITMAPSTREAMW
#define DVPFUNCNAME_LOADTEXT				DVPFUNCNAME_LOADTEXTW
#define DVPFUNCNAME_SHOWPROPERTIES			DVPFUNCNAME_SHOWPROPERTIESW
#define DVPFUNCNAME_SHOWPROPERTIESSTREAM	DVPFUNCNAME_SHOWPROPERTIESSTREAMW
#define DVPFUNCNAME_GETFILEINFOFILE			DVPFUNCNAME_GETFILEINFOFILEW
#define DVPFUNCNAME_GETFILEINFOFILESTREAM	DVPFUNCNAME_GETFILEINFOFILESTREAMW
#else
#define DVPFUNCNAME_IDENTIFY				DVPFUNCNAME_IDENTIFYA
#define DVPFUNCNAME_IDENTIFYFILE			DVPFUNCNAME_IDENTIFYFILEA
#define DVPFUNCNAME_IDENTIFYFILESTREAM		DVPFUNCNAME_IDENTIFYFILESTREAMA
#define DVPFUNCNAME_IDENTIFYFILEBYTES		DVPFUNCNAME_IDENTIFYFILEBYTESA
#define DVPFUNCNAME_LOADBITMAP				DVPFUNCNAME_LOADBITMAPA
#define DVPFUNCNAME_LOADBITMAPSTREAM		DVPFUNCNAME_LOADBITMAPSTREAMA
#define DVPFUNCNAME_LOADTEXT				DVPFUNCNAME_LOADTEXTA
#define DVPFUNCNAME_SHOWPROPERTIES			DVPFUNCNAME_SHOWPROPERTIESA
#define DVPFUNCNAME_SHOWPROPERTIESSTREAM	DVPFUNCNAME_SHOWPROPERTIESSTREAMA
#define DVPFUNCNAME_GETFILEINFOFILE			DVPFUNCNAME_GETFILEINFOFILEA
#define DVPFUNCNAME_GETFILEINFOFILESTREAM	DVPFUNCNAME_GETFILEINFOFILESTREAMA
#endif

// dwStreamFlags for the Stream functions
#define DVPSF_Slow					(1<<0)		// Stream refers to 'slow' media
#define DVPSF_NoRandomSeek			(1<<1)		// Stream does not support random seek (sequential only)

// dwFlags for the CreateViewer function
#define DVPCVF_Border				(1<<1)		// Create viewer window with a border
#define DVPCVF_Preview				(1<<2)		// Viewer is being used for the Lister View Pane
#define DVPCVF_ReturnTabs			(1<<3)		// Pass tab keypresses (via NM_KEYDOWN message) to parent window

// Data structure for the DVP_LoadText function
#define DVPCVF_FromStream			(1<<0)


enum
{
	DVPText_Plain,
	DVPText_Rich,
	DVPText_HTML
};

typedef struct DVPLoadTextDataA
{
	UINT		cbSize;
	DWORD		dwFlags;
	HWND		hWndParent;
	LPSTR		lpszFile;
	LPSTREAM	lpInStream;
	DWORD		dwStreamFlags;
	LPSTREAM	lpOutStream;
	int			iOutTextType;
	CHAR		tchPreferredViewer[40];
	HANDLE		hAbortEvent;
} DVPLOADTEXTDATAA, * LPDVPLOADTEXTDATAA;

typedef struct DVPLoadTextDataW
{
	UINT		cbSize;
	DWORD		dwFlags;
	HWND		hWndParent;
	LPWSTR		lpszFile;
	LPSTREAM	lpInStream;
	DWORD		dwStreamFlags;
	LPSTREAM	lpOutStream;
	int			iOutTextType;
	WCHAR		tchPreferredViewer[40];
	HANDLE		hAbortEvent;
} DVPLOADTEXTDATAW, * LPDVPLOADTEXTDATAW;

#ifdef UNICODE
#define DVPLOADTEXTDATA		DVPLOADTEXTDATAW
#define LPDVPLOADTEXTDATA	LPDVPLOADTEXTDATAW
#else
#define DVPLOADTEXTDATA		DVPLOADTEXTDATAA
#define LPDVPLOADTEXTDATA	LPDVPLOADTEXTDATAA
#endif

// File Info stuff
typedef struct DVPFileInfoHeader
{
	UINT		cbSize;
	UINT		uiMajorType;
	DWORD		dwFlags;
} DVPFILEINFOHEADER, * LPDVPFILEINFOHEADER;

typedef struct DVPFileInfoMusicA
{
	DVPFILEINFOHEADER	hdr;
	LPSTR				lpszAlbum;
	UINT				cchAlbumMax;
	LPSTR				lpszArtist;
	UINT				cchArtistMax;
	LPSTR				lpszTitle;
	UINT				cchTitleMax;
	LPSTR				lpszGenre;
	UINT				cchGenreMax;
	LPSTR				lpszComment;
	UINT				cchCommentMax;
	LPSTR				lpszFormat;
	UINT				cchFormatMax;
	LPSTR				lpszEncoder;
	UINT				cchEncoderMax;
	DWORD				dwBitRate;
	DWORD				dwSampleRate;
	DWORD				dwDuration;
	int					iTrackNum;
	int					iYear;
	int					iNumChannels;
	DWORD				dwMusicFlags;
	LPSTR				lpszCodec;
	UINT				cchCodecMax;
} DVPFILEINFOMUSICA, * LPDVPFILEINFOMUSICA;

typedef struct DVPFileInfoMusicW
{
	DVPFILEINFOHEADER	hdr;
	LPWSTR				lpszAlbum;
	UINT				cchAlbumMax;
	LPWSTR				lpszArtist;
	UINT				cchArtistMax;
	LPWSTR				lpszTitle;
	UINT				cchTitleMax;
	LPWSTR				lpszGenre;
	UINT				cchGenreMax;
	LPWSTR				lpszComment;
	UINT				cchCommentMax;
	LPWSTR				lpszFormat;
	UINT				cchFormatMax;
	LPWSTR				lpszEncoder;
	UINT				cchEncoderMax;
	DWORD				dwBitRate;
	DWORD				dwSampleRate;
	DWORD				dwDuration;
	int					iTrackNum;
	int					iYear;
	int					iNumChannels;
	DWORD				dwMusicFlags;
	LPWSTR				lpszCodec;
	UINT				cchCodecMax;
} DVPFILEINFOMUSICW, * LPDVPFILEINFOMUSICW;

#ifdef UNICODE
#define DVPFILEINFOMUSIC	DVPFILEINFOMUSICW
#define LPDVPFILEINFOMUSIC	LPDVPFILEINFOMUSICW
#else
#define DVPFILEINFOMUSIC	DVPFILEINFOMUSICA
#define LPDVPFILEINFOMUSIC	LPDVPFILEINFOMUSICA
#endif

#define DVPMusicFlag_VBR			(1<<0)
#define DVPMusicFlag_VBRAccurate	(1<<1)
#define DVPMusicFlag_JointStereo	(1<<2)

typedef struct DVPFileInfoMovieA
{
	DVPFILEINFOHEADER	hdr;
	SIZE				szVideoSize;
	int					iNumBits;
	DWORD				dwDuration;
	DWORD				dwFrames;
	double				flFrameRate;
	DWORD				dwDataRate;
	POINT				ptAspectRatio;
	DWORD				dwAudioBitRate;
	DWORD				dwAudioSampleRate;
	int					iNumChannels;
	LPSTR				lpszVideoCodec;
	UINT				cchVideoCodecMax;
	LPSTR				lpszAudioCodec;
	UINT				cchAudioCodecMax;
} DVPFILEINFOMOVIEA, * LPDVPFILEINFOMOVIEA;

typedef struct DVPFileInfoMovieW
{
	DVPFILEINFOHEADER	hdr;
	SIZE				szVideoSize;
	int					iNumBits;
	DWORD				dwDuration;
	DWORD				dwFrames;
	double				flFrameRate;
	DWORD				dwDataRate;
	POINT				ptAspectRatio;
	DWORD				dwAudioBitRate;
	DWORD				dwAudioSampleRate;
	int					iNumChannels;
	LPWSTR				lpszVideoCodec;
	UINT				cchVideoCodecMax;
	LPWSTR				lpszAudioCodec;
	UINT				cchAudioCodecMax;
} DVPFILEINFOMOVIEW, * LPDVPFILEINFOMOVIEW;

#ifdef UNICODE
#define DVPFILEINFOMOVIE	DVPFILEINFOMOVIEW
#define LPDVPFILEINFOMOVIE	LPDVPFILEINFOMOVIEW
#else
#define DVPFILEINFOMOVIE	DVPFILEINFOMOVIEA
#define LPDVPFILEINFOMOVIE	LPDVPFILEINFOMOVIEA
#endif


#if ( VIEWERPLUGINVERSION >= 4 )

// Data structure passed to DVP_InitEx
typedef struct DVPInitExData
{
	UINT			cbSize;
	HWND			hwndDOpusMsgWindow;
	DWORD			dwOpusVerMajor;
	DWORD			dwOpusVerMinor;
	LPWSTR			pszLanguageName;
} DVPINITEXDATA, * LPDVPINITEXDATA;

#endif


// Function prototypes
typedef BOOL (*PFNDVPINIT)(void);
#if ( VIEWERPLUGINVERSION >= 4 )
typedef BOOL (*PFNDVPINITEX)(LPDVPINITEXDATA pInitExData);
typedef BOOL (*PFNDVPUSBSAFE)(LPOPUSUSBSAFEDATA pUSBSafeData);
#endif
typedef void (*PFNDVPUNINIT)(void);
typedef BOOL (*PFNDVPIDENTIFYA)(LPVIEWERPLUGININFOA lpVPInfo);
typedef BOOL (*PFNDVPIDENTIFYW)(LPVIEWERPLUGININFOW lpVPInfo);
typedef BOOL (*PFNDVPIDENTIFYFILEA)(HWND hWnd,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,HANDLE hAbortEvent);
typedef BOOL (*PFNDVPIDENTIFYFILEW)(HWND hWnd,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,HANDLE hAbortEvent);
typedef BOOL (*PFNDVPIDENTIFYFILESTREAMA)(HWND hWnd,LPSTREAM lpStream,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,DWORD dwStreamFlags);
typedef BOOL (*PFNDVPIDENTIFYFILESTREAMW)(HWND hWnd,LPSTREAM lpStream,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,DWORD dwStreamFlags);
typedef BOOL (*PFNDVPIDENTIFYFILEBYTESA)(HWND hWnd,LPSTR lpszName,LPBYTE lpData,UINT uiDataSize,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,DWORD dwStreamFlags);
typedef BOOL (*PFNDVPIDENTIFYFILEBYTESW)(HWND hWnd,LPWSTR lpszName,LPBYTE lpData,UINT uiDataSize,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,DWORD dwStreamFlags);
typedef HBITMAP (*PFNDVPLOADBITMAPA)(HWND hWnd,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,LPSIZE lpszDesiredSize,HANDLE hAbortEvent);
typedef HBITMAP (*PFNDVPLOADBITMAPW)(HWND hWnd,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,LPSIZE lpszDesiredSize,HANDLE hAbortEvent);
typedef HBITMAP (*PFNDVPLOADBITMAPSTREAMA)(HWND hWnd,LPSTREAM lpStream,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,LPSIZE lpszDesiredSize,DWORD dwStreamFlags);
typedef HBITMAP (*PFNDVPLOADBITMAPSTREAMW)(HWND hWnd,LPSTREAM lpStream,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,LPSIZE lpszDesiredSize,DWORD dwStreamFlags);
typedef BOOL (*PFNDVPLOADTEXTA)(LPDVPLOADTEXTDATAA lpLoadTextData);
typedef BOOL (*PFNDVPLOADTEXTW)(LPDVPLOADTEXTDATAW lpLoadTextData);
typedef HWND (*PFNDVPSHOWPROPERTIESA)(HWND hWndParent,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo);
typedef HWND (*PFNDVPSHOWPROPERTIESW)(HWND hWndParent,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo);
typedef HWND (*PFNDVPSHOWPROPERTIESSTREAMA)(HWND hWndParent,LPSTREAM lpStream,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,DWORD dwStreamFlags);
typedef HWND (*PFNDVPSHOWPROPERTIESSTREAMW)(HWND hWndParent,LPSTREAM lpStream,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,DWORD dwStreamFlags);
typedef HWND (*PFNDVPCREATEVIEWER)(HWND hWndParent,LPRECT lpRc,DWORD dwFlags);
typedef HWND (*PFNDVPCONFIGURE)(HWND hWndParent,HWND hWndNotify,DWORD dwNotifyData);
typedef HWND (*PFNDVPABOUT)(HWND hWndParent);
typedef BOOL (*PFNDVPGETFILEINFOFILEA)(HWND hWnd,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,LPDVPFILEINFOHEADER lpFIH,HANDLE hAbortEvent);
typedef BOOL (*PFNDVPGETFILEINFOFILEW)(HWND hWnd,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,LPDVPFILEINFOHEADER lpFIH,HANDLE hAbortEvent);
typedef BOOL (*PFNDVPGETFILEINFOFILESTREAMA)(HWND hWnd,LPSTREAM lpStream,LPSTR lpszName,LPVIEWERPLUGINFILEINFOA lpVPFileInfo,LPDVPFILEINFOHEADER lpFIH,DWORD dwStreamFlags);
typedef BOOL (*PFNDVPGETFILEINFOFILESTREAMW)(HWND hWnd,LPSTREAM lpStream,LPWSTR lpszName,LPVIEWERPLUGINFILEINFOW lpVPFileInfo,LPDVPFILEINFOHEADER lpFIH,DWORD dwStreamFlags);

#ifdef UNICODE
#define PFNDVPIDENTIFY				PFNDVPIDENTIFYW
#define PFNDVPIDENTIFYFILE			PFNDVPIDENTIFYFILEW
#define PFNDVPIDENTIFYFILESTREAM	PFNDVPIDENTIFYFILESTREAMW
#define PFNDVPIDENTIFYFILEBYTES		PFNDVPIDENTIFYFILEBYTESW
#define PFNDVPLOADBITMAP			PFNDVPLOADBITMAPW
#define PFNDVPLOADBITMAPSTREAM		PFNDVPLOADBITMAPSTREAMW
#define PFNDVPLOADTEXT				PFNDVPLOADTEXTW
#define PFNDVPSHOWPROPERTIES		PFNDVPSHOWPROPERTIESW
#define PFNDVPSHOWPROPERTIESSTREAM	PFNDVPSHOWPROPERTIESSTREAMW
#define PFNDVPGETFILEINFOFILE		PFNDVPGETFILEINFOFILEW
#define PFNDVPGETFILEINFOFILESTREAM	PFNDVPGETFILEINFOFILESTREAMW
#else
#define PFNDVPIDENTIFY				PFNDVPIDENTIFYA
#define PFNDVPIDENTIFYFILE			PFNDVPIDENTIFYFILEA
#define PFNDVPIDENTIFYFILESTREAM	PFNDVPIDENTIFYFILESTREAMA
#define PFNDVPIDENTIFYFILEBYTES		PFNDVPIDENTIFYFILEBYTESA
#define PFNDVPLOADBITMAP			PFNDVPLOADBITMAPA
#define PFNDVPLOADBITMAPSTREAM		PFNDVPLOADBITMAPSTREAMA
#define PFNDVPLOADTEXT				PFNDVPLOADTEXTA
#define PFNDVPSHOWPROPERTIES		PFNDVPSHOWPROPERTIESA
#define PFNDVPSHOWPROPERTIESSTREAM	PFNDVPSHOWPROPERTIESSTREAMA
#define PFNDVPGETFILEINFOFILE		PFNDVPGETFILEINFOFILEA
#define PFNDVPGETFILEINFOFILESTREAM	PFNDVPGETFILEINFOFILESTREAMA
#endif


#ifdef UNICODE
#define DVP_Identify				DVP_IdentifyW
#define DVP_IdentifyFile			DVP_IdentifyFileW
#define DVP_IdentifyFileStream		DVP_IdentifyFileStreamW
#define DVP_IdentifyFileBytes		DVP_IdentifyFileBytesW
#define DVP_LoadBitmap				DVP_LoadBitmapW
#define DVP_LoadBitmapStream		DVP_LoadBitmapStreamW
#define DVP_LoadText				DVP_LoadTextW
#define DVP_ShowProperties			DVP_ShowPropertiesW
#define DVP_ShowPropertiesStream	DVP_ShowPropertiesStreamW
#define DVP_GetFileInfoFile			DVP_GetFileInfoFileW
#define DVP_GetFileInfoFileStream	DVP_GetFileInfoFileStreamW
#else
#define DVP_Identify				DVP_IdentifyA
#define DVP_IdentifyFile			DVP_IdentifyFileA
#define DVP_IdentifyFileStream		DVP_IdentifyFileStreamA
#define DVP_IdentifyFileBytes		DVP_IdentifyFileBytesA
#define DVP_LoadBitmap				DVP_LoadBitmapA
#define DVP_LoadBitmapStream		DVP_LoadBitmapStreamA
#define DVP_LoadText				DVP_LoadTextA
#define DVP_ShowProperties			DVP_ShowPropertiesA
#define DVP_ShowPropertiesStream	DVP_ShowPropertiesStreamA
#define DVP_GetFileInfoFile			DVP_GetFileInfoFileA
#define DVP_GetFileInfoFileStream	DVP_GetFileInfoFileStreamA
#endif


// Image frame size used by Opus
#if ( VIEWERPLUGINVERSION >= 4 )
#define OPUSVIEWER_IMAGE_FRAME_SIZE		14
#else
#define OPUSVIEWER_IMAGE_FRAME_SIZE		8
#endif

#endif

