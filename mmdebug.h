#ifndef LIBMMQT_DEBUG_H
#define LIBMMQT_DEBUG_H

#include <QDebug>

#define mmDebug() \
	qDebug() << "libmm-qt" << QCoreApplication::applicationName() << '(' << QCoreApplication::applicationPid() << ')' << __PRETTY_FUNCTION__ << ':'

#endif
