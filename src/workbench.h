#pragma once
#include "stdafx.h"
#include "editor.h"

typedef enum {
	FOCUS_LEFT,
	FOCUS_RIGHT,
	// FOCUS_COMMAND,
	// FOCUS_OUTPUT,
} FocusState;

typedef struct {
	FocusState focus;

	Editor left;
	Editor right;

	// TODO: Output panel
	// TODO: Command Palette
} Workbench;
