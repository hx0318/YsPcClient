#include "pagingwidget.h"
#include "ui_pagingwidget.h"

PagingWidget::PagingWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PagingWidget)
{
	ui->setupUi(this);
}

PagingWidget::~PagingWidget()
{
	delete ui;
}

void PagingWidget::ClearOtherChecked(QPushButton *PButtonCur)
{
	if(PButtonCur->isChecked() == false)
		PButtonCur->setChecked(true);
	if(ui->PButtonIndex1->isChecked() && ui->PButtonIndex1 != PButtonCur)
	{
		ui->PButtonIndex1->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex2->isChecked() && ui->PButtonIndex2 != PButtonCur)
	{
		ui->PButtonIndex2->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex3->isChecked() && ui->PButtonIndex3 != PButtonCur)
	{
		ui->PButtonIndex3->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex4->isChecked() && ui->PButtonIndex4 != PButtonCur)
	{
		ui->PButtonIndex4->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex5->isChecked() && ui->PButtonIndex5 != PButtonCur)
	{
		ui->PButtonIndex5->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex6->isChecked() && ui->PButtonIndex6 != PButtonCur)
	{
		ui->PButtonIndex6->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex7->isChecked() && ui->PButtonIndex7 != PButtonCur)
	{
		ui->PButtonIndex7->setChecked(false);
		return ;
	}
	if(ui->PButtonEllipsis->isChecked() && ui->PButtonEllipsis != PButtonCur)
	{
		ui->PButtonEllipsis->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex99->isChecked() && ui->PButtonIndex99 != PButtonCur)
	{
		ui->PButtonIndex99->setChecked(false);
		return ;
	}
	if(ui->PButtonIndex100->isChecked() && ui->PButtonIndex100 != PButtonCur)
		ui->PButtonIndex100->setChecked(false);
}

void PagingWidget::on_PButtonIndex1_clicked()
{
	ClearOtherChecked(ui->PButtonIndex1);
	emit signal_SendPageMark(ui->PButtonIndex1->text().toInt());
}

void PagingWidget::on_PButtonIndex2_clicked()
{
	ClearOtherChecked(ui->PButtonIndex2);
	if(ui->PButtonIndex2->text() != "…")
		emit signal_SendPageMark(ui->PButtonIndex2->text().toInt());
	else
	{
		if(ui->PButtonIndex3->text() != "4")
		{
			int PageMark = ui->PButtonIndex3->text().toInt();
			ui->PButtonIndex3->setText(QString("%1").arg(PageMark - 1));
			ui->PButtonIndex4->setText(QString("%1").arg(PageMark));
			ui->PButtonIndex5->setText(QString("%1").arg(PageMark + 1));
			ui->PButtonIndex6->setText(QString("%1").arg(PageMark + 2));
			ui->PButtonIndex7->setText(QString("%1").arg(PageMark + 3));
			ui->PButtonEllipsis->setText(QString("%1").arg(PageMark + 4));
			ui->PButtonIndex99->setText("…");
		}
		else
		{
			ui->PButtonIndex2->setText("2");
			ui->PButtonIndex3->setText("3");
			ui->PButtonIndex4->setText("4");
			ui->PButtonIndex5->setText("5");
			ui->PButtonIndex6->setText("6");
			ui->PButtonIndex7->setText("7");
			ui->PButtonEllipsis->setText("…");
			ui->PButtonIndex99->setText(QString("%1").arg(m_PageCount-1));
		}
		ClearOtherChecked(ui->PButtonIndex3);
		emit signal_SendPageMark(ui->PButtonIndex3->text().toInt());
	}
}

void PagingWidget::on_PButtonIndex3_clicked()
{
	ClearOtherChecked(ui->PButtonIndex3);
	emit signal_SendPageMark(ui->PButtonIndex3->text().toInt());
}

void PagingWidget::on_PButtonIndex4_clicked()
{
	ClearOtherChecked(ui->PButtonIndex4);
	emit signal_SendPageMark(ui->PButtonIndex4->text().toInt());
}

void PagingWidget::on_PButtonIndex5_clicked()
{
	ClearOtherChecked(ui->PButtonIndex5);
	emit signal_SendPageMark(ui->PButtonIndex5->text().toInt());
}

void PagingWidget::on_PButtonIndex6_clicked()
{
	ClearOtherChecked(ui->PButtonIndex6);
	emit signal_SendPageMark(ui->PButtonIndex6->text().toInt());
}

void PagingWidget::on_PButtonIndex7_clicked()
{
	ClearOtherChecked(ui->PButtonIndex7);
	emit signal_SendPageMark(ui->PButtonIndex7->text().toInt());
}

void PagingWidget::on_PButtonEllipsis_clicked()
{
	ClearOtherChecked(ui->PButtonEllipsis);
	if(ui->PButtonEllipsis->text() == "…")
	{
		ui->PButtonIndex2->setText("…");
		ui->PButtonIndex3->setText("4");
		ui->PButtonIndex4->setText("5");
		ui->PButtonIndex5->setText("6");
		ui->PButtonIndex6->setText("7");
		ui->PButtonIndex7->setText("8");
		ui->PButtonEllipsis->setText("9");
		if(m_PageCount == 11)
			ui->PButtonIndex99->setText("10");
		else
			ui->PButtonIndex99->setText("…");
		ClearOtherChecked(ui->PButtonIndex7);
		emit signal_SendPageMark(ui->PButtonIndex7->text().toInt());
	}
	else
		emit signal_SendPageMark(ui->PButtonEllipsis->text().toInt());
}

void PagingWidget::on_PButtonIndex99_clicked()
{
	ClearOtherChecked(ui->PButtonIndex99);
	if(ui->PButtonIndex99->text() != "…")
		emit signal_SendPageMark(ui->PButtonIndex99->text().toInt());
	else
	{
		int PageMark = ui->PButtonEllipsis->text().toInt();
		ui->PButtonIndex3->setText(QString("%1").arg(PageMark - 4));
		ui->PButtonIndex4->setText(QString("%1").arg(PageMark - 3));
		ui->PButtonIndex5->setText(QString("%1").arg(PageMark - 2));
		ui->PButtonIndex6->setText(QString("%1").arg(PageMark - 1));
		ui->PButtonIndex7->setText(QString("%1").arg(PageMark));
		ui->PButtonEllipsis->setText(QString("%1").arg(PageMark + 1));
		if(PageMark + 3 == m_PageCount)
			ui->PButtonIndex99->setText(QString("%1").arg(PageMark + 2));
		ClearOtherChecked(ui->PButtonEllipsis);
		emit signal_SendPageMark(ui->PButtonEllipsis->text().toInt());
	}
}

void PagingWidget::on_PButtonIndex100_clicked()
{
	ClearOtherChecked(ui->PButtonIndex100);
	emit signal_SendPageMark(ui->PButtonIndex100->text().toInt());
}

void PagingWidget::InitButtons(uint PageCount)
{
	ui->PButtonIndex1->show();
	ui->PButtonIndex2->show();
	ui->PButtonIndex3->show();
	ui->PButtonIndex4->show();
	ui->PButtonIndex5->show();
	ui->PButtonIndex6->show();
	ui->PButtonIndex7->show();
	ui->PButtonEllipsis->show();
	ui->PButtonIndex99->show();
	ui->PButtonIndex100->show();
	ui->LabelGo->show();
	ui->SBoxIndex->show();
	ui->PButtonGo->show();

	if(PageCount <= 10)
	{
		ui->LabelGo->hide();
		ui->SBoxIndex->hide();
		ui->PButtonGo->hide();
	}

	m_PageCount = PageCount;
	switch(m_PageCount)
	{
	case 0:
		ui->PButtonIndex1->hide();
		ui->PButtonIndex2->hide();
		ui->PButtonIndex3->hide();
		ui->PButtonIndex4->hide();
		ui->PButtonIndex5->hide();
		ui->PButtonIndex6->hide();
		ui->PButtonIndex7->hide();
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 1:
		ui->PButtonIndex2->hide();
		ui->PButtonIndex3->hide();
		ui->PButtonIndex4->hide();
		ui->PButtonIndex5->hide();
		ui->PButtonIndex6->hide();
		ui->PButtonIndex7->hide();
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 2:
		ui->PButtonIndex3->hide();
		ui->PButtonIndex4->hide();
		ui->PButtonIndex5->hide();
		ui->PButtonIndex6->hide();
		ui->PButtonIndex7->hide();
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 3:
		ui->PButtonIndex4->hide();
		ui->PButtonIndex5->hide();
		ui->PButtonIndex6->hide();
		ui->PButtonIndex7->hide();
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 4:
		ui->PButtonIndex5->hide();
		ui->PButtonIndex6->hide();
		ui->PButtonIndex7->hide();
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 5:
		ui->PButtonIndex6->hide();
		ui->PButtonIndex7->hide();
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 6:
		ui->PButtonIndex7->hide();
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 7:
		ui->PButtonEllipsis->hide();
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 8:
		ui->PButtonEllipsis->setText("8");
		ui->PButtonIndex99->hide();
		ui->PButtonIndex100->hide();
		break;
	case 9:
		ui->PButtonEllipsis->setText("8");
		ui->PButtonIndex99->setText("9");
		ui->PButtonIndex100->hide();
		break;
	case 10:
		ui->PButtonEllipsis->setText("8");
		ui->PButtonIndex99->setText("9");
		ui->PButtonIndex100->setText("10");
		break;
	default:
		ui->PButtonIndex99->setText(QString("%1").arg(m_PageCount-1));
		ui->PButtonIndex100->setText(QString("%1").arg(m_PageCount));
		break;
	}
	if(m_PageCount > 0)
		on_PButtonIndex1_clicked();
	else
		emit signal_SendPageMark(0);
}

void PagingWidget::on_PButtonGo_clicked()
{
	int inx = ui->SBoxIndex->value();
	if(inx > (int)m_PageCount)
		return ;
	emit signal_SendPageMark(inx);
	ui->PButtonIndex1->setChecked(false);
	ui->PButtonIndex2->setChecked(false);
	ui->PButtonIndex3->setChecked(false);
	ui->PButtonIndex4->setChecked(false);
	ui->PButtonIndex5->setChecked(false);
	ui->PButtonIndex6->setChecked(false);
	ui->PButtonIndex7->setChecked(false);
	ui->PButtonEllipsis->setChecked(false);
	ui->PButtonIndex99->setChecked(false);
	ui->PButtonIndex100->setChecked(false);
}
