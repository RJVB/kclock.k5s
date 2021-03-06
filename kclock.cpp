// kclock - Clock screen saver for KDE
//
// Copyright (c) 2003, 2006, 2007, 2008 Melchior FRANZ <mfranz # kde : org>
// Copyright (c) 2017 René J.V. Bertin <rjvbertin@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <QCheckBox>
#include <QColor>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

#include <QApplication>
#include <QCommandLineParser>
#include <QWidget>
#include <QWindow>

#include <QDebug>

#include <KAboutData>
#include <KColorButton>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <KMessageBox>

#include "kclock.h"


const int COLOR_BUTTON_WIDTH = 80;
const int TIMER_INTERVAL = 100;
const int MAX_CLOCK_SIZE = 10;
const unsigned int DEFAULT_CLOCK_SIZE = 8;
const bool DEFAULT_KEEP_CENTERED = false;

class KClockSaverInterface : public KScreenSaverInterface {
public:
    virtual KAboutData *aboutData()
    {
        KAboutData *aboutData = new KAboutData( QStringLiteral("kclock.k5s"), i18n( "Clock Screen Saver" ),
            QStringLiteral("5.0"), // version string
            i18n( "An Analog Clock \"Screen Saver\"" ),
            KAboutLicense::GPL,
            i18n( "(c) Melchior FRANZ (c) 2003, 2006, 2007\n(c) 2017, René J.V. Bertin" ) );
        aboutData->addAuthor( i18n( "Melchior FRANZ" ), i18n( "(Former) Maintainer and Developer" ), QStringLiteral("mfranz@kde.org") );
        aboutData->addAuthor( i18n( "René J.V. Bertin" ), i18n( "Port to KF5" ), QStringLiteral("rjvbertin@gmail.com") );
        aboutData->setTranslator( i18nc( "NAME OF TRANSLATORS", "Your names" ), i18nc( "EMAIL OF TRANSLATORS", "Your emails" ) );
        return aboutData;
    }

    virtual KScreenSaver *create(QWidget *id) 
    {
        return new KClockWidget(id);
    }

    virtual KScreenSaver *create(WId id)
    {
        return new KClockWidget(id);
    }

    virtual QDialog *setup()
    {
        return new KClockSetup();
    }
};



int main(int argc, char *argv[])
{
#if 0
    qunsetenv("SESSION_MANAGER");
    QApplication app(argc, argv);

    KAboutData aboutData( QStringLiteral("kclock.k5s"), i18n( "Clock Screen Saver" ),
        QStringLiteral("5.0"), // version string
        i18n( "An Analog Clock \"Screen Saver\"" ),
        KAboutLicense::GPL,
        i18n( "(c) Melchior FRANZ (c) 2003, 2006, 2007\n(c) 2017, René J.V. Bertin" ) );
    aboutData.addAuthor( i18n( "Melchior FRANZ" ), i18n( "(Former) Maintainer and Developer" ), QStringLiteral("mfranz@kde.org") );
    aboutData.addAuthor( i18n( "René J.V. Bertin" ), i18n( "Port to KF5" ), QStringLiteral("rjvbertin@gmail.com") );
    aboutData.setTranslator( i18nc( "NAME OF TRANSLATORS", "Your names" ), i18nc( "EMAIL OF TRANSLATORS", "Your emails" ) );

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    parser.setApplicationDescription(app.translate("main", "The KClock analog clock screensaver from KDE4's kdeartwork"));
    const QCommandLineOption setupOption(QStringLiteral("setup"), i18n("Set up the screensaver"));
    const QCommandLineOption rootOption(QStringLiteral("root"), i18n("Run in the root XWindow or fullscreen"));
    const QCommandLineOption demoOption(QStringLiteral("demo"), i18n("Run in demo mode (default)"));
    parser.addOptions({setupOption, rootOption, demoOption});
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    int ret;
    if (parser.isSet(setupOption)) {
        KClockSetup setup;
        setup.show();
        ret = app.exec();
    } else {
        KClockWidget kclock(0, !parser.isSet(rootOption));
        ret = app.exec();
    }
    return ret;
#else
    KClockSaverInterface k5s;
    return kScreenSaverMain(argc, argv, k5s);
#endif
}


//-----------------------------------------------------------------------------


KClockSetup::KClockSetup(QWidget *parent) :
    QDialog(parent),
    _saver(0)
{
    setWindowTitle(i18n("Setup Clock Screen Saver"));

    readSettings();

    QVBoxLayout *top = new QVBoxLayout(this);

    QHBoxLayout *hbox = new QHBoxLayout;
    top->addLayout(hbox);


    QGroupBox *colgroup = new QGroupBox(i18n("Colors"), this);
    QGridLayout *grid = new QGridLayout();

    QLabel *label;
    KColorButton *colorButton;

    label = new QLabel(i18n("&Hour-hand:"));
    colorButton = new KColorButton(_hourColor);
    colorButton->setFixedWidth(COLOR_BUTTON_WIDTH);
    label->setBuddy(colorButton);
    connect(colorButton, SIGNAL(changed(QColor)),
            SLOT(slotHourColor(QColor)));
    grid->addWidget(label, 1, 1);
    grid->addWidget(colorButton, 1, 2);

    label = new QLabel(i18n("&Minute-hand:"));
    colorButton = new KColorButton(_minColor);
    colorButton->setFixedWidth(COLOR_BUTTON_WIDTH);
    label->setBuddy(colorButton);
    connect(colorButton, SIGNAL(changed(QColor)),
            SLOT(slotMinColor(QColor)));
    grid->addWidget(label, 2, 1);
    grid->addWidget(colorButton, 2, 2);

    label = new QLabel(i18n("&Second-hand:"));
    colorButton = new KColorButton(_secColor);
    colorButton->setFixedWidth(COLOR_BUTTON_WIDTH);
    label->setBuddy(colorButton);
    connect(colorButton, SIGNAL(changed(QColor)),
            SLOT(slotSecColor(QColor)));
    grid->addWidget(label, 3, 1);
    grid->addWidget(colorButton, 3, 2);

    label = new QLabel(i18n("Scal&e:"));
    colorButton = new KColorButton(_scaleColor);
    colorButton->setFixedWidth(COLOR_BUTTON_WIDTH);
    label->setBuddy(colorButton);
    connect(colorButton, SIGNAL(changed(QColor)),
            SLOT(slotScaleColor(QColor)));
    grid->addWidget(label, 4, 1);
    grid->addWidget(colorButton, 4, 2);

    label = new QLabel(i18n("&Background:"));
    colorButton = new KColorButton(_bgndColor);
    colorButton->setFixedWidth(COLOR_BUTTON_WIDTH);
    label->setBuddy(colorButton);
    connect(colorButton, SIGNAL(changed(QColor)),
            SLOT(slotBgndColor(QColor)));
    grid->addWidget(label, 5, 1);
    grid->addWidget(colorButton, 5, 2);

    hbox->addWidget(colgroup);
    colgroup->setLayout(grid);

    _saver = new KClockWidget(this);
    _saver->setFixedSize(220, 165);
    _saver->show();
    hbox->addWidget(_saver);

    label = new QLabel(i18n("Si&ze:"), this);
    top->addWidget(label);
    QSlider *qs = new QSlider(Qt::Horizontal);
    label->setBuddy(qs);
    qs->setRange(0, MAX_CLOCK_SIZE);
    qs->setSliderPosition(_size);
    qs->setTickInterval(1);
    qs->setTickPosition(QSlider::TicksBelow);
    connect(qs, SIGNAL(valueChanged(int)), this, SLOT(slotSliderMoved(int)));
    top->addWidget(qs);

    QHBoxLayout *qsscale = new QHBoxLayout;
    qsscale->setContentsMargins(0, 0, 0, 0);
    qsscale->setSpacing(0);
    label = new QLabel(i18n("Small"), this);
    label->setAlignment(Qt::AlignLeading);
    qsscale->addWidget(label);
    label = new QLabel(i18n("Medium"), this);
    label->setAlignment(Qt::AlignHCenter);
    qsscale->addWidget(label);
    label = new QLabel(i18n("Big"), this);
    label->setAlignment(Qt::AlignTrailing);
    qsscale->addWidget(label);
    top->addLayout(qsscale);

    QCheckBox *keepCentered = new QCheckBox(i18n("&Keep clock centered"), this);
    keepCentered->setChecked(_keepCentered);
    connect(keepCentered, SIGNAL(stateChanged(int)), SLOT(slotKeepCenteredChanged(int)));
    top->addWidget(keepCentered);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    QPushButton *helpButton = buttonBox->addButton(QDialogButtonBox::Help);
    if (helpButton) {
        helpButton->setText(i18n("A&bout"));
    }
    connect(buttonBox, &QDialogButtonBox::accepted, this, &KClockSetup::slotOk);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::helpRequested, this, &KClockSetup::slotHelp);
    top->addWidget(buttonBox);

    top->addStretch();

}


KClockSetup::~KClockSetup()
{
    delete _saver;
}


void KClockSetup::readSettings()
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup settings(config, "Settings");

    _keepCentered = settings.readEntry("KeepCentered", DEFAULT_KEEP_CENTERED);
    _size = settings.readEntry("Size", DEFAULT_CLOCK_SIZE);
    if (_size > MAX_CLOCK_SIZE)
        _size = MAX_CLOCK_SIZE;

    KConfigGroup colors(config, "Colors");
    QColor c = Qt::black;
    _bgndColor = colors.readEntry("Background", c);

    c = Qt::white;
    _scaleColor = colors.readEntry("Scale", c);
    _hourColor = colors.readEntry("HourHand", c);
    _minColor = colors.readEntry("MinuteHand", c);

    c = Qt::red;
    _secColor = colors.readEntry("SecondHand", c);

    if (_saver) {
        _saver->setBgndColor(_bgndColor);
        _saver->setScaleColor(_scaleColor);
        _saver->setHourColor(_hourColor);
        _saver->setMinColor(_minColor);
        _saver->setSecColor(_secColor);
    }
}


void KClockSetup::slotOk()
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup settings(config, "Settings");
    settings.writeEntry("Size", _size);
    settings.writeEntry("KeepCentered", _keepCentered);
    settings.sync();

    KConfigGroup colors(config, "Colors");
    colors.writeEntry("Background", _bgndColor);
    colors.writeEntry("Scale", _scaleColor);
    colors.writeEntry("HourHand", _hourColor);
    colors.writeEntry("MinuteHand", _minColor);
    colors.writeEntry("SecondHand", _secColor);
    colors.sync();
    accept();
}


void KClockSetup::slotHelp()
{
    KMessageBox::about(this, QLatin1String("<qt>") + i18n(
            "Clock Screen Saver<br>"
            "Version 5.0<br>"
            "<nobr>Melchior FRANZ (c) 2003, 2006, 2007</nobr>") +
            QLatin1String("<br><a href=\"mailto:mfranz@kde.org\">mfranz@kde.org</a>"
            "<br>René J.V. Bertin (c) 2017</nobr>") +
            QLatin1String("<br><a href=\"mailto:rjvbertin@gmail.com\">rjvbertin@gmail.com</a>"
            "</qt>"), QString(), KMessageBox::AllowLink);
}


void KClockSetup::slotBgndColor(const QColor &color)
{
    _bgndColor = color;
    if (_saver) {
        _saver->setBgndColor(_bgndColor);
    }
}


void KClockSetup::slotScaleColor(const QColor &color)
{
    _scaleColor = color;
    if (_saver) {
        _saver->setScaleColor(_scaleColor);
    }
}


void KClockSetup::slotHourColor(const QColor &color)
{
    _hourColor = color;
    if (_saver) {
        _saver->setHourColor(_hourColor);
    }
}


void KClockSetup::slotMinColor(const QColor &color)
{
    _minColor = color;
    if (_saver) {
        _saver->setMinColor(_minColor);
    }
}


void KClockSetup::slotSecColor(const QColor &color)
{
    _secColor = color;
    if (_saver) {
        _saver->setSecColor(_secColor);
    }
}


void KClockSetup::slotSliderMoved(int v)
{
    if (_saver) {
        _saver->resizeClock(_size = v);
    }
}


void KClockSetup::slotKeepCenteredChanged(int c)
{
    if (_saver) {
        _saver->setKeepCentered(_keepCentered = c);
    }
}


//-----------------------------------------------------------------------------


ClockPainter::ClockPainter(QPaintDevice *device, int diameter) :
    QPainter(device)
{
    setRenderHint(QPainter::Antialiasing);
    translate(diameter / 2.0, diameter / 2.0);
    scale(diameter / 2000.0, -diameter / 2000.0);
    setPen(Qt::NoPen);
}


void ClockPainter::drawTick(double angle, double from, double to, double width, const QColor &color, bool shadow)
{
    save();
    rotate(90.0 - angle);

    if (shadow) {
        width += 1.0;
        setBrush(QColor(100, 100, 100));
    } else {
        setBrush(color);
    }
    drawRect(QRectF(from, -width / 2.0, to - from, width));
    restore();
}


void ClockPainter::drawDisc(double width, const QColor &color, bool shadow)
{
    if (shadow) {
        width += 1.0;
        setBrush(QColor(100, 100, 100));
    } else {
        setBrush(color);
    }
    drawEllipse(QRectF(-width, -width, 2.0 * width, 2.0 * width));
}


void ClockPainter::drawHand(double angle, double length, double width, const QColor &color, bool disc)
{
    if (disc) {
        drawDisc(width * 1.3, color, true);
    }
    drawTick(angle, 0.0, length, width, color, true);

    if (disc) {
        drawDisc(width * 1.3, color, false);
    }
    drawTick(angle, 0.0, length, width, color, false);

}


void ClockPainter::drawScale(const QColor &color)
{
    for (int i = 0; i < 360; i += 6) {
        if (i % 30) {
            drawTick(i, 920.0, 980.0, 15.0, color);
        } else {
            drawTick(i, 825.0, 980.0, 40.0, color);
        }
    }
}



//-----------------------------------------------------------------------------



KClockWidget::KClockWidget(QWidget *id, bool windowed)
    : KScreenSaver(id),
    _timer(this),
    _xstep(1),
    _ystep(-1),
    _hour(-1),
    _minute(-1),
    _second(-1)
{
    Q_UNUSED(windowed);
    if (id) {
        qWarning() << Q_FUNC_INFO << "using widget" << id;
    }
    init();
}

KClockWidget::KClockWidget(WId id, bool windowed)
    : KScreenSaver(id),
    _timer(this),
    _xstep(1),
    _ystep(-1),
    _hour(-1),
    _minute(-1),
    _second(-1)
{
    Q_UNUSED(windowed);
    init();
}

void KClockWidget::init()
{
    self->setAttribute(Qt::WA_NoSystemBackground);
    self->setMinimumSize(50, 50);
    readSettings();
    resizeClock(_size);

    QPalette p = self->palette();
    p.setColor(self->backgroundRole(), _bgndColor);
    self->setPalette(p);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    show();
    if (self != this) {
        self->show();
    }
}


void KClockWidget::readSettings()
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup settings(config, "Settings");
    _keepCentered = settings.readEntry("KeepCentered", DEFAULT_KEEP_CENTERED);
    _size = settings.readEntry("Size", DEFAULT_CLOCK_SIZE);
    if (_size > MAX_CLOCK_SIZE) {
        _size = MAX_CLOCK_SIZE;
    }

    KConfigGroup colors(config, "Colors");
    QColor c = Qt::black;
    setBgndColor(colors.readEntry("Background", c));

    c = Qt::white;
    setScaleColor(colors.readEntry("Scale", c));
    setHourColor(colors.readEntry("HourHand", c));
    setMinColor(colors.readEntry("MinuteHand", c));

    c = Qt::red;
    setSecColor(colors.readEntry("SecondHand", c));
}


void KClockWidget::setKeepCentered(bool b)
{
    _keepCentered = b;
    if (b) {
        _x = (width() - _diameter) / 2;
        _y = (height() - _diameter) / 2;
    }
    update();
    if (self != this) {
        self->update();
    }
}


void KClockWidget::resizeClock(int size)
{
    _size = size;
    _diameter = qMin(width(), height()) * (_size + 4) / 14;
    _x = (width() - _diameter) / 2;
    _y = (height() - _diameter) / 2;
    update();
    if (self != this) {
        self->update();
    }
}


void KClockWidget::resizeEvent(QResizeEvent *)
{
    resizeClock(_size);
}


void KClockWidget::showEvent(QShowEvent *)
{
    _second = -1;
    slotTimeout();
    _timer.start(TIMER_INTERVAL);
}


void KClockWidget::hideEvent(QHideEvent *)
{
    _timer.stop();
}


void KClockWidget::slotTimeout()
{
    QTime t = QTime::currentTime();
    int s = t.second();
    if (s == _second) {
        return;
    }

    _second = _secColor != _bgndColor ? s : 0;
    _hour = t.hour();
    _minute = t.minute();

    if (!_keepCentered) {
        int i;
        _x += _xstep;
        if (_x <= 0) {
            _x = 0, _xstep = 1;
        } else if (_x >= (i = width() - _diameter)) {
            _x = i, _xstep = -1;
        }

        _y += _ystep;
        if (_y <= 0) {
            _y = 0, _ystep = 1;
        } else if (_y >= (i = height() - _diameter)) {
            _y = i, _ystep = -1;
        }
    }
    update();
    if (self != this) {
        self->update();
    }
}


void KClockWidget::paintEvent(QPaintEvent *)
{
    double hour_angle = _hour * 30.0 + _minute * .5 + _second / 120.0;
    double minute_angle = _minute * 6.0 + _second * .1;
    double second_angle = _second * 6.0;

    QImage clock(_diameter, _diameter, QImage::Format_RGB32);
    ClockPainter c(&clock, _diameter);
    c.fillRect(-1000, -1000, 2000, 2000, _bgndColor);

    if (_scaleColor != _bgndColor) {
        c.drawScale(_scaleColor);
    }
    if (_hourColor != _bgndColor) {
        c.drawHand(hour_angle, 600.0, 55.0, _hourColor, false);
    }
    if (_minColor != _bgndColor) {
        c.drawHand(minute_angle, 900.0, 40.0, _minColor);
    }
    if (_secColor != _bgndColor) {
        c.drawHand(second_angle, 900.0, 30.0, _secColor);
    }

    QPainter p(self);
    p.drawImage(_x, _y, clock);
    p.eraseRect(0, 0, _x, height());                                        // left ver
    p.eraseRect(_x + _diameter, 0, width(), height());                      // right ver
    p.eraseRect(_x, 0, _diameter, _y);                                      // top hor
    p.eraseRect(_x, _y + _diameter, _diameter, height() - _y - _diameter);  // bottom hor
}

