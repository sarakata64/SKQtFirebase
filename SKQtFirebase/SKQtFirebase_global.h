#ifndef SKQTFIREBASE_GLOBAL_H
#define SKQTFIREBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SKQTFIREBASE_LIBRARY)
#  define SKQTFIREBASE_EXPORT Q_DECL_EXPORT
#else
#  define SKQTFIREBASE_EXPORT Q_DECL_IMPORT
#endif
 extern QString _idToken ;
 extern QString _accessToken ;
 extern QByteArray _loginData ;
 extern QString _apikey ;
#endif // SKQTFIREBASE_GLOBAL_H
