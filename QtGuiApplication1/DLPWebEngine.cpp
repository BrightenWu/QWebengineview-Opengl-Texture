#include "dlpwebengine.h"
#include <QGraphicsScene>

using namespace CUSTOM_WEBENGINE;

QWebView::QWebView(QWidget *parent /* = nullptr */)
	:QWebEngineView(parent)
{

}

QWebView::~QWebView()
{

}

QWebEngineView *QWebView::createWindow(QWebEnginePage::WebWindowType type)
{
	return this;
}

//////////////////////////////////////////////////////////////////////////

CDLPWebEngine::CDLPWebEngine()
	:m_lastMouseButtons(Qt::NoButton)
{
	//ÆôÓÃ²å¼þ
	QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

	m_pWebView = new QWebView();
	m_pScene = new QGraphicsScene();

	m_pWebView->move(0, 0);
	m_pScene->addWidget(m_pWebView);
}

CDLPWebEngine::~CDLPWebEngine()
{
	//m_pWebView->close();
	//m_pScene->clear();

	if (m_pScene != nullptr)
	{
		delete m_pScene;
		m_pScene = nullptr;
	}
}

unsigned int CDLPWebEngine::getWebTexId() const
{
	GLint iWebTex = 0;
	QOpenGLContext *pContext = QOpenGLContext::currentContext();
	if (pContext!=nullptr)
	{
		QOpenGLFunctions *glFuncs = pContext->functions();
		glFuncs->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &iWebTex);
	}

	return iWebTex;
}

bool CDLPWebEngine::loadUrl(const std::wstring &url)
{
	QUrl qUrl(QString::fromStdWString(url));

	if (qUrl.isValid())
	{
		m_pWebView->load(qUrl);
		return true;
	}
	else
	{
		return false;
	}
}

const std::wstring CDLPWebEngine::getCurrentUrl() const
{
	QUrl url = m_pWebView->url();

	if (url.isValid())
	{
		return url.toString().toStdWString();
	}
	else
	{
		return L"";
	}
}

void CDLPWebEngine::setWebViewSize(const WebSize &size)
{
	m_pWebView->setFixedSize(size.nWidth, size.nHeight);
}

const WebSize CDLPWebEngine::getCurrentSize() const
{
	return WebSize(m_pWebView->size().width(), m_pWebView->size().height());
}

void CDLPWebEngine::sendMouseEvent(const MouseEventData &sUiData)
{
	Qt::Orientation eWheelOrientation=Qt::Orientation(sUiData.eWheelOrientation);
	Qt::KeyboardModifiers modifiers;

	if (sUiData.uModifiers&ShiftModifier)
	{
		modifiers |= Qt::ShiftModifier;
	}

	if (sUiData.uModifiers&ControlModifier)
	{
		modifiers |= Qt::ControlModifier;
	}

	if (sUiData.uModifiers&AltModifier)
	{
		modifiers |= Qt::AltModifier;
	}

	switch (sUiData.eEventType)
	{
	case Mouse_Move:
	{
		QPointF ptNewMouse(sUiData.sPoint.fMouseMoveX, sUiData.sPoint.fMouseMoveY);
		QPoint ptGlobal = ptNewMouse.toPoint();
		QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
		mouseEvent.setWidget(NULL);
		mouseEvent.setPos(ptNewMouse);
		mouseEvent.setScenePos(ptGlobal);
		mouseEvent.setScreenPos(ptGlobal);
		mouseEvent.setLastPos(m_ptLastMouse);
		mouseEvent.setLastScenePos(m_pWebView->mapToGlobal(m_ptLastMouse.toPoint()));
		mouseEvent.setLastScreenPos(m_pWebView->mapToGlobal(m_ptLastMouse.toPoint()));
		mouseEvent.setButtons(m_lastMouseButtons);
		mouseEvent.setButton(Qt::NoButton);
		mouseEvent.setModifiers(modifiers);
		mouseEvent.setAccepted(false);

		m_ptLastMouse = ptNewMouse;
		QApplication::sendEvent(m_pScene, &mouseEvent);
	}
	break;

	case Mouse_Press:
	{
		Qt::MouseButton button = Qt::MouseButton(sUiData.eMouseBtn);

		m_lastMouseButtons |= button;

		QPoint ptGlobal = m_ptLastMouse.toPoint();
		QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
		mouseEvent.setWidget(NULL);
		mouseEvent.setPos(m_ptLastMouse);
		mouseEvent.setButtonDownPos(button, m_ptLastMouse);
		mouseEvent.setButtonDownScenePos(button, ptGlobal);
		mouseEvent.setButtonDownScreenPos(button, ptGlobal);
		mouseEvent.setScenePos(ptGlobal);
		mouseEvent.setScreenPos(ptGlobal);
		mouseEvent.setLastPos(m_ptLastMouse);
		mouseEvent.setLastScenePos(ptGlobal);
		mouseEvent.setLastScreenPos(ptGlobal);
		mouseEvent.setButtons(m_lastMouseButtons);
		mouseEvent.setButton(button);
		mouseEvent.setModifiers(modifiers);
		mouseEvent.setAccepted(false);

		QApplication::sendEvent(m_pScene, &mouseEvent);
	}
	break;

	case Mouse_Release:
	{
		Qt::MouseButton button = Qt::MouseButton(sUiData.eMouseBtn);
		m_lastMouseButtons &= ~button;

		QPoint ptGlobal = m_ptLastMouse.toPoint();
		QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseRelease);
		mouseEvent.setWidget(NULL);
		mouseEvent.setPos(m_ptLastMouse);
		mouseEvent.setScenePos(ptGlobal);
		mouseEvent.setScreenPos(ptGlobal);
		mouseEvent.setLastPos(m_ptLastMouse);
		mouseEvent.setLastScenePos(ptGlobal);
		mouseEvent.setLastScreenPos(ptGlobal);
		mouseEvent.setButtons(m_lastMouseButtons);
		mouseEvent.setButton(button);
		mouseEvent.setModifiers(modifiers);
		mouseEvent.setAccepted(false);

		QApplication::sendEvent(m_pScene, &mouseEvent);
	}
	break;

	case Mouse_Wheel:
	{
		QPoint ptGlobal = m_ptLastMouse.toPoint();
		QGraphicsSceneWheelEvent mouseEvent(QEvent::GraphicsSceneWheel);
		mouseEvent.setWidget(NULL);
		mouseEvent.setPos(m_ptLastMouse);
		mouseEvent.setScenePos(ptGlobal);
		mouseEvent.setScreenPos(ptGlobal);
		mouseEvent.setDelta(120 * sUiData.uWheelStep);
		mouseEvent.setOrientation(eWheelOrientation);
		mouseEvent.setButtons(m_lastMouseButtons);
		mouseEvent.setModifiers(modifiers);
		mouseEvent.setAccepted(false);

		QApplication::sendEvent(m_pScene, &mouseEvent);
	}
	break;

	default:
		Q_ASSERT(0);
		break;
	}
}

void CDLPWebEngine::setMuted(bool muted)
{
	QWebEnginePage *pWebPage = m_pWebView->page();
	if (pWebPage != nullptr)
	{
		pWebPage->setAudioMuted(muted);
	}
}

bool CUSTOM_WEBENGINE::CDLPWebEngine::isMuted() const
{
	QWebEnginePage *pWebPage = m_pWebView->page();
	if (pWebPage != nullptr)
	{
		return pWebPage->isAudioMuted();
	}

	return true;
}

void CDLPWebEngine::moveMouse(MouseMovePoint sPoint, unsigned int uModifiers)
{
	MouseEventData sMove;
	sMove.eEventType = Mouse_Move;
	sPoint.fMouseMoveX = sPoint.fMouseMoveX;
	sPoint.fMouseMoveY = sPoint.fMouseMoveY;
	sMove.sPoint = sPoint;
	sMove.uModifiers = uModifiers;
	sendMouseEvent(sMove);
}

void CDLPWebEngine::pressMouse(MouseBtn eBtn, unsigned int uModifiers)
{
	MouseEventData sPress;
	sPress.eEventType = Mouse_Press;
	sPress.eMouseBtn = eBtn;
	sPress.uModifiers = uModifiers;
	sendMouseEvent(sPress);
}

void CDLPWebEngine::releaseMouse(MouseBtn eBtn, unsigned int uModifiers)
{
	MouseEventData sRelease;
	sRelease.eEventType = Mouse_Release;
	sRelease.eMouseBtn = eBtn;
	sRelease.uModifiers = uModifiers;
	sendMouseEvent(sRelease);
}

void CDLPWebEngine::wheelMouse(WheelOrientation eWheelOrientation, unsigned int uWheelStep, unsigned int uModifiers)
{
	MouseEventData sWheel;
	sWheel.eEventType = Mouse_Wheel;
	sWheel.eWheelOrientation = eWheelOrientation;
	sWheel.uWheelStep = uWheelStep;
	sWheel.uModifiers = uModifiers;
	sendMouseEvent(sWheel);
}

void CDLPWebEngine::triggerAction(WebPageAction eAction)
{
	m_pWebView->triggerPageAction(QWebEnginePage::WebAction(eAction));
}
