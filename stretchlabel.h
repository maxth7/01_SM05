#ifndef STRETCHLABEL_H
#define STRETCHLABEL_H
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QFileInfo>

class StretchLabel : public QLabel
{
    Q_OBJECT
public:
    explicit StretchLabel(QWidget *parent = nullptr);
    void setGif(const QString &filePath);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMovie *movie;
};
#endif // STRETCHLABEL_H
