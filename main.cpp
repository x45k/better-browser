#include "mainwindow.h"
#include <QIcon>
#include <QDir>
#include <QApplication>
#include <QPainter>
#include <QBitmap>
#include <QImage>
#include <QPainterPath>

// if your wondering why i did all this just to round an image, rather than pre-rounding it, i cant tell you

QPixmap roundImage(const QPixmap &pixmap, int radius)
{
    if (pixmap.isNull()) {
        return QPixmap();
    }

    QPixmap rounded(pixmap.size());
    rounded.fill(Qt::transparent);

    QPainter painter(&rounded);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRect rect = QRect(0, 0, pixmap.width(), pixmap.height());
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);

    painter.setClipPath(path);
    painter.drawPixmap(rect, pixmap);

    return rounded;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QString appDir = QCoreApplication::applicationDirPath();
    QDir appBundleDir(appDir);
    appBundleDir.cdUp();
    appBundleDir.cdUp();
    appBundleDir.cdUp();
    appBundleDir.cdUp();
    appBundleDir.cdUp();

    QString logoPath = appBundleDir.path() + "/resources/logo.png";
    QPixmap originalPixmap(logoPath);

    if (!originalPixmap.isNull()) {
        QPixmap roundedPixmap = roundImage(originalPixmap, 20);
        a.setWindowIcon(QIcon(roundedPixmap));
    } else {
        qWarning() << "Failed to load logo image from:" << logoPath;
    }

    return a.exec();
}
