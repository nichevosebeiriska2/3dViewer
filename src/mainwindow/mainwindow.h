#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QSlider>

#include "../glwidget/glwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

		void AddObject(ObjectWrapper* ptr);
		void SliderXValueChanged(int i);
		void SliderYValueChanged(int i);

private:
    GLWidget *glWidget;
    QHBoxLayout *m_main_layout;
    QSlider *m_slider_x;
    QSlider *m_slider_y;

protected:
  QMenu *CreateMenuShaders();

};

#endif // MAINWINDOW_H