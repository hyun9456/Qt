# Video Load Server
Qt에서 Video 영상을 불러들여 TCP 통신을 통해 Client로 일정 FPS로 Frame의 평균적인 RGB값을 보내는 Server 프로그램

## 1. Video Frame Grabber

**QAbstructVideoSurface** 클래스를 상속받아 만든 클래스로 **QMediaPlayer**와 연결하여 영상의 각 Frame 별로의 처리를 도와주는 역할  
*m_rgbMode*에 따라 0일 경우 Average, 1일 경우 Median, 2일 경우 Center RGB 값을 반환함

참조 : [https://forum.qt.io/topic/63206/trying-to-capture-qimage-of-video-frame](https://forum.qt.io/topic/63206/trying-to-capture-qimage-of-video-frame)
[https://stackoverflow.com/questions/37724602/how-to-save-a-frame-using-qmediaplayer](https://stackoverflow.com/questions/37724602/how-to-save-a-frame-using-qmediaplayer)

## 2. TCP Server Socket

**QTcpServer** 클래스를 상속받아 만든 클래스로 Client와 연결된 Socket인 담긴 **ServerThread**를 관리하는 역할

## 3. Server Thread

**QThread** 클래스를 상속받아 만든 클래스로 Client와 연결된 Socket을 가지고 있으며 **QTimer**를 통해 msec단위의 일정 FPS로 *m_block*에 저장된 데이터를 전송하는 역할

참조 : [https://www.bogotobogo.com/Qt/Qt5_QTcpServer_Multithreaded_Client_Server.php](https://www.bogotobogo.com/Qt/Qt5_QTcpServer_Multithreaded_Client_Server.php)

## Error During Development

* 영상 재생시 codec 관련 오류

		DirectShowPlayerService::doRender: Unresolved error code 80040266  
	> Codec 설치하여 해결  
	> 참조 : [http://www.qt-dev.com/board.php?board=qnaboard&command=body&no=604](http://www.qt-dev.com/board.php?board=qnaboard&command=body&no=604)
* Head 파일에 Q_OBJECT 추가 후 오류

		tcpserversocket.obj:-1: error: LNK2001: "public: virtual struct QMetaObject const * __cdecl TcpServerSocket::metaObject(void)const " (?metaObject@TcpServerSocket@@UEBAPEBUQMetaObject@@XZ) 외부 기호를 확인할 수 없습니다.
		tcpserversocket.obj:-1: error: LNK2001: "public: virtual void * __cdecl TcpServerSocket::qt_metacast(char const *)" (?qt_metacast@TcpServerSocket@@UEAAPEAXPEBD@Z) 외부 기호를 확인할 수 없습니다.
		tcpserversocket.obj:-1: error: LNK2001: "public: virtual int __cdecl TcpServerSocket::qt_metacall(enum QMetaObject::Call,int,void * *)" (?qt_metacall@TcpServerSocket@@UEAAHW4Call@QMetaObject@@HPEAPEAX@Z) 외부 기호를 확인할 수 없습니다.
	>Project Clean 후 Build하여 해결

* Smart Pointer 관련 오류

		C:\Qt\5.12.6\msvc2017_64\include\QtCore\qsharedpointer_impl.h:321: error: C2440: '초기화 중': 'X *'에서 'VideoFrameGrabber *'(으)로 변환할 수 없습니다.
		with
		[
		    X=MainWindow
		]
	> QSharedPointer<T>에서 T에 맞는 형식으로 생성 후 초기화   
	> ex) QSharedPointer<VideoFrameGrabber> grabber(new VideoFrameGrabber(this));

* 다른 Thread 간 함수 호출 문제

		ASSERT failure in QCoreApplication::sendEvent: "Cannot send events to objects owned by a different thread. Current thread 0x0x1f1ff7bc100. Receiver '' (of type 'QNativeSocketEngine') was created in thread 0x0x1f1829ec8f0", file kernel\qcoreapplication.cpp, line 578
	> QMetaObject::invokeMethod 사용하여 해결  
	> 참조 : [https://thesmithfam.org/blog/2010/02/07/talking-to-qt-threads/](https://thesmithfam.org/blog/2010/02/07/talking-to-qt-threads/)
