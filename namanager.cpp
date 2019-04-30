#include "namanager.h"

NAManager::NAManager(QObject *parent) : QNetworkAccessManager(parent)
{
	connect(this, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_reply_finished(QNetworkReply*)));

	RType = Post;
	RContent = "";
	UserData = nullptr;
}

void NAManager::SendRequest(QString url, QString param, RequestType type, void *user_data)
{
	RType = type;
	RContent = param;
	UserData = user_data;

	QNetworkRequest request;
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::ContentTypeHeader,QString("application/x-www-form-urlencoded"));

	switch (type)
	{
	case Get:
		get(request);
		break;
	case Post:
		post(request, param.toUtf8());
		break;
	default:
		break;
	}
}

void NAManager::slot_reply_finished(QNetworkReply *reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray bytes = reply->readAll();
		if(RType == Post)
		{
			QJsonParseError json_error;
			QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
			if(json_error.error != QJsonParseError::NoError)
			{
				qDebug() << reply->url() << ":" << RContent << ":" << bytes << ":json_error.error:" << json_error.error;
				emit signal_reply_finished(QJsonObject(),UserData);
			}
			else
			{
				if(parse_doucment.isObject())
					emit signal_reply_finished(parse_doucment.object(), UserData);
				else
					qDebug() << reply->url() << ":" << RContent << ":not object";
			}
		}
		else
		{
			QPixmap pixmap = QPixmap();
			pixmap.loadFromData(bytes);
			if(pixmap.isNull())
				qDebug() << reply->url() << ":" << RContent << ":pixmap.isNull()";
			emit signal_reply_finished(pixmap, UserData);
		}
	}
	else
	{
		qDebug() << reply->url() << ":" << RContent << ":reply->error():" << reply->error();
		if(RType == Post)
			emit signal_reply_finished(QJsonObject(),UserData);
		else
			emit signal_reply_finished(QPixmap(), UserData);
	}

	reply->deleteLater();
}

void NAManager::UploadPng(QString server_url, QString png_path)
{
	RContent = png_path;

	QFile *f = new QFile(png_path);
	if(f->open(QIODevice::ReadOnly) == false)
	{
		qDebug() << "NAManager::UploadPng:f->open == false:" << png_path;
		delete f;
		f = nullptr;
		return ;
	}

	QHttpPart hp;
	hp.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
	hp.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data;name=\"imgfile\";filename=\"%1\"").
																	 arg(png_path)));
	hp.setBodyDevice(f);

	QHttpMultiPart *hmp = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	hmp->append(hp);
	f->setParent(hmp);

	QNetworkRequest ne;
	ne.setUrl(QUrl(server_url));
	QNetworkReply *nr = post(ne, hmp);
	hmp->setParent(nr);
}
