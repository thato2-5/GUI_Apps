/*
* File: mainwindow.cpp
* Author: Thato Monyamane
* Description: This is the main class for the music player. It can however only load a maximum of 10 files.
* Version: 1.00
* Date: 10/08/2023
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->songDurationLabel->setText("00:00");    // Initialize with a default value

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this); // Initialize the playlist

    // Set the playlist for the player
    player->setPlaylist(playlist);

    connect(ui->playPushButton, &QPushButton::clicked, this, &MainWindow::playClicked);
    connect(ui->pausePushButton, &QPushButton::clicked, this, &MainWindow::pauseClicked);
    connect(ui->stopPushButton, &QPushButton::clicked, this, &MainWindow::stopClicked);
    connect(ui->volUpPushButton, &QPushButton::clicked, this, &MainWindow::volumeUpClicked);
    connect(ui->volDwnPushButton, &QPushButton::clicked, this, &MainWindow::volumeDownClicked);
    connect(ui->addPushButton, &QPushButton::clicked, this, &MainWindow::addSongClicked);
    connect(ui->removePushButton, &QPushButton::clicked, this, &MainWindow::removeSongClicked);
    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &MainWindow::handleError);
    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::handlePlaybackStateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateDurationLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playClicked()
{
    if (player->isMuted())
        player->setMuted(false);    // Unmute before playing

    if (player->volume() == 0)
        player->setVolume(50);  // set a reasonable initial volume level

    if (playlist->isEmpty())
    {
        QMessageBox::critical(this, "Error", "Playlist is empty.");
        return;
    }

    QListWidgetItem *selectedItem = ui->playlist->currentItem();
    if (selectedItem)
    {
        QString filePath = selectedItem->text();
        QUrl mediaUrl = QUrl::fromLocalFile(filePath);
        if (mediaUrl.isValid())
        {
            player->setMedia(mediaUrl);
            if (player->state() == QMediaPlayer::PausedState)
                player->play();
            player->play();
            QMessageBox::information(this, "Success", "Playing: " + filePath);
        }
        else
        {
            QMessageBox::critical(this, "Error", "Invalid media source: " + filePath);
        }
    }
}

void MainWindow::pauseClicked()
{
    if (player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
    }
    QMessageBox::information(this, "Success", "Pause button clicked");
}

void MainWindow::stopClicked()
{
    QMessageBox::information(this, "Success", "Stop button clicked");
    if (player->state() != QMediaPlayer::StoppedState)
    {
        player->stop();
    }
}

void MainWindow::volumeUpClicked()
{
    int newVolume = qMin(player->volume() + 10, 100);   // Limit volume to 100
    player->setVolume(newVolume);
}

void MainWindow::volumeDownClicked()
{
    int newVolume = qMax(player->volume() - 10, 0); // Limit volume to 0
    player->setVolume(newVolume);
}

void MainWindow::addSongClicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Music", "", "Audio Files (*.mp3 *.ogg *.wav)");
    for (const QString &file : files)
    {
        QListWidgetItem *item = new QListWidgetItem(file);
        ui->playlist->addItem(item);
        playlist->addMedia(QUrl::fromLocalFile(file));  // Add the media to the playlist
        QMessageBox::information(this, "Success", "Added song " + file);
    }

}

void MainWindow::removeSongClicked()
{
    QList<QListWidgetItem *> selectedItems = ui->playlist->selectedItems();
    if (!selectedItems.isEmpty())
    {
        for (QListWidgetItem *item: selectedItems)
        {
            int index = ui->playlist->row(item);
            playlist->removeMedia(index);
            delete ui->playlist->takeItem(index);
        }
    }
}

void MainWindow::handleError(QMediaPlayer::Error error)
{
    switch (error)
    {
        case QMediaPlayer::NoError:
            break;
        case QMediaPlayer::ResourceError:
            QMessageBox::critical(this, "Error", "Resource error occurred.");
            break;
        case QMediaPlayer::FormatError:
            QMessageBox::critical(this, "Error", "Unsupported media format.");
            break;
        case QMediaPlayer::NetworkError:
            QMessageBox::critical(this, "Error", "Newtwork error occured.");
            break;
        case QMediaPlayer::AccessDeniedError:
            QMessageBox::critical(this, "Error", "Access denied.");
            break;
        case QMediaPlayer::ServiceMissingError:
            QMessageBox::critical(this, "Error", "Service missing error.");
            break;
        default:
            QMessageBox::critical(this, "Error", "An unknown error occured.");
            break;
    }
}

void MainWindow::handlePlaybackStateChanged(QMediaPlayer::State newState)
{
    switch (newState) {
    case QMediaPlayer::PlayingState:
        QMessageBox::information(this, "Playback State", "Playback started.");
        ui->playPushButton->setEnabled(false);  // Disable the play button
        ui->pausePushButton->setEnabled(true);   // Enable the play button
        ui->stopPushButton->setEnabled(true);   // Enable the stop button
        break;
    case QMediaPlayer::PausedState:
        QMessageBox::information(this, "Playback State", "Playback paused.");
        ui->playPushButton->setEnabled(true);  // Enable the play button
        ui->pausePushButton->setEnabled(false);   // Disable the play button
        ui->stopPushButton->setEnabled(true);   // Enable the stop button
        break;
    case QMediaPlayer::StoppedState:
        QMessageBox::information(this, "Playback State", "Playback stopped.");
        ui->playPushButton->setEnabled(true);  // Enable the play button
        ui->pausePushButton->setEnabled(false);   // Disable the play button
        ui->stopPushButton->setEnabled(false);   // Disable the stop button
        break;
    }
}

void MainWindow::updateDurationLabel(qint64 position)
{
    /*
     * Normal song count...
    QString durationStr = QString("%1:%2")
                            .arg(position / 60000, 2, 10, QChar('0')) // Minutes
                            .arg((position / 1000) % 60, 2, 10, QChar('0')); // Seconds

    ui->songDurationLabel->setText(durationStr);
    */
    qint64 duration = player->duration();   // Total duration of the song in milliseconds
    qint64 remainingTime = duration - position;     // Remaining time in milliseconds
    QString remainingTimeStr = QString("-%1:%2")
                                   .arg(remainingTime / 60000, 2, 10, QChar('0')) // Minutes
                                   .arg((remainingTime / 1000) % 60, 2, 10, QChar('0')); // Seconds

    ui->songDurationLabel->setText(remainingTimeStr);
}
