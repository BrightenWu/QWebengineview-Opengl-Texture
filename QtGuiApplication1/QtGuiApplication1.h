#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtGuiApplication1.h"
#include <QOpenGLFramebufferObject>
#include <QGraphicsScene>
#include<QOpenGLWidget>
#include <QOpenGLTexture>
#include "DLPWebEngine.h"

class QCustomGLWidget;
namespace CUSTOM_WEBENGINE
{
	class CDLPWebEngine;
}

class WebShowEx2 : public QWidget
{
	Q_OBJECT

public:
	WebShowEx2();
	~WebShowEx2();

public slots:
	void Init();
	void OnTimeoutPumpEvents();

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
	CUSTOM_WEBENGINE::CDLPWebEngine *m_pWeb;
	QCustomGLWidget *m_glWidget;
	GLuint m_uTexture;
};

class QCustomGLWidget :public QOpenGLWidget
{
	Q_OBJECT

public:
	QCustomGLWidget(QWidget *parent = 0);
	~QCustomGLWidget();

protected:
	void paintGL() Q_DECL_OVERRIDE;

public:
	GLuint m_uTexture;
};