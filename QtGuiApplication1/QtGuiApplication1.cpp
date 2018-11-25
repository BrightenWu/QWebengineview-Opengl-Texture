#include "QtGuiApplication1.h"
#include <QTimer>
#include "glut.h"
#include <QPushButton>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

using namespace CUSTOM_WEBENGINE;

//////////////////////////////////////////////////////////////////////////
#define DELETE_BUFFER(p) do{if (p != nullptr){delete[] p;p = nullptr;}}while(0);

WebShowEx2::WebShowEx2()
	: m_glWidget(nullptr)
{
	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->addStretch(3);
	QPushButton *pChangeSizeButton = new QPushButton("ChangeSize",this);
	QPushButton *pToggleMuteButton = new QPushButton("ToggleMute", this);
	QPushButton *pClickButton = new QPushButton("ClickLink", this);
	QPushButton *pWheelButton = new QPushButton("Wheel", this);
	QPushButton *pBackButton = new QPushButton("Back", this);
	QPushButton *pForwardButton = new QPushButton("Forward", this);
	QPushButton *pChangeUrlButton = new QPushButton("ChangeUrl", this);

	connect(pChangeSizeButton, &QPushButton::clicked, [&] {
		m_pWeb->setWebViewSize(WebSize(1280, 720));
	});

	connect(pToggleMuteButton, &QPushButton::clicked, [&] {m_pWeb->setMuted(!m_pWeb->isMuted()); });
	connect(pClickButton, &QPushButton::clicked, [&] {
		MouseMovePoint sPoint;
		sPoint.fMouseMoveX = 37;
		sPoint.fMouseMoveY = 159;  //473
		m_pWeb->moveMouse(sPoint);

		m_pWeb->pressMouse(LeftBtn);
		m_pWeb->releaseMouse(LeftBtn);
	});

	connect(pWheelButton, &QPushButton::clicked, [&] {
		m_pWeb->wheelMouse(Vertical, -5);
		//m_pWeb->triggerAction(SelectAll);
	});
	connect(pBackButton, &QPushButton::clicked, [&] {
		m_pWeb->triggerAction(Back);
	});
	connect(pForwardButton, &QPushButton::clicked, [&] {
		m_pWeb->triggerAction(Forward);
	});

	connect(pChangeUrlButton, &QPushButton::clicked, [&] {
		m_pWeb->loadUrl(L"http://toolofna.com/#!/work/eyes-on-gigi");

		//加载网址后需重新获取纹理ID
		//m_pContext = QOpenGLContext::currentContext();
		//if (m_pContext != nullptr) {
		//	m_pContext->makeCurrent(m_pContext->surface());
		//	QOpenGLFunctions *glFuncs = m_pContext->functions();
		//	glFuncs->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &iWebTex);
		//}
	});

	pLayout->addWidget(pChangeSizeButton);
	pLayout->addWidget(pToggleMuteButton);
	pLayout->addWidget(pClickButton);
	pLayout->addWidget(pWheelButton);
	pLayout->addWidget(pBackButton);
	pLayout->addWidget(pForwardButton);
	pLayout->addWidget(pChangeUrlButton);

	m_pWeb = new CDLPWebEngine();
}

WebShowEx2::~WebShowEx2()
{
}

void WebShowEx2::Init()
{
	m_pWeb->loadUrl(L"https://blog.csdn.net/zuishikonghuan/article/details/47979603");

	GLint uID = m_pWeb->getWebTexId();
	m_glWidget = new QCustomGLWidget();
	m_glWidget->show();
	m_glWidget->m_uTexture = uID;

	QTimer *m_pPumpEventsTimer = new QTimer(this);
	connect(m_pPumpEventsTimer, SIGNAL(timeout()), this, SLOT(OnTimeoutPumpEvents()));
	m_pPumpEventsTimer->setInterval(40);
	m_pPumpEventsTimer->start();
}

void WebShowEx2::OnTimeoutPumpEvents()
{
	if(m_glWidget!=nullptr)
	{
		m_glWidget->update();
	}
}

void WebShowEx2::closeEvent(QCloseEvent * event)
{
	__super::closeEvent(event);

	DELETE_BUFFER(m_pWeb);
}

//////////////////////////////////////////////////////////////////////////

QCustomGLWidget::QCustomGLWidget(QWidget *parent /* = 0 */) :QOpenGLWidget(parent), m_uTexture(0)
{
	this->setFixedSize(QSize(800, 600));
}

QCustomGLWidget::~QCustomGLWidget()
{

}

void QCustomGLWidget::paintGL()
{
	if (m_uTexture == 0)
	{
		return;
	}


	//LARGE_INTEGER m_liPerfFreq = { 0 };
	////获取每秒多少CPU Performance Tick 
	//QueryPerformanceFrequency(&m_liPerfFreq);

	//LARGE_INTEGER m_liPerfStart = { 0 };
	//QueryPerformanceCounter(&m_liPerfStart);


	QOpenGLPaintDevice device(this->size());
	QPainter painter(&device);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	painter.beginNativePainting();

	glClearColor(0, 0, 0, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (GLdouble)this->width() / this->height(), 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -2, 1, 0, 0.0, 0.0, 0, 0, 1);

	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glColor4f(1, 1, 1, 1);
	glBegin(GL_TRIANGLES);

	// front faces
	//glNormal3f(0, 0, 1);
	// face v0-v1-v2
	glTexCoord2f(1, 1);  glVertex3f(.5, .5, .5);
	glTexCoord2f(0, 1);  glVertex3f(-.5, .5, .5);
	glTexCoord2f(0, 0);  glVertex3f(-.5, -.5, .5);
	// face v2-v3-v0
	glTexCoord2f(0, 0);  glVertex3f(-.5, -.5, .5);
	glTexCoord2f(1, 0);  glVertex3f(.5, -.5, .5);
	glTexCoord2f(1, 1);  glVertex3f(.5, .5, .5);

	// right faces
	////glNormal3f(1, 0, 0);
	// face v0-v3-v4
	glTexCoord2f(0, 1);  glVertex3f(.5, .5, .5);
	glTexCoord2f(0, 0);  glVertex3f(.5, -.5, .5);
	glTexCoord2f(1, 0);  glVertex3f(.5, -.5, -.5);
	// face v4-v5-v0
	glTexCoord2f(1, 0);  glVertex3f(.5, -.5, -.5);
	glTexCoord2f(1, 1);  glVertex3f(.5, .5, -.5);
	glTexCoord2f(0, 1);  glVertex3f(.5, .5, .5);

	// top faces
	//glNormal3f(0, 1, 0);
	// face v0-v5-v6
	glTexCoord2f(1, 0);  glVertex3f(.5, .5, .5);
	glTexCoord2f(1, 1);  glVertex3f(.5, .5, -.5);
	glTexCoord2f(0, 1);  glVertex3f(-.5, .5, -.5);
	// face v6-v1-v0
	glTexCoord2f(0, 1);  glVertex3f(-.5, .5, -.5);
	glTexCoord2f(0, 0);  glVertex3f(-.5, .5, .5);
	glTexCoord2f(1, 0);  glVertex3f(.5, .5, .5);

	// left faces
	//glNormal3f(-1, 0, 0);
	// face  v1-v6-v7
	glTexCoord2f(1, 1);  glVertex3f(-.5, .5, .5);
	glTexCoord2f(0, 1);  glVertex3f(-.5, .5, -.5);
	glTexCoord2f(0, 0);  glVertex3f(-.5, -.5, -.5);
	// face v7-v2-v1
	glTexCoord2f(0, 0);  glVertex3f(-.5, -.5, -.5);
	glTexCoord2f(1, 0);  glVertex3f(-.5, -.5, .5);
	glTexCoord2f(1, 1);  glVertex3f(-.5, .5, .5);

	// bottom faces
	//glNormal3f(0, -1, 0);
	// face v7-v4-v3
	glTexCoord2f(0, 0);  glVertex3f(-.5, -.5, -.5);
	glTexCoord2f(1, 0);  glVertex3f(.5, -.5, -.5);
	glTexCoord2f(1, 1);  glVertex3f(.5, -.5, .5);
	// face v3-v2-v7
	glTexCoord2f(1, 1);  glVertex3f(.5, -.5, .5);
	glTexCoord2f(0, 1);  glVertex3f(-.5, -.5, .5);
	glTexCoord2f(0, 0);  glVertex3f(-.5, -.5, -.5);

	// back faces
	//glNormal3f(0, 0, -1);
	// face v4-v7-v6
	glTexCoord2f(0, 0);  glVertex3f(.5, -.5, -.5);
	glTexCoord2f(1, 0);  glVertex3f(-.5, -.5, -.5);
	glTexCoord2f(1, 1);  glVertex3f(-.5, .5, -.5);
	// face v6-v5-v4
	glTexCoord2f(1, 1);  glVertex3f(-.5, .5, -.5);
	glTexCoord2f(0, 1);  glVertex3f(.5, .5, -.5);
	glTexCoord2f(0, 0);  glVertex3f(.5, -.5, -.5);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	//glutSwapBuffers();

	painter.endNativePainting();

	//LARGE_INTEGER liPerfNow = { 0 };
	//// 计算CPU运行到现在的时间
	//QueryPerformanceCounter(&liPerfNow);
	//double time = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000.0) / m_liPerfFreq.QuadPart);
}