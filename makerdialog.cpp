#include "makerdialog.h"
#include "ui_makerdialog.h"
#include <QtWidgets/QPushButton>
#include <QPainter>
#include <memory.h>
#include <QProcess>
#include "faultformation.h"
#include "platedrift.h"
#include "particledeposition.h"
#include "pd_dla.h"
#include "pd_sand.h"
#include "terrainutil.h"

MakerDialog::MakerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakerDialog)
{
    ui->setupUi(this);
    modeling = new FaultFormation(terrain);

    connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(btn_start_clicked()));
    connect(ui->btn_step, SIGNAL(clicked()), this, SLOT(btn_step_clicked()));
    connect(ui->btn_erosion, SIGNAL(clicked()), this, SLOT(btn_erosion_clicked()));
    connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(btn_save_clicked()));
    connect(ui->btn_run, SIGNAL(clicked()), this, SLOT(btn_run_clicked()));

    QImage gi("grey.bmp", "bmp");
    greyimage = gi.scaled(w, h);
    greyimage.fill(0);

    QString str;
    str.setNum(terrain.getWidth());
    ui->edt_width->setText(str);
    str.setNum(terrain.getHeight());
    ui->edt_height->setText(str);
}

MakerDialog::~MakerDialog()
{
    delete ui;
}

void MakerDialog::btn_start_clicked()
{
    if(modeling)
    {
		ui->btn_step->setEnabled(true);
		ui->btn_erosion->setEnabled(true);
		ui->btn_save->setEnabled(true);
		ui->btn_run->setEnabled(true);
        modeling->start();
        updateimage();
    }
}

void MakerDialog::btn_step_clicked()
{
    if(modeling)
    {
        modeling->step();
        updateimage();
    }
}

void MakerDialog::btn_erosion_clicked()
{
    if(modeling)
    {
        modeling->erosion();
        updateimage();
    }
}

void MakerDialog::btn_save_clicked()
{
    greyimage.save("terrainmake.bmp","bmp");
}

void MakerDialog::btn_run_clicked()
{
    greyimage.save("terrainmake.bmp","bmp");
    QProcess::startDetached("terraintest.exe",QStringList());
}

void MakerDialog::updateimage()
{
    for(unsigned int j = 0;j < terrain.getHeight();j++)
    {
        for(unsigned int i = 0;i < terrain.getWidth();i++)
        {
            greyimage.setPixel(i, j, (unsigned char)terrain.getData()[j*terrain.getWidth() + i]);
        }
    }

    this->update();
}

void MakerDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, greyimage);
}

void MakerDialog::setModeling(TerrainModeling *newmodeling)
{
    if(modeling != nullptr)
    {
        delete modeling;
    }
    modeling = newmodeling;
	ui->btn_start->setEnabled(true);
	ui->btn_step->setEnabled(false);
	ui->btn_erosion->setEnabled(false);
	ui->btn_save->setEnabled(false);
    ui->btn_run->setEnabled(false);
}

void MakerDialog::setTerrainSize(unsigned int width, unsigned int height)
{
    assert(canbeTerrainWidth(width) && canbeTerrainWidth(height));
    terrain.reset(width, height);

	greyimage = greyimage.scaled(terrain.getWidth(), terrain.getHeight());
	greyimage.fill(0);
    updateimage();
}

void MakerDialog::on_rdb_fault_clicked()
{
    setModeling(new FaultFormation(terrain));
}

void MakerDialog::on_rdb_plate_clicked()
{
    setModeling(new PlateDrift(terrain));
}

void MakerDialog::on_rdb_particle_clicked()
{
    setModeling(new ParticleDeposition(terrain));
}

void MakerDialog::on_rdb_particle_sand_clicked()
{
    setModeling(new PD_Sand(terrain));
}

void MakerDialog::on_edt_width_textChanged(const QString &arg1)
{
    int val = arg1.toInt();
    ui->btn_setsize->setEnabled(canbeTerrainWidth(val));
}

void MakerDialog::on_edt_height_textChanged(const QString &arg1)
{
    int val = arg1.toInt();
    ui->btn_setsize->setEnabled(canbeTerrainWidth(val));
}

void MakerDialog::on_btn_setsize_clicked()
{
	setTerrainSize(ui->edt_width->text().toInt(), ui->edt_height->text().toInt());
    ui->btn_step->setEnabled(false);
	ui->btn_erosion->setEnabled(false);
    ui->btn_save->setEnabled(false);
    ui->btn_run->setEnabled(false);
}
