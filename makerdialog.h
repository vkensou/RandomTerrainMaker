#ifndef MAKERDIALOG_H
#define MAKERDIALOG_H

#include <QDialog>
#include "terrain.h"
#include "faultformation.h"

namespace Ui {
class MakerDialog;
}
class TerrainModeling;
class MakerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MakerDialog(QWidget *parent = 0);
    ~MakerDialog();

private:
    Ui::MakerDialog *ui;
    unsigned int w{513}, h{513};
    Terrain terrain{w, h};
    TerrainModeling *modeling{nullptr};
    QImage greyimage;
    void updateimage();
    void paintEvent(QPaintEvent *);
    void setModeling(TerrainModeling *);
    void setTerrainSize(unsigned int width, unsigned int height);

private slots:
    void btn_start_clicked();
    void btn_step_clicked();
    void btn_erosion_clicked();
    void btn_save_clicked();
    void btn_run_clicked();

    void on_rdb_fault_clicked();
    void on_rdb_plate_clicked();
    void on_rdb_particle_clicked();
    void on_rdb_particle_sand_clicked();
    void on_edt_width_textChanged(const QString &arg1);
    void on_edt_height_textChanged(const QString &arg1);
    void on_btn_setsize_clicked();
};

#endif // MAKERDIALOG_H
