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
#include <QDesktopWidget>
#include <QApplication>

#include <QDebug>

#ifdef HAVE_X11
    #include <QX11Info>
#endif

#include <KWindowSystem>

//-----------------------------------------------------------------------------

KScreenSaver::KScreenSaver(QWidget *id)
    : QWidget(id)
    , self(this)
{
    isXCB = KWindowSystem::isPlatformX11();
}

KScreenSaver::KScreenSaver(WId id)
    : QWidget(QWidget::find(id))
{
    isXCB = KWindowSystem::isPlatformX11();
    if (id) {
        self = parentWidget();
        if (!self) {
            self = QWidget::createWindowContainer(QWindow::fromWinId(id));
        }
        qWarning() << Q_FUNC_INFO << "K5S widget this=" << this << "uses self=" << self << "for WId" << id
            << "( -> WId=" << self->winId() << ")";
    }
}

KScreenSaver::~KScreenSaver()
{
    destroy( false, false );
}

bool KScreenSaver::event(QEvent* e)
{
    bool r = QWidget::event(e);
    if (e->type() == QEvent::Polish) {
        self->setAttribute(Qt::WA_StyledBackground, false);
    }
    if ((e->type() == QEvent::Resize) && self && self != this) {
        self->resize( size() );
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
