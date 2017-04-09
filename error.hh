#ifndef __MVGAME_ERROR_H__
#define __MVGAME_ERROR_H__

enum mv_ErrorFormat {
	TERMBOX,
	STDIO
};

void mv_error(char *message, enum mv_ErrorFormat format);		// If 'format' is TERMBOX, then a pseudo-gui window with an OK button will appear. If it is STDIO then it is assumed that termbox has not been initialized and the error message will be printed using printf().

#endif
