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
#include <QEvent>
#include <QKeyEvent>
#include <QWindow>
#include <QApplication>

#include <QDebug>

#ifdef HAVE_X11
    #include <QX11Info>
#endif

//-----------------------------------------------------------------------------

KScreenSaver::KScreenSaver(QWidget *id)
    : QWidget(id)
    , embeddedWidget(0)
{
    isXCB = QGuiApplication::platformName().contains(QLatin1String("xcb"));
}

KScreenSaver::KScreenSaver(WId id)
    : QWidget()
    , embeddedWidget(0)
{
    if (id) {
        create(id, false, true);
//         QWidget::createWindowContainer(QWindow::fromWinId(id))
    }
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

bool KScreenSaver::eventFilter( QObject *, QEvent *e )
{
    if (e->type() == QEvent::KeyPress) {
        keyPressEvent( (QKeyEvent *)e );
        return true;
    } else if( e->type() == QEvent::Close ) {
        // In demo mode, screensaver's QWidget does create()
        // with winId of the DemoWidget, which results in two QWidget's
        // sharing the same winId and Qt delivering events only to one of them.
        qApp->quit();
    }
    return false;
}

void KScreenSaver::keyPressEvent(QKeyEvent *e)
{
    if (e->text() == QLatin1String("q")) {
        qApp->quit();
    }
}

void KScreenSaver::closeEvent(QCloseEvent *)
{
    qApp->quit();
}

KScreenSaverInterface::~KScreenSaverInterface()
{
}

KAboutData *KScreenSaverInterface::aboutData()
{
    return 0;
}

KScreenSaver *KScreenSaverInterface::create(WId id)
{
    QWindow *saveWindow = QWindow::fromWinId(id);
    return create(QWidget::createWindowContainer(saveWindow));
}

QDialog* KScreenSaverInterface::setup()
{
    return 0;
}



// #include "kscreensaver.moc"
