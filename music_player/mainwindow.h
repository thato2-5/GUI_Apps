#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void playClicked();
    void pauseClicked();
    void stopClicked();
    void volumeUpClicked();
    void volumeDownClicked();
    void addSongClicked();
    void removeSongClicked();
    void handleError(QMediaPlayer::Error error);
    void handlePlaybackStateChanged(QMediaPlayer::State newState);
    void updateDurationLabel(qint64 position);
private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
};
#endif // MAINWINDOW_H
