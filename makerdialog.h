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
    unsigned int w{129}, h{129};
    Terrain terrain{w, h};
    TerrainModeling *modeling{nullptr};
    QImage greyimage;
    void updateimage();
    void paintEvent(QPaintEvent *);
    void setModeling(TerrainModeling *);
private slots:
    void btn_init_clicked();
    void btn_start_clicked();
    void btn_step_clicked();
    void btn_erosion_clicked();
    void btn_save_clicked();
    void btn_run_clicked();

    void on_rdb_fault_clicked();
    void on_rdb_plate_clicked();
    void on_rdb_particle_clicked();
    void on_rdb_particle_sand_clicked();
};

#endif // MAKERDIALOG_H
