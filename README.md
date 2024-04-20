# TheGameOfLifeCPP
Game of Life made with C++ console buffers

Just type the name of one of the test input files, or write your own using empty template.

Issues:
Game needs to be run with admin rights, because of the stupid SetConsoleCursorPosition function.
I don't know why it only works this way. Code was written long ago so I don't care.
If you run without admin rights there will be flickering caused by cursor position difference between screen buffers.
Resizing console window after starting life also can cause troubles.
