#ifndef SC_VBAPALETTE_HXX
#define SC_VBAPALETTE_HXX

#include "vbahelper.hxx"
#include <document.hxx>
#include <com/sun/star/container/XIndexAccess.hpp>

class ScVbaPalette
{
private:
	SfxObjectShell* m_pShell;
public:
	ScVbaPalette(  SfxObjectShell* pShell ) : m_pShell( pShell ){}
	// if no palette available e.g. because the document doesn't have a 
	// palette defined then a default palette will be returned.
	css::uno::Reference< css::container::XIndexAccess > getPalette();
};

#endif //SC_VBAPALETTE_HXX

