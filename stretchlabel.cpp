#include "stretchlabel.h"
StretchLabel::StretchLabel(QWidget *parent)
    : QLabel(parent), movie(new QMovie(this))
{
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void StretchLabel::setGif(const QString &filePath)
{
    const QFileInfo checkFile(filePath);

    if (!checkFile.exists() || !checkFile.isFile()) {
        qDebug() << "The file does not exist or is not a file:" << filePath;
        return;
    }

    // Stop the old animation before loading the new one
    if (movie->state() == QMovie::Running) {
        movie->stop();
    }

    movie->setFileName(filePath);

    if (movie->isValid()) {
        setMovie(movie);
        movie->start();
        qDebug() << "GIF uploaded successfully:" << filePath;
    } else {
        qDebug() << "Error loading GIF:" << filePath;
    }
}

void StretchLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!movie) {
        return;
    }

    const QPixmap currentPixmap = movie->currentPixmap();

    if (currentPixmap.isNull()) {
        // We don't display the message every time, only when debugging.
        // qDebug() << "The current pixmap is empty";
        return;
    }

    QPainter painter(this);
    painter.drawPixmap(rect(), currentPixmap.scaled(
                                   size(),
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation
                                   ));
}
