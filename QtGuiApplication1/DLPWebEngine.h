/************************************************************************
Programmer: Brighten Wu
Date: 2018.04.08
Issue&P.S.:
1.html5��mp4��H.264�޷����ţ���Ҫ�޸����ñ���Դ�롣
2.	��html5��Ƶ��Ҫ��װflash���pepflashplayer��PPAPI��������ܲ�����Ƶ��
************************************************************************/

#ifndef DLPQTWEBENGINE_H
#define DLPQTWEBENGINE_H

#include <string>
#include <QPoint>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

class QGraphicsScene;

namespace CUSTOM_WEBENGINE
{
	struct WebSize
	{
		WebSize() :nWidth(0), nHeight(0) {}
		WebSize(int w, int h) { nWidth = w; nHeight = h; }

		int nWidth;
		int nHeight;
	};

	struct MouseMovePoint
	{
		MouseMovePoint() :fMouseMoveX(0.0), fMouseMoveY(0.0) {}

		double fMouseMoveX;
		double fMouseMoveY;
	};

	enum MouseBtn
	{
		NoBtn = 0x00,
		LeftBtn = 0x01,
		RightBtn = 0x02,
		MidBtn = 0x04
	};

	enum KeyboardModifier
	{
		NoModifier = 0x00,
		ShiftModifier = 0x01,
		ControlModifier = 0x02,
		AltModifier = 0x04
	};

	enum WheelOrientation
	{
		Horizontal = 0x1,
		Vertical = 0x2
	};

	enum WebPageAction
	{
		Back,
		Forward,
		Stop,
		Reload,

		Cut,
		Copy,
		Paste,

		Undo,
		Redo,
		SelectAll
	};

	class QWebView : public QWebEngineView
	{
		Q_OBJECT

	public:
		QWebView(QWidget *parent = nullptr);
		~QWebView();

	protected:
		virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
	};

	class CDLPWebEngine
	{
	public:
		CDLPWebEngine();
		virtual ~CDLPWebEngine();

		unsigned int getWebTexId() const;  //��ȡ��ҳ����ID

		bool loadUrl(const std::wstring &url);  //������ҳ
		const std::wstring getCurrentUrl() const;  //��ȡ��ǰ��ַ

		void setWebViewSize(const WebSize &size);  //������������ڴ�С
		const WebSize getCurrentSize() const;

		void	setMuted(bool muted);  //������ҳ����
		bool isMuted() const;

		//����ƶ������¡��ͷš����ֹ���
		void moveMouse(MouseMovePoint sPoint, unsigned int uModifiers = NoModifier);
		void pressMouse(MouseBtn eBtn, unsigned int uModifiers = NoModifier);
		void releaseMouse(MouseBtn eBtn, unsigned int uModifiers = NoModifier);
		void wheelMouse(WheelOrientation eWheelOrientation, unsigned int uWheelStep, unsigned int uModifiers = NoModifier);

		//��ҳ����
		void triggerAction(WebPageAction eAction);

	private:
		enum MouseEvent
		{
			Mouse_Move,  //����ƶ�
			Mouse_Press,  //��갴��
			Mouse_Release,  //����ͷ�
			Mouse_Wheel  //����
		};

		struct MouseEventData
		{
			MouseEventData() :eEventType(Mouse_Move), uModifiers(NoModifier),
				eWheelOrientation(Vertical), uWheelStep(0), eMouseBtn(NoBtn) {}

			MouseEvent eEventType;  //�¼�����
			MouseMovePoint sPoint;  //����ƶ�����
			unsigned int uModifiers;  //�����޸ļ�KeyboardModifier���
			WheelOrientation eWheelOrientation;  //��������:Horizontal Vertical
			unsigned int uWheelStep;  //��������,������;Ϊ��ʱ�������ֳ�ʹ���߹���
			MouseBtn eMouseBtn;  //�������±�־
		};

		void sendMouseEvent(const MouseEventData &sUiData);  //����������ڷ�������¼�

	public:
		QWebView *m_pWebView;

	private:
		QGraphicsScene * m_pScene;

		Qt::MouseButtons m_lastMouseButtons;
		QPointF m_ptLastMouse;
	};
}

#endif // DLPQTWEBENGINE_H
