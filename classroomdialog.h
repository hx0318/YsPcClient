#ifndef CLASSROOMDIALOG_H
#define CLASSROOMDIALOG_H

#include <QDialog>
#include "libpdu.h"
#include <QLabel>
#include "mediawidget.h"
#include <QTimer>
#include <QHostInfo>
#include "studentwidget.h"
#include <queue>
#include <QScrollBar>
#include <coursewarewidget.h>
#include "screenshotdialog.h"
#include "screenshotwidget.h"
#include "giftwidget.h"
#include "barragewidget.h"
#include "updowndisposewidget.h"
#include "k12msgs.h"
#include "center_media.h"
#include "media_player.h"
#include <QMutex>

using namespace k12msg;

namespace Ui {
class ClassroomDialog;
}

class ClassroomDialog : public QDialog, public k12mcu2ui, public k12msg::msg_observer, public video_play
{
	Q_OBJECT

public:
    explicit ClassroomDialog(int id,QString McuIp,int McuPort,QString ChatMsgServerIP,int ChatMsgServerPort,QString Name,QString dstip,int dstport,int CoursewareId,int crcpId,
                             QDateTime ClassBegin,QDateTime ClassOver,QWidget *parent = 0);
    ~ClassroomDialog();
    void record_msg(record_msg_t t, void *);

    int msg(k12mcu2ui_type t, void *arg);
    void ss_start_class(uint32_t uid);
    void start_class_ack(int code);
    void ss_stop_class(uint32_t uid);
    void stop_class_ack(int code);
    void get_class_state_ack(int state);
    void ss_application_share(uint32_t uid);
    void application_share_ack(int code);
    void ss_cancel_application(uint32_t uid);
    void cancel_application_ack(int code);
    void ss_reply_share(uint32_t uid, int reply);
    void reply_share_ack(uint32_t uid, int reply);
    void ss_start_share(uint32_t uid);
    void start_share_ack(int code);
    void ss_stop_share(uint32_t uid);
    void stop_share_ack(int code);
    void get_share_users_ack(int code, int cnt, uint32_t *uid);
    void ss_start_tag(uint32_t uid);
    void start_tag_ack(int code);
    void ss_stop_tag(uint32_t uid);
    void stop_tag_ack(int code);
    void get_tag_users_ack(int code, int cnt, uint32_t *uid);
    void ss_cancel_share(uint32_t uid);
    void cancel_share_ack(int code);
    void p2p_msg(uint32_t src_uid, const char *con, uint32_t con_len);
    void p2all_msg(uint32_t src_uid, const char *con, uint32_t con_len);
    void get_classing_user_ack(int code, uint32_t uid);
    void p2p_msg_ack(uint32_t src_uid, const char *con, uint32_t con_len);
    void p2all_msg_ack(uint32_t src_uid, const char *con, uint32_t con_len);
    void user_net_status(uint32_t uid, int state);
    void recieved_msg(const char *msg, const char *fromid, const char *toid);
    void connect2msgs_state(int state);
    void disconnect_from_msgs();
    void play_progress(uint32_t, uint32_t);
    void video_data(const uint8_t *data, int w, int h);

protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
	Ui::ClassroomDialog *ui;
	Libpdu *pdu;
    QPushButton *m_PButtonClassClose;
    MediaWidget *MediaWTeacher;
    MediaWidget *MediaWStudentC;
    int m_crcpId;
    int m_PduStatus;//0-未连接MCU或连接MCU失败，1-连接上MCU，2-开课成功，3-被挤掉，4-先停录制
    QTimer m_TimerCheckMedia;
    int VideoStatus;
    int AudioInStatus;
    QMap<int, int> MapNeedVideo;
    std::queue<k12mcu2ui_video_data_t> video_datas;
    QMutex queue_mutex;
    QMap<int, int> MapOnlyAudio;
    QSize m_SizeGraffiti;
    CoursewareWidget *m_CourWidShare;//被共享的教案
    QTimer m_TimerDocumentPlay;
    int m_iCurSharingPageIndex;//被共享教案当前页码
    QRect m_RectDocument;
    int m_AlbumId;
    int m_CoursewareId;
    UpDownDisposeWidget *m_uddw;
    k12msg::k12msgs *m_k12msgs_Chat;
    QTimer m_TimerClassOver;
    QDateTime m_ClassBegin;
    QDateTime m_ClassOver;
    QLabel *m_LabelClassOver;
    QDateTime m_ClassEnter;
    center_media *m_CenterMedia;
    QString m_McuIp;
    int m_McuPort;
    QString m_dstip;
    int m_dstport;
    int m_iRecSeconds;
    QTimer m_TimerRecord;
	media_player *play;
	QLabel *m_LabelCloseWait;
	bool m_IsEight;

    MediaWidget *GetMediaWidget(int user_id);
    MediaWidget *GetMediaWidgetVipNull();
    StudentWidget *GetStudentWidget(int UserId,int UserType = 1);
    int GetMeidaAudioOnCount();
    void checkSharingFileExist(QString filename, int pageindex);
    void ChangeGraffitiBarrage();
	void InsertMedia();
	void CloseClassroom();

signals:
	void signal_K12MCU2UI_CONNECT2MCU_STATE(int);
    void signal_K12MCU2UI_DISCONNECT_FROM_MCU();
    void signal_start_class_ack(int);
    void signal_stop_class_ack(int code);
    void signal_K12MCU2UI_NEW_M_COMING(quint32 userid, int mediaType);
    void signal_K12MCU2UI_VIDEO_DATA();
    void signal_K12MCU2UI_CLOSE_MEDIA(quint32 userid, int mediaType);
    void signal_K12MCU2UI_GET_USER_LIST(user_media_t* user, quint32 cnt);
    void signal_K12MCU2UI_USER_ENTER_ROOM(user_media_t);
    void signal_K12MCU2UI_USER_LEAVE_ROOM(int UserId);
    void signal_SetGraffitiPixmapSize(QSize);
    void signal_CursorChanged(int inx);
    void signal_BeSqueezedOut();
    void signal_p2p_msg(quint32, const char *, quint32);
    void signal_p2all_msg(quint32, const char *, quint32);
    void signal_recieved_msg(QJsonObject obj);
    void signal_record_msg(int,int);
    void signal_play_progress(int bytesSent, int bytesTotal);

private slots:
    void slot_init();
    void slot_K12MCU2UI_CONNECT2MCU_STATE(int);
    void slot_K12MCU2UI_DISCONNECT_FROM_MCU();
    void slot_start_class_ack(int);
    void slot_stop_class_ack(int code);
    void slot_PButtonClassClose_clicked();
    void slot_send_openClass(QJsonObject obj,void*);
    void slot_send_stopClass(QJsonObject obj,void*);
    void slot_VideoOpen(int UserId);
    void slot_AudioOpen(int UserId);
    void slot_VideoClose(int UserId);
    void slot_AudioClose(int UserId);
    void slot_sendMcuMsg(QJsonObject obj, QString strRecvUserId = "*");
    void slot_CheckMedia();
    void slot_K12MCU2UI_NEW_M_COMING(quint32 userid, int mediaType);
    void slot_K12MCU2UI_VIDEO_DATA();
    void slot_K12MCU2UI_CLOSE_MEDIA(quint32 userid, int mediaType);
    void slot_SWMPSRight_clicked();
    void slot_SWMPSLeft_clicked();
    void slot_K12MCU2UI_GET_USER_LIST(user_media_t* user, quint32 cnt);
    void slot_K12MCU2UI_USER_ENTER_ROOM(user_media_t);
    void slot_GetEnterUser(QJsonObject,void*);
    void slot_K12MCU2UI_USER_LEAVE_ROOM(int UserId);
    void slot_PButtonStudentList_clicked();
    void slot_PButtonVip_clicked(bool);
    void slot_PButtonAuditor_clicked(bool);
    void slot_SetGraffitiSizeFinished();
    void slot_PButtonMouseClickEffect_clicked();
    void slot_PButtonPencil_clicked();
    void slot_PButtonGlowSticks_clicked();
    void slot_PButtonFile_clicked();
    void slot_PButtonScreenClear_clicked();
    void slot_PButtonNext_clicked();
    void slot_PButtonPrev_clicked();
    void slot_PButtonPlay_clicked(bool);
    void slot_DocumentPlay();
    void slot_MediaUp(int UserId);
    void slot_MediaDown(int UserId);
    void slot_PButtonFullScreen_clicked(bool IsChecked);
    void slot_PButtonHistoricalNewsList_clicked();
    void slot_PButtonBarrageClose_clicked();
    void slot_PButtonScreenshotTool_clicked();
    void slot_screenshot(QRect rt);
    void slot_PButtonGiftList_clicked();
    void slot_GetAlbumList(QJsonObject,void*);
    void slot_GetAlbumCreate(QJsonObject,void*);
    void slot_screenshot_upload(QJsonObject obj,void*);
    void slot_AlbumAddPicture(QJsonObject obj,void*);
    void slot_PButtonScreenshotAlbum_clicked();
    void slot_GetCourseware(QJsonObject,void*);
    void slot_ShareDocumentCur(CoursewareWidget *cw);
    void slot_GetGiftList(QJsonObject,void*);
    void slot_GetBarrageList(QJsonObject,void*);
    void slot_TeacherDialog_close();
    void slot_PButtonDeviceDetection_clicked();
    void slot_PButtonDevice_clicked(bool);
    void slot_PButtonSystem_clicked(bool);
    void slot_p2p_msg(quint32, const char *, quint32);
    void slot_p2all_msg(quint32, const char *, quint32);
    void slot_recieved_msg(QJsonObject obj);
    void slot_McuMsgArrived(QJsonObject);
    void slot_udw_arrive(UpDownWidget *udw);
    void slot_TimerClassOver_timeout();
	void slot_PButtonRecordOperate_clicked();
    void slot_record_msg(int,int);
    void slot_TimerRecord_timeout();
    void slot_CBoxChoiceCamera_activated(int inx);
    void slot_CBoxInputAudio_activated(int inx);
    void slot_CBoxOutputAudio_activated(int inx);
    void slot_ButtonPlayTestAudio(bool);
    void slot_play_progress(int bytesSent, int bytesTotal);
};

#endif // CLASSROOMDIALOG_H
