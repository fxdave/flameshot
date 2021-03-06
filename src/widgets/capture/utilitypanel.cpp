// Copyright(c) 2017-2018 Alejandro Sirgo Rica & Contributors
//
// This file is part of Flameshot.
//
//     Flameshot is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     Flameshot is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with Flameshot.  If not, see <http://www.gnu.org/licenses/>.

// Based on Lightscreen areadialog.h, Copyright 2017  Christian Kaiser <info@ckaiser.com.ar>
// released under the GNU GPL2  <https://www.gnu.org/licenses/gpl-2.0.txt>

// Based on KDE's KSnapshot regiongrabber.cpp, revision 796531, Copyright 2007 Luca Gugelmann <lucag@student.ethz.ch>
// released under the GNU LGPL  <http://www.gnu.org/licenses/old-licenses/library.txt>

#include "utilitypanel.h"
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QTimer>
#include <QScrollArea>
#include <QWheelEvent>

UtilityPanel::UtilityPanel(QWidget *parent) : QWidget(parent) {
    initInternalPanel();
    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_showAnimation = new QPropertyAnimation(m_internalPanel, "geometry", this);
    m_showAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    m_showAnimation->setDuration(300);

    m_hideAnimation = new QPropertyAnimation(m_internalPanel, "geometry", this);
    m_hideAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    m_hideAnimation->setDuration(300);

    connect(m_hideAnimation, &QPropertyAnimation::finished,
            m_internalPanel, &QWidget::hide);
}

void UtilityPanel::addToolWidget(QWidget *w) {
    if (m_toolWidget) {
        m_toolWidget->deleteLater();
        m_toolWidget = w;
        m_upLayout->addWidget(w);
    }
}

void UtilityPanel::pushWidget(QWidget *w) {
    m_layout->addWidget(w);
}

void UtilityPanel::toggle() {
    if (m_internalPanel->isHidden()) {
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_showAnimation->setStartValue(QRect(-width(), 0, 0, height()));
        m_showAnimation->setEndValue(QRect(0, 0, width(), height()));
        m_internalPanel->show();
        m_showAnimation->start();
    } else {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        m_hideAnimation->setStartValue(QRect(0, 0, width(), height()));
        m_hideAnimation->setEndValue(QRect(-width(), 0, 0, height()));
        m_hideAnimation->start();
    }
}

void UtilityPanel::initInternalPanel() {
    m_internalPanel = new QScrollArea(this);
    m_internalPanel->setAttribute(Qt::WA_NoMousePropagation);
    QWidget *widget = new QWidget();
    m_internalPanel->setWidget(widget);
    m_internalPanel->setWidgetResizable(true);

    m_layout = new QVBoxLayout();
    m_upLayout = new QVBoxLayout();
    m_layout->addLayout(m_upLayout);
    widget->setLayout(m_layout);

    QColor bgColor = palette().background().color();
    bgColor.setAlphaF(0.0);
    m_internalPanel->setStyleSheet(QString("QScrollArea {background-color: %1}")
                                   .arg(bgColor.name()));
    m_internalPanel->hide();
}
