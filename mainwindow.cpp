#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QtMath>
#include <QMouseEvent>
#include <QEvent>
#include <QElapsedTimer>
#include <QSlider>
#include <QPushButton>
#include <QSize>
#include <QShortcut>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblOriginal->setScaledContents(false);
    ui->lblProcessed->setScaledContents(false);

    rubberBand = new QRubberBand(QRubberBand::Rectangle, ui->lblProcessed);
    ui->lblProcessed->installEventFilter(this);

    connect(ui->sliderBrightness, &QSlider::valueChanged, this, &MainWindow::on_sliderBrightness_valueChanged);
    connect(ui->sliderBrightness, &QSlider::sliderPressed, this, &MainWindow::on_sliderBrightness_sliderPressed);
    connect(ui->sliderBrightness, &QSlider::sliderReleased, this, &MainWindow::on_sliderBrightness_sliderReleased);

    QSlider *blurSlider = ui->centralwidget->findChild<QSlider*>("sliderBlur");
    if (blurSlider) {
        connect(blurSlider, &QSlider::valueChanged, this, &MainWindow::on_sliderBlur_valueChanged);
        connect(blurSlider, &QSlider::sliderPressed, this, &MainWindow::on_sliderBlur_sliderPressed);
        connect(blurSlider, &QSlider::sliderReleased, this, &MainWindow::on_sliderBlur_sliderReleased);
    }

    QSize btnIconSize(20, 20);
    if (ui->btnGridFrame) {
        for (QPushButton *b : ui->btnGridFrame->findChildren<QPushButton*>())
            b->setIconSize(btnIconSize);
    }

    if (ui->btnLoad) ui->btnLoad->setIconSize(btnIconSize);
    if (ui->btnSave) ui->btnSave->setIconSize(btnIconSize);
    if (ui->btnUndo) ui->btnUndo->setIconSize(btnIconSize);
    if (ui->btnClear) ui->btnClear->setIconSize(btnIconSize);

    QShortcut *sc = new QShortcut(QKeySequence("Ctrl+M"), this);
    connect(sc, &QShortcut::activated, this, &MainWindow::on_btnMerge_clicked);

    srand(static_cast<unsigned>(time(nullptr)));
    updateViews();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateViews();
}

void MainWindow::pushUndo()
{
    if (!processed.isNull())
        undoStack.push(processed);
}

void MainWindow::updateViews()
{
    if (!original.isNull()) {
        QPixmap p = QPixmap::fromImage(original);
        ui->lblOriginal->setPixmap(p.scaled(ui->lblOriginal->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->lblOriginal->setPixmap(QPixmap());
        ui->lblOriginal->setText("Original Image");
    }

    if (!processed.isNull()) {
        QPixmap p = QPixmap::fromImage(processed);
        ui->lblProcessed->setPixmap(p.scaled(ui->lblProcessed->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->lblProcessed->setPixmap(QPixmap());
        ui->lblProcessed->setText("Processed Image");
    }

    if (QLabel *lbl = ui->centralwidget->findChild<QLabel*>("lblBlurValue")) {
        QSlider *s = ui->centralwidget->findChild<QSlider*>("sliderBlur");
        if (s) lbl->setText(QString::number(s->value()));
    }
}

void MainWindow::on_btnLoad_clicked()
{
    QString f = QFileDialog::getOpenFileName(this, "Open image", QString(), "Images (*.png *.jpg *.jpeg *.bmp *.tga)");
    if (f.isEmpty()) return;

    QImage img;
    img.load(f);
    if (img.isNull()) {
        QMessageBox::warning(this, "Error", "Cannot load image");
        return;
    }

    original = img.convertToFormat(QImage::Format_RGB888);
    processed = original;
    undoStack.clear();

    ui->sliderBrightness->setValue(100);
    ui->lblSliderValue->setText("100%");
    if (QSlider *s = ui->centralwidget->findChild<QSlider*>("sliderBlur")) s->setValue(0);
    updateViews();
}

void MainWindow::on_btnSave_clicked()
{
    if (processed.isNull()) {
        QMessageBox::information(this, "No image", "No processed image to save");
        return;
    }

    QString f = QFileDialog::getSaveFileName(this, "Save image", QString(), "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    if (f.isEmpty()) return;
    processed.save(f);
}

void MainWindow::on_btnUndo_clicked()
{
    if (!undoStack.isEmpty()) {
        processed = undoStack.pop();
        updateViews();
    }
}

void MainWindow::on_btnClear_clicked()
{
    if (original.isNull()) return;
    processed = original;
    undoStack.clear();
    ui->sliderBrightness->setValue(100);
    ui->lblSliderValue->setText("100%");
    if (QSlider *s = ui->centralwidget->findChild<QSlider*>("sliderBlur")) s->setValue(0);
    updateViews();
}

void MainWindow::on_btnGrey_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = toGrey(processed);
    updateViews();
}

void MainWindow::on_btnBW_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = toBW(processed);
    updateViews();
}

void MainWindow::on_btnInvert_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = invertImg(processed);
    updateViews();
}

void MainWindow::on_btnPurple_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = purpleImg(processed);
    updateViews();
}

void MainWindow::on_btnEdges_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = edgesImg(processed);
    updateViews();
}

void MainWindow::on_btnRotate90_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = rotateImg(processed, 1);
    updateViews();
}

void MainWindow::on_btnRotate180_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = rotateImg(processed, 2);
    updateViews();
}

void MainWindow::on_btnRotate270_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = rotateImg(processed, 3);
    updateViews();
}

void MainWindow::on_btnBrightLight_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int p = QInputDialog::getInt(this, "Brightness", "Enter percentage:", 20, -100, 500, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = brightnessImg(processed, 1 + p / 100.0);
    updateViews();
    ui->sliderBrightness->setValue(int((1 + p / 100.0) * 100));
    ui->lblSliderValue->setText(QString::number(ui->sliderBrightness->value()) + "%");
}

void MainWindow::on_btnBrightDark_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int p = QInputDialog::getInt(this, "Brightness", "Enter percentage:", 20, -100, 500, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = brightnessImg(processed, 1 - p / 100.0);
    updateViews();
    ui->sliderBrightness->setValue(int((1 - p / 100.0) * 100));
    ui->lblSliderValue->setText(QString::number(ui->sliderBrightness->value()) + "%");
}

void MainWindow::on_btnFlipH_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = flipImg(processed, true);
    updateViews();
}

void MainWindow::on_btnFlipV_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = flipImg(processed, false);
    updateViews();
}

void MainWindow::on_btnCrop_clicked()
{
    if (processed.isNull()) return;
    if (selectionRectImage.isNull() || selectionRectImage.width() == 0 || selectionRectImage.height() == 0) {
        QMessageBox::information(this, "Crop", "Please select an area on the processed image by dragging the mouse.");
        return;
    }
    pushUndo();
    processed = cropImg(processed,
                        selectionRectImage.x(),
                        selectionRectImage.y(),
                        selectionRectImage.width(),
                        selectionRectImage.height());
    selectionRectImage = QRect();
    rubberBand->hide();
    updateViews();
}

void MainWindow::on_btnResize_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int w = QInputDialog::getInt(this, "Resize", "New Width:", processed.width(), 1, 10000, 1, &ok);
    if (!ok) return;
    int h = QInputDialog::getInt(this, "Resize", "New Height:", processed.height(), 1, 10000, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = resizeImg(processed, w, h);
    updateViews();
}

static QImage scaleIfNeeded(const QImage &img, int w, int h)
{
    if (img.width() == w && img.height() == h) return img;
    return img.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGB888);
}

void MainWindow::on_btnMerge_clicked()
{
    if (processed.isNull()) {
        QMessageBox::information(this, "Merge", "Load an image first");
        return;
    }

    QString f = QFileDialog::getOpenFileName(this, "Enter the image you want to merge", QString(), "Images (*.png *.jpg *.jpeg *.bmp *.tga)");
    if (f.isEmpty()) return;

    QImage img2;
    img2.load(f);
    if (img2.isNull()) {
        QMessageBox::warning(this, "Error", "Cannot load image");
        return;
    }

    QImage img1 = processed.convertToFormat(QImage::Format_RGB888);
    img2 = img2.convertToFormat(QImage::Format_RGB888);

    if (img1.width() != img2.width() || img1.height() != img2.height()) {
        bool ok;
        int choice = QInputDialog::getInt(this,
                                          "Resize / Merge options",
                                          "Images differ in size. Choose:\n1) Resize image1 to image2\n2) Resize image2 to image1\n3) Use common center-cropped pixels (output = min width x min height)",
                                          3, 1, 3, 1, &ok);
        if (!ok) return;

        if (choice == 1) {
            pushUndo();
            processed = resizeImg(processed, img2.width(), img2.height());
            img1 = processed.convertToFormat(QImage::Format_RGB888);
        } else if (choice == 2) {
            img2 = img2.scaled(img1.width(), img1.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGB888);
        } else {
            int commonW = qMin(img1.width(), img2.width());
            int commonH = qMin(img1.height(), img2.height());
            if (commonW <= 0 || commonH <= 0) {
                QMessageBox::warning(this, "Merge", "No overlapping area between images");
                return;
            }
            auto centerCrop = [](const QImage &src, int w, int h) -> QImage {
                int sx = qMax(0, (src.width() - w) / 2);
                int sy = qMax(0, (src.height() - h) / 2);
                return src.copy(sx, sy, w, h).convertToFormat(QImage::Format_RGB888);
            };
            QImage c1 = centerCrop(img1, commonW, commonH);
            QImage c2 = centerCrop(img2, commonW, commonH);
            pushUndo();
            QImage ans(commonW, commonH, QImage::Format_RGB888);
            for (int y = 0; y < commonH; ++y) {
                const uchar *row1 = c1.scanLine(y);
                const uchar *row2 = c2.scanLine(y);
                uchar *rowAns = ans.scanLine(y);
                for (int x = 0; x < commonW; ++x) {
                    int idx = x * 3;
                    int r = (int(row1[idx]) + int(row2[idx])) / 2;
                    int g = (int(row1[idx + 1]) + int(row2[idx + 1])) / 2;
                    int b = (int(row1[idx + 2]) + int(row2[idx + 2])) / 2;
                    rowAns[idx] = uchar(r);
                    rowAns[idx + 1] = uchar(g);
                    rowAns[idx + 2] = uchar(b);
                }
            }
            processed = ans;
            mergeMode = false;
            updateViews();
            return;
        }
    }

    img1 = processed.convertToFormat(QImage::Format_RGB888);
    img2 = img2.convertToFormat(QImage::Format_RGB888);
    int w = img1.width();
    int h = img1.height();
    if (img2.width() != w || img2.height() != h) {
        img2 = img2.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGB888);
    }
    pushUndo();
    QImage ans(w, h, QImage::Format_RGB888);
    for (int y = 0; y < h; ++y) {
        const uchar *row1 = img1.scanLine(y);
        const uchar *row2 = img2.scanLine(y);
        uchar *rowAns = ans.scanLine(y);
        for (int x = 0; x < w; ++x) {
            int idx = x * 3;
            int r = (int(row1[idx]) + int(row2[idx])) / 2;
            int g = (int(row1[idx + 1]) + int(row2[idx + 1])) / 2;
            int b = (int(row1[idx + 2]) + int(row2[idx + 2])) / 2;
            rowAns[idx] = uchar(r);
            rowAns[idx + 1] = uchar(g);
            rowAns[idx + 2] = uchar(b);
        }
    }
    processed = ans;
    mergeMode = false;
    updateViews();
}

static QImage fastBoxBlur(const QImage &src, int radius, int iterations = 1)
{
    if (src.isNull()) return src;
    if (radius <= 0) return src;

    int w = src.width(), h = src.height();
    QImage tmp(w, h, QImage::Format_RGB888);
    tmp.fill(qRgb(0, 0, 0));
    QImage dst = src.convertToFormat(QImage::Format_RGB888);

    auto boxBlurHorizontal = [&](const QImage &in, QImage &out, int r) {
        int width = in.width(), height = in.height();
        for (int y = 0; y < height; ++y) {
            unsigned long long sumR = 0, sumG = 0, sumB = 0;
            int left = 0, right = -1;
            int wl = 0;
            for (int x = 0; x < width; ++x) {
                while (right < std::min(width - 1, x + r)) {
                    ++right;
                    QRgb p = in.pixel(right, y);
                    sumR += qRed(p);
                    sumG += qGreen(p);
                    sumB += qBlue(p);
                    ++wl;
                }
                while (left < std::max(0, x - r)) {
                    QRgb p = in.pixel(left, y);
                    sumR -= qRed(p);
                    sumG -= qGreen(p);
                    sumB -= qBlue(p);
                    ++left;
                    --wl;
                }
                if (wl > 0) {
                    out.setPixel(x, y, qRgb(int(sumR / wl), int(sumG / wl), int(sumB / wl)));
                } else {
                    out.setPixel(x, y, in.pixel(x, y));
                }
            }
        }
    };

    auto boxBlurVertical = [&](const QImage &in, QImage &out, int r) {
        int width = in.width(), height = in.height();
        for (int x = 0; x < width; ++x) {
            unsigned long long sumR = 0, sumG = 0, sumB = 0;
            int top = 0, bottom = -1;
            int wl = 0;
            for (int y = 0; y < height; ++y) {
                while (bottom < std::min(height - 1, y + r)) {
                    ++bottom;
                    QRgb p = in.pixel(x, bottom);
                    sumR += qRed(p);
                    sumG += qGreen(p);
                    sumB += qBlue(p);
                    ++wl;
                }
                while (top < std::max(0, y - r)) {
                    QRgb p = in.pixel(x, top);
                    sumR -= qRed(p);
                    sumG -= qGreen(p);
                    sumB -= qBlue(p);
                    ++top;
                    --wl;
                }
                if (wl > 0) {
                    out.setPixel(x, y, qRgb(int(sumR / wl), int(sumG / wl), int(sumB / wl)));
                } else {
                    out.setPixel(x, y, in.pixel(x, y));
                }
            }
        }
    };

    for (int it = 0; it < iterations; ++it) {
        boxBlurHorizontal(dst, tmp, radius);
        boxBlurVertical(tmp, dst, radius);
    }

    return dst;
}

void MainWindow::on_sliderBrightness_sliderPressed()
{
    sliderBase = processed;
}

void MainWindow::on_sliderBrightness_valueChanged(int v)
{
    if (sliderBase.isNull()) return;
    double factor = v / 100.0;
    processed = brightnessImg(sliderBase, factor);
    if (QLabel *lbl = ui->centralwidget->findChild<QLabel*>("lblSliderValue")) {
        lbl->setText(QString::number(v) + "%");
    }
    updateViews();
}

void MainWindow::on_sliderBrightness_sliderReleased()
{
    pushUndo();
    sliderBase = QImage();
}

QImage MainWindow::blurImg(const QImage &in, int iterations)
{
    int val = iterations;
    int radius = qBound(0, val / 8, 12);
    int iters = (val > 60) ? 2 : 1;
    return fastBoxBlur(in, radius, iters);
}

void MainWindow::on_btnBlurWeak_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = blurImg(processed, 16);
    updateViews();
}

void MainWindow::on_btnBlurMed_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = blurImg(processed, 32);
    updateViews();
}

void MainWindow::on_btnBlurStrong_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = blurImg(processed, 64);
    updateViews();
}

void MainWindow::on_sliderBlur_sliderPressed()
{
    sliderBase = processed;
}

void MainWindow::on_sliderBlur_valueChanged(int v)
{
    if (sliderBase.isNull()) return;
    processed = blurImg(sliderBase, v);
    if (QLabel *lbl = ui->centralwidget->findChild<QLabel*>("lblBlurValue"))
        lbl->setText(QString::number(v));
    updateViews();
}

void MainWindow::on_sliderBlur_sliderReleased()
{
    pushUndo();
    sliderBase = QImage();
}

void MainWindow::on_btnInfrared_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = infraredImg(processed);
    updateViews();
}

void MainWindow::on_btnSunlight_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = sunlightImg(processed);
    updateViews();
}

void MainWindow::on_btnFrameSimple_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = frameImg(processed, 1, 40);
    updateViews();
}

void MainWindow::on_btnFrameDouble_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = frameImg(processed, 2, 50);
    updateViews();
}

void MainWindow::on_btnFrameFancy_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = frameImg(processed, 3, 45);
    updateViews();
}

void MainWindow::on_btnSkew_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int deg = QInputDialog::getInt(this, "Skew", "Degree (-45 .. 45):", 10, -45, 45, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = skewImg(processed, deg);
    updateViews();
}

void MainWindow::on_btnFourFilters_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = fourFiltersImg(processed);
    updateViews();
}

void MainWindow::on_btnOldTV_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = oldTVImg(processed);
    updateViews();
}

void MainWindow::on_btnOilPainting_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int r = QInputDialog::getInt(this, "Oil Painting", "Radius (1..5):", 2, 1, 10, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = oilPaintingImg(processed, r);
    updateViews();
}

void MainWindow::on_btnAnime_clicked()
{
    if (processed.isNull()) return;
    pushUndo();
    processed = animeImg(processed);
    updateViews();
}

void MainWindow::on_btnWarmCold_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int choice = QInputDialog::getInt(this, "Warm/Cold", "Choice 1=Warm 2=Cold:", 1, 1, 2, 1, &ok);
    if (!ok) return;
    int str = QInputDialog::getInt(this, "Strength", "1=light 2=medium 3=strong:", 2, 1, 3, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = warmColdImg(processed, choice, str);
    updateViews();
}

void MainWindow::on_btnWarm_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int str = QInputDialog::getInt(this, "Warm", "Strength 1=light 2=medium 3=strong:", 2, 1, 3, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = warmColdImg(processed, 1, str);
    updateViews();
}

void MainWindow::on_btnCold_clicked()
{
    if (processed.isNull()) return;
    bool ok;
    int str = QInputDialog::getInt(this, "Cold", "Strength 1=light 2=medium 3=strong:", 2, 1, 3, 1, &ok);
    if (!ok) return;
    pushUndo();
    processed = warmColdImg(processed, 2, str);
    updateViews();
}

QImage MainWindow::toGrey(const QImage &in)
{
    QImage out = in.convertToFormat(QImage::Format_RGB888);
    for (int x = 0; x < out.width(); x++) {
        for (int y = 0; y < out.height(); y++) {
            QRgb p = out.pixel(x, y);
            int a = (qRed(p) + qGreen(p) + qBlue(p)) / 3;
            out.setPixel(x, y, qRgb(a, a, a));
        }
    }
    return out;
}

QImage MainWindow::toBW(const QImage &in)
{
    QImage out = toGrey(in);
    for (int x = 0; x < out.width(); x++) {
        for (int y = 0; y < out.height(); y++) {
            int v = qRed(out.pixel(x, y)) > 127 ? 255 : 0;
            out.setPixel(x, y, qRgb(v, v, v));
        }
    }
    return out;
}

QImage MainWindow::invertImg(const QImage &in)
{
    QImage out = in.convertToFormat(QImage::Format_RGB888);
    for (int x = 0; x < out.width(); x++)
        for (int y = 0; y < out.height(); y++) {
            QRgb p = out.pixel(x, y);
            out.setPixel(x, y, qRgb(255 - qRed(p), 255 - qGreen(p), 255 - qBlue(p)));
        }
    return out;
}

QImage MainWindow::purpleImg(const QImage &in)
{
    QImage out = in.convertToFormat(QImage::Format_RGB888);
    for (int x = 0; x < out.width(); x++)
        for (int y = 0; y < out.height(); y++) {
            int r = qRed(out.pixel(x, y)), g = qGreen(out.pixel(x, y)), b = qBlue(out.pixel(x, y));
            r = qMin(255, r + 50);
            g = qMax(0, g - 50);
            b = qMin(255, b + 50);
            out.setPixel(x, y, qRgb(r, g, b));
        }
    return out;
}

QImage MainWindow::edgesImg(const QImage &in)
{
    QImage out = in.convertToFormat(QImage::Format_RGB888);
    QImage tmp = out;
    for (int x = 1; x < out.width() - 1; x++)
        for (int y = 1; y < out.height() - 1; y++) {
            int contx = 0, conty = 0;
            QRgb p1 = tmp.pixel(x + 1, y), p2 = tmp.pixel(x - 1, y), p3 = tmp.pixel(x, y + 1), p4 = tmp.pixel(x, y - 1);
            contx = qAbs(qRed(p1) - qRed(p2));
            conty = qAbs(qRed(p3) - qRed(p4));
            int cont = qMin(255, contx + conty);
            cont = 255 - cont;
            out.setPixel(x, y, qRgb(cont, cont, cont));
        }
    return out;
}

QImage MainWindow::rotateImg(const QImage &in, int times90)
{
    QImage cur = in;
    times90 = (times90 % 4 + 4) % 4;
    for (int t = 0; t < times90; t++) {
        QImage r(cur.height(), cur.width(), QImage::Format_RGB888);
        for (int i = 0; i < cur.width(); i++)
            for (int j = 0; j < cur.height(); j++)
                r.setPixel(j, cur.width() - 1 - i, cur.pixel(i, j));
        cur = r;
    }
    return cur;
}

QImage MainWindow::brightnessImg(const QImage &in, double factor)
{
    QImage out = in.convertToFormat(QImage::Format_RGB888);
    for (int x = 0; x < out.width(); x++)
        for (int y = 0; y < out.height(); y++) {
            int r = qBound(0, int(qRed(out.pixel(x, y)) * factor), 255);
            int g = qBound(0, int(qGreen(out.pixel(x, y)) * factor), 255);
            int b = qBound(0, int(qBlue(out.pixel(x, y)) * factor), 255);
            out.setPixel(x, y, qRgb(r, g, b));
        }
    return out;
}

QImage MainWindow::flipImg(const QImage &in, bool horizontal)
{
    if (horizontal) return in.flipped(Qt::Horizontal);
    else return in.flipped(Qt::Vertical);
}

QImage MainWindow::resizeImg(const QImage &in, int w, int h)
{
    return in.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGB888);
}

QImage MainWindow::cropImg(const QImage &in, int x, int y, int w, int h)
{
    return in.copy(x, y, w, h).convertToFormat(QImage::Format_RGB888);
}

QImage MainWindow::infraredImg(const QImage &in)
{
    QImage out = in.convertToFormat(QImage::Format_RGB888);
    for (int x = 0; x < out.width(); x++)
        for (int y = 0; y < out.height(); y++) {
            int r = qRed(out.pixel(x, y)), g = qGreen(out.pixel(x, y)), b = qBlue(out.pixel(x, y));
            int red = qMin(255, int(r * 1.25 + g * 0.31 + b * 0.31));
            int green = int(g * 0.11);
            int blue = int(b * 0.11);
            out.setPixel(x, y, qRgb(red, green, blue));
        }
    return out;
}

QImage MainWindow::sunlightImg(const QImage &in)
{
    QImage tmp = in.convertToFormat(QImage::Format_RGB888);
    for (int x = 0; x < tmp.width(); x++)
        for (int y = 0; y < tmp.height(); y++) {
            int r = qRed(tmp.pixel(x, y)), g = qGreen(tmp.pixel(x, y)), b = qBlue(tmp.pixel(x, y));
            int newR = qBound(0, int(1.1 * r + 10), 255);
            int newG = qBound(0, int(1.05 * g + 5), 255);
            int newB = qBound(0, int(0.95 * b), 255);
            tmp.setPixel(x, y, qRgb(newR, newG, newB));
        }
    return tmp;
}

QImage MainWindow::frameImg(const QImage &in, int style, int thickness)
{
    QImage img = in.convertToFormat(QImage::Format_RGB888);
    int nWidth = int(img.width() + 1.5 * thickness);
    int nHeight = int(img.height() + 1.5 * thickness);
    QImage framed(nWidth, nHeight, QImage::Format_RGB888);
    framed.fill(qRgb(0, 0, 0));
    for (int i = 0; i < nWidth; i++) {
        for (int j = 0; j < nHeight; j++) {
            bool inside = (i >= thickness && i < img.width() + thickness && j >= thickness && j < img.height() + thickness);
            if (inside) {
                QRgb p = img.pixel(i - thickness, j - thickness);
                framed.setPixel(i, j, p);
            } else {
                if (style == 1) {
                    framed.setPixel(i, j, qRgb(0, 0, 250));
                } else if (style == 2) {
                    framed.setPixel(i, j, qRgb(245, 0, 0));
                } else {
                    if ((i + j) % 20 < 10) framed.setPixel(i, j, qRgb(200, 200, 200));
                    else framed.setPixel(i, j, qRgb(15, 15, 15));
                }
            }
        }
    }
    return framed;
}

QImage MainWindow::skewImg(const QImage &in, int degree)
{
    double radian = degree * M_PI / 180.0;
    double factor = tan(radian);
    int newW = in.width();
    int newH = in.height() + int(in.width() * fabs(factor));
    QImage image(newW, newH, QImage::Format_RGB888);
    image.fill(qRgb(0, 0, 0));
    for (int i = 0; i < newW; i++) {
        for (int j = 0; j < newH; j++) {
            double nj = j - factor * i;
            if (nj >= 0 && nj < in.height()) {
                int n1 = int(floor(nj));
                int n2 = int(ceil(nj));
                double w = nj - n1;
                if (n1 == n2) {
                    image.setPixel(i, j, in.pixel(i, n1));
                } else {
                    if (n1 >= 0 && n2 < in.height()) {
                        QRgb p1 = in.pixel(i, n1);
                        QRgb p2 = in.pixel(i, n2);
                        int r = int((1 - w) * qRed(p1) + w * qRed(p2));
                        int g = int((1 - w) * qGreen(p1) + w * qGreen(p2));
                        int b = int((1 - w) * qBlue(p1) + w * qBlue(p2));
                        image.setPixel(i, j, qRgb(r, g, b));
                    }
                }
            }
        }
    }
    return image;
}

QImage MainWindow::fourFiltersImg(const QImage &in)
{
    QImage img = in.convertToFormat(QImage::Format_RGB888);
    int mw = img.width() / 2;
    int mh = img.height() / 2;
    QImage org = img;
    for (int i = mw; i < img.width(); i++) {
        for (int j = 0; j < mh; j++) {
            QRgb p = org.pixel(i, j);
            img.setPixel(i, j, qRgb(255 - qRed(p), 255 - qGreen(p), 255 - qBlue(p)));
        }
    }
    for (int i = 0; i < mw; i++) {
        for (int j = mh; j < img.height(); j++) {
            QRgb p = org.pixel(i, j);
            int a = (qRed(p) + qGreen(p) + qBlue(p)) / 3;
            img.setPixel(i, j, qRgb(a, a, a));
        }
    }
    QImage tmp = org;
    for (int i = mw + 1; i < img.width() - 1; i++) {
        for (int j = mh + 1; j < img.height() - 1; j++) {
            int cx = 0, cy = 0;
            QRgb p1 = tmp.pixel(i + 1, j), p2 = tmp.pixel(i - 1, j), p3 = tmp.pixel(i, j + 1), p4 = tmp.pixel(i, j - 1);
            cx = qAbs(qRed(p1) - qRed(p2));
            cy = qAbs(qRed(p3) - qRed(p4));
            int c = qMin(255, cx + cy);
            c = 255 - c;
            img.setPixel(i, j, qRgb(c, c, c));
        }
    }
    return img;
}

QImage MainWindow::oldTVImg(const QImage &in)
{
    QImage img = in.convertToFormat(QImage::Format_RGB888);
    QImage temp = img;
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            QRgb p = img.pixel(i, j);
            int r = qRed(p), g = qGreen(p), b = qBlue(p);
            int rv = r, gv = g, bv = b;
            if (j % 12 < 5) rv = int(rv * 0.15);
            rv += (rand() % 120) - 60;
            rv = qBound(0, rv, 255);
            if (j % 12 < 5) gv = int(gv * 0.15);
            gv += (rand() % 120) - 60;
            gv = qBound(0, gv, 255);
            if (j % 12 < 5) bv = int(bv * 0.15);
            bv += (rand() % 120) - 60;
            bv = qBound(0, bv, 255);
            temp.setPixel(i, j, qRgb(rv, gv, bv));
        }
    }
    return temp;
}

QImage MainWindow::oilPaintingImg(const QImage &in, int radius)
{
    if (radius <= 0) radius = 2;
    const int BINS = 32;
    QImage src = in.convertToFormat(QImage::Format_RGB888);
    int W = src.width(), H = src.height();
    QImage out(W, H, QImage::Format_RGB888);
    std::vector<int> count(BINS);
    std::vector<int> sumR(BINS);
    std::vector<int> sumG(BINS);
    std::vector<int> sumB(BINS);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            std::fill(count.begin(), count.end(), 0);
            std::fill(sumR.begin(), sumR.end(), 0);
            std::fill(sumG.begin(), sumG.end(), 0);
            std::fill(sumB.begin(), sumB.end(), 0);
            for (int dy = -radius; dy <= radius; ++dy) {
                int ny = y + dy;
                if (ny < 0 || ny >= H) continue;
                for (int dx = -radius; dx <= radius; ++dx) {
                    int nx = x + dx;
                    if (nx < 0 || nx >= W) continue;
                    QRgb p = src.pixel(nx, ny);
                    int r = qRed(p), g = qGreen(p), b = qBlue(p);
                    int intensity = (r + g + b) / 3;
                    int bin = (intensity * (BINS - 1)) / 255;
                    count[bin] += 1;
                    sumR[bin] += r;
                    sumG[bin] += g;
                    sumB[bin] += b;
                }
            }
            int best = 0;
            int bestCount = count[0];
            for (int b = 1; b < BINS; ++b) {
                if (count[b] > bestCount) {
                    bestCount = count[b];
                    best = b;
                }
            }
            if (bestCount > 0) {
                int r = sumR[best] / bestCount;
                int g = sumG[best] / bestCount;
                int b = sumB[best] / bestCount;
                out.setPixel(x, y, qRgb(r, g, b));
            } else {
                out.setPixel(x, y, src.pixel(x, y));
            }
        }
    }
    return out;
}

QImage MainWindow::animeImg(const QImage &in)
{
    QImage img = in.convertToFormat(QImage::Format_RGB888);
    QImage result = img;
    int levels = 4;
    for (int y = 0; y < img.height(); y++)
        for (int x = 0; x < img.width(); x++) {
            QRgb p = img.pixel(x, y);
            int r = (qRed(p) / (256 / levels)) * (256 / levels);
            int g = (qGreen(p) / (256 / levels)) * (256 / levels);
            int b = (qBlue(p) / (256 / levels)) * (256 / levels);
            result.setPixel(x, y, qRgb(r, g, b));
        }

    QImage edge(img.width(), img.height(), QImage::Format_RGB888);
    edge.fill(qRgb(0, 0, 0));
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int y = 1; y < img.height() - 1; y++)
        for (int x = 1; x < img.width() - 1; x++) {
            double sumX = 0, sumY = 0;
            for (int j = -1; j <= 1; j++)
                for (int i = -1; i <= 1; i++) {
                    int gray = (qRed(img.pixel(x + i, y + j)) + qGreen(img.pixel(x + i, y + j)) + qBlue(img.pixel(x + i, y + j))) / 3;
                    sumX += gx[j + 1][i + 1] * gray;
                    sumY += gy[j + 1][i + 1] * gray;
                }
            int mag = int(sqrt(sumX * sumX + sumY * sumY));
            int m = (mag > 90) ? 255 : 0;
            edge.setPixel(x, y, qRgb(m, m, m));
        }

    for (int y = 0; y < img.height(); y++)
        for (int x = 0; x < img.width(); x++) {
            QRgb e = edge.pixel(x, y);
            if (qRed(e) > 200) result.setPixel(x, y, qRgb(0, 0, 0));
        }

    return result;
}

QImage MainWindow::warmColdImg(const QImage &in, int choice, int strength)
{
    QImage img = in.convertToFormat(QImage::Format_RGB888);
    QImage result = img;
    double factor;
    switch (strength) {
    case 1: factor = 0.5; break;
    case 2: factor = 1.0; break;
    case 3: factor = 1.5; break;
    default: factor = 1.0; break;
    }

    for (int y = 0; y < img.height(); y++)
        for (int x = 0; x < img.width(); x++) {
            QRgb p = img.pixel(x, y);
            int r = qRed(p), g = qGreen(p), b = qBlue(p);
            if (choice == 1) {
                r = qMin(255, int(r * (1.05 + 0.1 * factor) + 5 * factor));
                g = qMin(255, int(g * (1.02 + 0.05 * factor)));
                b = qMax(0, int(b * (1.0 - 0.1 * factor) - 5 * factor));
            } else {
                r = qMax(0, int(r * (1.0 - 0.1 * factor) - 5 * factor));
                g = qMin(255, int(g * 1.0));
                b = qMin(255, int(b * (1.05 + 0.1 * factor) + 5 * factor));
            }
            result.setPixel(x, y, qRgb(r, g, b));
        }

    return result;
}

QPoint MainWindow::labelToImage(const QLabel *label, const QPoint &pt, const QImage &img)
{
    if (img.isNull()) return QPoint();
    QSize lbl = label->size();
    QSize imgSize = img.size();
    imgSize.scale(lbl, Qt::KeepAspectRatio);
    int xoff = (lbl.width() - imgSize.width()) / 2;
    int yoff = (lbl.height() - imgSize.height()) / 2;
    int ix = pt.x() - xoff;
    int iy = pt.y() - yoff;
    if (ix < 0 || iy < 0 || ix >= imgSize.width() || iy >= imgSize.height()) return QPoint(-1, -1);
    double sx = double(img.width()) / double(imgSize.width());
    double sy = double(img.height()) / double(imgSize.height());
    return QPoint(int(ix * sx), int(iy * sy));
}

QRect MainWindow::labelToImageRect(const QLabel *label, const QRect &rect, const QImage &img)
{
    QPoint p1 = labelToImage(label, rect.topLeft(), img);
    QPoint p2 = labelToImage(label, rect.bottomRight(), img);
    if (p1.x() < 0 || p2.x() < 0) return QRect();
    return QRect(p1, p2).normalized();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lblProcessed) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            selecting = true;
            rubberOrigin = me->pos();
            rubberBand->setGeometry(QRect(rubberOrigin, QSize()));
            rubberBand->show();
            return true;
        } else if (event->type() == QEvent::MouseMove && selecting) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            QRect r(rubberOrigin, me->pos());
            rubberBand->setGeometry(r.normalized());
            return true;
        } else if (event->type() == QEvent::MouseButtonRelease && selecting) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            selecting = false;
            rubberBand->hide();
            QRect r = QRect(rubberOrigin, me->pos()).normalized();
            selectionRectImage = labelToImageRect(ui->lblProcessed, r, processed);
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
