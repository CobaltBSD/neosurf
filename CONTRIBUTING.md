# Contributing to NeoSurf

This file is currently a work in progress.

## Styling
* For sake of conformity, please use proper tab characters instead of 4 or 8 spaces for indentation.
* Ensure lines are not longer than 80 characters.

## Macros
NeoSurf takes a strict stance on macro usage. Preprocessor macros should be entirely avoided, except:
* Including header files (obviously)
* #pragma once
* #if, #elif, #else for disabling or enabling certain sections of code depending on the build configuration
This means that constants or what can be implemented using macros should always be done so as proper C code. Do note that some legacy NetSurf code included in this repository contains complex macros.
The reason for this stance is that heavy macro usage can drastically hinder code understandability, which is an issue that often manifests in the codebases of other large C projects.
