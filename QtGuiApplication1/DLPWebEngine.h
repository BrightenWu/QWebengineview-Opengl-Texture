/************************************************************************
Programmer: Brighten Wu
Date: 2018.04.08
Issue&P.S.:
1.html5的mp4、H.264无法播放，需要修改配置编译源码。
2.	非html5视频需要安装flash插件pepflashplayer（PPAPI插件）才能播放视频。
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

		unsigned int getWebTexId() const;  //获取网页纹理ID

		bool loadUrl(const std::wstring &url);  //加载网页
		const std::wstring getCurrentUrl() const;  //获取当前网址

		void setWebViewSize(const WebSize &size);  //设置浏览器窗口大小
		const WebSize getCurrentSize() const;

		void	setMuted(bool muted);  //设置网页静音
		bool isMuted() const;

		//鼠标移动、按下、释放、滚轮滚动
		void moveMouse(MouseMovePoint sPoint, unsigned int uModifiers = NoModifier);
		void pressMouse(MouseBtn eBtn, unsigned int uModifiers = NoModifier);
		void releaseMouse(MouseBtn eBtn, unsigned int uModifiers = NoModifier);
		void wheelMouse(WheelOrientation eWheelOrientation, unsigned int uWheelStep, unsigned int uModifiers = NoModifier);

		//网页操作
		void triggerAction(WebPageAction eAction);

	private:
		enum MouseEvent
		{
			Mouse_Move,  //鼠标移动
			Mouse_Press,  //鼠标按下
			Mouse_Release,  //鼠标释放
			Mouse_Wheel  //滚轮
		};

		struct MouseEventData
		{
			MouseEventData() :eEventType(Mouse_Move), uModifiers(NoModifier),
				eWheelOrientation(Vertical), uWheelStep(0), eMouseBtn(NoBtn) {}

			MouseEvent eEventType;  //事件类型
			MouseMovePoint sPoint;  //鼠标移动坐标
			unsigned int uModifiers;  //键盘修改键KeyboardModifier组合
			WheelOrientation eWheelOrientation;  //滚动方向:Horizontal Vertical
			unsigned int uWheelStep;  //滚动步数,有正负;为负时，鼠标滚轮朝使用者滚动
			MouseBtn eMouseBtn;  //鼠标键按下标志
		};

		void sendMouseEvent(const MouseEventData &sUiData);  //向浏览器窗口发送鼠标事件

	public:
		QWebView *m_pWebView;

	private:
		QGraphicsScene * m_pScene;

		Qt::MouseButtons m_lastMouseButtons;
		QPointF m_ptLastMouse;
	};
}

#endif // DLPQTWEBENGINE_H
