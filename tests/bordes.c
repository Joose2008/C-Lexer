/* bordes.c - Casos borde: codigo sin espacios, IDs con '_' y digitos,
 * ceros a la izquierda, numeros largos, escapes en strings/chars y directivas */
int x=1;int y=2;
  	
_int _ _a1 B2 c3d4_5
0 00 007 1024 9999999999
"a" "" "con\tescapes\n\"ok\""
'a' '\n' '\'' '\\'
#ifdef X
#include <stdio.h>
#endif
;
