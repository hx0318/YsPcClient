#ifndef NAMANAGER_H
#define NAMANAGER_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QHttpMultiPart>
#include <QFile>

enum RequestType { Post, Get };

class NAManager : public QNetworkAccessManager
{
	Q_OBJECT

public:
	explicit NAManager(QObject *parent = nullptr);
	void SendRequest(QString url, QString param, RequestType type = Post, void *user_data = nullptr);
	void UploadPng(QString server_url, QString png_path);

private:
	RequestType RType;
	QString RContent;
	void *UserData;

signals:
	void signal_reply_finished(QJsonObject object, void *user_data = nullptr);
	void signal_reply_finished(QPixmap pixmap, void *user_data = nullptr);

private slots:
	void slot_reply_finished(QNetworkReply *reply);
};

#endif // NAMANAGER_H
