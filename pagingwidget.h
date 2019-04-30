#ifndef PAGINGWIDGET_H
#define PAGINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#pragma execution_character_set("utf-8")

namespace Ui {
class PagingWidget;
}

class PagingWidget : public QWidget
{
	Q_OBJECT

public:
	explicit PagingWidget(QWidget *parent = 0);
	~PagingWidget();
	void InitButtons(uint PageCount);

private:
	Ui::PagingWidget *ui;
	uint m_PageCount;

	void ClearOtherChecked(QPushButton *PButtonCur);

signals:
	void signal_SendPageMark(uint PageMark);

private slots:
	void on_PButtonIndex1_clicked();
	void on_PButtonIndex2_clicked();
	void on_PButtonIndex3_clicked();
	void on_PButtonIndex4_clicked();
	void on_PButtonIndex5_clicked();
	void on_PButtonIndex6_clicked();
	void on_PButtonIndex7_clicked();
	void on_PButtonEllipsis_clicked();
	void on_PButtonIndex99_clicked();
	void on_PButtonIndex100_clicked();
	void on_PButtonGo_clicked();
};

#endif // PAGINGWIDGET_H
