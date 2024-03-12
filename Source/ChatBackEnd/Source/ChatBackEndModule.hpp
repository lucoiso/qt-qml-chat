// Copyright Notices: [...]

#ifndef CHATBACKENDMODULE_H
#define CHATBACKENDMODULE_H

#ifdef BUILD_DLL
#define CHATBACKENDMODULE_API _declspec(dllexport)
#else
#define CHATBACKENDMODULE_API _declspec(dllimport)
#endif
#endif
