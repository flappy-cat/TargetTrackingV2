#ifndef RECTI_GLOBAL_H
#define RECTI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RECTI_LIBRARY)
#  define RECTISHARED_EXPORT Q_DECL_EXPORT
#else
#  define RECTISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RECTI_GLOBAL_H
