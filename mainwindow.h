#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QStack>
#include <QRubberBand>
#include <QRect>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_btnLoad_clicked();
    void on_btnSave_clicked();
    void on_btnUndo_clicked();
    void on_btnRedo_clicked();
    void on_btnClear_clicked();
    void on_btnHelp_clicked();

    void on_btnGrey_clicked();
    void on_btnBW_clicked();
    void on_btnInvert_clicked();
    void on_btnPurple_clicked();
    void on_btnEdges_clicked();

    void on_btnRotate90_clicked();
    void on_btnRotate180_clicked();
    void on_btnRotate270_clicked();
    void on_btnFlipH_clicked();
    void on_btnFlipV_clicked();

    void on_btnBrightLight_clicked();
    void on_btnBrightDark_clicked();

    void on_btnCrop_clicked();
    void on_btnResize_clicked();
    void on_btnMerge_clicked();

    void on_btnBlurWeak_clicked();
    void on_btnBlurMed_clicked();
    void on_btnBlurStrong_clicked();
    void on_sliderBlur_sliderPressed();
    void on_sliderBlur_valueChanged(int v);
    void on_sliderBlur_sliderReleased();

    void on_btnInfrared_clicked();
    void on_btnSunlight_clicked();

    void on_btnFrameSimple_clicked();
    void on_btnFrameDouble_clicked();
    void on_btnFrameFancy_clicked();
    void on_btnSkew_clicked();
    void on_btnFourFilters_clicked();
    void on_btnOldTV_clicked();
    void on_btnOilPainting_clicked();
    void on_btnAnime_clicked();

    void on_btnWarmCold_clicked();
    void on_btnWarm_clicked();
    void on_btnCold_clicked();

    void on_sliderBrightness_sliderPressed();
    void on_sliderBrightness_valueChanged(int v);
    void on_sliderBrightness_sliderReleased();

private:
    Ui::MainWindow *ui;

    QImage original;
    QImage processed;
    QImage sliderBase;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;

    QRubberBand *rubberBand = nullptr;
    bool selecting = false;
    QPoint rubberOrigin;
    QRect selectionRectImage;

    QImage mergeImage;
    bool mergeMode = false;

    void pushUndo();
    void updateViews();
    void adjustProcessedToOriginalSize();


    QImage toGrey(const QImage &in);
    QImage toBW(const QImage &in);
    QImage invertImg(const QImage &in);
    QImage purpleImg(const QImage &in);
    QImage edgesImg(const QImage &in);
    QImage rotateImg(const QImage &in, int times90);
    QImage brightnessImg(const QImage &in, double factor);
    QImage flipImg(const QImage &in, bool horizontal);
    QImage resizeImg(const QImage &in, int w, int h);
    QImage cropImg(const QImage &in, int x, int y, int w, int h);
    QImage blurImg(const QImage &in, int iterations);
    QImage infraredImg(const QImage &in);
    QImage sunlightImg(const QImage &in);
    QImage frameImg(const QImage &in, QColor color, int thickness);
    QImage fancyFrameImg(const QImage &in, int thickness);
    QImage oldTVImg(const QImage &in);
    QImage fourFiltersImg(const QImage &in, const QString &tl, const QString &tr, const QString &bl, const QString &br);
    QImage applyFilterByName(const QImage &in, const QString &filterName);

    QImage skewImg(const QImage &in, int degree);
    QImage oilPaintingImg(const QImage &in, int radius);
    QImage animeImg(const QImage &in);
    QImage warmColdImg(const QImage &in, int choice, int strength);

    QPoint labelToImage(const QLabel *label, const QPoint &pt, const QImage &img);
    QRect labelToImageRect(const QLabel *label, const QRect &rect, const QImage &img);
};

#endif

