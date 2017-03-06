/* This file is part of the KDE libraries

    Copyright (c) 2001  Martin R. Jones <mjones@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kscreensaver.h"

#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QDebug>
#include <krandom.h>

#ifdef HAVE_X11
    #include <QX11Info>
    #include <X11/Xlib.h>
#endif

//-----------------------------------------------------------------------------

KScreenSaver::KScreenSaver( QWidget *id )
    : QWidget(id)
    , embeddedWidget(0)
{
    isXCB = QGuiApplication::platformName().contains(QLatin1String("xcb"));
}

KScreenSaver::~KScreenSaver()
{
    destroy( false, false );
}

bool KScreenSaver::event(QEvent* e)
{
    bool r = QWidget::event(e);
    if (e->type() == QEvent::Polish) {
        setAttribute(Qt::WA_StyledBackground, false);
    }
    if ((e->type() == QEvent::Resize) && embeddedWidget) {
        embeddedWidget->resize( size() );
    }
    return r;
}

void KScreenSaver::embed(QWidget *w)
{
    w->resize( size() );
    QApplication::sendPostedEvents();
#if defined(HAVE_X11) //FIXME
    if (isXCB) {
        XReparentWindow(QX11Info::display(), w->winId(), winId(), 0, 0);
    }
#endif
    w->setGeometry( 0, 0, width(), height() );
    embeddedWidget = w;
    QApplication::sendPostedEvents();
}

KScreenSaverInterface::~KScreenSaverInterface()
{
}

KAboutData *KScreenSaverInterface::aboutData()
{
    return 0;
}

QDialog* KScreenSaverInterface::setup()
{
    return 0;
}



// #include "kscreensaver.moc"
