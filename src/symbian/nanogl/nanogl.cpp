/*
Copyright (C) 2007-2008 Olli Hinkka

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <e32std.h>

#include "nanogl.h"
#include "glesinterface.h"
#include "gl.h"

_LIT(KGLESLibraryName, "libGLES_CM.dll");

RLibrary* glesLib = NULL;
GlESInterface* glEsImpl = NULL;
static const TInt KGLEsFunctionCount = 175;

extern void InitGLStructs();

int CreateGlEsInterface()
    {
    glEsImpl = new GlESInterface;
    if (!glEsImpl)
        {
        return -1;
        }
    Mem::FillZ(glEsImpl, sizeof(GlESInterface));     
    TUint32** ptr = (TUint32**)(glEsImpl);
    for (TInt count = 0; count < KGLEsFunctionCount; count++)
        {
        *ptr++ = (TUint32*)(glesLib->Lookup(count+1));
        }
    return 0;
    }

int nanoGL_Init()
    {
    glesLib = new RLibrary;
    if (!glesLib)
        {
        return -1;
        }
    if (glesLib->Load(KGLESLibraryName))
        {
        delete glesLib;
        glesLib = NULL;
        return -1;
        }
    if (CreateGlEsInterface() == -1)
        {
        if (glesLib)
            {
            glesLib->Close();
            }
        delete glesLib;
        glesLib = NULL;
        
        return -1;
        }
    InitGLStructs();
    return 0;
    }

void nanoGL_Destroy()
    {
    if (glEsImpl)
        {
        delete glEsImpl;
        glEsImpl = NULL;
        }
    if (glesLib)
        {
        glesLib->Close();
        }
    delete glesLib;
    glesLib = NULL;
    }
    
