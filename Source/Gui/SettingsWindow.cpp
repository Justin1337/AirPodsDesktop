//
// AirPodsDesktop - AirPods Desktop User Experience Enhancement Program.
// Copyright (C) 2021 SpriteOvO
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include "SettingsWindow.h"

#include <QPushButton>
#include <QCheckBox>
#include <QToolTip>
#include <QLabel>


using namespace std::chrono_literals;

namespace Gui
{
    class TipLabel : public QLabel
    {
        Q_OBJECT

    private:
        constexpr static auto content = "(?)";

    public:
        TipLabel(QString text, QWidget *parent) :
            QLabel{content, parent},
            _text{std::move(text)}
        {
            QPalette palette = this->palette();
            palette.setColor(QPalette::WindowText, Qt::darkGray);
            setPalette(palette);
        }

    private:
        QString _text;

        void enterEvent(QEvent *event) override
        {
            QToolTip::showText(QCursor::pos(), _text, this);
        }

        void leaveEvent(QEvent *event) override
        {
            QToolTip::hideText();
        }
    };


    SettingsWindow::SettingsWindow(QWidget *parent) :
        QDialog{parent}
    {
        _ui.setupUi(this);

        InitUi();

        LoadCurrent();

        connect(_ui.buttonBox, &QDialogButtonBox::accepted, this, &SettingsWindow::Save);
        connect(_ui.buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked,
            this, &SettingsWindow::LoadDefault
        );

        connect(_checkBoxAutoRun, &QCheckBox::toggled, this,
            [this](bool checked) { _data.auto_run = checked; }
        );

        connect(_checkBoxLowAudioLatency, &QCheckBox::toggled, this,
            [this](bool checked) { _data.low_audio_latency = checked; }
        );

        connect(_checkBoxAutomaticEarDetection, &QCheckBox::toggled, this,
            [this](bool checked) { _data.automatic_ear_detection = checked; }
        );
    }

    void SettingsWindow::InitUi()
    {
        _checkBoxAutoRun = new QCheckBox{tr("Launch when system starts"), this};
        _checkBoxLowAudioLatency = new QCheckBox{tr("Low audio latency mode"), this};
        _checkBoxAutomaticEarDetection = new QCheckBox{tr("Automatic ear detection"), this};

        _ui.gridLayout->addWidget(_checkBoxAutoRun, 0, 0);

        _ui.gridLayout->addWidget(_checkBoxLowAudioLatency, 1, 0);
        _ui.gridLayout->addWidget(
            new TipLabel{
                tr("It improves the problem of short audio not playing, but may increase battery consumption."),
                this
            }, 1, 1, Qt::AlignLeft
        );

        _ui.gridLayout->addWidget(_checkBoxAutomaticEarDetection, 2, 0);
        _ui.gridLayout->addWidget(
            new TipLabel{
                tr("Pause the media when you remove the AirPods and play it when both AirPods are put back on."),
                this
            }, 2, 1, Qt::AlignLeft
        );
    }

    void SettingsWindow::LoadCurrent()
    {
        _data = Core::Settings::GetCurrent();
        Update();
    }

    void SettingsWindow::LoadDefault()
    {
        _data = Core::Settings::GetDefault();
        Update();
    }

    void SettingsWindow::Save()
    {
        Core::Settings::SaveToCurrentAndLocal(_data);
    }

    void SettingsWindow::Update()
    {
        _checkBoxAutoRun->setChecked(_data.auto_run);
        _checkBoxLowAudioLatency->setChecked(_data.low_audio_latency);
        _checkBoxAutomaticEarDetection->setChecked(_data.automatic_ear_detection);
    }

    void SettingsWindow::showEvent(QShowEvent *event)
    {
        LoadCurrent();
    }

} // namespace Gui

#include "SettingsWindow.moc"