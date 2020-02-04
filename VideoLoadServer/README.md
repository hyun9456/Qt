# Video Load Server
Qt���� Video ������ �ҷ��鿩 TCP ����� ���� Client�� ���� FPS�� Frame�� ������� RGB���� ������ Server ���α׷�

## 1. Video Frame Grabber

**QAbstructVideoSurface** Ŭ������ ��ӹ޾� ���� Ŭ������ **QMediaPlayer**�� �����Ͽ� ������ �� Frame ������ ó���� �����ִ� ����  
*m_rgbMode*�� ���� 0�� ��� Average, 1�� ��� Median, 2�� ��� Center RGB ���� ��ȯ��

���� : [https://forum.qt.io/topic/63206/trying-to-capture-qimage-of-video-frame](https://forum.qt.io/topic/63206/trying-to-capture-qimage-of-video-frame)
[https://stackoverflow.com/questions/37724602/how-to-save-a-frame-using-qmediaplayer](https://stackoverflow.com/questions/37724602/how-to-save-a-frame-using-qmediaplayer)

## 2. TCP Server Socket

**QTcpServer** Ŭ������ ��ӹ޾� ���� Ŭ������ Client�� ����� Socket�� ��� **ServerThread**�� �����ϴ� ����

## 3. Server Thread

**QThread** Ŭ������ ��ӹ޾� ���� Ŭ������ Client�� ����� Socket�� ������ ������ **QTimer**�� ���� msec������ ���� FPS�� *m_block*�� ����� �����͸� �����ϴ� ����

���� : [https://www.bogotobogo.com/Qt/Qt5_QTcpServer_Multithreaded_Client_Server.php](https://www.bogotobogo.com/Qt/Qt5_QTcpServer_Multithreaded_Client_Server.php)

## Error During Development

* ���� ����� codec ���� ����

		DirectShowPlayerService::doRender: Unresolved error code 80040266  
	> Codec ��ġ�Ͽ� �ذ�  
	> ���� : [http://www.qt-dev.com/board.php?board=qnaboard&command=body&no=604](http://www.qt-dev.com/board.php?board=qnaboard&command=body&no=604)
* Head ���Ͽ� Q_OBJECT �߰� �� ����

		tcpserversocket.obj:-1: error: LNK2001: "public: virtual struct QMetaObject const * __cdecl TcpServerSocket::metaObject(void)const " (?metaObject@TcpServerSocket@@UEBAPEBUQMetaObject@@XZ) �ܺ� ��ȣ�� Ȯ���� �� �����ϴ�.
		tcpserversocket.obj:-1: error: LNK2001: "public: virtual void * __cdecl TcpServerSocket::qt_metacast(char const *)" (?qt_metacast@TcpServerSocket@@UEAAPEAXPEBD@Z) �ܺ� ��ȣ�� Ȯ���� �� �����ϴ�.
		tcpserversocket.obj:-1: error: LNK2001: "public: virtual int __cdecl TcpServerSocket::qt_metacall(enum QMetaObject::Call,int,void * *)" (?qt_metacall@TcpServerSocket@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z) �ܺ� ��ȣ�� Ȯ���� �� �����ϴ�.
	>Project Clean �� Build�Ͽ� �ذ�

* Smart Pointer ���� ����

		C:\Qt\5.12.6\msvc2017_64\include\QtCore\qsharedpointer_impl.h:321: error: C2440: '�ʱ�ȭ ��': 'X *'���� 'VideoFrameGrabber *'(��)�� ��ȯ�� �� �����ϴ�.
		with
		[
		    X=MainWindow
		]
	> QSharedPointer<T>���� T�� �´� �������� ���� �� �ʱ�ȭ   
	> ex) QSharedPointer<VideoFrameGrabber> grabber(new VideoFrameGrabber(this));

* �ٸ� Thread �� �Լ� ȣ�� ����

		ASSERT failure in QCoreApplication::sendEvent: "Cannot send events to objects owned by a different thread. Current thread 0x0x1f1ff7bc100. Receiver '' (of type 'QNativeSocketEngine') was created in thread 0x0x1f1829ec8f0", file kernel\qcoreapplication.cpp, line 578
	> QMetaObject::invokeMethod ����Ͽ� �ذ�  
	> ���� : [https://thesmithfam.org/blog/2010/02/07/talking-to-qt-threads/](https://thesmithfam.org/blog/2010/02/07/talking-to-qt-threads/)
