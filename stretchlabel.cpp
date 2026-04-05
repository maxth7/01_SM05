#include "stretchlabel.h"

StretchLabel::StretchLabel(QWidget *parent)
    : QLabel(parent), movie(new QMovie(this))
{
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
void StretchLabel::setGif(const QString &filePath)
{
    QFileInfo checkFile(filePath);
    if (!checkFile.exists() || !checkFile.isFile()) {
        qDebug() << "Файл не существует или это не файл: " << filePath;
        return;
    }else{
//       qDebug() << "Файл существует и это  файл: " << filePath;
    }
     movie->setFileName(filePath);
    if (movie->isValid()) {
        setMovie(movie);
        movie->start();
    } else {
        qDebug() << "Ошибка загрузки GIF: " << filePath;
    }



}
// void StretchLabel::resizeEvent(QResizeEvent *event)
// { из-за этой процедуры размер окна растет по высоте!!!!!!!!!!!!!!!
//      // resizeEventCount++; // Увеличиваем счетчик при каждом вызове

//    // QLabel::resizeEvent(event); // Вызываем родительскую реализацию resizeEvent
//     if (movie && !movie->fileName().isEmpty()) {
//          qDebug() << "StretchLabel::resizeEvent";
//                movie->setScaledSize(this->size());
//     }
//     QLabel::resizeEvent(event); // Вызываем родительскую реализацию resizeEvent
// }

// void StretchLabel::paintEvent(QPaintEvent *event)
// {
//     if (movie) {
//     qDebug() << "void StretchLabel::paintEvent(QPaintEvent *event) { вошли в if (movie)";
//         QPainter painter(this);

//         painter.drawPixmap(rect(),
//                            movie->currentPixmap().scaled(
//                                size(),
//                                Qt::IgnoreAspectRatio,
//                                Qt::SmoothTransformation)
//                            );

//     }

//   // QLabel::paintEvent(event); // Вызываем родительскую реализацию resizeEvent
// }
void StretchLabel::paintEvent(QPaintEvent *event)
{
    if (movie) {
        QPixmap currentPixmap = movie->currentPixmap();
        if (!currentPixmap.isNull()) {
            QPainter painter(this);
            painter.drawPixmap(rect(),
                               currentPixmap.scaled(
                                   size(),
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation)
                               );
        } else {
            qDebug() << "Текущий pixmap пуст!";
        }
    }
}
