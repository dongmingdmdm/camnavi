/*
      This file is part of FolkiGpu.

    FolkiGpu is free software: you can redistribute it and/or modify
    it under the terms of the GNU Leeser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Leeser General Public License
    along with FolkiGpu.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
      FolkiGpu is a demonstration software developed by Aurelien Plyer during
    his phd at Onera (2008-2011). For more information please visit :
      - http://www.onera.fr/dtim-en/gpu-for-image/folkigpu.php
      - http://www.plyer.fr (author homepage)
*/

#ifndef __LI_GESTION_BUFFER_HPP
#define __LI_GESTION_BUFFER_HPP

#include <GL/glew.h>


namespace LIVA {

class MyVbo {

public:
	MyVbo(){ 
		lock = false; 
		vbo = 0;
	}
	~MyVbo(){ 
		if(vbo != 0)
			glDeleteBuffers(1, &vbo);
	}

	GLuint vbo;

	bool lock;
};

}//namespace LIVA
#endif
