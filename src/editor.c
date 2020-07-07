#include "stdafx.h"
#include "editor.h"
#include "log.h"

void destroyEditor(Editor *editor) {
	destroyBuffer(&editor->buffer);
	*editor = (Editor){0};
}
